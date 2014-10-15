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

#include "LayerMapAdapter.h"

// STL includes.
#include <cmath>

// Local includes.
#include "ImageManager.h"

namespace qmapcontrol
{
    LayerMapAdapter::LayerMapAdapter(const std::string& name, const std::shared_ptr<MapAdapter>& mapadapter, const int& zoom_minimum, const int& zoom_maximum, QObject* parent)
        : Layer(LayerType::LayerMapAdapter, name, zoom_minimum, zoom_maximum, parent),
          m_mapadapter(mapadapter)
    {

    }

    const std::shared_ptr<MapAdapter> LayerMapAdapter::getMapAdapter() const
    {
        // Gain a read lock to protect the map adapter.
        QReadLocker locker(&m_mapadapter_mutex);

        // Return the map adapter.
        return m_mapadapter;
    }

    void LayerMapAdapter::setMapAdapter(const std::shared_ptr<MapAdapter>& mapadapter)
    {
        // Scope the locker to ensure the mutex is release as soon as possible.
        {
            // Gain a write lock to protect the map adapter.
            QWriteLocker locker(&m_mapadapter_mutex);

            // Set the map adapter.
            m_mapadapter = mapadapter;
        }

        // Emit to redraw layer.
        emit requestRedraw();
    }

    bool LayerMapAdapter::mousePressEvent(const QMouseEvent* /*mouse_event*/, const PointWorldCoord& /*mouse_point_coord*/, const int& /*controller_zoom*/) const
    {
        // Do nothing.
        return false;
    }

    void LayerMapAdapter::draw(QPainter& painter, const RectWorldPx& backbuffer_rect_px, const int& controller_zoom) const
    {
        // Gain a read lock to protect the map adapter.
        QReadLocker locker(&m_mapadapter_mutex);

        // Check the layer is visible and a map adapter is set.
        if(isVisible(controller_zoom) && m_mapadapter != nullptr)
        {
            // Check we have a base url to fetch map!
            if(m_mapadapter->getBaseUrl().isEmpty())
            {
                // We cannot fetch map tiles as no base url set!
                qDebug() << "Map adapater base url is empty!";
            }
            else
            {
                // The current tile size.
                const QSizeF tile_size_px(ImageManager::get().tileSizePx(), ImageManager::get().tileSizePx());

                // Calculate the tiles to draw.
                const int furthest_tile_left = std::floor(backbuffer_rect_px.leftPx() / tile_size_px.width());
                const int furthest_tile_top = std::floor(backbuffer_rect_px.topPx() / tile_size_px.height());
                const int furthest_tile_right = std::floor(backbuffer_rect_px.rightPx() / tile_size_px.width());
                const int furthest_tile_bottom = std::floor(backbuffer_rect_px.bottomPx() / tile_size_px.height());

                // Loop through the tiles to draw (left to right).
                for(int i = furthest_tile_left; i <= furthest_tile_right; ++i)
                {
                    // Loop through the tiles to draw (top to bottom).
                    for(int j = furthest_tile_top; j <= furthest_tile_bottom; ++j)
                    {
                        // Check the tile is valid.
                        if(m_mapadapter->isTileValid(i, j, controller_zoom))
                        {
                            // Calculate the top left point.
                            const PointWorldPx top_left_px(i * tile_size_px.width(), j * tile_size_px.height());

                            // Draw the tile.
                            painter.drawPixmap(top_left_px.rawPoint(), ImageManager::get().getImage(m_mapadapter->tileQuery(i, j, controller_zoom)));
                        }
                    }
                }

                // Prefetch the next set of rows/column tiles (ready for when the user starts panning).
                const int prefetch_tile_left = furthest_tile_left - 1;
                const int prefetch_tile_top = furthest_tile_top - 1;
                const int prefetch_tile_right = furthest_tile_right + 1;
                const int prefetch_tile_bottom = furthest_tile_bottom + 1;

                // Fetch the top/bottom rows.
                for (int i = prefetch_tile_left; i <= prefetch_tile_right; ++i)
                {
                    // Top row - check the tile is valid.
                    if(m_mapadapter->isTileValid(i, prefetch_tile_top, controller_zoom))
                    {
                        // Prefetch the tile.
                        ImageManager::get().prefetchImage(m_mapadapter->tileQuery(i, prefetch_tile_top, controller_zoom));
                    }

                    // Bottom row - check the tile is valid.
                    if(m_mapadapter->isTileValid(i, prefetch_tile_bottom, controller_zoom))
                    {
                        // Prefetch the tile.
                        ImageManager::get().prefetchImage(m_mapadapter->tileQuery(i, prefetch_tile_bottom, controller_zoom));
                    }
                }

                // Fetch the left/right columns.
                for (int j = prefetch_tile_top; j <= prefetch_tile_bottom; ++j)
                {
                    // Left column - check the tile is valid.
                    if(m_mapadapter->isTileValid(prefetch_tile_left, j, controller_zoom))
                    {
                        // Prefetch the tile.
                        ImageManager::get().prefetchImage(m_mapadapter->tileQuery(prefetch_tile_left, j, controller_zoom));
                    }

                    // Right column - check the tile is valid.
                    if(m_mapadapter->isTileValid(prefetch_tile_right, j, controller_zoom))
                    {
                        // Prefetch the tile.
                        ImageManager::get().prefetchImage(m_mapadapter->tileQuery(prefetch_tile_right, j, controller_zoom));
                    }
                }
            }
        }
    }
}
