#include "qplot.h"

#include <QAction>
#include <QDebug>
#include <QMessageBox>
#include <QProcess>
#include "ui_qplot.h"


void qplot::update_color_button() {
    QString style = "background-color: rgb(";
    style += QString::number(new_graph_color.red());
    style += ",";
    style += QString::number(new_graph_color.green());
    style += ",";
    style += QString::number(new_graph_color.blue());
    style += ")";
    ui->new_color->setStyleSheet(style);
}

QListWidgetItem* qplot::createElement(QString expression, QColor color, QString path_to_icon)
{
    QListWidgetItem *a = new QListWidgetItem();
    a->setBackground(QBrush(color));
    a->setText("y = " + expression);
    a->setIcon(QIcon(QFileInfo(qApp->arguments().at(0)).path() + "/icons/" + path_to_icon));
    return a;
}

qplot::qplot(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::qplot)
{
    ui->setupUi(this);


    // QFile file("stylesheet.qss");
    // if (file.open(QFile::ReadOnly)) {
    //     QString styleSheet = QLatin1String(file.readAll());

    //     qApp->setStyleSheet(styleSheet);
    // }


    QString icon_prefis = QFileInfo(qApp->arguments().at(0)).path() + "/icons/";

#ifdef linux
    this->setWindowIcon(QIcon(icon_prefis + "appicon.svg"));
    ui->btn_not_math->setIcon(QIcon(icon_prefis + "pencil.svg"));

    ui->actionCreate->setIcon(QIcon(icon_prefis + "ui/document-new.svg"));
    ui->actionOpen->setIcon(QIcon(icon_prefis + "ui/document-open.svg"));
    ui->actionSave->setIcon(QIcon(icon_prefis + "ui/document-save.svg"));
    ui->actionSave_As->setIcon(QIcon(icon_prefis + "ui/document-save-as.svg"));
    ui->action_Qt->setIcon(QIcon(icon_prefis + "ui/qt.svg"));
    ui->action->setIcon(QIcon(icon_prefis+"ui/info.svg"));
    ui->action_2->setIcon(QIcon(icon_prefis+"ui/info.svg"));
#endif

#ifdef _WIN32
    this->setWindowIcon(QIcon(icon_prefis + "win/appicon.png"));
    ui->btn_not_math->setIcon(QIcon(icon_prefis + "win/pencil.png"));

    ui->actionCreate->setIcon(QIcon(icon_prefis + "win/ui/document-new.png"));
    ui->actionOpen->setIcon(QIcon(icon_prefis + "win/ui/document-open.png"));
    ui->actionSave->setIcon(QIcon(icon_prefis + "win/ui/document-save.png"));
    ui->actionSave_As->setIcon(QIcon(icon_prefis + "win/ui/document-save-as.png"));
    ui->action_Qt->setIcon(QIcon(icon_prefis + "win/ui/qt.png"));
    ui->action->setIcon(QIcon(icon_prefis+"win/ui/info.png"));
    ui->action_2->setIcon(QIcon(icon_prefis+"win/ui/info.png"));
#endif


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

    if (qApp->arguments().size() > 1) {
        this->load(qApp->arguments().at(1));
    }

    QApplication::removeTranslator(&trans);
    bool res = trans.load(QFileInfo(qApp->arguments().at(0)).path() + "/QPlot_" + QLocale::system().name()+".qm", ".");

    if (!res) {
        QMessageBox::critical(this, tr("Error"), tr("Translations file not exist or not loading."));
    }

    QApplication::installTranslator(&trans);
    ui->retranslateUi(this);

    new_graph_color = QColor::fromRgb(100, 100, 255);
    update_color_button();
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

        } catch (mu::Parser::exception_type &e) {
            flag = false;
            QString str = tr("The function you entered is not correct.");
            str = str + "\n" + QString::fromStdString(e.GetMsg());
            QMessageBox::critical(this, tr("Error"), str);
        }

        if (flag) {
            ui->le_func->clear();
            this->formuls.push_back(expression);

            QString ptoIcon = "";
#ifdef linux
            ptoIcon = "function-svgrepo-com.svg";
#endif

#ifdef _WIN32
            ptoIcon = "win/function-svgrepo-com.png");
#endif
            ui->lv_functions_view->addItem(createElement(expression, new_graph_color, ptoIcon));

            graphs.push_back(create_graph(expression));
            ui->plotWidget->addGraph();

            QPen pen;
            pen.setWidth(1);



            colors.push_back(new_graph_color);

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

            QString ptoIcon = "";

#ifdef linux
            ptoIcon = "function-svgrepo-com.svg";
#endif

#ifdef _WIN32
            ptoIcon = "win/function-svgrepo-com.png";
#endif

            this->formuls.push_back(object.keys().at(i));
            graphs.push_back(create_graph(object.keys().at(i)));
            QColor color;
            QList<QString> st = object.value(object.keys().at(i)).toString().split(' ');
            color.setRed(st.at(0).toInt());
            color.setGreen(st.at(1).toInt());
            color.setBlue(st.at(2).toInt());

            this->colors.push_back(color);


            ui->lv_functions_view->addItem(createElement(object.keys().at(i), color, ptoIcon));
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
        QString expression = "";
        QString ptIcon = "";

        for (int i = 0; i < object.length(); i++) {

            if (QFileInfo(object.keys().at(i)).exists()) {
                expression = QFileInfo(object.keys().at(i)).fileName();
            } else {
                expression = object.keys().at(i);
            }

#ifdef linux
            ptIcon = "function-svgrepo-com.svg";
#endif

#ifdef _WIN32
            ptIcon = "win/function-svgrepo-com.png")     ;
