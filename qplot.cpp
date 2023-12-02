#include "qplot.h"
#include <QAction>
#include <QMessageBox>
#include <QProcess>
#include <QDebug>
#include "ui_qplot.h"
#include <dlfcn.h>

double qplot::plugin_load(const char* path, double x)
{
    void* obj = dlopen(path, RTLD_NOW);
    if(obj == NULL) {
        std::cerr << dlerror() << std::endl;
        return 0;
    }

    double (*f)(double);
    double res = 0;
    f = (double(*)(double))dlsym(obj, "func");

    if (f == NULL) {
        std::cerr << dlerror() << std::endl;
    } else {
        res = f(x);
    }
    dlclose(obj);
    return res;
}

qplot::qplot(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::qplot)
{
    ui->setupUi(this);
    QIcon icon(QFileInfo(qApp->arguments().at(0)).path() + "/icons/appicon.svg");
    QIcon btn_icon(QFileInfo(qApp->arguments().at(0)).path() + "/icons/pencil.svg");
    this->setWindowIcon(icon);
    ui->btn_not_math->setIcon(btn_icon);

    editdialog = new editDialog(this);
    Editor = new editror(this);

    connect(ui->actionSave, &QAction::triggered, this, &qplot::action_save);
    connect(ui->actionSave_As, &QAction::triggered, this, &qplot::action_save_as);
    connect(ui->actionOpen, &QAction::triggered, this, &qplot::action_open);
    connect(ui->actionCreate, &QAction::triggered, this, &qplot::action_create);

    ui->lv_functions_view->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->lv_functions_view,
            &QListWidget::customContextMenuRequested,
            this,
            &qplot::slotCustomMenuRequested);

    if (ui->sb_step->value() == 0) {
        ui->sb_step->setValue(0.01);
    }

    this->xBegin = ui->sb_xBegin->value();
    this->xEnd = ui->sb_xEnd->value();
    this->step = ui->sb_step->value();

    ui->settings_widget->setHidden(true);
    ui->plotWidget->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    ui->plotWidget->xAxis->setLabel("x");
    ui->plotWidget->yAxis->setLabel("y");

    ui->plotWidget->replot();

    if (qApp->arguments().size()>1) {
        this->load(qApp->arguments().at(1));
    }


    QApplication::removeTranslator(&trans);
    trans.load(QFileInfo(qApp->arguments().at(0)).path() + "/QPlot_" + QLocale::system().name(), ".");
    QApplication::installTranslator(&trans);
    ui->retranslateUi(this);
}

qplot::~qplot()
{
    delete ui;
}

double randint(double min = 0, double max = 100, double x = 0)
{
    srand(time(NULL) + rand() + x);
    return (double) (((int) rand() % (int) (max - min)) + min);
}

double toRad(double x)
{
    return x * (M_PI / 180);
}

double toDeg(double x)
{
    return x * (180 / M_PI);
}

graph_vect qplot::create_graph(QString expression)
{
    graph_vect V;
    double X = xBegin;
    double var_X = X;
    N = (xEnd - xBegin) / step + 2;

    mu::Parser p;
    // p.DefineFun("rnd", randint);

    // p.DefineFun("rad", toRad);
    // p.DefineFun("deg", toDeg);

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
            // s.DefineFun("rnd", randint);
            // s.DefineFun("rad", toRad);
            // s.DefineFun("deg", toDeg);
            s.DefineVar("x", &a);
            s.SetExpr(expression.toStdString());
            s.Eval();

        } catch (mu::Parser::exception_type &e) {
            flag = false;
            QString str = tr("The function you entered is not correct.");
            str = str + "\n" + QString::fromStdString(e.GetMsg());
            QMessageBox::critical(this, tr("Error"), str);
        }

        if (flag) {
            ui->le_func->clear();
            this->formuls.push_back(expression);
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

            for (int i = 0; i < ui->lv_functions_view->count(); i++) {
                ui->plotWidget->graph(i)->addData(graphs.at(i).x, graphs.at(i).y);
                pen.setColor(colors.at(i));
                ui->plotWidget->graph(i)->setPen(pen);
            }

            ui->plotWidget->replot();
            this->isSaveProject = false;
        }
    }
}

