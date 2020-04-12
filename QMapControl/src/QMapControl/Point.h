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

#pragma once

// Qt includes.
#include <QtCore/QPointF>
#include <QtCore/QRectF>

// STL includes.
#include <vector>

// Local includes.
#include "qmapcontrol_global.h"

/*!
 * @author Chris Stylianou <chris5287@gmail.com>
 */
namespace qmapcontrol
{

    class QMAPCONTROL_EXPORT PointWorldCoord
    {
    public:
        PointWorldCoord() : m_raw_point() {}
        PointWorldCoord(const qreal& longitude, const qreal& latitude) : m_raw_point(longitude, latitude) { } /// = default; @todo re-add once MSVC supports default/delete syntax.
        inline QPointF rawPoint() const { return m_raw_point; }
        inline qreal longitude() const { return m_raw_point.x(); }
        inline qreal latitude() const { return m_raw_point.y(); }
        inline void setLongitude(const qreal& longitude) { m_raw_point.setX(longitude); }
        inline void setLatitude(const qreal& latitude) { m_raw_point.setY(latitude); }

        inline bool operator==(const PointWorldCoord& p) { return m_raw_point == p.rawPoint(); }
        inline bool operator!=(const PointWorldCoord& p)  { return m_raw_point != p.rawPoint(); }
    protected:
        QPointF m_raw_point;
    };

    class QMAPCONTROL_EXPORT RectWorldCoord
    {
    public:
        RectWorldCoord(const PointWorldCoord& top_left, const PointWorldCoord& bottom_right) : m_raw_rect(top_left.rawPoint(), bottom_right.rawPoint()) { } /// = default; @todo re-add once MSVC supports default/delete syntax.
        RectWorldCoord(const PointWorldCoord& top_left, const QSizeF& size) : m_raw_rect(top_left.rawPoint(), size) { } /// = default; @todo re-add once MSVC supports default/delete syntax.
        inline QRectF rawRect() const { return m_raw_rect; }
        inline PointWorldCoord topLeftCoord() const { return PointWorldCoord(m_raw_rect.left(), m_raw_rect.top()); }
        inline PointWorldCoord topRightCoord() const { return PointWorldCoord(m_raw_rect.right(), m_raw_rect.top()); }
        inline PointWorldCoord bottomRightCoord() const { return PointWorldCoord(m_raw_rect.right(), m_raw_rect.bottom()); }
        inline PointWorldCoord bottomLeftCoord() const { return PointWorldCoord(m_raw_rect.left(), m_raw_rect.bottom()); }

        inline std::vector<PointWorldCoord> toStdVector()
        {
            // Create a vector to return the points.
            std::vector<PointWorldCoord> return_points;

            // Add the points.
            return_points.push_back(topLeftCoord());
            return_points.push_back(topRightCoord());
            return_points.push_back(bottomRightCoord());
            return_points.push_back(bottomLeftCoord());

            // Return the points.
            return return_points;
        }

    public:
        static RectWorldCoord fromQRectF(const QRectF& rect) { return RectWorldCoord(rect); }
    private:
        RectWorldCoord(const QRectF& rect) : m_raw_rect(rect) { }
    private:
        QRectF m_raw_rect;
    };


    class QMAPCONTROL_EXPORT PointPx
    {
    public:
        PointPx() : m_raw_point() { }
        PointPx(const qreal& x, const qreal& y) : m_raw_point(x, y) { } /// = default; @todo re-add once MSVC supports default/delete syntax.
        inline QPointF rawPoint() const { return m_raw_point; }
        inline qreal x() const { return m_raw_point.x(); }
        inline qreal y() const { return m_raw_point.y(); }
        inline void setX(const qreal& x) { m_raw_point.setX(x); }
        inline void setY(const qreal& y) { m_raw_point.setY(y); }
        //inline PointPx operator+(const PointPx &p) const { return PointPx(m_raw_point.x() + p.rawPoint().x(), m_raw_point.y() + p.rawPoint().y()); }
        //inline PointPx operator-(const PointPx &p) const { return PointPx(m_raw_point.x() - p.rawPoint().x(), m_raw_point.y() - p.rawPoint().y()); }
        inline PointPx operator/(qreal c) const { return PointPx(m_raw_point.x() / c, m_raw_point.y() / c); }
    protected:
        QPointF m_raw_point;
    };

