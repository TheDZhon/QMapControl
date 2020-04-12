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

#include "ESRIShapefile.h"

// Local includes.
#include "Projection.h"

#include <QDebug>

namespace qmapcontrol
{
    ESRIShapefile::ESRIShapefile(const std::string& file_path, const std::string& layer_name, const int& zoom_minimum, const int& zoom_maximum)
        : m_layer_name(layer_name), m_zoom_minimum(zoom_minimum), m_zoom_maximum(zoom_maximum)
    {
        // Register OGR drivers.
        OGRRegisterAll();

        // Open the file.
        m_ogr_data_set = OGRSFDriverRegistrar::Open(file_path.c_str(), FALSE);
    }

    ESRIShapefile::ESRIShapefile(OGRDataSource *datasource, const std::string &layer_name, const int &zoom_minimum, const int &zoom_maximum)
        : m_ogr_data_set(datasource), m_layer_name(layer_name), m_zoom_minimum(zoom_minimum), m_zoom_maximum(zoom_maximum)
    {
    }

    ESRIShapefile::~ESRIShapefile()
    {
        // Do we have a dataset open?
        if(m_ogr_data_set != nullptr)
        {
            // Close the data set.
            OGRDataSource::DestroyDataSource(m_ogr_data_set);
        }
    }

    const QPen& ESRIShapefile::getPenPolygon() const
    {
        // Do we have a pen?
        if(m_pen_polygon == nullptr)
        {
            // Create a default pen.
            m_pen_polygon = std::make_shared<QPen>();
        }

        // Get the pen to draw with.
        return *(m_pen_polygon.get());
    }

    void ESRIShapefile::setPenPolygon(const std::shared_ptr<QPen>& pen)
    {
        // Set the pen to draw with.
        m_pen_polygon = pen;

        // Emit that we need to redraw to display this change.
        emit requestRedraw();
    }

    void ESRIShapefile::setPenPolygon(const QPen& pen)
    {
        // Set the pen to draw with.
        m_pen_polygon = std::make_shared<QPen>(pen);

        // Emit that we need to redraw to display this change.
        emit requestRedraw();
    }

    const QBrush& ESRIShapefile::getBrushPolygon() const
    {
        // Do we have a brush?
        if(m_brush_polygon == nullptr)
        {
            // Create a default brush.
            m_brush_polygon = std::make_shared<QBrush>();
        }

        // Get the brush to draw with.
        return *(m_brush_polygon.get());
    }

    void ESRIShapefile::setBrushPolygon(const std::shared_ptr<QBrush>& brush)
    {
        // Set the brush to draw with.
        m_brush_polygon = brush;

        // Emit that we need to redraw to display this change.
        emit requestRedraw();
    }

    void ESRIShapefile::setBrushPolygon(const QBrush& brush)
    {
        // Set the brush to draw with.
        m_brush_polygon = std::make_shared<QBrush>(brush);

        // Emit that we need to redraw to display this change.
        emit requestRedraw();
    }

    const QPen& ESRIShapefile::getPenLineString() const
    {
        // Do we have a pen?
        if(m_pen_linestring == nullptr)
        {
            // Create a default pen.
            m_pen_linestring = std::make_shared<QPen>();
        }

        // Get the pen to draw with.
        return *(m_pen_linestring.get());
    }

    void ESRIShapefile::setPenLineString(const std::shared_ptr<QPen>& pen)
    {
        // Set the pen to draw with.
        m_pen_linestring = pen;

        // Emit that we need to redraw to display this change.
        emit requestRedraw();
    }

    void ESRIShapefile::setPenLineString(const QPen& pen)
    {
        // Set the pen to draw with.
        m_pen_linestring = std::make_shared<QPen>(pen);

        // Emit that we need to redraw to display this change.
        emit requestRedraw();
    }

