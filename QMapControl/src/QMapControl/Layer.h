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
#include <QtCore/QVariant>
#include <QtGui/QMouseEvent>
#include <QtGui/QPainter>

// STL includes.
#include <map>
#include <string>

// Local includes.
#include "qmapcontrol_global.h"
#include "Point.h"

namespace qmapcontrol
{
    //! Layer class
    /*!
     * Layer can display "stuff".
     * See the specialised layer types to display each type of stuff (eg: MapAdapters, Geometries, etc...).
     *
     * @author Kai Winter <kaiwinter@gmx.de>
     * @author Chris Stylianou <chris5287@gmail.com>
     */
    class QMAPCONTROL_EXPORT Layer : public QObject
    {
        Q_OBJECT
    public:
        //! Layer types.
        enum class LayerType
        {
            /// Layer that draws a MapAdapter.
            LayerMapAdapter,
            /// Layer that draws Geometries.
            LayerGeometry,
            /// Layer that draws ESRI Shapefiles.
            LayerESRIShapefile
        };

    protected:
        //! Layer constructor
        /*!
         * This is used to construct a layer.
         * @param layer_type The layer type.
         * @param name The name of the layer.
         * @param zoom_minimum The minimum zoom level to show this geometry at.
         * @param zoom_maximum The maximum zoom level to show this geometry at.
         * @param parent QObject parent ownership.
         */
        Layer(const LayerType& layer_type, const std::string& name, const int& zoom_minimum = 0, const int& zoom_maximum = 17, QObject* parent = 0);

    public:
        //! Disable copy constructor.
        ///Layer(const Layer&) = delete; @todo re-add once MSVC supports default/delete syntax.

        //! Disable copy assignment.
        ///Layer& operator=(const Layer&) = delete; @todo re-add once MSVC supports default/delete syntax.

        //! Destructor.
        virtual ~Layer() { } /// = default; @todo re-add once MSVC supports default/delete syntax.

        /*!
         * Fetches the layer type.
         * @return the layer type.
         */
        LayerType getLayerType() const;

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
         * Whether the layer is currently visible.
         * @param controller_zoom The current controller zoom.
         * @return whether the layer is visible.
         */
        bool isVisible(const int& controller_zoom) const;

        /*!
         * \brief Whether the layer is currently visible, independently from the zoom factor
         * \return whether the layer is visible.
         */
        bool isVisible() const;

        /*!
         * Set the visibility of the layer.
         * @param visible Whether the layer should be visible.
         */
        void setVisible(const bool& visible);

        /*!
         * Whether mouse events are currently enabled.
         * @return whether mouse events are enabled.
         */
        bool isMouseEventsEnabled() const;

        /*!
         * Set whether the layer should handle mouse events.
         * @param enable Whether the layer should handle mouse events.
         */
        void setMouseEventsEnabled(const bool& enable);

        /*!
         * Handles mouse press events (such as left-clicking an item on the layer).
         * @param mouse_event The mouse event.
         * @param mouse_point_coord The mouse point on the map in coord.
         * @param controller_zoom The current controller zoom.
         * @return true if mouse press was handled by layer.
         */
        virtual bool mousePressEvent(const QMouseEvent* mouse_event, const PointWorldCoord& mouse_point_coord, const int& controller_zoom) const = 0;

        /*!
         * Draws each map adapter and geometry to a pixmap using the provided painter.
         * @param painter The painter that will draw to the pixmap.
         * @param backbuffer_rect_px Only draw map tiles/geometries that are contained in the backbuffer rect (pixels).
         * @param controller_zoom The current controller zoom.
         */
        virtual void draw(QPainter& painter, const RectWorldPx& backbuffer_rect_px, const int& controller_zoom) const = 0;

    signals:
        /*!
         * Signal emitted when a change has occurred that requires the layer to be redrawn.
         */
        void requestRedraw() const;

    private:
        //! Disable copy constructor.
        Layer(const Layer&); /// @todo remove once MSVC supports default/delete syntax.

        //! Disable copy assignment.
        Layer& operator=(const Layer&); /// @todo remove once MSVC supports default/delete syntax.

    private:
        /// The layer type.
        LayerType m_layer_type;

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