#endif

            this->formuls.push_back(object.keys().at(i));

            graphs.push_back(create_graph(object.keys().at(i)));


            QColor color;
            QList<QString> st = object.value(object.keys().at(i)).toString().split(' ');
            color.setRed(st.at(0).toInt());
            color.setGreen(st.at(1).toInt());
            color.setBlue(st.at(2).toInt());


            ui->lv_functions_view->addItem(createElement(expression, color, ptIcon));

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
    QString icon_prefis = QFileInfo(qApp->arguments().at(0)).path() + "/icons/";

    QMenu *menu = new QMenu(this);
    QAction *remove_sub_action = new QAction(tr("Remove"), this);
    QAction *edit_sub_menu = new QAction(tr("Edit"), this);
    // QAction *x_inter = new QAction(tr("Roots"), this);

#ifdef linux
    remove_sub_action->setIcon(QIcon(icon_prefis+"ui/edit-delete.svg"));
    edit_sub_menu->setIcon(QIcon(icon_prefis+"ui/document-edit.svg"));
#endif

#ifdef _WIN32
    remove_sub_action->setIcon(QIcon(icon_prefis+"win/ui/edit-delete.png"));
    edit_sub_menu->setIcon(QIcon(icon_prefis+"win/ui/document-edit.png"));
#endif

    menu->addAction(edit_sub_menu);
    menu->addAction(remove_sub_action);

    connect(remove_sub_action, &QAction::triggered, this, [this] {
        int i = ui->lv_functions_view->currentRow();

        if (i != -1) {

            QListWidgetItem *it = ui->lv_functions_view->takeItem(ui->lv_functions_view->currentRow());

            delete it;

            this->graphs.removeAt(i);
            this->colors.removeAt(i);
            this->formuls.removeAt(i);
            ui->plotWidget->removeGraph(i);
            ui->plotWidget->replot();
            this->isSaveProject = false;
        } else {
            QMessageBox::warning(this, tr("Warning"), tr("Nothing to delete"));
        }
    });

    connect(edit_sub_menu, &QAction::triggered, this, [this] {
        int i = ui->lv_functions_view->currentRow();

        if (i != -1) {
            bool f = true;
            editdialog->setColor(this->colors.at(i));
            editdialog->setFormul(this->formuls.at(i));
            if (editdialog->exec()) {
                try {
                    if (!QFileInfo(editdialog->getFormul()).exists()
                        && (QFileInfo(editdialog->getFormul()).path().trimmed() == ".so"
                            || QFileInfo(editdialog->getFormul()).path().trimmed() == ".dll")) {
                        mu::Parser s;

                        double a = 1;
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

                    QPen pen;
                    pen.setWidth(1);
                    pen.setColor(colors.at(i));

                    this->graphs[i] = create_graph(this->formuls.at(i));

                    ui->lv_functions_view->item(i)->setText("y = " + this->formuls.at(i));
#ifdef linux
                    ui->lv_functions_view->item(i)->setIcon(QIcon(QFileInfo(qApp->arguments().at(0)).path() + "/icons/function-svgrepo-com.svg"));
#endif
#ifdef _WIN32
                    ui->lv_functions_view->item(i)->setIcon(QIcon(QFileInfo(qApp->arguments().at(0)).path() + "/icons/win/function-svgrepo-com.png"));
#endif
                    ui->lv_functions_view->item(i)->setBackground(QBrush(editdialog->getColor()));

                    ui->plotWidget->graph(i)->setData(this->graphs.at(i).x, this->graphs.at(i).y);
                    ui->plotWidget->graph(i)->setPen(pen);
                    ui->plotWidget->replot();
                    this->isSaveProject = false;
                }
            }
        } else {
            QMessageBox::warning(this, tr("Warning"), tr("Nothing to change"));
        }
    });

    menu->popup(ui->lv_functions_view->mapToGlobal(pos));
}

void qplot::on_btn_not_math_clicked()
{
    Editor->clear();

    if (Editor->exec() == 1) {
        QString term = Editor->getTernar();

        ui->le_func->setText(term);
    }
}


void qplot::on_new_color_clicked()
{
    new_graph_color = QColorDialog::getColor(new_graph_color, this, tr("Select color"));

    update_color_button();

}



void qplot::on_action_Qt_triggered()
{
    QMessageBox::aboutQt(this);
}


void qplot::on_action_triggered()
{
    QMessageBox::about(this, tr("Information about programm"),
                       tr("This program is the product of an individual project. "
                          "It is under the GPLv3 license dated June 29, 2007. "
                          "The source code of the program is available on the github repository (https://github.com/Thunderb0rn/QPlot/)."
                          " The code can be used for private purposes, distributed, modified, but making it private is prohibited by the license. "
                          "The program uses Qt5 for Windows and Qt5/6 for Linux."));
}


void qplot::on_action_2_triggered()
{
    PromRules *p = new PromRules();

    p->show();
}


void qplot::update_from_ram()
{
    if (points_of_graph != -1) {
        ui->plotWidget->removeGraph(points_of_graph);
    }
    ui->plotWidget->clearGraphs();
    ui->plotWidget->clearItems();

    for (int i = 0; i < graphs.size(); i++)
    {
        QPen pen;
        pen.setWidth(1);

        ui->plotWidget->addGraph();

        ui->plotWidget->graph(i)->setData(this->graphs.at(i).x, this->graphs.at(i).y);
        pen.setColor(colors.at(i));
        ui->plotWidget->graph(i)->setPen(pen);
    }

    ui->plotWidget->replot();
    ui->plotWidget->update();

}

