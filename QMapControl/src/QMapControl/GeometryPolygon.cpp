#include "GeometryPolygon.h"

// Local includes.
#include "Projection.h"

namespace qmapcontrol
{
    GeometryPolygon::GeometryPolygon(const std::vector<PointWorldCoord>& points, const int& zoom_minimum, const int& zoom_maximum)
        : Geometry(Geometry::GeometryType::GeometryPolygon, zoom_minimum, zoom_maximum),
          m_points(points)
    {

    }

    std::vector<PointWorldCoord> GeometryPolygon::points() const
    {
        // Return the points.
        return m_points;
    }

    void GeometryPolygon::setPoints(const std::vector<PointWorldCoord>& points, const bool& disable_redraw)
    {
        // Set the new points.
        m_points = points;

        // Should we redraw?
        if(disable_redraw == false)
        {
            // Emit to redraw to display this change.
            emit requestRedraw();
        }
    }

    const QPolygonF GeometryPolygon::toQPolygonF() const
    {
        // The QPolygonF to return.
        QPolygonF return_polygon;

        // Loop through each point to add it to the QPolygonF.
        for(const auto& point : m_points)
        {
            // Add the point.
            return_polygon.append(point.rawPoint());
        }

        // Return the QPolygonF.
        return return_polygon;
    }

    RectWorldCoord GeometryPolygon::boundingBox(const int& /*controller_zoom*/) const
    {
        // Create a polygon of the points.
        QPolygonF polygon;

        // Loop through each point to add to the polygon.
        for(const auto& point : m_points)
        {
            // Add the point to be drawn.
            polygon.append(point.rawPoint());
        }

        // Return the bounding box.
        return RectWorldCoord::fromQRectF(polygon.boundingRect());
    }

    bool GeometryPolygon::touches(const Geometry* geometry, const int& controller_zoom) const
    {
        // Default return success.
        bool return_touches(false);

        // Check we are visible and the geometry to compare against is valid.
        if(isVisible(controller_zoom) && geometry != nullptr)
        {
            // Switch to the correct geometry type.
            switch(geometry->geometryType())
            {
                case GeometryType::GeometryLineString:
                {
                    /// @todo Line String calculation.

                    // Finished.
                    break;
                }
                case GeometryType::GeometryPoint:
                case GeometryType::GeometryWidget:
                {
                    // Check if the polygon (bounding box) intersects with our polygon.
                    if(QPolygonF(geometry->boundingBox(controller_zoom).rawRect()).intersected(toQPolygonF()).empty() == false)
                    {
                        // Set that we have touched.
                        return_touches = true;
                    }

                    // Finished.
                    break;
                }
                case GeometryType::GeometryPolygon:
                {
                    // Check if the poylgons intersect.
                    if(static_cast<const GeometryPolygon*>(geometry)->toQPolygonF().intersected(toQPolygonF()).empty() == false)
                    {
                        // Set that we have touched.
                        return_touches = true;
                    }

                    // Finished.
                    break;
                }
            }

            // Have we touched?
            if(return_touches)
            {
                // Emit that the geometry has been clicked.
                emit geometryClicked(this);
            }
        }

        // Return our success.
        return return_touches;
    }

    void GeometryPolygon::draw(QPainter& painter, const RectWorldCoord& backbuffer_rect_coord, const int& controller_zoom)
    {
        // Check the geometry is visible.
        if(isVisible(controller_zoom))
        {
            // Does the polygon intersect with the backbuffer rect?
            if(QPolygonF(backbuffer_rect_coord.rawRect()).intersected(toQPolygonF()).empty() == false)
            {
                // Create a polygon of the points.
                QPolygonF polygon;

                // Loop through each point to add to the polygon.
                for(const auto& point : m_points)
                {
                    // Add the point to be drawn.
                    polygon.append(projection::get().toPointWorldPx(point, controller_zoom).rawPoint());
                }

                // Set the pen to use.
                painter.setPen(pen());

                // Set the brush to use.
                painter.setBrush(brush());

                // Draw the polygon line.
                painter.drawPolygon(polygon);
            }
        }
    }
}
