#include "homemenu.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    HomeMenu menu;
    menu.show();
    return a.exec();
}