    class QMAPCONTROL_EXPORT PointWorldPx : public PointPx
    {
    public:
        PointWorldPx() : PointPx() { }
        PointWorldPx(const qreal& x, const qreal& y) : PointPx(x, y) { } /// = default; @todo re-add once MSVC supports default/delete syntax.
        inline PointWorldPx operator+(const PointPx &p) const { return PointWorldPx(m_raw_point.x() + p.rawPoint().x(), m_raw_point.y() + p.rawPoint().y()); }
        inline PointWorldPx operator-(const PointPx &p) const { return PointWorldPx(m_raw_point.x() - p.rawPoint().x(), m_raw_point.y() - p.rawPoint().y()); }
        inline PointPx operator+(const PointWorldPx &p) const { return PointPx(m_raw_point.x() + p.rawPoint().x(), m_raw_point.y() + p.rawPoint().y()); }
        inline PointPx operator-(const PointWorldPx &p) const { return PointPx(m_raw_point.x() - p.rawPoint().x(), m_raw_point.y() - p.rawPoint().y()); }
        //inline QPointF &operator*=(qreal c);
        //inline QPointF &operator/=(qreal c);
    };

    class QMAPCONTROL_EXPORT PointViewportPx : public PointPx
    {
    public:
        PointViewportPx(const qreal& x, const qreal& y) : PointPx(x, y) { } /// = default; @todo re-add once MSVC supports default/delete syntax.
        inline PointViewportPx operator+(const PointPx &p) const { return PointViewportPx(m_raw_point.x() + p.rawPoint().x(), m_raw_point.y() + p.rawPoint().y()); }
        inline PointViewportPx operator-(const PointPx &p) const { return PointViewportPx(m_raw_point.x() - p.rawPoint().x(), m_raw_point.y() - p.rawPoint().y()); }
        inline PointPx operator+(const PointViewportPx &p) const { return PointPx(m_raw_point.x() + p.rawPoint().x(), m_raw_point.y() + p.rawPoint().y()); }
        inline PointPx operator-(const PointViewportPx &p) const { return PointPx(m_raw_point.x() - p.rawPoint().x(), m_raw_point.y() - p.rawPoint().y()); }
        //inline QPointF &operator*=(qreal c);
        //inline QPointF &operator/=(qreal c);
    };

    class QMAPCONTROL_EXPORT RectPx
    {
    public:
        RectPx() : m_raw_rect() { }
        RectPx(const PointPx& top_left, const PointPx& bottom_right) : m_raw_rect(top_left.rawPoint(), bottom_right.rawPoint()) { } /// = default; @todo re-add once MSVC supports default/delete syntax.
        RectPx(const PointPx& top_left, const QSizeF& size) : m_raw_rect(top_left.rawPoint(), size) { } /// = default; @todo re-add once MSVC supports default/delete syntax.
        inline QRectF rawRect() const { return m_raw_rect; }
        inline qreal leftPx() const { return m_raw_rect.left(); }
        inline qreal topPx() const { return m_raw_rect.top(); }
        inline qreal rightPx() const { return m_raw_rect.right(); }
        inline qreal bottomPx() const { return m_raw_rect.bottom(); }
        inline PointWorldPx centerPx() const { return PointWorldPx(m_raw_rect.center().x(), m_raw_rect.center().y()); }
        inline PointWorldPx topLeftPx() const { return PointWorldPx(leftPx(), topPx()); }
        inline PointWorldPx bottomRightPx() const { return PointWorldPx(rightPx(), bottomPx()); }
    protected:
        QRectF m_raw_rect;
    };

    class QMAPCONTROL_EXPORT RectWorldPx : public RectPx
    {
    public:
        RectWorldPx() : RectPx() { }
        RectWorldPx(const PointWorldPx& top_left, const PointWorldPx& bottom_right) : RectPx(top_left, bottom_right) { } /// = default; @todo re-add once MSVC supports default/delete syntax.
        RectWorldPx(const PointWorldPx& top_left, const QSizeF& size) : RectPx(top_left, size) { } /// = default; @todo re-add once MSVC supports default/delete syntax.
    };

    class QMAPCONTROL_EXPORT RectViewportPx : public RectPx
    {
    public:
        RectViewportPx() : RectPx() { }
        RectViewportPx(const PointViewportPx& top_left, const PointViewportPx& bottom_right) : RectPx(top_left, bottom_right) { } /// = default; @todo re-add once MSVC supports default/delete syntax.
        RectViewportPx(const PointViewportPx& top_left, const QSizeF& size) : RectPx(top_left, size) { } /// = default; @todo re-add once MSVC supports default/delete syntax.
    };

}
