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

#include "Layer.h"

namespace qmapcontrol
{
    Layer::Layer(const LayerType& layer_type, const std::string& name, const int& zoom_minimum, const int& zoom_maximum, QObject* parent)
        : QObject(parent),
          m_layer_type(layer_type),
          m_visible(true),
          m_mouse_events_enabled(true),
          m_name(name),
          m_zoom_minimum(zoom_minimum),
          m_zoom_maximum(zoom_maximum)
    {

    }

    Layer::LayerType Layer::getLayerType() const
    {
        // Return the layer type.
        return m_layer_type;
    }

    std::string Layer::getName() const
    {
        // Return the layer's name.
        return m_name;
    }

    QVariant Layer::getMetadata(const std::string& key) const
    {
        // Default return value.
        QVariant return_value;

        // Find the key.
        const auto find_itr = m_metadata.find(key);
        if(find_itr != m_metadata.end())
        {
            // Fetch the value.
            return_value = find_itr->second;
        }

        // Return the value.
        return return_value;
    }

    void Layer::setMetadata(const std::string& key, const QVariant& value)
    {
        // Set the meta-data.
        m_metadata[key] = value;
    }

    bool Layer::isVisible(const int& controller_zoom) const
    {
        // Default visibility.
        bool visibility(m_visible);

        // Check whether the controller zoom is within range.
        if(m_zoom_minimum > controller_zoom || m_zoom_maximum < controller_zoom)
        {
            // Outside of supported zoom levels.
            visibility = false;
        }

        // Return the visibility.
        return visibility;
    }

    void Layer::setVisible(const bool& visible)
    {
        // Set the visibility.
        m_visible = visible;

        // Emit to redraw layer.
        emit requestRedraw();
    }

    bool Layer::isMouseEventsEnabled() const
    {
        // Return the enabled value.
        return m_mouse_events_enabled;
    }

    void Layer::setMouseEventsEnabled(const bool& enable)
    {
        // Set whether to enable mouse events.
        m_mouse_events_enabled = enable;
    }
}
