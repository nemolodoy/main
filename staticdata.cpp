#include "staticdata.h"
#include <iostream>

QMap<QString, QString> StaticData::departments = {
    {"01 Управление","ОТДЕЛ 01"},
    {"08 Центральный склад","ОТДЕЛ 08"},
    {"09 Отдел безопасности","ОТДЕЛ 09"},
    {"13 Комплексный отдел разработки ПО","ОТДЕЛ 13"},
    {"14 Комплексный отдел разработки ПО","ОТДЕЛ 14"},
    {"20 Отдел разработки ПО тактических задач РЛС","ОТДЕЛ 20"},
    {"21 Отдел разработки аппартано-ориентированного ПО","ОТДЕЛ 21"},
    {"22 Отдел разработки иммитационных моделей","ОТДЕЛ 22"},
    {"23 Отдел разработки ПО обработки и визуализации данных","ОТДЕЛ 23"},
    {"24 Отдел разработки системного ПО","ОТДЕЛ 24"},
    {"25 Отдел разработки встроенного ПО","ОТДЕЛ 25"},
    {"29 Отдел разработки ПО ПЛК", "ОТДЕЛ 29"},
    {"31 Отдел авторского набора и сопровождения", "ОТДЕЛ 31"},
    {"34 Отдел разработки ПО для Unix-платформ", "ОТДЕЛ 34"},
    {"63 Отдел стандартизации и качества", "ОТДЕЛ 63"},
    {"65 Отдел автоматизации", "ОТДЕЛ 65"},
    {"73 Отдел комплексного обслуживания зданий и сооружений", "ОТДЕЛ 73"},
    {"75 Отдел эксплуатации ЭВТ и связи", "ОТДЕЛ 75"},
    {"81 Канцелярия", "ОТДЕЛ 81"},
    {"82 Отдел технического обеспечения", "ОТДЕЛ 82"},
    {"89 Отдел кадров", "ОТДЕЛ 89"},
    {"91 Бухгалтерско-финансовый отдел", "ОТДЕЛ 91"},
    {"92 Отдел экономического планирования", "ОТДЕЛ 92"},
    {"Все сотрудники", "ОТДЕЛ %%"}
};