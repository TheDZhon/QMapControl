#include "GeometryPointShape.h"

// Local includes.
#include "Projection.h"

namespace qmapcontrol
{
    GeometryPointShape::GeometryPointShape(const qreal& longitude, const qreal& latitude, const QSizeF& size_px, const int& zoom_minimum, const int& zoom_maximum)
        : GeometryPoint(PointWorldCoord(longitude, latitude), zoom_minimum, zoom_maximum),
          m_size_px(size_px),
          m_alignment_type(AlignmentType::Middle),
          m_rotation(0.0)
    {

    }

    GeometryPointShape::GeometryPointShape(const PointWorldCoord& point_coord, const QSizeF& size_px, const int& zoom_minimum, const int& zoom_maximum)
        : GeometryPoint(point_coord, zoom_minimum, zoom_maximum),
          m_size_px(size_px),
          m_alignment_type(AlignmentType::Middle),
          m_rotation(0.0)
    {

    }

    void GeometryPointShape::setPen(const std::shared_ptr<QPen>& pen)
    {
        // Set the pen to draw with.
        Geometry::setPen(pen);

        // Update the shape.
        updateShape();
    }

    void GeometryPointShape::setPen(const QPen& pen)
    {
        // Set the pen to draw with.
        Geometry::setPen(pen);

        // Update the shape.
        updateShape();
    }

    void GeometryPointShape::setBrush(const std::shared_ptr<QBrush>& brush)
    {
        // Set the brush to draw with.
        Geometry::setBrush(brush);

        // Update the shape.
        updateShape();
    }

    void GeometryPointShape::setBrush(const QBrush& brush)
    {
        // Set the brush to draw with.
        Geometry::setBrush(brush);

        // Update the shape.
        updateShape();
    }

    const QSizeF& GeometryPointShape::sizePx() const
    {
        // Return the size of the shape (pixels).
        return m_size_px;
    }

    void GeometryPointShape::setSizePx(const QSizeF& size_px, const bool& update_shape)
    {
        // Set the size of the shape (pixels).
        m_size_px = size_px;

        // Should we updated the shape?
        if(update_shape)
        {
            // Update the shape.
            updateShape();
        }
        else
        {
            // Just emit that we need to redraw to display this change.
            emit requestRedraw();
        }
    }

    const Geometry::AlignmentType& GeometryPointShape::alignmentType() const
    {
        // Return the alignment type.
        return m_alignment_type;
    }

    void GeometryPointShape::setAlignmentType(const AlignmentType& alignment_type, const bool& update_shape)
    {
        // Set the alignment type.
        m_alignment_type = alignment_type;

        // Should we updated the shape?
        if(update_shape)
        {
            // Update the shape.
            updateShape();
        }
        else
        {
            // Just emit that we need to redraw to display this change.
            emit requestRedraw();
        }
    }

    const qreal& GeometryPointShape::rotation() const
    {
        // Return the rotation.
        return m_rotation;
    }

    void GeometryPointShape::setRotation(const qreal& rotation, const bool& update_shape)
    {
        // Set the rotation.
        m_rotation = rotation;

        // Should we updated the shape?
        if(update_shape)
        {
            // Update the shape.
            updateShape();
        }
        else
        {
            // Just emit that we need to redraw to display this change.
            emit requestRedraw();
        }
    }

    RectWorldCoord GeometryPointShape::boundingBox(const int& controller_zoom) const
    {
        // Calculate the world point in pixels.
        const PointWorldPx point_px(projection::get().toPointWorldPx(coord(), controller_zoom));

        // Calculate the top-left point.
        const PointWorldPx top_left_point_px(calculateTopLeftPoint(point_px, m_alignment_type, m_size_px));

        // Calculate the bottom-right point.
        const PointWorldPx bottom_right_point_px(top_left_point_px.x() + m_size_px.width(), top_left_point_px.y() + m_size_px.height());

        // Return the converted coord points.
        return RectWorldCoord(projection::get().toPointWorldCoord(top_left_point_px, controller_zoom), projection::get().toPointWorldCoord(bottom_right_point_px, controller_zoom));
    }

    void GeometryPointShape::updateShape()
    {
        // Emit that we need to redraw to display this change.
        emit requestRedraw();
    }
}
