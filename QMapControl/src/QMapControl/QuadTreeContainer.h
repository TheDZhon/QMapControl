/*
*
* This file is part of QMapControl,
* an open-source cross-platform map widget
*
* Copyright (C) 2007 - 2008 Kai Winter
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with QMapControl. If not, see <http://www.gnu.org/licenses/>.
*
* Contact e-mail: kaiwinter@gmx.de
* Program URL   : http://qmapcontrol.sourceforge.net/
*
*/

#pragma once

// STD includes.
#include <memory>
#include <set>
#include <vector>
#include <stdexcept>

// Local includes.
#include "qmapcontrol_global.h"
#include "Point.h"

/*!
 * @author Chris Stylianou <chris5287@gmail.com>
 */
namespace qmapcontrol
{
    /*!
     * Based on: http://en.wikipedia.org/wiki/Quadtree
     */
    template <class T>
    class QMAPCONTROL_EXPORT QuadTreeContainer
    {
    public:
        //! Constuctor.
        /*!
         * Quad Tree Container constructor.
         * @param capacity The number of items this quad tree container can store before it's children are created/used.
         * @param boundary_coord The bounding box area that this quad tree container covers in coordinates.
         */
        QuadTreeContainer(const size_t& capacity, const RectWorldCoord& boundary_coord)
            : m_capacity(capacity),
              m_boundary_coord(boundary_coord)
        {
            // Reserve the container size.
            m_points.reserve(capacity);
        }

        //! Disable copy constructor.
        ///QuadTreeContainer(const QuadTreeContainer&) = delete; @todo re-add once MSVC supports default/delete syntax.

        //! Disable copy assignment.
        ///QuadTreeContainer& operator=(const QuadTreeContainer&) = delete; @todo re-add once MSVC supports default/delete syntax.

        //! Destructor.
        virtual ~QuadTreeContainer() { } /// = default; @todo re-add once MSVC supports default/delete syntax.

        /*!
         * Fetches objects within the specified bounding box range.
         * @param return_points The objects that are within the specified range are added to this.
         * @param range_coord The bounding box range.
         */
        void query(std::vector<T>& return_points, const RectWorldCoord& range_coord) const
        {
            // Does the range intersect with our boundary.
            if(range_coord.rawRect().intersects(m_boundary_coord.rawRect()))
            {
                // Check whether any of our points are contained in the range.
                for(const auto& point : m_points)
                {
                    // Is the point contained by the query range.
                    if(range_coord.rawRect().contains(point.first.rawPoint()))
                    {
                        // Add to the return points.
                        return_points.push_back(point.second);
                    }
                }

                // Do we have any child quad tree nodes?
                if(m_child_north_east != nullptr)
                {
                    // Search each child and add the points they return.
                    m_child_north_east->query(return_points, range_coord);
                    m_child_north_west->query(return_points, range_coord);
                    m_child_south_east->query(return_points, range_coord);
                    m_child_south_west->query(return_points, range_coord);
                }
            }
        }

        /*!
         * Inserts an object into the quad tree container.
         * @param point_coord The objects's point in coordinates.
         * @param object The object to insert.
         * @return whether the object was inserted into this quad tree container.
         */
        bool insert(const PointWorldCoord& point_coord, const T& object)
        {
            // Keep track of our success.
            bool success(false);

            // Does this boundary contain the point?
            if(m_boundary_coord.rawRect().contains(point_coord.rawPoint()))
            {
                // Have we reached our capacity?
                if(m_points.size() < m_capacity)
                {
                    // Add the point.
                    m_points.emplace_back(point_coord, object);

                    // Update our success.
                    success = true;
                }
                else
                {
                    // Do we already have child quad tree nodes?
                    if(m_child_north_east == nullptr)
                    {
                        // We need to create the child quad tree nodes before we continue.
                        subdivide();
                    }

                    // Try inserting into north east.
                    if(m_child_north_east->insert(point_coord, object))
                    {
                        // Update our success.
                        success = true;
                    }
                    // Try inserting into north west.
                    else if(m_child_north_west->insert(point_coord, object))
                    {
                        // Update our success.
                        success = true;
                    }
                    // Try inserting into south east.
                    else if(m_child_south_east->insert(point_coord, object))
                    {
                        // Update our success.
                        success = true;
                    }
                    // Try inserting into south west.
                    else if(m_child_south_west->insert(point_coord, object))
                    {
                        // Update our success.
                        success = true;
                    }
                    else
                    {
                        // We cannot insert, fail!
                        throw std::runtime_error("Unable to insert into quad tree container.");
                    }
                }
            }

            // Return our success.
            return success;
        }

