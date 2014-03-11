// Qt includes.
#include <QApplication>

// Local includes.
#include "multidemo.h"

int main(int argc, char *argv[])
{
    // Create a QApplication.
    QApplication app(argc, argv);

    // Create a Multidemo widget.
    Multidemo multidemo;
    multidemo.resize(480, 640);
    multidemo.setWindowTitle("QMapControl: Multidemo");

    // Show the widget.
    multidemo.show();

    // Execute the application.
    return app.exec();
}
