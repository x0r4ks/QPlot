#include "editror.h"
#include "ui_editror.h"
#include <QDebug>

editror::editror(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::editror)
{
    ui->setupUi(this);
}

editror::~editror()
{
    delete ui;
}


void editror::clear()
{
    ui->textEdit->clear();
}

QString editror::getTernar()
{
    QString plainText = ui->textEdit->toPlainText();
    QList<QString> lines = plainText.split('\n');
    QList<QString> line;
    QString res ="";

    for (int i = 0; i < lines.size(); i++)
    {
        line = lines.at(i).split(" ");
        for (int j = 0; j < line.size(); j++)
        {

            if (line.at(j) == "if") {
                res += "(";
                for (int jk = j+1; jk < line.size(); jk++)
                {
                    res += line.at(jk);
                }
                res += ")?";

                break;
            } else if (line.at(j) == "else") {
                res += ":";
                break;
            } else {
                res += "("+line.at(j)+")";
            }
        }
    }
    return res;


}