    void ESRIShapefile::draw(QPainter& painter, const RectWorldPx& backbuffer_rect_px, const int& controller_zoom) const
    {
        // Check whether the controller zoom is within range?
        if(m_zoom_minimum > controller_zoom || m_zoom_maximum < controller_zoom)
        {
            // Outside of supported zoom levels.
        }
        else
        {
            // Calculate the world coordinates.
            const RectWorldCoord backbuffer_rect_coord(projection::get().toPointWorldCoord(backbuffer_rect_px.topLeftPx(), controller_zoom), projection::get().toPointWorldCoord(backbuffer_rect_px.bottomRightPx(), controller_zoom));

            // Do we have a data set open?
            if(m_ogr_data_set != nullptr)
            {
                // Do we have a layer name set?
                if(m_layer_name.empty() == false)
                {
                    // Get layer.
                    const auto ogr_layer(m_ogr_data_set->GetLayerByName(m_layer_name.c_str()));
                    if(ogr_layer == nullptr)
                    {
                        // Invalid layer name!
                    }
                    else
                    {
                        // Reset reading.
                        ogr_layer->ResetReading();

                        // Set the Spatial Filter.
                        ogr_layer->SetSpatialFilterRect(backbuffer_rect_coord.rawRect().left(), backbuffer_rect_coord.rawRect().top(), backbuffer_rect_coord.rawRect().right(), backbuffer_rect_coord.rawRect().bottom());

                        // Loop through features.
                        OGRFeature* ogr_feature;
                        while((ogr_feature = ogr_layer->GetNextFeature()) != nullptr)
                        {
                            // Draw the feature.
                            drawFeature(ogr_feature, painter, controller_zoom);

                            // Destroy the feature.
                            OGRFeature::DestroyFeature(ogr_feature);
                        }
                    }
                }
                else
                {
                    // Loop through and draw each layer.
                    for(int i = 0; i < m_ogr_data_set->GetLayerCount(); ++i)
                    {
                        // Get layer.
                        const auto ogr_layer(m_ogr_data_set->GetLayer(i));
                        if(ogr_layer != nullptr)
                        {
                            // Reset reading.
                            ogr_layer->ResetReading();

                            // Set the Spatial Filter.
                            ogr_layer->SetSpatialFilterRect(backbuffer_rect_coord.rawRect().left(), backbuffer_rect_coord.rawRect().top(), backbuffer_rect_coord.rawRect().right(), backbuffer_rect_coord.rawRect().bottom());

                            // Loop through features.
                            OGRFeature* ogr_feature;
                            while((ogr_feature = ogr_layer->GetNextFeature()) != nullptr)
                            {
                                // Draw the feature.
                                drawFeature(ogr_feature, painter, controller_zoom);

                                // Destroy the feature.
                                OGRFeature::DestroyFeature(ogr_feature);
                            }
                        }
                    }
                }
            }
        }
    }

