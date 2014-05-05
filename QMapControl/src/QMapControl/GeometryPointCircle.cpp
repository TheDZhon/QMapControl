#include "GeometryPointCircle.h"

namespace qmapcontrol
{
    GeometryPointCircle::GeometryPointCircle(const PointWorldCoord& point_coord, const QSizeF& size_px, const int& zoom_minimum, const int& zoom_maximum)
        : GeometryPointImage(point_coord, QPixmap(size_px.toSize()), zoom_minimum, zoom_maximum)
    {
        // Update the shape (to draw the initial image pixmap).
        updateShape();
    }

    void GeometryPointCircle::updateShape()
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

        // Draw the ellipse.
        const double center_px(image_pixmap.width() / 2.0);
        painter.drawEllipse(PointWorldPx(center_px, center_px).rawPoint(), center_px - pen().widthF(), center_px - pen().widthF());

        // Set the image pixmap.
        setImage(image_pixmap, false);
    }
}
