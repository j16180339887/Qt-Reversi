#include "SuperWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SuperWindow w;
    w.show();

    return a.exec();
}
