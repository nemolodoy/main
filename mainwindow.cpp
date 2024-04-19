#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "person.h"
#include "staticdata.h"
#include <QSqlQueryModel>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include <QSqlQuery>
#include <QStandardItemModel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qDebug() << "Доступные драйверы:" << QSqlDatabase::drivers();
    QSqlDatabase db = QSqlDatabase::addDatabase("QIBASE");
    db.setHostName("172.18.110.56");
    db.setDatabaseName("C:\\ACS\\Base\\ACS.fdb");
    db.setUserName("ADM");
    db.setPassword("700");
    db.setPort(3050);

//    QSqlDatabase daysDatabase = QSqlDatabase::addDatabase("QIBASE");
//    daysDatabase.setHostName("172.18.110.56");
//    daysDatabase.setDatabaseName("C:\\ACS\\Base\\ACS_LOG.fdb");
//    daysDatabase.setUserName("ADM");
//    daysDatabase.setPassword("700");
//    daysDatabase.setPort(3050);

//    if (daysDatabase.open()){
//        qDebug() << "Соежинение отрыто (динамеческие таблицы)";
//    }

    if(db.open()) {
        qDebug() << "Соединение открыто";
        auto query = new QSqlQuery(db);
        auto statement = QString("select persid, nio, fio, department, doljnost, mail ") +
                               QString("from personnel ") +
                               QString("where pgruppa <> 'ГОСТИ' ") +
                               QString("order by department, fio");
        qDebug() << statement;
        query->prepare(statement);
        if(query->exec()) {
            qDebug() << "Запрос выполнен";
            auto tableModel = new QSqlQueryModel();
            tableModel->setQuery(std::move(*query));
            tableModel->setHeaderData(1, Qt::Horizontal, QObject::tr("НИО"));
            tableModel->setHeaderData(2, Qt::Horizontal, QObject::tr("ФИО"));
            tableModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Отдел"));
            tableModel->setHeaderData(4, Qt::Horizontal, QObject::tr("Должность"));
            tableModel->setHeaderData(5, Qt::Horizontal, QObject::tr("Эл. почта"));

            ui->tableView->setModel(tableModel);
            ui->tableView->hideColumn(0);
            ui->tableView->resizeColumnsToContents();
            ui->tableView->show();
        } else {
            qDebug() << "проблема при выполнении запроса:";
            qDebug() << query->lastError().text();
        }
    } else {
        qDebug() << "проблема при открытии соединения с бд:";
        qDebug() << db.lastError().text();
    }

    auto departmentModel = new QStandardItemModel;
    for (auto i = StaticData::departments.constBegin(); i != StaticData::departments.constEnd(); i++){
        QStandardItem *item = new QStandardItem(i.key());
        item->setEditable(false);
        departmentModel->appendRow(item);
    }
    ui->department->setModel(departmentModel);
    ui->department->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    auto personId = ui->tableView->model()->data(index.sibling(index.row(), 0)).toString();
    auto query = new QSqlQuery(db);
    auto statement = QString("select persid, fio, department, doljnost, foto, mobile, auto, tabelnomer, nio ") +
                           QString("from personnel ") +
                           QString("where persid = :persid");
    query->prepare(statement);
    query->bindValue(":persid", personId);
    if(query->exec()) {
        Person dialog(this, query);
        dialog.exec();
    }
}

void MainWindow::on_calendarWidget_clicked(const QDate &date)
{

}


void MainWindow::on_department_clicked(const QModelIndex &index)
{
    auto department = index.data();
    auto dep = StaticData::departments.value(department.toString());
    auto query = new QSqlQuery(db);
    auto statement = QString("select persid, nio, fio, department, doljnost, mail ") +
            QString("from personnel ") +
            QString("where department like :dep ") +
            QString("order by department, fio");
    query->prepare(statement);
    query->bindValue(":dep",dep);
    if(query->exec()) {
        qDebug() << "Запрос выполнен";
        auto tableModel = new QSqlQueryModel();
        tableModel->setQuery(std::move(*query));
        tableModel->setHeaderData(1, Qt::Horizontal, QObject::tr("НИО"));
        tableModel->setHeaderData(2, Qt::Horizontal, QObject::tr("ФИО"));
        tableModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Отдел"));
        tableModel->setHeaderData(4, Qt::Horizontal, QObject::tr("Должность"));
        tableModel->setHeaderData(5, Qt::Horizontal, QObject::tr("Эл. почта"));

        ui->tableView->setModel(tableModel);
        ui->tableView->hideColumn(0);
        ui->tableView->resizeColumnsToContents();
        ui->tableView->show();
    } else {
        qDebug() << "проблема при выполнении запроса:";
        qDebug() << query->lastError().text();
    }
}


void MainWindow::on_previousDayButton_clicked()
{
    ui->calendarWidget->setSelectedDate(QDate::currentDate());
}

