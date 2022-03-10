#include "widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget MainWidget;
    MainWidget.show();
    return a.exec();
}
