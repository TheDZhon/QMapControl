# Include sample configurations.
include(../Samples.pri)

# Target name.
TARGET = Mapviewer

# Target version.
VERSION = 0.1

# Build a library.
TEMPLATE = app

# Add header files.
HEADERS +=                  \
    src/mapviewer.h         \

# Add source files.
SOURCES +=                  \
    src/main.cpp            \
    src/mapviewer.cpp       \
