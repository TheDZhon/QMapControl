# Include common configurations.
include(../QMapControl.pri)

# Windows specific options.
win32 {
    # Capture whether this is a release/debug build.
    CONFIG(debug, debug|release) {
        # Add QMapControl library.
        LIBS += -L../../src/QMapControl/lib -lqmapcontrold1
    }
    CONFIG(release, debug|release) {
        # Add QMapControl library.
        LIBS += -L../../src/QMapControl/lib -lqmapcontrol1
    }
}

# Unix specific options.
unix {
    # Capture whether this is a release/debug build.
    CONFIG(debug, debug|release) {
        # Add QMapControl library.
        LIBS += -L../../src/QMapControl/lib -lqmapcontrold
    }
    CONFIG(release, debug|release) {
        # Add QMapControl library.
        LIBS += -L../../src/QMapControl/lib -lqmapcontrol
    }
}

# OSX specific options.
macx {
    # Disable app bundling.
    CONFIG -= app_bundle
}

# Include paths.
INCLUDEPATH += ../../src

# Include GDAL-required files.
contains(DEFINES, QMC_GDAL) {
    message(Building with GDAL support...)

    # Add GDAL include path.
    INCLUDEPATH += $$(QMC_GDAL_INC)

    # Add GDAL library path and library (windows).
    win32:LIBS += -L$$(QMC_GDAL_LIB) -lgdal_i

    # Add GDAL library path and library (unix).
    unix:LIBS += -L$$(QMC_GDAL_LIB) -lgdal
}

# Target install directory.
DESTDIR = bin

# Ensure libs are copied to bin directory.
target_libs.commands = -$(INSTALL_FILE) $$system_path(../../src/QMapControl/lib/*) $$system_path(bin/.)
QMAKE_EXTRA_TARGETS += target_libs

# Capture whether this is a release/debug build.
CONFIG(debug, debug|release) {
    TARGET_TYPE = debug
}
CONFIG(release, debug|release) {
    TARGET_TYPE = release
}

# Install details.
# Has a prefix been specified?
!isEmpty(prefix) {
    # Change prefix to PREFIX.
    PREFIX = $${prefix}/$$TARGET_TYPE
}
isEmpty(PREFIX) {
    # Default to parent directory.
    PREFIX = ../../../$$TARGET_TYPE
}
# Install target to $${PREFIX}/bin.
target.path = $${PREFIX}/bin
target.depends = target_libs
# Install libs (bin also contains copy of libs).
install_libs.path = $${PREFIX}
install_libs.files = bin
# Install target and libs.
INSTALLS += target install_libs
