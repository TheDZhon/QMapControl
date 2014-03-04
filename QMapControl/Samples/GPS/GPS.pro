# Include sample configurations.
include(../Samples.pri)

# Target name.
TARGET = GPS

# Target version.
VERSION = 0.1

# Build a library.
TEMPLATE = app

# Add header files.
HEADERS +=                  \
    src/gps.h               \
    src/gps_neo.h           \

# Add source files.
SOURCES +=                  \
    src/main.cpp            \
    src/gps.cpp             \
    src/gps_neo.cpp         \