    void ESRIShapefile::drawFeature(OGRFeature* ogr_feature, QPainter& painter, const int& controller_zoom) const
    {
        // Fetch geometries.
        const auto ogr_geometry(ogr_feature->GetGeometryRef());
        if(ogr_geometry == nullptr)
        {
            // No geometry to fetch!
        }
        // Is it a polygon.
        else if(wkbFlatten(ogr_geometry->getGeometryType()) == wkbPolygon)
        {
            // Cast to a polygon.
            const auto ogr_polygon(static_cast<OGRPolygon*>(ogr_geometry));

            // Fetch the exterior ring.
            const auto ogr_exterior_ring(ogr_polygon->getExteriorRing());
            if(ogr_exterior_ring == nullptr)
            {
                // No exterior ring!
            }
            else
            {
                QPainterPath path;

                // Prepare storage for point.
                OGRPoint ogr_point;

                // Create a polygon of the points.
                QPolygonF polygon_px;

                // Loop through the points.
                for(int i = 0; i < ogr_exterior_ring->getNumPoints(); ++i)
                {
                    // Fetch the point.
                    ogr_exterior_ring->getPoint(i, &ogr_point);

                    // Add the point to be drawn.
                    polygon_px.append(projection::get().toPointWorldPx(PointWorldCoord(ogr_point.getX(), ogr_point.getY()), controller_zoom).rawPoint());
                }

                path.addPolygon(polygon_px);

                QPainterPath inp;
                for (int i = 0; i < ogr_polygon->getNumInteriorRings(); ++i) {
                    auto inn = ogr_polygon->getInteriorRing(i);
                    QPolygonF pf;
                    for (int j = 0; j < inn->getNumPoints(); ++j) {
                        inn->getPoint(j, &ogr_point);
                        pf.append(projection::get().toPointWorldPx(PointWorldCoord(ogr_point.getX(), ogr_point.getY()), controller_zoom).rawPoint());
                    }
                    inp.addPolygon(pf);
                }

                path = path.subtracted(inp);

                // Set the pen to use.
                painter.setPen(getPenPolygon());

                // Set the brush to use.
                painter.setBrush(getBrushPolygon());

                // Draw the polygon line.
                painter.drawPath(path);
            }
        }
        else if(wkbFlatten(ogr_geometry->getGeometryType()) == wkbMultiPolygon)
        {
            qDebug() << "MultiPoly";
            // Cast to a multi polygon.
            const auto ogr_multi_polygon(static_cast<OGRMultiPolygon*>(ogr_geometry));
            if(ogr_multi_polygon == nullptr)
            {
                // No multi polygons!
            }
            else
            {
                QPainterPath path;

                // Loop through each polygon.
                for(int i = 0; i < ogr_multi_polygon->getNumGeometries(); ++i)
                {
                    // Cast to a polygon.
                    const auto ogr_polygon(static_cast<OGRPolygon*>(ogr_multi_polygon->getGeometryRef(i)));

                    // Fetch the exterior ring.
                    const auto ogr_exterior_ring(ogr_polygon->getExteriorRing());
                    if(ogr_exterior_ring == nullptr)
                    {
                        // No exterior ring!
                    }
                    else
                    {
                        // Prepare storage for point.
                        OGRPoint ogr_point;

                        // Create a polygon of the points.
                        QPolygonF polygon_px;

                        // Loop through the points.
                        for(int i = 0; i < ogr_exterior_ring->getNumPoints(); ++i)
                        {
                            // Fetch the point.
                            ogr_exterior_ring->getPoint(i, &ogr_point);

                            // Add the point to be drawn.
                            polygon_px.append(projection::get().toPointWorldPx(PointWorldCoord(ogr_point.getX(), ogr_point.getY()), controller_zoom).rawPoint());
                        }

                        path.addPolygon(polygon_px);

                        QPainterPath inp;
                        for (int i = 0; i < ogr_polygon->getNumInteriorRings(); ++i) {
                            auto inn = ogr_polygon->getInteriorRing(i);
                            QPolygonF pf;
                            for (int j = 0; j < inn->getNumPoints(); ++j) {
                                inn->getPoint(j, &ogr_point);
                                pf.append(projection::get().toPointWorldPx(PointWorldCoord(ogr_point.getX(), ogr_point.getY()), controller_zoom).rawPoint());
                            }
                            inp.addPolygon(pf);
                        }

                        path = path.subtracted(inp);

                    }
                }

                // Set the pen to use.
                painter.setPen(getPenPolygon());

                // Set the brush to use.
                painter.setBrush(getBrushPolygon());

                // Draw the polygon line.
                painter.drawPath(path);

            }
        }
        else if(wkbFlatten(ogr_geometry->getGeometryType()) == wkbLineString) // wkbLineString
        {
            // Cast to a line string.
            const auto ogr_line_string(static_cast<OGRLineString*>(ogr_geometry));

            // Prepare storage for point.
            OGRPoint ogr_point;

            // Create a polygon of the points.
            QPolygonF polygon_line_px;

            // Loop through the points.
            for(int i = 0; i < ogr_line_string->getNumPoints(); ++i)
            {
                // Fetch the point.
                ogr_line_string->getPoint(i, &ogr_point);

                // Add the point to be drawn.
                polygon_line_px.append(projection::get().toPointWorldPx(PointWorldCoord(ogr_point.getX(), ogr_point.getY()), controller_zoom).rawPoint());
            }

            // Set the pen to use.
            painter.setPen(getPenLineString());

            // Draw the polygon line.
            painter.drawPolyline(polygon_line_px);
        }
    }
}