void qplot::action_save()
{
    /* Save format is json
     * {
     *  "formul": {"color": "255, 255, 255"}
     * }
    */
    if (project_name != " ") {
        QJsonDocument document;
        QJsonObject s;

        for (int i = 0; i < formuls.count(); i++) {
            QColor color = colors.at(i);
            QString color_str = "";
            color_str += QString::number(color.red()) + " " + QString::number(color.green()) + " "
                         + QString::number(color.blue());
            s.insert(formuls.at(i), color_str);
        }

        document.setObject(s);

        QFile f;
        f.setFileName(project_name);
        f.open(QIODevice::WriteOnly);
        f.write(document.toJson());
        f.close();
        this->isSaveProject = true;
    } else {
        this->action_save_as();
    }
}

void qplot::action_save_as()
{
    project_name = QFileDialog::getSaveFileName(this, tr("Save"), "", "QPLOT (*.qplot)");

    QJsonDocument document;
    QJsonObject s;

    for (int i = 0; i < formuls.count(); i++) {
        QColor color = colors.at(i);
        QString color_str = "";
        color_str += QString::number(color.red()) + " " + QString::number(color.green()) + " "
                     + QString::number(color.blue());
        s.insert(formuls.at(i), color_str);
    }

    document.setObject(s);

    QFile f;
    f.setFileName(project_name);
    f.open(QIODevice::WriteOnly);
    f.write(document.toJson());
    f.close();
    this->isSaveProject = true;
}

void qplot::load(QString path)
{
    QString new_project_name = path;

    if (this->project_name == new_project_name) {
        QMessageBox::information(this, tr("Info"), tr("This file is opened."));
    } else {
        project_name = new_project_name;
        QFile f;
        f.setFileName(project_name);
        f.open(QIODevice::ReadOnly);
        QJsonDocument document = QJsonDocument::fromJson(f.readAll());
        f.close();

        QJsonObject object = document.object();

        this->formuls.clear();
        this->colors.clear();
        this->graphs.clear();

        ui->plotWidget->clearGraphs();
        ui->plotWidget->clearItems();

        this->formuls = QList<QString>();
        this->colors = QList<QColor>();
        this->graphs = QList<graph_vect>();
        ui->lv_functions_view->clear();

        for (int i = 0; i < object.length(); i++) {
            ui->lv_functions_view->addItem("y = " + object.keys().at(i));

            this->formuls.push_back(object.keys().at(i));
            graphs.push_back(create_graph(object.keys().at(i)));
            QColor color;
            QList<QString> st = object.value(object.keys().at(i)).toString().split(' ');
            color.setRed(st.at(0).toInt());
            color.setGreen(st.at(1).toInt());
            color.setBlue(st.at(2).toInt());

            this->colors.push_back(color);
            ui->plotWidget->addGraph();
        }

        QPen pen;
        pen.setWidth(1);

        for (int i = 0; i < ui->lv_functions_view->count(); i++) {
            ui->plotWidget->graph(i)->addData(graphs.at(i).x, graphs.at(i).y);
            pen.setColor(colors.at(i));
            ui->plotWidget->graph(i)->setPen(pen);
        }
        ui->plotWidget->replot();
        this->isSaveProject = false;
    }
}

