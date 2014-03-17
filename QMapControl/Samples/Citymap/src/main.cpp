// Qt includes.
#include <QApplication>

// Local includes.
#include "citymap.h"

int main(int argc, char *argv[])
{
    // Create a QApplication.
    QApplication app(argc, argv);

    // Create a Citymap widget.
    Citymap citymap;
    citymap.resize(400, 590);
    citymap.setWindowTitle("QMapControl: Citymap");

    // Show the widget.
    citymap.show();

    // Execute the application.
    return app.exec();
}
