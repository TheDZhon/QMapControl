#include "GeometryPoint.h"

// Local includes.
#include "GeometryPolygon.h"
#include "Projection.h"

namespace qmapcontrol
{
    GeometryPoint::GeometryPoint(const qreal& longitude, const qreal& latitude, const int& zoom_minimum, const int& zoom_maximum)
        : Geometry(Geometry::GeometryType::GeometryPoint, zoom_minimum, zoom_maximum),
          m_point_coord(PointWorldCoord(longitude, latitude))
    {

    }

    GeometryPoint::GeometryPoint(const PointWorldCoord& point_coord, const int& zoom_minimum, const int& zoom_maximum)
        : Geometry(Geometry::GeometryType::GeometryPoint, zoom_minimum, zoom_maximum),
          m_point_coord(point_coord)
    {

    }

    const PointWorldCoord& GeometryPoint::coord() const
    {
        // Return the point to be displayed.
        return m_point_coord;
    }

    void GeometryPoint::setCoord(const PointWorldCoord& point)
    {
        // Has the point change?
        if(m_point_coord != point)
        {
            // Set the new point.
            m_point_coord = point;

            // Emit that we need to redraw to display this change.
            emit requestRedraw();

            // Emit that the position has changed.
            emit positionChanged(this);
        }
    }

    RectWorldCoord GeometryPoint::boundingBox(const int& controller_zoom) const
    {
        // Calculate the world point in pixels.
        const PointWorldPx point_px(projection::get().toPointWorldPx(m_point_coord, controller_zoom));

        // Add 'fuzzy-factor' around point.
        /// @TODO expose the fuzzy factor as a setting?
        const QSizeF object_size_px(1.0, 1.0);

        // Calculate the top-left point.
        const PointWorldPx top_left_point_px(calculateTopLeftPoint(point_px, AlignmentType::Middle, object_size_px));

        // Calculate the bottom-right point.
        const PointWorldPx bottom_right_point_px(top_left_point_px.x() + object_size_px.width(), top_left_point_px.y() + object_size_px.height());

        // Return the converted coord points.
        return RectWorldCoord(projection::get().toPointWorldCoord(top_left_point_px, controller_zoom), projection::get().toPointWorldCoord(bottom_right_point_px, controller_zoom));
    }

    bool GeometryPoint::touches(const Geometry* geometry, const int& controller_zoom) const
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
                    // Check if the bounding boxes intersect.
                    if(geometry->boundingBox(controller_zoom).rawRect().intersects(boundingBox(controller_zoom).rawRect()))
                    {
                        // Set that we have touched.
                        return_touches = true;
                    }

                    // Finished.
                    break;
                }
                case GeometryType::GeometryPolygon:
                {
                    // Check if the poylgon intersects with our bounding box.
                    if(static_cast<const GeometryPolygon*>(geometry)->toQPolygonF().intersected(boundingBox(controller_zoom).rawRect()).empty() == false)
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

    void GeometryPoint::draw(QPainter& painter, const RectWorldCoord& backbuffer_rect_coord, const int& controller_zoom)
    {
        // Check the geometry is visible.
        if(isVisible(controller_zoom))
        {
            // Is the point within the backbuffer rect?
            if(backbuffer_rect_coord.rawRect().contains(m_point_coord.rawPoint()))
            {
                // Calculate the point in pixels.
                const PointWorldPx point_px(projection::get().toPointWorldPx(m_point_coord, controller_zoom));

                // Set the pen to use.
                painter.setPen(pen());

                // Draw the point.
                painter.drawPoint(point_px.rawPoint());

                // Do we have a meta-data value and should we display it at this zoom?
                if(controller_zoom >= m_metadata_displayed_zoom_minimum && metadata(m_metadata_displayed_key).isNull() == false)
                {
                    /// @todo calculate correct alignment for metadata displayed offset.

                    // Draw the text next to the point with an offset.
                    painter.drawText((point_px + PointPx(m_metadata_displayed_alignment_offset_px, -m_metadata_displayed_alignment_offset_px)).rawPoint(), metadata(m_metadata_displayed_key).toString());
                }
            }
        }
    }

}