void qplot::action_open()
{
    QString new_project_name = QFileDialog::getOpenFileName(this, tr("Open"), "", "QPLOT (*.qplot)");

    if (this->project_name == new_project_name) {
        QMessageBox::information(this, tr("Info"), tr("This file is opened."));
    } else {
        project_name = new_project_name;
        QFile f;
        f.setFileName(project_name);
        f.open(QIODevice::ReadOnly);
        QJsonDocument document = QJsonDocument::fromJson(f.readAll());
        f.close();

        QJsonObject object = document.object();

        this->formuls.clear();
        this->colors.clear();
        this->graphs.clear();

        ui->plotWidget->clearGraphs();
        ui->plotWidget->clearItems();

        this->formuls = QList<QString>();
        this->colors = QList<QColor>();
        this->graphs = QList<graph_vect>();
        ui->lv_functions_view->clear();

        for (int i = 0; i < object.length(); i++) {
            if (QFileInfo(object.keys().at(i)).exists()) {
                ui->lv_functions_view->addItem("y = " + QFileInfo(object.keys().at(i)).fileName());
            } else {
                ui->lv_functions_view->addItem("y = " + object.keys().at(i));
            }

            this->formuls.push_back(object.keys().at(i));

            if (QFileInfo(object.keys().at(i)).exists()) {
                graphs.push_back(create_py_graph(object.keys().at(i)));
            } else {
                graphs.push_back(create_graph(object.keys().at(i)));
            }

            QColor color;
            QList<QString> st = object.value(object.keys().at(i)).toString().split(' ');
            color.setRed(st.at(0).toInt());
            color.setGreen(st.at(1).toInt());
            color.setBlue(st.at(2).toInt());

            this->colors.push_back(color);
            ui->plotWidget->addGraph();
        }

        QPen pen;
        pen.setWidth(1);

        for (int i = 0; i < ui->lv_functions_view->count(); i++) {
            ui->plotWidget->graph(i)->addData(graphs.at(i).x, graphs.at(i).y);
            pen.setColor(colors.at(i));
            ui->plotWidget->graph(i)->setPen(pen);
        }
        ui->plotWidget->replot();
        this->isSaveProject = false;
    }
}

void qplot::action_create()
{
    if (this->isSaveProject) {
        this->project_name = " ";

        this->formuls.clear();
        this->colors.clear();
        this->graphs.clear();

        ui->plotWidget->clearGraphs();
        ui->plotWidget->clearItems();

        this->formuls = QList<QString>();
        this->colors = QList<QColor>();
        this->graphs = QList<graph_vect>();
        ui->lv_functions_view->clear();
        ui->plotWidget->replot();
    } else {
        if (QMessageBox::question(this, tr("Warning"), tr("This project not save.\nSave?"))
            == QMessageBox::Yes) {
            this->action_save_as();

            this->project_name = " ";

            this->formuls.clear();
            this->colors.clear();
            this->graphs.clear();

            ui->plotWidget->clearGraphs();
            ui->plotWidget->clearItems();

            this->formuls = QList<QString>();
            this->colors = QList<QColor>();
            this->graphs = QList<graph_vect>();
            ui->lv_functions_view->clear();
            ui->plotWidget->replot();
        }
    }
}

