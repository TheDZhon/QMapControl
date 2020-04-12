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
#include <QtCore/QReadWriteLock>

// STL includes.
#include <memory>

// Local includes.
#include "qmapcontrol_global.h"
#include "ESRIShapefile.h"
#include "Layer.h"

namespace qmapcontrol
{
    //! Layer class
    /*!
     * Layer that can display a ESRI Shapefile.
     *
     * @author Chris Stylianou <chris5287@gmail.com>
     */
    class QMAPCONTROL_EXPORT LayerESRIShapefile : public Layer
    {
        Q_OBJECT
    public:
        //! Layer constructor
        /*!
         * This is used to construct a layer.
         * @param name The name of the layer.
         * @param zoom_minimum The minimum zoom level to show this geometry at.
         * @param zoom_maximum The maximum zoom level to show this geometry at.
         * @param parent QObject parent ownership.
         */
        LayerESRIShapefile(const std::string& name, const int& zoom_minimum = 0, const int& zoom_maximum = 17, QObject* parent = 0);

        //! Disable copy constructor.
        ///LayerESRIShapefile(const LayerESRIShapefile&) = delete; @todo re-add once MSVC supports default/delete syntax.

        //! Disable copy assignment.
        ///LayerESRIShapefile& operator=(const LayerESRIShapefile&) = delete; @todo re-add once MSVC supports default/delete syntax.

        //! Destructor.
        virtual ~LayerESRIShapefile() { } /// = default; @todo re-add once MSVC supports default/delete syntax.

        /*!
         * Adds an ESRI Shapefile object to this Layer.
         * @param esri_shapefile The ESRI Shapefile to add.
         * @param disable_redraw Whether to disable the redraw call after the ESRI Shapefile is added.
         */
        void addESRIShapefile(const std::shared_ptr<ESRIShapefile>& esri_shapefile, const bool& disable_redraw = false);

        /*!
         * Removes an ESRI Shapefile object from this Layer.
         * @param esri_shapefile The ESRI Shapefile to remove.
         * @param disable_redraw Whether to disable the redraw call after the ESRI Shapefile is removed.
         */
        void removeESRIShapefile(const std::shared_ptr<ESRIShapefile>& esri_shapefile, const bool& disable_redraw = false);

        /*!
         * Removes all ESRI Shapefile objects from this Layer.
         * @param disable_redraw Whether to disable the redraw call after all ESRI Shapefiles are removed.
         */
        void clearESRIShapefiles(const bool& disable_redraw = false);

        /*!
         * Handles mouse press events (such as left-clicking an item on the layer).
         * @param mouse_event The mouse event.
         * @param mouse_point_coord The mouse point on the map in coord.
         * @param controller_zoom The current controller zoom.
         */
        bool mousePressEvent(const QMouseEvent* mouse_event, const PointWorldCoord& mouse_point_coord, const int& controller_zoom) const final;

        /*!
         * Draws each map adapter and geometry to a pixmap using the provided painter.
         * @param painter The painter that will draw to the pixmap.
         * @param backbuffer_rect_px Only draw map tiles/geometries that are contained in the backbuffer rect (pixels).
         * @param controller_zoom The current controller zoom.
         */
        void draw(QPainter& painter, const RectWorldPx& backbuffer_rect_px, const int& controller_zoom) const final;


        int getShapefileCount() const { return m_esri_shapefiles.size(); }
        std::shared_ptr<ESRIShapefile> getShapefile(int idx) const;

    private:
        /// List of ESRI Shapefiles draw by this layer.
        std::vector<std::shared_ptr<ESRIShapefile>> m_esri_shapefiles;

        /// Mutex to protect ESRI Shapefiles.
        mutable QReadWriteLock m_esri_shapefiles_mutex;
    };
}
