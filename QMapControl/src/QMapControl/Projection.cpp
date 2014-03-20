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

#include "Projection.h"

// STL includes.
#include <memory>

// Local includes.
#include "ProjectionEquirectangular.h"
#include "ProjectionSphericalMercator.h"

namespace qmapcontrol
{
    namespace
    {
        /// Singleton instance of Projection Interface.
        std::unique_ptr<Projection> m_instance = nullptr;
    }

    Projection& projection::get()
    {
        // Does the singleton instance exist?
        if(m_instance == nullptr)
        {
            // Create a default instance (Spherical Mercator).
            set(EPSG::SphericalMercator);
        }

        // Return the reference to the instance object.
        return *(m_instance.get());
    }

    void projection::set(const EPSG& type)
    {
        // Equirectangular ?
        if(type == EPSG::Equirectangular)
        {
            // Create a Equirectangular instance.
            m_instance.reset(new ProjectionEquirectangular);
        }
        else
        {
            // Default to a Spherical Mercator instance.
            m_instance.reset(new ProjectionSphericalMercator);
        }
    }
}
