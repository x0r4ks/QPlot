#ifndef EDITROR_H
#define EDITROR_H

#include <QDialog>
#include <QPushButton>
#include <QString>

namespace Ui {
class editror;
}

class editror : public QDialog
{
    Q_OBJECT

public:
    explicit editror(QWidget *parent = nullptr);
    ~editror();

    void clear();
    QString getTernar();

private:
    Ui::editror *ui;
    QString coded;
};

#endif // EDITROR_H
