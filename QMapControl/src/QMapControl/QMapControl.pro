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
    GeometryCurve.h                             \
    GeometryLineString.h                        \
    GeometryPoint.h                             \
    GeometryPointArrow.h                        \
    GeometryPointCircle.h                       \
    GeometryPointImage.h                        \
    GeometryPointImageFixed.h                   \
    GeometryPointWidget.h                       \
    GeometryPolygon.h                           \
    GPS_Position.h                              \
    ImageManager.h                              \
    Layer.h                                     \
    MapAdapter.h                                \
    MapAdapterGoogle.h                          \
    MapAdapterOSM.h                             \
    MapAdapterTile.h                            \
    MapAdapterWMS.h                             \
    MapAdapterYahoo.h                           \
    NetworkManager.h                            \
    Projection.h                                \
    ProjectionEquirectangular.h                 \
    ProjectionSphericalMercator.h               \
    QMapControl.h                               \
    QuadTreeContainer.h                         \
    QProgressIndicator\QProgressIndicator.h     \

# Add source files.
SOURCES +=                                      \
    Geometry.cpp                                \
    GeometryCurve.cpp                           \
    GeometryLineString.cpp                      \
    GeometryPoint.cpp                           \
    GeometryPointArrow.cpp                      \
    GeometryPointCircle.cpp                     \
    GeometryPointImage.cpp                      \
    GeometryPointImageFixed.cpp                 \
    GeometryPointWidget.cpp                     \
    GeometryPolygon.cpp                         \
    GPS_Position.cpp                            \
    ImageManager.cpp                            \
    Layer.cpp                                   \
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
    QProgressIndicator\QProgressIndicator.cpp   \

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