        /*!
         * Removes an object from the quad tree container.
         * @param point_coord The objects's point in coordinates.
         * @param object The object to remove.
         */
        void erase(const PointWorldCoord& point_coord, const T& object)
        {
            // Does this boundary contain the point?
            if(m_boundary_coord.rawRect().contains(point_coord.rawPoint()))
            {
                // Check whether any of our points are contained in the range.
                auto itr_point = m_points.begin();
                while(itr_point != m_points.end())
                {
                    // Have we found the object?
                    if(itr_point->second == object)
                    {
                        // Remove the object from the container.
                        itr_point = m_points.erase(itr_point);
                    }
                    else
                    {
                        // Move on to the next point.
                        ++itr_point;
                    }
                }

                // Do we have child quad tree nodes?
                if(m_child_north_east != nullptr)
                {
                    // Search each child and remove the object if found.
                    m_child_north_east->erase(point_coord, object);
                    m_child_north_west->erase(point_coord, object);
                    m_child_south_east->erase(point_coord, object);
                    m_child_south_west->erase(point_coord, object);
                }
            }
        }

        /*!
         * Removes all objects from the quad tree container.
         */
        void clear()
        {
            // Clear the points.
            m_points.clear();

            // Reset the child nodes.
            m_child_north_east.reset(nullptr);
            m_child_north_west.reset(nullptr);
            m_child_south_east.reset(nullptr);
            m_child_south_west.reset(nullptr);
        }

    private:
        //! Disable copy constructor.
        QuadTreeContainer(const QuadTreeContainer&); /// @todo remove once MSVC supports default/delete syntax.

        //! Disable copy assignment.
        QuadTreeContainer& operator=(const QuadTreeContainer&); /// @todo remove once MSVC supports default/delete syntax.

        /*!
         * Creates the child nodes.
         */
        void subdivide()
        {
            // Calculate half the size of the boundary.
            const QSizeF half_size = m_boundary_coord.rawRect().size() / 2.0;

            // Construct the north east child.
            const RectWorldCoord north_east(PointWorldCoord(m_boundary_coord.rawRect().left() + half_size.width(), m_boundary_coord.rawRect().top()), half_size);
            m_child_north_east.reset(new QuadTreeContainer<T>(m_capacity, north_east));

            // Construct the north west child.
            const RectWorldCoord north_west(PointWorldCoord(m_boundary_coord.rawRect().left(), m_boundary_coord.rawRect().top()), half_size);
            m_child_north_west.reset(new QuadTreeContainer<T>(m_capacity, north_west));

            // Construct the south east child.
            const RectWorldCoord south_east(PointWorldCoord(m_boundary_coord.rawRect().left() + half_size.width(), m_boundary_coord.rawRect().top() + half_size.height()), half_size);
            m_child_south_east.reset(new QuadTreeContainer<T>(m_capacity, south_east));

            // Construct the south west child.
            const RectWorldCoord south_west(PointWorldCoord(m_boundary_coord.rawRect().left(), m_boundary_coord.rawRect().top() + half_size.height()), half_size);
            m_child_south_west.reset(new QuadTreeContainer<T>(m_capacity, south_west));
        }

    private:
        /// Quad tree node capacity.
        const size_t m_capacity;

        /// Boundary of this quad tree node.
        const RectWorldCoord m_boundary_coord;

        /// Points in this quad tree node.
        std::vector<std::pair<PointWorldCoord, T>> m_points;

        /// Child: north east quad tree node.
        std::unique_ptr<QuadTreeContainer> m_child_north_east;

        /// Child: north west quad tree node.
        std::unique_ptr<QuadTreeContainer> m_child_north_west;

        /// Child: south east quad tree node.
        std::unique_ptr<QuadTreeContainer> m_child_south_east;

        /// Child: south west quad tree node.
        std::unique_ptr<QuadTreeContainer> m_child_south_west;
    };
}
