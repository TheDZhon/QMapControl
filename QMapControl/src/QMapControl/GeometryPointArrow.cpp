#include "GeometryPointArrow.h"

namespace qmapcontrol
{
    GeometryPointArrow::GeometryPointArrow(const PointWorldCoord& point_coord, const QSizeF& size_px, const int& zoom_minimum, const int& zoom_maximum)
        : GeometryPointImage(point_coord, QPixmap(size_px.toSize()), zoom_minimum, zoom_maximum)
    {
        // Update the shape (to draw the initial image pixmap).
        updateShape();
    }

    void GeometryPointArrow::updateShape()
    {
        // Create a pixmap of the required size.
        QPixmap image_pixmap(sizePx().toSize());

        // Reset the image pixmap.
        image_pixmap.fill(Qt::transparent);

        // Create a painter for the image pixmap.
        QPainter painter(&image_pixmap);

        // Ensure antialiasing is enabled.
        painter.setRenderHints(QPainter::Antialiasing, true);

        // Set the pen and brush.
        painter.setPen(pen());
        painter.setBrush(brush());

        // Add points to create arrow shape.
        QPolygonF arrow;
        arrow << PointPx((image_pixmap.width() / 2.0), 0.0).rawPoint();
        arrow << PointPx(image_pixmap.width(), image_pixmap.height()).rawPoint();
        arrow << PointPx((image_pixmap.width() / 2.0), (image_pixmap.height() / 2.0)).rawPoint();
        arrow << PointPx(0.0, image_pixmap.height()).rawPoint();

        // Draw the arrow.
        painter.drawPolygon(arrow);

        // Set the image pixmap.
        setImage(image_pixmap, false);
    }
}
