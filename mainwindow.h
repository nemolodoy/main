#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_tableView_clicked(const QModelIndex &index);

    void on_calendarWidget_clicked(const QDate &date);

    void on_department_clicked(const QModelIndex &index);

    void on_previousDayButton_clicked();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
};
#endif // MAINWINDOW_H
