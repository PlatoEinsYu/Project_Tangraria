#include "updaterserver.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    UpdaterServer w;
    w.show();
    return a.exec();
}
