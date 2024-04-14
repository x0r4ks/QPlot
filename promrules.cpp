#include "promrules.h"
#include "ui_promrules.h"

PromRules::PromRules(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PromRules)
{
    ui->setupUi(this);

    ui->textBrowser->setFontPointSize(15);
    this->ui->textBrowser->setPlainText(tr("about_prop"));


}

PromRules::~PromRules()
{
    delete ui;
}

void PromRules::on_ok_btn_clicked()
{
    this->close();
}

