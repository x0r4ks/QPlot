#ifndef QPLOT_H
#define QPLOT_H

#include <QMainWindow>
#include <QVector>
#include <QListWidgetItem>
#include <QList>
#include <muparser/muParser.h>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "graph_vect.h"
#include <editdialog.h>



QT_BEGIN_NAMESPACE
namespace Ui { class qplot; }
QT_END_NAMESPACE

class qplot : public QMainWindow
{
    Q_OBJECT

public:
    qplot(QWidget *parent = nullptr);
    ~qplot();

private slots:
    void on_btn_approw_clicked();

    void on_btn_add_func_clicked();

//    void on_lv_functions_view_itemDoubleClicked(QListWidgetItem *item);

    void action_save();
    void action_save_as();
    void action_open();
    void action_create();

    void slotCustomMenuRequested(QPoint pos);

private:
    Ui::qplot *ui;
    int N;
    double xBegin, xEnd, step;
    bool isSaveProject = true;

    editDialog *editdialog;

    QString project_name = " ";

    QList<QString> formuls;
    QList<QColor> colors;
    QList<graph_vect> graphs;

    graph_vect create_graph(QString expression);




};
#endif // QPLOT_H
