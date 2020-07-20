#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <QWidget>
#include <QTcpSocket>
#include <QByteArray>
#include <QHostAddress>
#include <QString>
#include <QKeyEvent>
#include <QObject>

QT_BEGIN_NAMESPACE
namespace Ui { class ChatClient; }
QT_END_NAMESPACE

class ChatClient : public QWidget
{
    Q_OBJECT

public:
    ChatClient(QWidget *parent = nullptr);
    ~ChatClient();


protected:



private slots:
    void on_ButtonConnect_clicked();

    void on_ButtonNickname_clicked();

    void on_ButtonDisconnect_clicked();

    void on_ButtonSend_clicked();

private:
    Ui::ChatClient *ui;

    QTcpSocket *tcpSocket;
    QString nickName;

};
#endif // CHATCLIENT_H
