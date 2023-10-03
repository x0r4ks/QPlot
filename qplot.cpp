#include "qplot.h"
#include "ui_qplot.h"
#include <QMessageBox>

qplot::qplot(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::qplot)
{
    ui->setupUi(this);

    if (ui->sb_step->value() == 0) {
        ui->sb_step->setValue(0.01);
    }

    this->xBegin = ui->sb_xBegin->value();
    this->xEnd = ui->sb_xEnd->value();
    this->step = ui->sb_step->value();

    ui->settings_widget->setHidden(true);
    ui->plotWidget->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

}

qplot::~qplot()
{
    delete ui;
}

graph_vect qplot::create_graph(QString expression)
{

    graph_vect V;
    double X = xBegin;
    double var_X = X;
    N = (xEnd - xBegin) / step + 2;


    mu::Parser p;
    p.DefineVar("x", &var_X);
    p.SetExpr(expression.toStdString());

    for (X = xBegin; X <= xEnd; X += step) {
        var_X = X;
        V.x.push_back(X);
        V.y.push_back(p.Eval());
    }
    return V;

}

void qplot::on_btn_approw_clicked()
{
    if (ui->sb_step->value() == 0) {
        ui->sb_step->setValue(0.01);
    }

    this->xBegin = ui->sb_xBegin->value();
    this->xEnd = ui->sb_xEnd->value();
    this->step = ui->sb_step->value();

    ui->plotWidget->replot();
}


void qplot::on_btn_add_func_clicked()
{
    QString expression = ui->le_func->text();
    if (expression.isEmpty()) {
        QMessageBox::warning(this, tr("Warning"), tr("Expression is empty!"));
    } else {
        bool flag = true;
        try {
            mu::Parser s;
            double a = 1;
            s.DefineVar("x", &a);
            s.SetExpr(expression.toStdString());
            s.Eval();

        } catch(mu::Parser::exception_type &e) {
            flag = false;
            QString str = tr("The function you entered is not correct.");
            str = str + "\n" + QString::fromStdString(e.GetMsg());
            QMessageBox::critical(this, tr("Error"), str);
        }

        if (flag) {
            ui->le_func->clear();
            ui->lv_functions_view->addItem("y = " + expression);
            graphs.push_back(create_graph(expression));
            ui->plotWidget->addGraph();


            QPen pen;
            pen.setWidth(1);

            unsigned int red = 100, green = 100, blue = 250;
            red = ui->sb_red->value();
            green = ui->sb_green->value();
            blue = ui->sb_blue->value();
            colors.push_back(QColor(red, green, blue));

            for(int i = 0; i < ui->lv_functions_view->count(); i++) {
                ui->plotWidget->graph(i)->addData(graphs.at(i).x, graphs.at(i).y);
                pen.setColor(colors.at(i));
                ui->plotWidget->graph(i)->setPen(pen);
            }

            ui->plotWidget->replot();
        }
    }
}


void qplot::on_lv_functions_view_itemDoubleClicked(QListWidgetItem *item)
{
    int i = ui->lv_functions_view->currentRow();

    QListWidgetItem *it = ui->lv_functions_view->takeItem(ui->lv_functions_view->currentRow());
    delete it;

    this->graphs.removeAt(i);
    this->colors.removeAt(i);
    this->formuls.removeAt(i);
    ui->plotWidget->removeGraph(i);
    ui->plotWidget->replot();
}

