#include "rootsforms.h"
#include "ui_rootsforms.h"

RootsForms::RootsForms(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RootsForms)
{
    ui->setupUi(this);
}

void RootsForms::setText(QString text)
{
    ui->textBrowser->setPlainText(text);
}

RootsForms::~RootsForms()
{
    delete ui;
}
