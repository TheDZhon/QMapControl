#include "GeometryPolygonImage.h"

// Local includes.
#include "Projection.h"

namespace qmapcontrol
{
    GeometryPolygonImage::GeometryPolygonImage(const PointWorldCoord& top_left_coord, const PointWorldCoord& bottom_right_coord, const std::string& filename, const int& zoom_minimum, const int& zoom_maximum)
        : GeometryPolygon(RectWorldCoord(top_left_coord, bottom_right_coord).toStdVector(), zoom_minimum, zoom_maximum),
          m_image(QPixmap(filename.c_str()))
    {

    }

    GeometryPolygonImage::GeometryPolygonImage(const PointWorldCoord& top_left_coord, const PointWorldCoord& bottom_right_coord, const QPixmap& image, const int& zoom_minimum, const int& zoom_maximum)
        : GeometryPolygon(RectWorldCoord(top_left_coord, bottom_right_coord).toStdVector(), zoom_minimum, zoom_maximum),
          m_image(image)
    {

    }

    void GeometryPolygonImage::draw(QPainter& painter, const RectWorldCoord& backbuffer_rect_coord, const int& controller_zoom)
    {
        // Check the geometry is visible.
        if(isVisible(controller_zoom))
        {
            // Fetch the image rect using the bounding box.
            const RectWorldCoord image_rect_coord(boundingBox(controller_zoom));

            // Does the image rect intersect with the backbuffer rect?
            if(backbuffer_rect_coord.rawRect().intersects(image_rect_coord.rawRect()))
            {
                // Calculate the image rect pixels.
                const RectWorldPx image_rect_px(projection::get().toPointWorldPx(image_rect_coord.topLeftCoord(), controller_zoom), projection::get().toPointWorldPx(image_rect_coord.bottomRightCoord(), controller_zoom));

                // Draw the pixmap.
                painter.drawPixmap(image_rect_px.rawRect().toRect(), m_image);
            }
        }
    }
}
