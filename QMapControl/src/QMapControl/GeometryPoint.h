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
#include <QWidget>

// Local includes.
#include "qmapcontrol_global.h"
#include "Geometry.h"

namespace qmapcontrol
{
    //! A geometric point to draw objects into maps.
    /*!
     * This class can be used to draw your custom QPixmap or other QWidgets (see GeometryPointWidget) into maps.
     * You can instantiate a Point with any Pixmap you want. The objects cares about collision detection (for clickable objects).
     *
     * Points emit click events, if the containing layer receives clickevents (the default).
     *
     * To create "zoomable objects" (objects that increases size on zooming), a base level has to be set.
     * The base level is the zoom level on which the point´s pixmap gets displayed on full size.
     * On lower zoom levels it gets displayed smaller and on higher zoom levels larger.
     * A minimal size can be set as well as a maximum size.
     * @see setBaselevel, setDrawMinimumPx, setDrawMaximumPx.
     *
     * @author Kai Winter <kaiwinter@gmx.de>
     * @author Chris Stylianou <chris5287@gmail.com>
     */
    class QMAPCONTROL_EXPORT GeometryPoint : public Geometry
    {
        Q_OBJECT
    public:
        //! Alignment types.
        enum class AlignmentType
        {
            /// Align on middle point.
            Middle,
            /// Align on top left point.
            TopLeft,
            /// Align on top right point.
            TopRight,
            /// Align on top middle point.
            TopMiddle,
            /// Align on bottom left point.
            BottomLeft,
            /// Align on bottom right point.
            BottomRight,
            /// Align on bottom middle point.
            BottomMiddle
        };

    public:
        //! Constructor.
        /*!
         * This constructor creates a point with no image or widget.
         * @param point_coord The x/y coordinate (longitude/latitude).
         * @param pen The pen to draw with.
         * @param zoom_minimum The minimum zoom level to show this geometry at.
         * @param zoom_maximum The maximum zoom level to show this geometry at.
         */
        GeometryPoint(const QPointF& point_coord, const QPen& pen = QPen(), const int& zoom_minimum = 0, const int& zoom_maximum = 17);

        //! Constructor.
        /*!
         * This constructor creates a point with no image or widget.
         * @param x_coord The x coordinate (longitude).
         * @param y_coord The y coordinate (latitude).
         * @param pen The pen to draw with.
         * @param zoom_minimum The minimum zoom level to show this geometry at.
         * @param zoom_maximum The maximum zoom level to show this geometry at.
         */
        GeometryPoint(const qreal& x_coord, const qreal& y_coord, const QPen& pen = QPen(), const int& zoom_minimum = 0, const int& zoom_maximum = 17);

        //! Constructor.
        /*!
         * This constructor creates a point which will display the given pixmap.
         * @param point_coord The x/y coordinate (longitude/latitude).
         * @param pixmap The pixmap to be displayed by this point.
         * @param pen The pen to draw with.
         * @param zoom_minimum The minimum zoom level to show this geometry at.
         * @param zoom_maximum The maximum zoom level to show this geometry at.
         */
        GeometryPoint(const QPointF& point_coord, const QPixmap& pixmap, const QPen& pen = QPen(), const int& zoom_minimum = 0, const int& zoom_maximum = 17);

        //! Disable copy constructor.
        ///GeometryPoint(const GeometryPoint&) = delete; @todo re-add once MSVC supports default/delete syntax.

        //! Disable copy assignment.
        ///GeometryPoint& operator=(const GeometryPoint&) = delete; @todo re-add once MSVC supports default/delete syntax.

        //! Destructor.
        virtual ~GeometryPoint() { } /// = default; @todo re-add once MSVC supports default/delete syntax.

        /*!
         * Fetches the x-coordinate (longitude).
         * @return the x-coordinate (longitude).
         */
        qreal x() const;

        /*!
         * Fetches the y-coordinate (latitude).
         * @return the y-coordinate (latitude).
         */
        qreal y() const;

        /*!
         * Fetches the x/y coordinate (longitude/latitude).
         * @return the x/y coordinate (longitude/latitude).
         */
        QPointF coordinate() const;

        /*!
         * Set the alignment type to use when drawing the geometry.
         * @param alignment_type The alignment type to set.
         */
        void setAlignmentType(const AlignmentType& alignment_type = AlignmentType::Middle);

        /*!
         * Use this method to set a zoom level on which the pixmap will be drawn at its real size.
         * Set to '-1' to always drawn at its real size.
         * On lower zoom levels, it will be drawn smaller.
         * On higher zoom levels, it will be drawn larger.
         * @see setMinsize, setMaxsize
         * @param zoom The zoom level where the point will be drawn at its real size.
         */
        void setBaseZoom(const int& zoom = -1);

        /*!
         * Set the minimum size a point/widget/pixmap can be drawn.
         * Set to 'QSizeF(-1.0, -1.0)' to disable the limitation.
         * @param size The minimum size to draw a point/widget/pixmap.
         */
        void setDrawMinimumPx(const QSizeF& size_px = QSizeF(-1.0, -1.0));

        /*!
         * Set the maximum size a point/widget/pixmap can be drawn.
         * Set to 'QSizeF(-1.0, -1.0)' to disable the limitation.
         * @param size The maximal size to draw a point/widget/pixmap.
         */
        void setDrawMaximumPx(const QSizeF& size_px = QSizeF(-1.0, -1.0));

