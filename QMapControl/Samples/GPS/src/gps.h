#ifndef GPS_H
#define GPS_H

#include <QtGui>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

#include <QMapControl/QMapControl.h>
#include "gps_neo.h"
using namespace qmapcontrol;
class GPS: public QWidget
{
        Q_OBJECT
        public:
                GPS();

                ~GPS();

        private:
                QPushButton* followgps;
                QLabel* gpsposition;
                QMapControl* mc;

        public slots:
                void updatePosition(float time, QPointF coordinate);

};

#endif
