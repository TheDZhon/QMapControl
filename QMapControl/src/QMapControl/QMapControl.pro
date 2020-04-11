# Include common configurations.
include(../../QMapControl.pri)

# Capture whether this is a release/debug build.
CONFIG(debug, debug|release) {
    # Target name.
    TARGET = qmapcontrold
}
CONFIG(release, debug|release) {
    # Target name.
    TARGET = qmapcontrol
}

# Target version.
VERSION = 1.0.0

# Build a library.
TEMPLATE = lib

# Required defines.
DEFINES += QMAPCONTROL_LIBRARY

# Add header files.
HEADERS +=                                      \
    qmapcontrol_global.h                        \
    Geometry.h                                  \
    GeometryLineString.h                        \
    GeometryPoint.h                             \
    GeometryPointShape.h                        \
    GeometryPointShapeScaled.h                  \
    GeometryPointArrow.h                        \
    GeometryPointCircle.h                       \
    GeometryPointImage.h                        \
    GeometryPointImageScaled.h                  \
    GeometryPolygon.h                           \
    GeometryPolygonImage.h                      \
    GeometryWidget.h                            \
    GPS_Position.h                              \
    ImageManager.h                              \
    Layer.h                                     \
    LayerGeometry.h                             \
    LayerMapAdapter.h                           \
    MapAdapter.h                                \
    MapAdapterGoogle.h                          \
    MapAdapterBing.h                            \
    MapAdapterOSM.h                             \
    MapAdapterOTM.h                             \
    MapAdapterTile.h                            \
    MapAdapterWMS.h                             \
    MapAdapterYahoo.h                           \
    NetworkManager.h                            \
    Point.h                                     \
    Projection.h                                \
    ProjectionEquirectangular.h                 \
    ProjectionSphericalMercator.h               \
    QMapControl.h                               \
    QuadTreeContainer.h                         \
# Third-party headers: QProgressIndicator
    QProgressIndicator.h                        \

# Add source files.
SOURCES +=                                      \
    Geometry.cpp                                \
    GeometryLineString.cpp                      \
    GeometryPoint.cpp                           \
    GeometryPointShape.cpp                      \
    GeometryPointShapeScaled.cpp                \
    GeometryPointArrow.cpp                      \
    GeometryPointCircle.cpp                     \
    GeometryPointImage.cpp                      \
    GeometryPointImageScaled.cpp                \
    GeometryPolygon.cpp                         \
    GeometryPolygonImage.cpp                    \
    GeometryWidget.cpp                          \
    GPS_Position.cpp                            \
    ImageManager.cpp                            \
    Layer.cpp                                   \
    LayerGeometry.cpp                           \
    LayerMapAdapter.cpp                         \
    MapAdapter.cpp                              \
    MapAdapterGoogle.cpp                        \
    MapAdapterBing.cpp                          \
    MapAdapterOSM.cpp                           \
    MapAdapterOTM.cpp                           \
    MapAdapterTile.cpp                          \
    MapAdapterWMS.cpp                           \
    MapAdapterYahoo.cpp                         \
    NetworkManager.cpp                          \
    Projection.cpp                              \
    ProjectionEquirectangular.cpp               \
    ProjectionSphericalMercator.cpp             \
    QMapControl.cpp                             \
# Third-party sources: QProgressIndicator
    QProgressIndicator.cpp                      \

# Include GDAL-required files.
contains(DEFINES, QMC_GDAL) {
    message(Building with GDAL support...)

    # Add header files.
    HEADERS +=                                  \
        ESRIShapefile.h                         \
        LayerESRIShapefile.h                    \

    # Add source files.
    SOURCES +=                                  \
        ESRIShapefile.cpp                       \
        LayerESRIShapefile.cpp                  \

    # Add GDAL include path.
    INCLUDEPATH += $$(QMC_GDAL_INC)

    # Add GDAL library path and library (windows).
    win32:LIBS += -L$$(QMC_GDAL_LIB) -lgdal_i

    # Add GDAL library path and library (unix).
    unix:LIBS += -L$$(QMC_GDAL_LIB) -lgdal
}

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
# Install headers to $${PREFIX}/include/QMapControl
headers.path = $${PREFIX}/include/QMapControl
headers.files = $${HEADERS}
# Install target to $${PREFIX}/lib.
target.path = $${PREFIX}/lib
# Install headers and target.
INSTALLS += target headers
