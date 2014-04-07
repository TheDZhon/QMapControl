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
#include <QtCore/QPoint>
#include <QtCore/QReadWriteLock>
#include <QtCore/QRect>
#include <QtCore/QSize>
#include <QtGui/QMouseEvent>
#include <QtGui/QPainter>

// STL includes.
#include <memory>
#include <set>

// Local includes.
#include "qmapcontrol_global.h"
#include "MapAdapter.h"
#include "Geometry.h"
#include "GeometryPointWidget.h"
#include "QuadTreeContainer.h"

namespace qmapcontrol
{
    //! Layer class
    /*!
     * Layer can display map and geometries (both static and dynamic).
     * - Maps and static geometries get "baked" onto a pixmap and are not redrawn until the user pans the view to a new
     * area not covered by the visible viewport.
     * - Dynamic geometries and always redrawn when the user pans the view (or geometry requests a redraw).
     *
     * @author Kai Winter <kaiwinter@gmx.de>
     * @author Chris Stylianou <chris5287@gmail.com>
     */
    class QMAPCONTROL_EXPORT Layer : public QObject
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
        Layer(const std::string& name, const int& zoom_minimum = 0, const int& zoom_maximum = 17, QObject* parent = 0);

        //! Disable copy constructor.
        ///Layer(const Layer&) = delete; @todo re-add once MSVC supports default/delete syntax.

        //! Disable copy assignment.
        ///Layer& operator=(const Layer&) = delete; @todo re-add once MSVC supports default/delete syntax.

        //! Destructor.
        virtual ~Layer() { } /// = default; @todo re-add once MSVC supports default/delete syntax.

        /*!
         * Returns the layer's name.
         * @return the name of this layer.
         */
        std::string	getName() const;

        /*!
         * Fetches a meta-data value.
         * @param key The meta-data key.
         * @return the meta-data value.
         */
        QVariant getMetadata(const std::string& key) const;

        /*!
         * Set a meta-data key/value.
         * @param key The meta-data key.
         * @param value The meta-data value.
         */
        void setMetadata(const std::string& key, const QVariant& value);

        /*!
         * Returns the Map Adapters from this Layer (Use this instead of the member variable for thread-safety).
         * @return a list of map adapters that are on this Layer.
         */
        const std::vector<std::shared_ptr<MapAdapter>> getMapAdapters() const;

        /*!
         * Add a Map Adater to this layer.
         * @param map_adapter The Map Adapter to add.
         */
        void addMapAdapter(const std::shared_ptr<MapAdapter>& map_adapter);

        /*!
         * Whether the layer is currently visible.
         * @param controller_zoom The current controller zoom.
         * @return if the layer is visible.
         */
        bool isVisible(const int& controller_zoom) const;

        /*!
         * Set the visibility of the layer.
         * @param visible Whether the layer should be visible.
         */
        void setVisible(const bool& visible);

        /*!
         * Set whether the layer should handle mouse events.
         * @param enable Whether the layer should handle mouse events.
         */
        void enableMouseEvents(const bool& enable);

        /*!
         * Returns the Geometry objects from this Layer (Use this instead of the member variable for thread-safety).
         * @param range_coord The bounding box range to limit the geometries that are fetched in coordinates.
         * @return a list of geometries that are on this Layer within the bounding box range.
         */
        const std::set<std::shared_ptr<Geometry>> getGeometries(const QRectF& range_coord) const;

        /*!
         * Returns the Geometry QWidgets from this Layer (Use this instead of the member variable for thread-safety).
         * @return a list of geometry widgets that are on this Layer.
         */
        const std::set<std::shared_ptr<GeometryPointWidget>> getGeometryWidgets() const;

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
        void mousePressEvent(const QMouseEvent* mouse_event, const PointWorldCoord& mouse_point_coord, const int& controller_zoom);

        /*!
         * Draws each map adapter and geometry to a pixmap using the provided painter.
         * @param painter The painter that will draw to the pixmap.
         * @param backbuffer_rect_px Only draw map tiles/geometries that are contained in the backbuffer rect (pixels).
         * @param controller_zoom The current controller zoom.
         */
        void draw(QPainter* painter, const RectWorldPx& backbuffer_rect_px, const int& controller_zoom) const;

        /*!
         * Moves any geometries that represent a widget, as these are not drawn to the actually pixmap.
         * @param offset_px The offset in pixels to remove from the coordinate pixel point.
         * @param controller_zoom The current controller zoom.
         */
        void moveGeometryWidgets(const PointPx& offset_px, const int& controller_zoom) const;

    private:
        /*!
         * Draws each map adapter to a pixmap using the provided painter.
         * @param painter The painter that will draw to the pixmap.
         * @param backbuffer_rect_px Only draw map tiles that are contained in the backbuffer rect (pixels).
         * @param controller_zoom The current controller zoom.
         */
        void drawMapAdapters(QPainter* painter, const RectWorldPx& backbuffer_rect_px, const int& controller_zoom) const;

        /*!
         * Draws each geometry to a pixmap using the provided painter.
         * @param painter The painter that will draw to the pixmap.
         * @param backbuffer_rect_px Only draw geometries that are contained in the backbuffer rect (pixels).
         * @param controller_zoom The current controller zoom.
         */
        void drawGeometries(QPainter* painter, const RectWorldPx& backbuffer_rect_px, const int& controller_zoom) const;

    signals:
        /*!
         * Signal emitted when a geometry is clicked.
         * @param geometry The clicked Geometry.
         */
        void geometryClicked(Geometry* geometry);

        /*!
         * Signal emitted when a change has occurred that requires the layer to be redrawn.
         */
        void requestRedraw() const;

    private:
        //! Disable copy constructor.
        Layer(const Layer&); /// @todo remove once MSVC supports default/delete syntax.

        //! Disable copy assignment.
        Layer& operator=(const Layer&); /// @todo remove once MSVC supports default/delete syntax.

        /// List of map adapters draw by this layer.
        std::vector<std::shared_ptr<MapAdapter>> m_mapadapters;

        /// Mutex to protect map adapters.
        mutable QReadWriteLock m_mapadapters_mutex;

        /// List of geometries draw by this layer.
        QuadTreeContainer<std::shared_ptr<Geometry>> m_geometries;

        /// Mutex to protect geometries.
        mutable QReadWriteLock m_geometries_mutex;

        /// List of geometry widgets draw by this layer.
        std::set<std::shared_ptr<GeometryPointWidget>> m_geometry_widgets;

        /// Mutex to protect geometry widgets.
        mutable QReadWriteLock m_geometry_widgets_mutex;

        /// Whether the layer is visible.
        bool m_visible;

        /// Whether mouse events are enabled.
        bool m_mouse_events_enabled;

        /// The layer name.
        const std::string m_name;

        /// Minimum zoom level to show this geometry.
        const int m_zoom_minimum;

        /// Maximum zoom level to show this geometry.
        const int m_zoom_maximum;

        /// Meta-data storage.
        std::map<std::string, QVariant> m_metadata;
    };
}
