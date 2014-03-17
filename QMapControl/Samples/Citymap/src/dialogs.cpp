#include "dialogs.h"

// Qt includes.
#include <QtWidgets/QHBoxLayout>

InfoDialog::InfoDialog(QWidget* parent)
    : QDialog(parent)
{
    // Set the dialog flags.
    setWindowFlags(Qt::Dialog | Qt::WindowStaysOnTopHint);

    // Reset the info text as read only.
    m_text_info = new QTextEdit();
    m_text_info->setReadOnly(true);

    // Create a new layout and add the widget.
    QHBoxLayout* layout = new QHBoxLayout();
    setLayout(layout);
    layout->addWidget(m_text_info);
}

void InfoDialog::setInfotext(const QString& text)
{
    // Set the text of the infotext.
    m_text_info->setHtml(text);
}
