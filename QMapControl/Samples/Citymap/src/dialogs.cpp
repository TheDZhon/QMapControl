#include "dialogs.h"

#include <QtWidgets/QHBoxLayout>

InfoDialog::InfoDialog(QWidget* parent)
	: QDialog(parent)
{
	setWindowFlags(Qt::Dialog | Qt::WindowStaysOnTopHint);
	infotext = new QTextEdit();
	infotext->setReadOnly(true);

	QHBoxLayout* layout = new QHBoxLayout();
	setLayout(layout);
	
	layout->addWidget(infotext);
}

void InfoDialog::setInfotext(QString text)
{
	infotext->setHtml(text);
}
