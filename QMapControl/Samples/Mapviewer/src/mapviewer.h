#pragma once

// Qt includes.
#include <QtWidgets/QMainWindow>

// QMapControl includes.
#include <QMapControl/QMapControl.h>

using namespace qmapcontrol;

/**
 * @author Kai Winter <kaiwinter@gmx.de>
 * @author Chris Stylianou <chris5287@gmail.com>
 */
class Mapviewer : public QMainWindow
{
    Q_OBJECT

public:
    //! Mapviewer constructor
    /*!
     * This is used to construct a Mapviewer QWidget.
     * @param parent QWidget parent ownership.
     */
    Mapviewer(QWidget* parent = 0);

    //! Disable copy constructor.
    ///Mapviewer(const Mapviewer&) = delete; @todo re-add once MSVC supports default/delete syntax.

    //! Disable copy assignment.
    ///Mapviewer& operator=(const Mapviewer&) = delete; @todo re-add once MSVC supports default/delete syntax.

    //! Destructor.
    ~Mapviewer() { } /// = default; @todo re-add once MSVC supports default/delete syntax.

private:
    /**
     * Handles QWidget resizing.
     * @param resize_event The QResizeEvent that occured.
     */
    void resizeEvent(QResizeEvent* resize_event);

private:
    /// The main map control.
    QMapControl* m_map_control;

};
