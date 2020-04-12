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

#include "LayerESRIShapefile.h"

namespace qmapcontrol
{
    LayerESRIShapefile::LayerESRIShapefile(const std::string& name, const int& zoom_minimum, const int& zoom_maximum, QObject* parent)
        : Layer(LayerType::LayerESRIShapefile, name, zoom_minimum, zoom_maximum, parent)
    {
        /// @todo set colours for polygon/linestring/point?!?!
    }

    void LayerESRIShapefile::addESRIShapefile(const std::shared_ptr<ESRIShapefile>& esri_shapefile, const bool& disable_redraw)
    {
        // Check the ESRI Shapefile is valid.
        if(esri_shapefile != nullptr)
        {
            // Gain a write lock to protect the ESRI Shapefiles.
            QWriteLocker locker(&m_esri_shapefiles_mutex);

            // Find the object in the container.
            const auto itr_find(std::find(m_esri_shapefiles.begin(), m_esri_shapefiles.end(), esri_shapefile));
            if(itr_find != m_esri_shapefiles.end())
            {
                // ESRI Shapefile already been added!
            }
            else
            {
                // Add the ESRI Shapefile.
                m_esri_shapefiles.push_back(esri_shapefile);

                // Should we redraw?
                if(disable_redraw == false)
                {
                    // Emit to redraw layer.
                    emit requestRedraw();
                }

                // ESRI Shapefile can request a redraw, e.g. when its position has been changed.
                // Connect the redraw signal to promulgate up as required.
                QObject::connect(esri_shapefile.get(), &ESRIShapefile::requestRedraw, this, &Layer::requestRedraw);
            }
        }
    }

    void LayerESRIShapefile::removeESRIShapefile(const std::shared_ptr<ESRIShapefile>& esri_shapefile, const bool& disable_redraw)
    {
        // Check the ESRI Shapefile is valid.
        if(esri_shapefile != nullptr)
        {
            // Gain a write lock to protect the ESRI Shapefiles.
            QWriteLocker locker(&m_esri_shapefiles_mutex);

            // Disconnect any signals that were previously connected.
            QObject::disconnect(esri_shapefile.get(), 0, this, 0);

            // Find the object in the container.
            const auto itr_find(std::find(m_esri_shapefiles.begin(), m_esri_shapefiles.end(), esri_shapefile));
            if(itr_find == m_esri_shapefiles.end())
            {
                // ESRI Shapefile does not exist.
            }
            else
            {
                // Remove the object.
                m_esri_shapefiles.erase(itr_find);
            }

            // Should we redraw?
            if(disable_redraw == false)
            {
                // Emit to redraw layer.
                emit requestRedraw();
            }
        }
    }

    void LayerESRIShapefile::clearESRIShapefiles(const bool& disable_redraw)
    {
        // Gain a write lock to protect the ESRI Shapefiles.
        QWriteLocker locker(&m_esri_shapefiles_mutex);

        // Remove all ESRI Shapefiles from the list.
        m_esri_shapefiles.clear();

        // Should we redraw?
        if(disable_redraw == false)
        {
            // Emit to redraw layer.
            emit requestRedraw();
        }
    }

    bool LayerESRIShapefile::mousePressEvent(const QMouseEvent* /*mouse_event*/, const PointWorldCoord& /*mouse_point_coord*/, const int& /*controller_zoom*/) const
    {
        // Do Nothing...
        return false;
    }

    void LayerESRIShapefile::draw(QPainter& painter, const RectWorldPx& backbuffer_rect_px, const int& controller_zoom) const
    {
        // Gain a read lock to protect the ESRI Shapefiles.
        QReadLocker locker(&m_esri_shapefiles_mutex);

        // Check the layer is visible.
        if(isVisible(controller_zoom))
        {
            // Loop through each ESRI Shapefile.
            for(const auto& esri_shapefile : m_esri_shapefiles)
            {
                // Save the current painter's state.
                painter.save();

                // Draw the ESRI Shapefile.
                esri_shapefile->draw(painter, backbuffer_rect_px, controller_zoom);

                // Restore the painter's state.
                painter.restore();
            }
        }
    }

    std::shared_ptr<ESRIShapefile> LayerESRIShapefile::getShapefile(int idx) const
    {
        QReadLocker lock(&m_esri_shapefiles_mutex);
        return m_esri_shapefiles[idx];
    }
}