        /*!
         * Fetches the geometry's pixmap.
         * @return the geometry's pixmap.
         */
        QPixmap getPixmap() const;

        /*!
         * Set the pixmap to draw.
         * @param pixmap The pixmap to draw.
         */
        void setPixmap(const QPixmap& pixmap);

        /*!
         * Fetches the widget.
         * @return The geometry's widget.
         */
        QWidget* getWidget() const;

        /*!
         * Sets the pen to draw the geometry with.
         * @param pen The QPen to used for drawing.
         */
        void setPen(const QPen& pen) final;

    public slots:
        /*!
         * Set the coordinate of the point.
         * @param point The coordinate to set.
         */
        void setCoordinate(const QPointF& point);

        /*!
         * Set the visibility of the geometry.
         * @param enabled Whether to make the geometry visible.
         */
        virtual void setVisible(const bool& enabled) override;

        /*!
         * Set the meta-data value to display with the geometry.
         * @param key The meta-data key.
         * @param zoom_minimum The minimum zoom to display the meta-data value at.
         * @param offset_px The offset that the meta-data value is displayed from in pixels.
         */
        void setMetadataDisplayed(const std::string& key, const int& zoom_minimum = 10, const double& offset_px = 5.0);

    public:
        /*!
         * The bounding box in world coordinates.
         * @param controller_zoom The current controller zoom.
         * @return the bounding box.
         */
        QRectF boundingBox(const int& controller_zoom) const final;

        /*!
         * Checks if any geometries are located inside the specified area.
         * @param area_px The polygon area in pixels to check for points.
         * @param controller_zoom The current controller zoom.
         * @return whether the geometries are located inside the specified area.
         */
        virtual bool touches(const QGraphicsItem& area_px, const int& controller_zoom) override;

        /*!
         * Draws the geometry to a pixmap using the provided painter.
         * @param painter The painter that will draw to the pixmap.
         * @param backbuffer_rect_px Only draw geometries that are contained in the backbuffer rect (pixels).
         * @param controller_zoom The current controller zoom.
         */
        virtual void draw(QPainter* painter, const QRectF& backbuffer_rect_px, const int& controller_zoom) override;

        /*!
         * Draws the geometry's widget to the screen.
         * @param offset_px The offset in pixels to remove from the coordinate pixel point.
         * @param controller_zoom The current controller zoom.
         */
        void moveWidget(const QPointF& offset_px, const int& controller_zoom);

    protected:
        //! Constructor.
        /*!
         * This constructor creates a point which will display the given widget (Use GeometryPointWidget).
         * @note IMPORTANT: You have to set the QMapControl as parent to the widget before this is constructed.
         * @note IMPORTANT: You have to set the size of the widget before this is constructed.
         * @param point_coord The x/y coordinate (longitude/latitude).
         * @param widget The widget to be displayed by this point.
         * @param pen The pen to draw with.
         * @param zoom_minimum The minimum zoom level to show this geometry at.
         * @param zoom_maximum The maximum zoom level to show this geometry at.
         */
        GeometryPoint(const QPointF& point_coord, QWidget* widget, const QPen& pen = QPen(), const int& zoom_minimum = 0, const int& zoom_maximum = 17);

        /*!
         * Updates the pixmap.
         */
        virtual void updatePixmap();

    private:
        //! Disable copy constructor.
        GeometryPoint(const GeometryPoint&); /// @todo remove once MSVC supports default/delete syntax.

        //! Disable copy assignment.
        GeometryPoint& operator=(const GeometryPoint&); /// @todo remove once MSVC supports default/delete syntax.

        /*!
         * Calculates the geometry size (widget/pixmap) in pixels for the given controller zoom.
         * @param controller_zoom The current controller zoom.
         * @return the geometry size (widget/pixmap) in pixels.
         */
        QSizeF calculateGeometrySizePx(const int& controller_zoom) const;

        /*!
         * Calculates the top-left point in pixels after the alignment type has been applied.
         * @param point_px The point in pixels to align.
         * @param geometry_size_px The geometry object (widget/pixmap) at this zoom level.
         * @return the top-left point in pixels.
         */
        QPointF calculateTopLeftPoint(const QPointF& point_px, const QSizeF geometry_size_px) const;

    private:
        /// The x/y coordinate (longitude/latitude).
        QPointF m_point_coord;

        /// The associated widget to draw.
        QWidget* m_widget;

        /// The associated pixmap to draw.
        QPixmap m_pixmap;

        /// The alignment type to use when drawing.
        AlignmentType m_alignment_type;

        /// The base zoom level.
        int m_base_zoom;

        /// The base zoom size (must be set when widget/pixmap is set!).
        QSizeF m_base_size_px;

        /// The minimum size a point/widget/pixmap can be drawn in pixels.
        QSizeF m_draw_minimum_px;

        /// The maximum size a point/widget/pixmap can be drawn in pixels.
        QSizeF m_draw_maximum_px;

        /// The meta-data's key for the value to display.
        std::string m_metadata_displayed_key;

        /// The minimum zoom to display the meta-data value at.
        int m_metadata_displayed_zoom_minimum;

        /// The offset that the meta-data value is displayed from in pixels.
        double m_metadata_displayed_offset_px;
    };
}
