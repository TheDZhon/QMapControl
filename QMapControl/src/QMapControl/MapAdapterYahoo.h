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
#include "MapAdapterTile.h"

namespace qmapcontrol
{
    //! MapAdapter for Yahoo Maps.
    /*!
     * @author Kai Winter <kaiwinter@gmx.de>
     * @author Chris Stylianou <chris5287@gmail.com>
     */
    class QMAPCONTROL_EXPORT MapAdapterYahoo : public MapAdapterTile
    {
        Q_OBJECT
    public:
        //! Constructor.
        /*!
         * This construct a default Yahoo MapAdapter.
         * @param parent QObject parent ownership.
         */
        explicit MapAdapterYahoo(QObject* parent = 0);

        //! Constructor.
        /*!
         * This construct a Yahoo MapAdapter for a specific url.
         */
        explicit MapAdapterYahoo(const QUrl& url);

        //! Disable copy constructor.
        ///MapAdapterYahoo(const MapAdapterYahoo&) = delete; @todo re-add once MSVC supports default/delete syntax.

        //! Disable copy assignment.
        ///MapAdapterYahoo& operator=(const MapAdapterYahoo&) = delete; @todo re-add once MSVC supports default/delete syntax.

        //! Destructor.
        ~MapAdapterYahoo() { } /// = default; @todo re-add once MSVC supports default/delete syntax.

    private:
        //! Disable copy constructor.
        MapAdapterYahoo(const MapAdapterYahoo&); /// @todo remove once MSVC supports default/delete syntax.

        //! Disable copy assignment.
        MapAdapterYahoo& operator=(const MapAdapterYahoo&); /// @todo remove once MSVC supports default/delete syntax.
    };
}
