#ifndef UPDATERSERVER_H
#define UPDATERSERVER_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QString>
#include <QFile>
#include <QTimer>
#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>


QT_BEGIN_NAMESPACE
namespace Ui { class UpdaterServer; }
QT_END_NAMESPACE

class UpdaterServer : public QWidget
{
    Q_OBJECT

public:
    UpdaterServer(QWidget *parent = nullptr);
    ~UpdaterServer();

    void sendData();

private slots:
    void on_ButtonSelect_clicked();

private:
    Ui::UpdaterServer *ui;

    QTcpServer *tcpServer;
    QTcpSocket *tcpSocket;

    QFile file;
    QString fileName;
    qint64 fileSize;
    qint64 sendSize;

    QTimer timer;


};
#endif // UPDATERSERVER_H
