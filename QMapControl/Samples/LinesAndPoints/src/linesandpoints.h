#pragma once

// Qt includes.
#include <QtWidgets/QMainWindow>

// QMapControl includes.
#include <QMapControl/QMapControl.h>
#include <QMapControl/Geometry.h>

using namespace qmapcontrol;

/**
 * @author Kai Winter <kaiwinter@gmx.de>
 * @author Chris Stylianou <chris5287@gmail.com>
 */
class LinesAndPoints : public QMainWindow
{
    Q_OBJECT
public:
    //! LinesAndPoints constructor
    /*!
     * This is used to construct a LinesAndPoints QWidget.
     * @param parent QWidget parent ownership.
     */
    LinesAndPoints(QWidget* parent = 0);

    //! Disable copy constructor.
    ///LinesAndPoints(const LinesAndPoints&) = delete; @todo re-add once MSVC supports default/delete syntax.

    //! Disable copy assignment.
    ///LinesAndPoints& operator=(const LinesAndPoints&) = delete; @todo re-add once MSVC supports default/delete syntax.

    //! Destructor.
    ~LinesAndPoints() { } /// = default; @todo re-add once MSVC supports default/delete syntax.

public slots:
    /*!
     * Called when a geometry is clicked.
     * @param geometry The clicked Geometry.
     */
    void geometryClickEvent(const Geometry* geometry);
    void resizeEvent(QResizeEvent* resize_event);

private:
    /// The main map control.
    QMapControl* m_map_control;
};
