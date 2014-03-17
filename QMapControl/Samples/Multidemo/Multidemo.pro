# Include sample configurations.
include(../Samples.pri)

# Target name.
TARGET = Multidemo

# Target version.
VERSION = 0.1

# Build a library.
TEMPLATE = app

# Add header files.
HEADERS +=                      \
    src/gps_modul.h             \
    src/multidemo.h             \

# Add source files.
SOURCES +=                      \
    src/main.cpp                \
    src/gps_modul.cpp           \
    src/multidemo.cpp           \

# Add resource files.
RESOURCES +=                    \
    ../resources/Resources.qrc  \
