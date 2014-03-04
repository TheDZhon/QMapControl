# What is QMapControl?
Based on the original QMapControl project (http://www.medieninf.de/qmapcontrol), this is an updated version of QMapControl that support Qt5.

QMapControl is a mapping library that provides a QWidget interface that you can use in your own applications.

Features:
- Maps: Supports WMS and 'Slippy' tile map services.
- Geometries: Add points, circles, lines, images and other QWidgets.
- Layers: Maps and/or geometries can be added to a layer, which can be shown/hidden as required.

## Prerequisites
### Compiler
- A modern C++ compiler that fully supports the C++11 standard:
  - Tested with GCC 4.8.2, Clang 3.4 and MSVC 11 (VS 2012).

### External Dependencies
- Qt 5 (http://qt-project.org)
  - Tested with Qt 5.2.1.
  
### Internal Dependencies
- QProgressIndicator (https://github.com/mojocorp/QProgressIndicator)
  - Version: [Commit hash - 0b54d4d](https://github.com/mojocorp/QProgressIndicator/tree/0b54d4d/)
  - License type: [GNU Lesser General Public License (LGPL) version 3](https://github.com/mojocorp/QProgressIndicator/blob/0b54d4d/LICENSE)

## Build
Run the following commands from the root directory to build QMapControl (replace `make` with `nmake` for MSVC):
```Shell
qmake prefix=INSTALL_LOCATION
make
````
To install QMapControl, run:
````
make install
````
Note: **INSTALL_LOCATION** must be an absolute path to the install directory required.
