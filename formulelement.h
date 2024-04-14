#ifndef FORMULELEMENT_H
#define FORMULELEMENT_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

class FormulElement : public QWidget
{
    Q_OBJECT
public:
    explicit FormulElement(QWidget *parent = nullptr, QColor color = QColor::fromRgb(255, 0, 255), QString text = "Empty");

    QColor img_color;

    void setColor(QColor color);

signals:

private:
    QLabel *lable;
    QPushButton *btn;
    QHBoxLayout *layout;
};

#endif // FORMULELEMENT_H
