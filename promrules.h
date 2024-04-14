#ifndef PROMRULES_H
#define PROMRULES_H

#include <QWidget>

namespace Ui {
class PromRules;
}

class PromRules : public QWidget
{
    Q_OBJECT

public:
    explicit PromRules(QWidget *parent = nullptr);
    ~PromRules();

private slots:
    void on_ok_btn_clicked();

private:
    Ui::PromRules *ui;
};

#endif // PROMRULES_H