void qplot::slotCustomMenuRequested(QPoint pos)
{
    QMenu *menu = new QMenu(this);
    QAction *remove_sub_action = new QAction(tr("Remove"), this);
    QAction *edit_sub_menu = new QAction(tr("Edit"), this);

    menu->addAction(remove_sub_action);
    menu->addAction(edit_sub_menu);

    connect(remove_sub_action, &QAction::triggered, this, [this] {
        int i = ui->lv_functions_view->currentRow();

        QListWidgetItem *it = ui->lv_functions_view->takeItem(ui->lv_functions_view->currentRow());

        delete it;

        this->graphs.removeAt(i);
        this->colors.removeAt(i);
        this->formuls.removeAt(i);
        ui->plotWidget->removeGraph(i);
        ui->plotWidget->replot();
        this->isSaveProject = false;
    });

    connect(edit_sub_menu, &QAction::triggered, this, [this] {
        int i = ui->lv_functions_view->currentRow();
        bool f = true;
        editdialog->setColor(this->colors.at(i));
        editdialog->setFormul(this->formuls.at(i));
        if (editdialog->exec()) {
            try {
                if (!QFileInfo(editdialog->getFormul()).exists() && \
                    (QFileInfo(editdialog->getFormul()).path().trimmed() == ".so" || QFileInfo(editdialog->getFormul()).path().trimmed() == ".dll")) {
                    mu::Parser s;

                    double a = 1;
                    // s.DefineFun("rnd", randint);
                    // s.DefineFun("rad", toRad);
                    // s.DefineFun("deg", toDeg);

                    s.DefineVar("x", &a);
                    s.SetExpr(editdialog->getFormul().toStdString());
                    s.Eval();
                   }

            } catch (mu::Parser::exception_type &e) {
                f = false;
                QString str = tr("The function you entered is not correct.");
                str = str + "\n" + QString::fromStdString(e.GetMsg());
                QMessageBox::critical(this, tr("Error"), str);
            }
            if (f) {
                this->colors[i] = editdialog->getColor();
                this->formuls[i] = editdialog->getFormul();



                if (QFileInfo(this->formuls.at(i)).exists() || \
                    (QFileInfo(editdialog->getFormul()).path().trimmed() == ".so" || QFileInfo(editdialog->getFormul()).path().trimmed() == ".dll")) {
                    this->graphs[i] = create_py_graph(this->formuls.at(i));
                } else {
                    this->graphs[i] = create_graph(this->formuls.at(i));
                }

                if (QFileInfo(this->formuls.at(i)).exists() || \
                    (QFileInfo(editdialog->getFormul()).path().trimmed() == ".so" || QFileInfo(editdialog->getFormul()).path().trimmed() == ".dll")) {
                    ui->lv_functions_view->item(i)->setText("y = " + QFileInfo(this->formuls.at(i)).fileName());
                } else {
                    ui->lv_functions_view->item(i)->setText("y = " + this->formuls.at(i));
                }



                QPen pen;
                pen.setWidth(1);

                ui->plotWidget->graph(i)->setData(this->graphs.at(i).x, this->graphs.at(i).y);

                pen.setColor(colors.at(i));
                ui->plotWidget->graph(i)->setPen(pen);

                ui->plotWidget->replot();
                this->isSaveProject = false;
            }
        }
    });

    menu->popup(ui->lv_functions_view->mapToGlobal(pos));
}

void qplot::on_btn_not_math_clicked(bool checked)
{
    Editor->clear();

    if (Editor->exec() == 1) {
        QString term = Editor->getTernar();

        ui->le_func->setText(term);
    }
}

void qplot::on_actionload_python_script_triggered()
{
    QString so_lib_path = QFileDialog::getOpenFileName(this, tr("Open"), "", "SO (*.so);; DLL (*.dll)");



    ui->le_func->clear();
    this->formuls.push_back(so_lib_path);
    ui->lv_functions_view->addItem("y = " + QFileInfo(so_lib_path).fileName());
    graphs.push_back(create_py_graph(so_lib_path));
    ui->plotWidget->addGraph();

    QPen pen;
    pen.setWidth(1);

    unsigned int red = 100, green = 100, blue = 250;
    red = ui->sb_red->value();
    green = ui->sb_green->value();
    blue = ui->sb_blue->value();
    colors.push_back(QColor(red, green, blue));

    for (int i = 0; i < ui->lv_functions_view->count(); i++) {
        ui->plotWidget->graph(i)->addData(graphs.at(i).x, graphs.at(i).y);
        pen.setColor(colors.at(i));
        ui->plotWidget->graph(i)->setPen(pen);
    }

    ui->plotWidget->replot();
    this->isSaveProject = false;

}

graph_vect qplot::create_py_graph(QString so_lib_path)
{
    graph_vect V;
    double X = xBegin;
    double var_X = X;
    N = (xEnd - xBegin) / step + 2;

    void* obj = dlopen(so_lib_path.toStdString().c_str(), RTLD_NOW);
    if(obj == NULL) {
        QMessageBox::critical(this, "Critical error", "dlerror, you plugin not support");
    }

    double (*f)(double);
    f = (double(*)(double))dlsym(obj, "func");

    if (f == NULL) {
       QMessageBox::critical(this, "Critical error", "dlerror, you plugin not support");
       exit(-1);
    } else {
        for (X = xBegin; X <= xEnd; X += step) {
            var_X = X;
            V.x.push_back(X);
            V.y.push_back(f(X));
        }
    }

    dlclose(obj);
    return V;
}

