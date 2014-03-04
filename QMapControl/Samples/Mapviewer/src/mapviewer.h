#ifndef MAPVIEWER_H
#define MAPVIEWER_H

#include <QtWidgets/QMainWindow>

#include <QMapControl/QMapControl.h>
#include <QMapControl/MapAdapterOSM.h>
#include <QMapControl/Layer.h>

using namespace qmapcontrol;
class Mapviewer : public QMainWindow
{
    Q_OBJECT

public:
    Mapviewer(QWidget *parent = 0);

    ~Mapviewer();

private:
    QMapControl* mc;
    std::shared_ptr<Layer> mainlayer;

    void addZoomButtons();

protected:
    virtual void resizeEvent ( QResizeEvent * event );
};

#endif
