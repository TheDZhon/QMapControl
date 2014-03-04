# Include sample configurations.
include(../Samples.pri)

# Target name.
TARGET = Citymap

# Target version.
VERSION = 0.1

# Build a library.
TEMPLATE = app

# Add header files.
HEADERS +=                  \
    src/citymap.h           \
    src/dialogs.h           \

# Add source files.
SOURCES +=                  \
    src/main.cpp            \
    src/citymap.cpp         \
    src/dialogs.cpp         \
