#ifndef UPDATERCLIENT_H
#define UPDATERCLIENT_H

#include <QWidget>
#include <QTcpSocket>
#include <QHostAddress>
#include <QString>
#include <QFile>
#include <QDebug>
#include <QDir>


QT_BEGIN_NAMESPACE
namespace Ui { class UpdaterClient; }
QT_END_NAMESPACE

class UpdaterClient : public QWidget
{
    Q_OBJECT

public:
    UpdaterClient(QWidget *parent = nullptr);
    ~UpdaterClient();



private slots:
    void on_ButtonConnect_clicked();

    void on_ButtonDisconnect_clicked();

    void on_ButtonUpdate_clicked();

private:
    Ui::UpdaterClient *ui;

    QTcpSocket *tcpSocket;

    QFile file;
    QString fileName;
    qint64 fileSize;
    qint64 recvSize;

    bool isHead;
};
#endif // UPDATERCLIENT_H
