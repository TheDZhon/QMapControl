# Include common configurations.
include(../QMapControl.pri)

# Windows specific options.
win32 {
    # Add QMapControl library.
    LIBS += -L../../src/QMapControl/lib -lqmapcontrol1
}

# Unix specific options.
unix {
    # Add QMapControl library.
    LIBS += -L../../src/QMapControl/lib -lqmapcontrol
}

# OSX specific options.
macx {
    # Disable app bundling.
    CONFIG -= app_bundle
}

# Include paths.
INCLUDEPATH += ../../src

# Target install directory.
DESTDIR = bin

# Ensure libs are copied to bin directory.
target_libs.commands = -$(INSTALL_FILE) $$system_path(../../src/QMapControl/lib/*) $$system_path(bin/.)
QMAKE_EXTRA_TARGETS += target_libs

# Install details.
# Has a prefix been specified?
!isEmpty(prefix) {
    # Change prefix to PREFIX.
    PREFIX = $${prefix}
}
isEmpty(PREFIX) {
    # Default to parent directory.
    PREFIX = ../../../
}
# Install target to $${PREFIX}/bin.
target.path = $${PREFIX}/bin
target.depends = target_libs
# Install libs (bin also contains copy of libs).
install_libs.path = $${PREFIX}
install_libs.files = bin
# Install target and libs.
INSTALLS += target install_libs
