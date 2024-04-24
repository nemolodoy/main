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
    staticDb = QSqlDatabase::addDatabase("QIBASE", "staticInfoDatabase");
    staticDb.setHostName("172.18.110.56");
    staticDb.setDatabaseName("C:\\ACS\\Base\\ACS.fdb");
    staticDb.setUserName("ADM");
    staticDb.setPassword("700");
    staticDb.setPort(3050);

    daysDb = QSqlDatabase::addDatabase("QIBASE", "daysInfoDatabase");
    daysDb.setHostName("172.18.110.56");
    daysDb.setDatabaseName("C:\\ACS\\Base\\ACS_LOG.fdb");
    daysDb.setUserName("ADM");
    daysDb.setPassword("700");
    daysDb.setPort(3050);

    if (daysDb.open()){
        qDebug() << "Соединение отрыто (динамеческие таблицы)";
    }

    if(staticDb.open()) {
        qDebug() << "Соединение открыто (статичесикие таблицы)";
        auto query = new QSqlQuery(staticDb);
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
        qDebug() << staticDb.lastError().text();
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
    auto query = new QSqlQuery(staticDb);
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
    auto query = new QSqlQuery(staticDb);
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


void MainWindow::on_dayInfo_toggled(bool checked)
{
        if (checked){
            QString result;
            QDate date = ui->calendarWidget->selectedDate();
            auto datePattern = "%"+date.toString("yyyyMMdd")+"%";
            QSqlQuery query = QSqlQuery(daysDb);
            auto tableStatement = QString("SELECT a.RDB$RELATION_NAME "
                                     "FROM RDB$RELATIONS a "
                                     "WHERE COALESCE(RDB$SYSTEM_FLAG, 0) = 0 AND a.RDB$RELATION_NAME LIKE :date");
            query.prepare(tableStatement);
            query.bindValue(":date", datePattern);
            query.exec();

            if (query.first()){
                result = query.value(0).toString();
                qDebug() << result;
            }
            QSqlQuery dataQuery = QSqlQuery(daysDb);
            QString infoStatement = QString("SELECT pers_id, fio, timecont, numobj ")
                    .append("FROM " + result + " ")
                    .append("WHERE fio IS NOT NULL ")
                    .append("ORDER BY fio");

            qDebug() << infoStatement;
            dataQuery.prepare(infoStatement);
            if (dataQuery.exec()){
                qDebug() << "Запрос выполнен";
                auto tableModel = new QSqlQueryModel();
                tableModel->setQuery(std::move(dataQuery));
                tableModel->setHeaderData(0, Qt::Horizontal, QObject::tr("КОД"));
                tableModel->setHeaderData(1, Qt::Horizontal, QObject::tr("ФИО"));
                tableModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Время входа/выхода"));
                tableModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Индекс проходной"));

                ui->tableView->setModel(tableModel);
                ui->tableView->hideColumn(0);
                ui->tableView->resizeColumnsToContents();
                ui->tableView->show();
            } else {
                qDebug() << "проблема при выполнении запроса:";
                qDebug() << dataQuery.lastError().text();
            }
        }
    }

