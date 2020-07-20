#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QString>
#include <QByteArray>

#define MAXNUM 5


QT_BEGIN_NAMESPACE
namespace Ui { class ChatServer; }
QT_END_NAMESPACE

class ChatServer : public QWidget
{
    Q_OBJECT

public:
    ChatServer(QWidget *parent = nullptr);
    ~ChatServer();

    int getFirstNullSocket();

private:
    Ui::ChatServer *ui;

    int OnlineNumber;//在线人数

    QTcpServer * tcpServer;
    QTcpSocket * tcpSocket[MAXNUM]={NULL};


};
#endif // CHATSERVER_H
