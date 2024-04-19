#ifndef PERSON_H
#define PERSON_H

#include <QDialog>
#include <ui_person.h>
#include <QSqlQuery>
QT_BEGIN_NAMESPACE
namespace Ui {
  class Person;
}
QT_END_NAMESPACE

class Person : public QDialog
{
    Q_OBJECT

public:
    explicit Person(QWidget *parent = nullptr, QSqlQuery* query = nullptr);
    virtual ~Person(){};

private:
    Ui::Dialog *ui;
};

#endif // PERSON_H
