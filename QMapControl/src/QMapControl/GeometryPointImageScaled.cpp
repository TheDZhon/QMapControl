#include "GeometryPointImageScaled.h"

// Local includes.
#include "Projection.h"

namespace qmapcontrol
{
    namespace
    {
        /// Default empty image pixmap.
        std::unique_ptr<QPixmap> m_image_null = nullptr;
    }

    GeometryPointImageScaled::GeometryPointImageScaled(const PointWorldCoord& point_coord, const std::shared_ptr<QPixmap>& image, const int& base_zoom, const int& zoom_minimum, const int& zoom_maximum)
        : GeometryPointShapeScaled(point_coord, image->size(), base_zoom, zoom_minimum, zoom_maximum),
          m_image(image)
    {

    }

    GeometryPointImageScaled::GeometryPointImageScaled(const PointWorldCoord& point_coord, const QPixmap& image, const int& base_zoom, const int& zoom_minimum, const int& zoom_maximum)
        : GeometryPointShapeScaled(point_coord, image.size(), base_zoom, zoom_minimum, zoom_maximum),
          m_image(std::make_shared<QPixmap>(image))
    {

    }

    GeometryPointImageScaled::GeometryPointImageScaled(const PointWorldCoord& point_coord, const std::string& filename, const int& base_zoom, const int& zoom_minimum, const int& zoom_maximum)
        : GeometryPointShapeScaled(point_coord, QSizeF(0, 0), base_zoom, zoom_minimum, zoom_maximum),
          m_image(std::make_shared<QPixmap>(filename.c_str()))
    {
        // Update the image size (as we have to wait for it to be loaded in the constructor).
        setSizePx(m_image->size());
    }

    const QPixmap& GeometryPointImageScaled::image() const
    {
        // Is the image pixmap currently null?
        if(m_image == nullptr)
        {
            // Have we already constructed the null image pixmap?
            if(m_image_null == nullptr)
            {
                // Construct the null image pixmap.
                m_image_null.reset(new QPixmap);
            }

            // Return the null image pixmap.
            return *(m_image_null.get());
        }
        else
        {
            // Return the image pixmap.
            return *(m_image.get());
        }
    }

    void GeometryPointImageScaled::setImage(const std::shared_ptr<QPixmap>& new_image, const bool& update_shape)
    {
        // Set the image pixmap.
        m_image = new_image;

        // Update the size (pixels).
        // This will also emit a redraw request.
        setSizePx(image().size(), update_shape);
    }

    void GeometryPointImageScaled::setImage(const QPixmap& new_image, const bool& update_shape)
    {
        // Set the pixmap.
        m_image = std::make_shared<QPixmap>(new_image);

        // Update the size (pixels).
        // This will also emit a redraw request.
        setSizePx(m_image->size(), update_shape);
    }

    void GeometryPointImageScaled::drawShape(QPainter &painter, const RectWorldPx &rect)
    {
        painter.drawPixmap(-rect.rawRect().width() / 2.0, -rect.rawRect().height() / 2.0, image());
    }

}
