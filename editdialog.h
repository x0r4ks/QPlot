#ifndef EDITDIALOG_H
#define EDITDIALOG_H

#include <QColor>
#include <QDialog>
#include <QPushButton>
#include <QString>

namespace Ui {
class editDialog;
}

class editDialog : public QDialog
{
    Q_OBJECT

public:
    explicit editDialog(QWidget *parent = nullptr);
    ~editDialog();

    QString getFormul();
    QColor getColor();

    void setFormul(QString formul);
    void setColor(QColor color);

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::editDialog *ui;
    QString formul = "";
    QColor color = QColor(0, 0, 0);
};

#endif // EDITDIALOG_H
