// Qt includes.
#include <QApplication>

// Local includes.
#include "mapviewer.h"

int main(int argc, char *argv[])
{
    // Create a QApplication.
    QApplication app(argc, argv);

    // Create a Multidemo widget.
    Mapviewer mapviewer;
    mapviewer.resize(380, 565);
    mapviewer.setWindowTitle("QMapControl: Mapviewer");

    // Show the widget.
    mapviewer.show();

    // Execute the application.
    return app.exec();
}
