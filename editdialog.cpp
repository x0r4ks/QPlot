#include "editdialog.h"
#include "ui_editdialog.h"

editDialog::editDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::editDialog)
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
    color = QColor(ui->sb_red->value(), ui->sb_green->value(), ui->sb_blue->value());
}


void editDialog::setColor(QColor color)
{
    ui->sb_red->setValue(color.red());
    ui->sb_green->setValue(color.green());
    ui->sb_blue->setValue(color.blue());
}

void editDialog::setFormul(QString formul)
{
    ui->le_formul->setText(formul);
}
