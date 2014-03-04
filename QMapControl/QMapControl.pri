# Add support for c++11.
CONFIG += c++11

# Hide debug output in release mode.
CONFIG(release, debug|release) : DEFINES += QT_NO_DEBUG_OUTPUT

# Required defines.
DEFINES +=                          \
    # Windows: To force math constants to be defined in <cmath>
    _USE_MATH_DEFINES               \
    # Windows: Extend MSVC 11 to allow more than 5 arguments for its 'faux' variadic templates.
    # @todo remove once MSVC supports standards-based variadic templates.
    _VARIADIC_MAX=6                 \

# Build MOC directory.
MOC_DIR = tmp

# Build object directory.
OBJECTS_DIR = obj

# Build resources directory.
RCC_DIR = resources

# Build UI directory.
UI_DIR = ui

# Target install directory.
DESTDIR = lib

# Add Qt modules.
QT +=                               \
    network                         \
    widgets                         \
