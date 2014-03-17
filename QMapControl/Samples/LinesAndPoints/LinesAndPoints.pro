# Include sample configurations.
include(../Samples.pri)

# Target name.
TARGET = Linesandpoints

# Target version.
VERSION = 0.1

# Build a library.
TEMPLATE = app

# Add header files.
HEADERS +=                      \
    src/LinesAndPoints.h        \

# Add source files.
SOURCES +=                      \
    src/main.cpp                \
    src/LinesAndPoints.cpp      \

# Add resource files.
RESOURCES +=                    \
    ../resources/Resources.qrc  \
