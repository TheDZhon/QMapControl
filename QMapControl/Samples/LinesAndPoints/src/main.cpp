// Qt includes.
#include <QApplication>

// Local includes.
#include "linesandpoints.h"

int main(int argc, char *argv[])
{
    // Create a QApplication.
    QApplication app(argc, argv);

    // Create a LinesAndPoints widget.
    LinesAndPoints linesandpoints;
    linesandpoints.resize(480, 640);
    linesandpoints.setWindowTitle("QMapControl: LinesAndPoints");

    // Show the widget.
    linesandpoints.show();

    // Execute the application.
    return app.exec();
}
