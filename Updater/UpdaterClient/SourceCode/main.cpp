#include "updaterclient.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    UpdaterClient w;
    w.show();
    return a.exec();
}
