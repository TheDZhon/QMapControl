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

#include "GPS_Position.h"

namespace qmapcontrol
{
    GPS_Position::GPS_Position(const qreal& time, const qreal& longitude_coord, const std::string& longitude_direction, const qreal& latitude_coord, const std::string& latitude_direction)
        : m_time(time),
          m_longitude_coord(longitude_coord),
          m_longitude_direction(longitude_direction),
          m_latitude_coord(latitude_coord),
          m_latitude_direction(latitude_direction)
    {

    }

    qreal GPS_Position::getTime() const
    {
        // Return the time.
        return m_time;
    }

    qreal GPS_Position::getLongitudeCoord() const
    {
        // Return the longitude coord.
        return m_longitude_coord;
    }

    std::string GPS_Position::getLongitudeDirection() const
    {
        // Return the longitude direction.
        return m_longitude_direction;
    }

    qreal GPS_Position::getLatitudeCoord() const
    {
        // Return the latitude coord.
        return m_latitude_coord;
    }

    std::string GPS_Position::getLatitudeDirection() const
    {
        // Return the latitude direction.
        return m_latitude_direction;
    }
}
