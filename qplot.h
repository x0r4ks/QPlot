#ifndef QPLOT_H
#define QPLOT_H

#include <QList>
#include <QListWidgetItem>
#include <QMainWindow>
#include <QVector>
#include <muparser/muParser.h>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "graph_vect.h"
#include <editdialog.h>
#include <editror.h>
#include <QTranslator>
#include <QLocale>

QT_BEGIN_NAMESPACE
namespace Ui {
class qplot;
}
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

    void on_btn_not_math_clicked(bool checked);

    void on_actionload_python_script_triggered();

private:
    Ui::qplot *ui;
    int N;
    double xBegin, xEnd, step;
    bool isSaveProject = true;
    QTranslator trans;

    editDialog *editdialog;
    editror *Editor;

    QString project_name = " ";

    QList<QString> formuls;
    QList<QColor> colors;
    QList<graph_vect> graphs;

    void load(QString path);

    double plugin_load(const char* path, double x);

    graph_vect create_graph(QString expression);
    graph_vect create_py_graph(QString python_script);
};
#endif // QPLOT_H
