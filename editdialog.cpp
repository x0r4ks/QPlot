#include "editdialog.h"
#include "ui_editdialog.h"
#include <QColorDialog>

editDialog::editDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::editDialog)
{
    ui->setupUi(this);
}

editDialog::~editDialog()
{
    delete ui;
}

QString editDialog::getFormul()
{
    return formul;
}

QColor editDialog::getColor()
{
    return color;
}

void editDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    formul = ui->le_formul->text();

}

void editDialog::setColor(QColor color)
{
    this->color = color;

    QString style = "background-color: rgb(";
    style += QString::number(color.red());
    style += ",";
    style += QString::number(color.green());
    style += ",";
    style += QString::number(color.blue());
    style += ")";
    ui->new_graph_color->setStyleSheet(style);
}

void editDialog::setFormul(QString formul)
{
    ui->le_formul->setText(formul);
}

void editDialog::on_new_graph_color_clicked()
{
    color = QColorDialog::getColor(color, this, tr("Select color"));

    QString style = "background-color: rgb(";
    style += QString::number(color.red());
    style += ",";
    style += QString::number(color.green());
    style += ",";
    style += QString::number(color.blue());
    style += ")";
    ui->new_graph_color->setStyleSheet(style);
}

