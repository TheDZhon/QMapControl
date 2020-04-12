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

#include "MapAdapterBing.h"
#include <QDebug>

namespace qmapcontrol
{
    namespace
    {
        /// @todo remove once MSVC supports initializer lists.
        std::set<projection::EPSG> supportedProjections()
        {
            std::set<projection::EPSG> projections;
            projections.insert(projection::EPSG::SphericalMercator);
            return projections;
        }
    }

    /// @todo implement layertype, since Bing has Satelite and Map layers.
    MapAdapterBing::MapAdapterBing(const MapAdapterBing::BingLayerType& layer_type, QObject* parent)
        : MapAdapter(QString("https://t.ssl.ak.tiles.virtualearth.net/tiles/a%q.jpeg?g=1000"), supportedProjections(), 17, 0, 0, parent) /// @todo remove once MSVC supports initializer lists.
    {
    // https://t.ssl.ak.tiles.virtualearth.net/tiles/a120303211033000013.jpeg?g=8318&n=z&c4w=1&src=h
    // https://t.ssl.ak.tiles.virtualearth.net/tiles/a120303211032112.jpeg?g=8318&n=z&c4w=1&src=h
    // https://t.ssl.ak.tiles.virtualearth.net/tiles/a12030321103.jpeg?g=8318&n=z&c4w=1&src=h
    // https://t.ssl.ak.tiles.virtualearth.net/tiles/a1203032112310101.jpeg?g=8318
    // https://t.ssl.ak.dynamic.tiles.virtualearth.net/comp/ch/12030321123101013?it=G,BX,RL
    // https://t.ssl.ak.dynamic.tiles.virtualearth.net/comp/ch/120303211211300?mkt=en-US&it=G,BX,RL&shading=hill&n=z&og=804&c4w=1&cstl=vb&src=h

    }

//    QString MapAdapterBing::layerTypeToString(const MapAdapterBing::BingLayerType& layer_type)
//    {
//        // Convert the enum to a 1-character representation.
//        switch (layer_type)
//        {
//            case BingLayerType::SATELLITE:
//                return "s";
//            case BingLayerType::MAPS:
//            default:
//                return "m";
//        }
//    }

    QString zxy2QuadKey(quint32 z, quint32 x, quint32 y){
        ///  Bing MAX zoom level = 19
        quint32 i;
        quint32 tmp, mask;
        QString buf;
        buf = "";

        for(i = z; i > 0; --i){
            tmp = 0;
            mask = 1 << (i - 1);
            if(x & mask)
                tmp++;
            if(y & mask)
                tmp+=2;
            buf += QString::number(tmp);
        }
        return buf;
    }

    QUrl MapAdapterBing::tileQuery(const int& x, const int& y, const int& zoom_controller) const
    {
        /// @todo Since Bing does not have a single 256x256 topmost image,
        /// there probably should be a pre-rendered STUB imabe loaded from FS.
        QString id = zxy2QuadKey(static_cast<quint32>(zoom_controller),
                                 static_cast<quint32>(x),
                                 static_cast<quint32>(y)
                                 );
        QString URL = getBaseUrl().toString().replace("%25q", id);
        qDebug() << "x = " << x << "; y = " << y << "; z = " << zoom_controller << "; " << id;
        return QUrl(URL);
    }
}
