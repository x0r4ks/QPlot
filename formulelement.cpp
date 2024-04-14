#include "formulelement.h"

#include <QSpacerItem>

FormulElement::FormulElement(QWidget *parent, QColor color, QString text)
    : QWidget{parent}
{
    this->img_color = color;


    btn = new QPushButton(this);
    btn->setEnabled(false);
    lable = new QLabel(this);

    lable->setText(text);

    QString style = "background-color: rgb(";
    style += QString::number(color.red());
    style += ",";
    style += QString::number(color.green());
    style += ",";
    style += QString::number(color.blue());
    style += ")";
    btn->setStyleSheet(style);

    layout = new QHBoxLayout();

    layout->addWidget(lable);

    layout->addSpacerItem(new QSpacerItem(1,1, QSizePolicy::Maximum, QSizePolicy::Maximum));
    layout->addWidget(btn);



}


void FormulElement::setColor(QColor color) {
    img_color = color;

    QString style = "background-color: rgb(";
    style += QString::number(color.red());
    style += ",";
    style += QString::number(color.green());
    style += ",";
    style += QString::number(color.blue());
    style += ")";
    btn->setStyleSheet(style);

}
