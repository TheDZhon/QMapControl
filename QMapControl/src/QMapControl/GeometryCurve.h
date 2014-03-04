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

// Local includes.
#include "qmapcontrol_global.h"
#include "Geometry.h"

namespace qmapcontrol
{
    //! A Curve Geometry, implemented to fullfil OGC Spec
    /*!
     * The Curve class is used by LineString as parent class.
     * This class could not be used directly.
     *
     * From the OGC Candidate Implementation Specification:
     * "A Curve is a 1-dimensional geometric object usually stored as a sequence of Points, with the subtype of Curve
     * specifying the form of the interpolation between Points. This specification defines only one subclass of Curve,
     * LineString, which uses a linear interpolation between Points."
     *
     * @author Kai Winter <kaiwinter@gmx.de>
     * @author Chris Stylianou <chris5287@gmail.com>
     */
    class QMAPCONTROL_EXPORT GeometryCurve : public Geometry
    {
        Q_OBJECT
    public:
        //! Disable copy constructor.
        ///GeometryCurve(const GeometryCurve&) = delete; @todo re-add once MSVC supports default/delete syntax.

        //! Disable copy assignment.
        ///GeometryCurve& operator=(const GeometryCurve&) = delete; @todo re-add once MSVC supports default/delete syntax.

        //! Destructor.
        virtual ~GeometryCurve() { } /// = default; @todo re-add once MSVC supports default/delete syntax.

    protected:
        //! Constructor.
        /*!
         * This construct a GeometryCurve.
         * @param geometry_type The geometry type.
         * @param pen The pen to draw with.
         * @param zoom_minimum The minimum zoom level to show this geometry at.
         * @param zoom_maximum The maximum zoom level to show this geometry at.
         */
        GeometryCurve(const GeometryType& geometry_type, const QPen& pen = QPen(), const int& zoom_minimum = 0, const int& zoom_maximum = 17);

    private:
        //! Disable copy constructor.
        GeometryCurve(const GeometryCurve&); /// @todo remove once MSVC supports default/delete syntax.

        //! Disable copy assignment.
        GeometryCurve& operator=(const GeometryCurve&); /// @todo remove once MSVC supports default/delete syntax.

    };
}
