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
#include <QColorDialog>
#include <QColor>

#include <QLocale>
#include <QTranslator>
#include "graph_vect.h"
#include <editdialog.h>
#include <editror.h>

#include "qcustomplot.h"
#include "qcustomplot.h"
#include "formulelement.h"
#include "promrules.h"

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

    void action_save();
    void action_save_as();
    void action_open();
    void action_create();

    void slotCustomMenuRequested(QPoint pos);

    void on_btn_not_math_clicked();
    void on_new_color_clicked();

    void on_action_Qt_triggered();

    void on_action_triggered();

    void on_action_2_triggered();

private:
    Ui::qplot *ui;
    int N, points_of_graph = -1;
    double xBegin, xEnd, step;
    bool isSaveProject = true;
    QColor new_graph_color;
    QTranslator trans;

    editDialog *editdialog;
    editror *Editor;

    QString project_name = " ";

    QList<QString> formuls;
    QList<QColor> colors;
    QList<graph_vect> graphs;

    void load(QString path);

    double plugin_load(const char *path, double x);

    graph_vect create_graph(QString expression);


    void update_color_button();
    void update_from_ram();

    QListWidgetItem* createElement(QString expression, QColor color, QString path_to_icon);
};
#endif // QPLOT_H
