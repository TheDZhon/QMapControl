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
#include <QtGui/QBrush>
#include <QtGui/QPainter>
#include <QtGui/QPen>

// GDAL includes.
#include <gdal/ogrsf_frmts.h>

// STL includes.
#include <memory>
#include <string>

// Local includes.
#include "qmapcontrol_global.h"
#include "Point.h"

namespace qmapcontrol
{
    //! ESRIShapefile class
    /*!
     * Object to present an ESRI Shapefile.
     *
     * @author Chris Stylianou <chris5287@gmail.com>
     */
    class QMAPCONTROL_EXPORT ESRIShapefile : public QObject
    {
        Q_OBJECT
    public:
        //! ESRIShapefile constructor
        /*!
         * This is used to construct a ESRIShapefile.
         * @param file_path The file path of the ESRI Shapefile.
         * @param layer_name The name of the layer.
         * @param zoom_minimum The minimum zoom level to show this ESRI Shapefile at.
         * @param zoom_maximum The maximum zoom level to show this ESRI Shapefile at.
         */
        explicit ESRIShapefile(const std::string& file_path, const std::string& layer_name, const int& zoom_minimum = 0, const int& zoom_maximum = 17);

        explicit ESRIShapefile(OGRDataSource *datasource, const std::string& layer_name, const int& zoom_minimum = 0, const int& zoom_maximum = 17);

        //! Destructor.
        virtual ~ESRIShapefile();

        /*!
         * Fetches the pen to draw the polygon with (outline).
         * @return the QPen to used for drawing.
         */
        const QPen& getPenPolygon() const;

        /*!
         * Sets the pen to draw the polygon with (outline).
         * @param pen The QPen to used for drawing.
         */
        void setPenPolygon(const std::shared_ptr<QPen>& pen);

        /*!
         * Sets the pen to draw the polygon with (outline).
         * @param pen The QPen to used for drawing.
         */
        void setPenPolygon(const QPen& pen);

        /*!
         * Fetches the brush to draw the polygon with (fill).
         * @return the QBrush to used for drawing.
         */
        const QBrush& getBrushPolygon() const;

        /*!
         * Sets the brush to draw the polygon with (fill).
         * @param brush The QBrush to used for drawing.
         */
        void setBrushPolygon(const std::shared_ptr<QBrush>& brush);

        /*!
         * Sets the brush to draw the polygon with (fill).
         * @param brush The QBrush to used for drawing.
         */
        void setBrushPolygon(const QBrush& brush);

        /*!
         * Fetches the pen to draw the linestring with (outline).
         * @return the QPen to used for drawing.
         */
        const QPen& getPenLineString() const;

        /*!
         * Sets the pen to draw the linestring with (outline).
         * @param pen The QPen to used for drawing.
         */
        void setPenLineString(const std::shared_ptr<QPen>& pen);

        /*!
         * Sets the pen to draw the linestring with (outline).
         * @param pen The QPen to used for drawing.
         */
        void setPenLineString(const QPen& pen);

        /*!
         * Draws ESRI Shapefiles to a pixmap using the provided painter.
         * @param painter The painter that will draw to the pixmap.
         * @param backbuffer_rect_px Only draw geometries that are contained in the backbuffer rect (pixels).
         * @param controller_zoom The current controller zoom.
         */
        void draw(QPainter& painter, const RectWorldPx& backbuffer_rect_px, const int& controller_zoom) const;

    protected:

        /*!
         * Draws ESRI Shapefile feature to a pixmap using the provided painter.
         * @param ogr_feature The feature to draw.
         * @param painter The painter that will draw to the pixmap.
         * @param controller_zoom The current controller zoom.
         */
        virtual void drawFeature(OGRFeature* ogr_feature, QPainter& painter, const int& controller_zoom) const;

    signals:
        /*!
         * Signal emitted when a change has occurred that requires the layer to be redrawn.
         */
        void requestRedraw() const;

    private:
        /// The OGR data set of the ESRI Shapefile.
        OGRDataSource* m_ogr_data_set;

        /// The layer name.
        std::string m_layer_name;

        /// Minimum zoom level to show this ESRI Shapefile.
        int m_zoom_minimum;

        /// Maximum zoom level to show this ESRI Shapefile.
        int m_zoom_maximum;

        /// The pen to use when drawing a polygon.
        mutable std::shared_ptr<QPen> m_pen_polygon;

        /// The brush to use when drawing a polygon.
        mutable std::shared_ptr<QBrush> m_brush_polygon;

        /// The pen to use when drawing a linestring.
        mutable std::shared_ptr<QPen> m_pen_linestring;
    };
}
