#pragma once

// Qt includes.
#include <QtWidgets/QDialog>
#include <QtWidgets/QTextEdit>

/**
 * @author Kai Winter <kaiwinter@gmx.de>
 * @author Chris Stylianou <chris5287@gmail.com>
 */
class InfoDialog : public QDialog
{
    Q_OBJECT

public:
    //! InfoDialog constructor
    /*!
     * This is used to construct a InfoDialog Widget.
     * @param parent QObject parent ownership.
     */
    InfoDialog(QWidget* parent = 0);

    //! Disable copy constructor.
    ///InfoDialog(const InfoDialog&) = delete; @todo re-add once MSVC supports default/delete syntax.

    //! Disable copy assignment.
    ///InfoDialog& operator=(const InfoDialog&) = delete; @todo re-add once MSVC supports default/delete syntax.

    //! Destructor.
    ~InfoDialog() { } /// = default; @todo re-add once MSVC supports default/delete syntax.

    /*!
     * Set the infotext.
     * @param text The text to set.
     */
    void setInfotext(const QString& text);

private:
    /// The text info widget.
    QTextEdit* m_text_info;

};
