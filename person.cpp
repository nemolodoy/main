#include "person.h"
#include "ui_person.h"
#include <QSqlQuery>
#include <QByteArray>
#include <QPixmap>
#include <QDate>

Person::Person(QWidget *parent, QSqlQuery* query) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    query->first();
    QByteArray byteArray = query->value("FOTO").toByteArray();
    QPixmap pixmap;
    pixmap.loadFromData(byteArray);
    if(!pixmap.isNull()) {
        ui->fotoLabel->setPixmap(pixmap.scaled(120, 160, Qt::KeepAspectRatio));
    }
    ui->fioLabel->setText(query->value("FIO").toString());
    auto mobile =  query->value("MOBILE").toString();
    ui->mobilePhoneLabel->setText(mobile.isEmpty() ? "-" : mobile);
    auto table = query->value("TABELNOMER").toString();
    auto cabinet = table.mid(table.lastIndexOf('.') + 1, 4);
    ui->cabinetLabel->setText(cabinet.isEmpty() ? "-" : cabinet);
    auto phone = table.mid(table.indexOf('.') + 1, 3);
    ui->phoneLabel->setText(phone.isEmpty() ? "-" : phone);
    auto vehicle = query->value("AUTO").toString();
    ui->transportLabel->setText(vehicle.isEmpty() ? "-" : vehicle);
    auto dep = query->value("NIO").toString();
    if(!dep.isEmpty()){
        dep = dep.append(", ");
    }
    dep = dep.append(query->value("DEPARTMENT").toString());
    ui->otdelLabel->setText(dep);
    ui->doljnostLabel->setText(query->value("DOLJNOST").toString().toLower());
}
