#ifndef ROOTSFORMS_H
#define ROOTSFORMS_H

#include <QWidget>

namespace Ui {
class RootsForms;
}

class RootsForms : public QWidget
{
    Q_OBJECT

public:
    explicit RootsForms(QWidget *parent = nullptr);
    ~RootsForms();

    void setText(QString text);

private:
    Ui::RootsForms *ui;
};

#endif // ROOTSFORMS_H
