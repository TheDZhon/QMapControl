# Include common configurations.
include(../../QMapControl.pri)

# Target name.
TARGET = qmapcontrol

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
    GeometryPointArrow.h                        \
    GeometryPointCircle.h                       \
    GeometryPointImage.h                        \
    GeometryPointImageFixed.h                   \
    GeometryPolygon.h                           \
    GeometryWidget.h                            \
    GPS_Position.h                              \
    ImageManager.h                              \
    Layer.h                                     \
    LayerGeometry.h                             \
    LayerMapAdapter.h                           \
    MapAdapter.h                                \
    MapAdapterGoogle.h                          \
    MapAdapterOSM.h                             \
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
    GeometryPointArrow.cpp                      \
    GeometryPointCircle.cpp                     \
    GeometryPointImage.cpp                      \
    GeometryPointImageFixed.cpp                 \
    GeometryPolygon.cpp                         \
    GeometryWidget.cpp                          \
    GPS_Position.cpp                            \
    ImageManager.cpp                            \
    Layer.cpp                                   \
    LayerGeometry.cpp                           \
    LayerMapAdapter.cpp                         \
    MapAdapter.cpp                              \
    MapAdapterGoogle.cpp                        \
    MapAdapterOSM.cpp                           \
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

    # Add GDAL library path and library.
    LIBS += -L$$(QMC_GDAL_LIB) -lgdal
}

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
# Install headers to $${PREFIX}/include/QMapControl
headers.path = $${PREFIX}/include/QMapControl
headers.files = $${HEADERS}
# Install target to $${PREFIX}/lib.
target.path = $${PREFIX}/lib
# Install headers and target.
INSTALLS += target headers
