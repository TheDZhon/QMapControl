// Qt includes.
#include <QApplication>

// Local includes.
#include "gps.h"

int main(int argc, char *argv[])
{
    // Create a QApplication.
    QApplication app(argc, argv);

    // Create a GPS widget.
    GPS gps;
    gps.resize(480, 640);
    gps.setWindowTitle("QMapControl: GPS");

    // Show the widget.
    gps.show();

    // Execute the application.
    return app.exec();
}
