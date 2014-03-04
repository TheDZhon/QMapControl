#ifndef LINESANDPOINTS_H
#define LINESANDPOINTS_H

#include <QtGui>
#include <QMapControl/QMapControl.h>
#include <QMapControl/MapAdapterOSM.h>
#include <QMapControl/Layer.h>
#include <QMapControl/GeometryPointImage.h>
#include <QMapControl/GeometryPointCircle.h>
#include <QMapControl/GeometryLineString.h>
using namespace qmapcontrol;
class LinesAndPoints : public QWidget
{
        Q_OBJECT
        public:
                LinesAndPoints(QWidget *parent = 0);
                ~LinesAndPoints();

        private:
                QMapControl* mc;
                void addZoomButtons();

        public slots:
                void geometryClicked(Geometry* geom);

        protected:
                void keyPressEvent(QKeyEvent* evnt);
};

#endif
