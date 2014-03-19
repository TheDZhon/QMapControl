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
#include "GeometryPoint.h"

namespace qmapcontrol
{
    //! A geometric point to draw widgets into maps.
    /*!
    * This class can be used to draw your QWidgets into maps.
    *
    * But keep in mind, that widgets always are drawn on top of all layers.
    * You also have to handle click events yourself.
    *
    * @author Kai Winter <kaiwinter@gmx.de>
    * @author Chris Stylianou <chris5287@gmail.com>
    */
    class QMAPCONTROL_EXPORT GeometryPointWidget : public GeometryPoint
    {
        Q_OBJECT
    public:
        //! Constructor.
        /*!
        * This constructor creates a point which will display the given widget.
        * @note IMPORTANT: You have to set the QMapControl as parent to the widget before this is constructed.
        * @note IMPORTANT: You have to set the size of the widget before this is constructed.
        * @param x_coord The x coordinate (longitude).
        * @param y_coord The y coordinate (latitude).
        * @param widget The widget to be displayed by this point.
        * @param pen The pen to draw with.
        * @param zoom_minimum The minimum zoom level to show this geometry at.
        * @param zoom_maximum The maximum zoom level to show this geometry at.
        */
        GeometryPointWidget(const qreal& x_coord, const qreal& y_coord, QWidget* widget, const QPen& pen = QPen(), const int& zoom_minimum = 0, const int& zoom_maximum = 17);

        //! Constructor.
        /*!
        * This constructor creates a point which will display the given widget.
        * @note IMPORTANT: You have to set the QMapControl as parent to the widget before this is constructed.
        * @note IMPORTANT: You have to set the size of the widget before this is constructed.
        * @param point_coord The x/y coordinate (longitude/latitude).
        * @param widget The widget to be displayed by this point.
        * @param pen The pen to draw with.
        * @param zoom_minimum The minimum zoom level to show this geometry at.
        * @param zoom_maximum The maximum zoom level to show this geometry at.
        */
        GeometryPointWidget(const QPointF& point_coord, QWidget* widget, const QPen& pen = QPen(), const int& zoom_minimum = 0, const int& zoom_maximum = 17);

        //! Disable copy constructor.
        ///GeometryPointWidget(const GeometryPointWidget&) = delete; @todo re-add once MSVC supports default/delete syntax.

        //! Disable copy assignment.
        ///GeometryPointWidget& operator=(const GeometryPointWidget&) = delete; @todo re-add once MSVC supports default/delete syntax.

        //! Destructor.
        virtual ~GeometryPointWidget() { } /// = default; @todo re-add once MSVC supports default/delete syntax.

    private:
        //! Disable copy constructor.
        GeometryPointWidget(const GeometryPointWidget&); /// @todo remove once MSVC supports default/delete syntax.

        //! Disable copy assignment.
        GeometryPointWidget& operator=(const GeometryPointWidget&); /// @todo remove once MSVC supports default/delete syntax.
    };
}
