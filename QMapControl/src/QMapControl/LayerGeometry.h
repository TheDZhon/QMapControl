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

// Qt includes.
#include <QtCore/QObject>
#include <QtCore/QReadWriteLock>

// STL includes.
#include <memory>
#include <set>

// Local includes.
#include "qmapcontrol_global.h"
#include "Geometry.h"
#include "GeometryWidget.h"
#include "Layer.h"
#include "QuadTreeContainer.h"

namespace qmapcontrol
{
    //! Layer class
    /*!
     * Layer that can display Geometries.
     *
     * @author Kai Winter <kaiwinter@gmx.de>
     * @author Chris Stylianou <chris5287@gmail.com>
     */
    class QMAPCONTROL_EXPORT LayerGeometry : public Layer
    {
        Q_OBJECT
    public:
        //! Layer constructor
        /*!
         * This is used to construct a layer.
         * @param name The name of the layer.
         * @param zoom_minimum The minimum zoom level to show this geometry at.
         * @param zoom_maximum The maximum zoom level to show this geometry at.
         * @param parent QObject parent ownership.
         */
        LayerGeometry(const std::string& name, const int& zoom_minimum = 0, const int& zoom_maximum = 17, QObject* parent = 0);

        //! Disable copy constructor.
        ///LayerGeometry(const LayerGeometry&) = delete; @todo re-add once MSVC supports default/delete syntax.

        //! Disable copy assignment.
        ///LayerGeometry& operator=(const LayerGeometry&) = delete; @todo re-add once MSVC supports default/delete syntax.

        //! Destructor.
        virtual ~LayerGeometry() { } /// = default; @todo re-add once MSVC supports default/delete syntax.

        /*!
         * Returns the Geometry objects from this Layer (Use this instead of the member variable for thread-safety).
         * @param range_coord The bounding box range to limit the geometries that are fetched in coordinates.
         * @return a list of geometries that are on this Layer within the bounding box range.
         */
        const std::set<std::shared_ptr<Geometry>> getGeometries(const RectWorldCoord& range_coord) const;

        /*!
         * Returns the Geometry QWidgets from this Layer (Use this instead of the member variable for thread-safety).
         * @return a list of geometry widgets that are on this Layer.
         */
        const std::set<std::shared_ptr<GeometryWidget>> getGeometryWidgets() const;

        /*!
         * Set if a Geometry object is on this Layer.
         * @param geometry The geometry we are looking for.
         * @param controller_zoom The current controller zoom.
         * @return whether the geometry is in this layer.
         */
        bool containsGeometry(const std::shared_ptr<Geometry>& geometry, const int& controller_zoom) const;

        /*!
         * Adds a Geometry object to this Layer.
         * @param geometry The new geometry to add.
         * @param disable_redraw Whether to disable the redraw call after the geometry is added.
         */
        void addGeometry(const std::shared_ptr<Geometry>& geometry, const bool& disable_redraw = false);

        /*!
         * Removes a Geometry object from this Layer.
         * @param geometry The geometry to remove.
         * @param disable_redraw Whether to disable the redraw call after the geometry is removed.
         */
        void removeGeometry(const std::shared_ptr<Geometry>& geometry, const bool& disable_redraw = false);

        /*!
         * Removes all Geometry objects from this Layer.
         */
        void clearGeometries();

        /*!
         * Handles mouse press events (such as left-clicking an item on the layer).
         * @param mouse_event The mouse event.
         * @param mouse_point_coord The mouse point on the map in coord.
         * @param controller_zoom The current controller zoom.
         */
        bool mousePressEvent(const QMouseEvent* mouse_event, const PointWorldCoord& mouse_point_coord, const int& controller_zoom) const final;

        /*!
         * Draws each map adapter and geometry to a pixmap using the provided painter.
         * @param painter The painter that will draw to the pixmap.
         * @param backbuffer_rect_px Only draw map tiles/geometries that are contained in the backbuffer rect (pixels).
         * @param controller_zoom The current controller zoom.
         */
        void draw(QPainter& painter, const RectWorldPx& backbuffer_rect_px, const int& controller_zoom) const final;

        /*!
         * Moves any geometries that represent a widget, as these are not drawn to the actually pixmap.
         * @param offset_px The offset in pixels to remove from the coordinate pixel point.
         * @param controller_zoom The current controller zoom.
         */
        void moveGeometryWidgets(const PointPx& offset_px, const int& controller_zoom) const;

    signals:
        /*!
         * Signal emitted when a geometry is clicked.
         * @param geometry The clicked Geometry.
         */
        void geometryClicked(const Geometry* geometry) const;

    private:
        /// List of geometries drawn by this layer.
        QuadTreeContainer<std::shared_ptr<Geometry>> m_geometries;

        /// Mutex to protect geometries.
        mutable QReadWriteLock m_geometries_mutex;

        /// List of geometry widgets drawn by this layer.
        std::set<std::shared_ptr<GeometryWidget>> m_geometry_widgets;

        /// Mutex to protect geometry widgets.
        mutable QReadWriteLock m_geometry_widgets_mutex;
    };
}
