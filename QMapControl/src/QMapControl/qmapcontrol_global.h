#pragma once

// Qt includes.
#include <QtCore/QtGlobal>

/*!
 * @author Kai Winter <kaiwinter@gmx.de>
 * @author Chris Stylianou <chris5287@gmail.com>
 */

#ifdef QMAPCONTROL_LIBRARY
    /// Defines that the specified function, variable or class should be exported.
    #define QMAPCONTROL_EXPORT Q_DECL_EXPORT
#else
    /// Defines that the specified function, variable or class should be imported.
    #define QMAPCONTROL_EXPORT Q_DECL_IMPORT
#endif
