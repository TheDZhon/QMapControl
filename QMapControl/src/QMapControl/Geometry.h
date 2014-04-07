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
#include <QtCore/QRectF>
#include <QtCore/QVariant>
#include <QtGui/QPainter>
#include <QtGui/QPen>

#include <QtWidgets/QGraphicsItem>

// STL includes.
#include <map>

// Local includes.
#include "qmapcontrol_global.h"

namespace qmapcontrol
{
    //! Main class for objects that should be painted in maps.
    /*!
     * Geometry is the root class of the hierarchy. Geometry is an abstract (non-instantiable) class.
     *
     * This class and the derived classes Point, Curve and LineString are leant on the Simple
     * Feature Specification of the Open Geospatial Consortium.
     * @see www.opengeospatial.com
     *
     * @author Kai Winter <kaiwinter@gmx.de>
     * @author Chris Stylianou <chris5287@gmail.com>
     */
    class QMAPCONTROL_EXPORT Geometry : public QObject
    {
        Q_OBJECT
    public:
        //! Geometry types.
        enum class GeometryType
        {
            /// GeometryCurve.
            ///GeometryCurve,
            /// GeometryLineString.
            GeometryLineString,
            /// GeometryPoint.
            GeometryPoint,
            /// Geometry that manages a QWidget.
            GeometryPointWidget,
            /// GeometryPolygon.
            GeometryPolygon
        };

    public:
        //! Disable copy constructor.
        ///Geometry(const Geometry&) = delete; @todo re-add once MSVC supports default/delete syntax.

        //! Disable copy assignment.
        ///Geometry& operator=(const Geometry&) = delete; @todo re-add once MSVC supports default/delete syntax.

        //! Destructor.
        virtual ~Geometry() { } /// = default; @todo re-add once MSVC supports default/delete syntax.

        /*!
         * Fetches the geometry type.
         * @return the geometry type.
         */
        GeometryType getGeometryType() const;

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
         * Fetches the pen to draw the geometry with.
         * @return the QPen to used for drawing.
         */
        const QPen& getPen() const;

        /*!
         * Sets the pen to draw the geometry with.
         * @param pen The QPen to used for drawing.
         */
        virtual void setPen(const QPen& pen);

        /*!
         * Fetches whether the geometry is visible.
         * @param controller_zoom The current controller zoom.
         * @return whether the geometry is visible.
         */
        bool isVisible(const int& controller_zoom) const;

    public slots:
        /*!
         * Set the visibility of the geometry.
         * @param enabled Whether to make the geometry visible.
         */
        virtual void setVisible(const bool& enabled);

    public:
        /*!
         * The bounding box in world coordinates.
         * @param controller_zoom The current controller zoom.
         * @return the bounding box.
         */
        virtual QRectF boundingBox(const int& controller_zoom) const = 0;

        /*!
         * Checks if any geometries are located inside the specified area.
         * @param area_px The polygon area in pixels to check for points.
         * @param controller_zoom The current controller zoom.
         * @return whether the geometries are located inside the specified area.
         */
        virtual bool touches(const QGraphicsItem& area_px, const int& controller_zoom) = 0;

        /*!
         * Draws the geometry to a pixmap using the provided painter.
         * @param painter The painter that will draw to the pixmap.
         * @param backbuffer_rect_px Only draw geometries that are contained in the backbuffer rect (pixels).
         * @param controller_zoom The current controller zoom.
         */
        virtual void draw(QPainter* painter, const QRectF& backbuffer_rect_px, const int& controller_zoom) = 0;

    protected:
        //! Constructor.
        /*!
         * This construct a Geometry.
         * @param geometry_type The geometry type.
         * @param pen The pen to draw with.
         * @param zoom_minimum The minimum zoom level to show this geometry at.
         * @param zoom_maximum The maximum zoom level to show this geometry at.
         */
        Geometry(const GeometryType& geometry_type, const QPen& pen = QPen(), const int& zoom_minimum = 0, const int& zoom_maximum = 17);

    signals:
        /*!
         * Signal emitted when a geometry is clicked.
         * @param geometry The clicked Geometry.
         */
        void geometryClicked(Geometry* geometry);

        /*!
         * Signal emitted when a geometry changes its position.
         * @param geometry The geometry that change position.
         */
        void positionChanged(Geometry* geometry);

        /*!
         * Signal emitted when a change has occurred that requires the layer to be redrawn.
         */
        void requestRedraw() const;

    private:
        //! Disable copy constructor.
        Geometry(const Geometry&); /// @todo remove once MSVC supports default/delete syntax.

        //! Disable copy assignment.
        Geometry& operator=(const Geometry&); /// @todo remove once MSVC supports default/delete syntax.

    private:
        /// Whether the geometry is visible.
        bool m_visible;

        /// The geometry type.
        const GeometryType m_geometry_type;

        /// The pen to use when drawing a geometry.
        QPen m_pen;

        /// Minimum zoom level to show this geometry.
        int m_zoom_minimum;

        /// Maximum zoom level to show this geometry.
        int m_zoom_maximum;

        /// Meta-data storage.
        std::map<std::string, QVariant> m_metadata;
    };
}
