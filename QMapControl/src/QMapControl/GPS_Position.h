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
#include <QString>

namespace qmapcontrol
{
    //! Represents a coordinate from a GPS receiver
    /*!
     * This class is used to represent a coordinate which has been parsed from a NMEA string.
     * This is not fully integrated in the API. An example which uses this data type can be found under Samples.
     *
     * @author Kai Winter <kaiwinter@gmx.de>
     * @author Chris Stylianou <chris5287@gmail.com>
     */
    class QMAPCONTROL_EXPORT GPS_Position
    {
    public:
        //! Constructor.
        /*!
         * This constructor a GPS Position.
         * @param time The time of the position.
         * @param longitude_coord The longitude of the position.
         * @param longitude_direction The longitude direction (E/W).
         * @param latitude_coord The latitude of the position.
         * @param latitude_direction The latitude direction (N/S).
         */
        GPS_Position(const qreal& time, const qreal& longitude_coord, const std::string& longitude_direction, const qreal& latitude_coord, const std::string& latitude_direction);

//        //! Copy constructor.
//        GPS_Position(const GPS_Position& other) = default; @todo re-add once MSVC supports default/delete syntax.

//        //! Copy assignment.
//        GPS_Position& operator=(const GPS_Position& other) = default; @todo re-add once MSVC supports default/delete syntax.

        //! Destructor.
        virtual ~GPS_Position() { } /// = default; @todo re-add once MSVC supports default/delete syntax.

        /*!
         * Fetches the time.
         * @return the time.
         */
        qreal getTime() const;

        /*!
         * Fetches the longitude coord.
         * @return the longitude coord.
         */
        qreal getLongitudeCoord() const;

        /*!
         * Fetches the longitude direction.
         * @return the longitude direction.
         */
        std::string getLongitudeDirection() const;

        /*!
         * Fetches the latitude coord.
         * @return the latitude coord.
         */
        qreal getLatitudeCoord() const;

        /*!
         * Fetches the latitude direction.
         * @return the latitude direction.
         */
        std::string getLatitudeDirection() const;

    private:
        /// Time of the string.
        qreal m_time;

        /// Longitude coordinate.
        qreal m_longitude_coord;

        /// Longitude direction (E/W).
        std::string m_longitude_direction;

        /// Latitude coordinate.
        qreal m_latitude_coord;

        /// Latitude direction (N/S).
        std::string m_latitude_direction;
    };
}
