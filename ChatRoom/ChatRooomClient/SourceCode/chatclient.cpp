#include "chatclient.h"
#include "ui_chatclient.h"

ChatClient::ChatClient(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChatClient)
{
    ui->setupUi(this);

    setWindowTitle("Tangraria聊天室客户端");

    tcpSocket = new QTcpSocket;
    ui->ButtonSend->setEnabled(false);
    ui->ButtonConnect->setEnabled(false);
    ui->ButtonDisconnect->setEnabled(false);

    connect(tcpSocket,&QTcpSocket::connected,
            [=]()
    {
        ui->textEditOutput->append("成功连接服务器");
    }
            );


    connect(tcpSocket,&QTcpSocket::readyRead,
            [=]()
    {
        QString text = tcpSocket->readAll();
        ui->textEditOutput->append(text);
    }
            );

    connect(tcpSocket,&QTcpSocket::disconnected,
            [=]()
    {
        ui->textEditOutput->append("已断开服务器连接");
    }
            );



}

ChatClient::~ChatClient()
{
    delete ui;
}


void ChatClient::on_ButtonConnect_clicked()
{
    if(tcpSocket == NULL)
    {
        tcpSocket = new QTcpSocket;
    }

    QString ip = ui->lineEditIP->text();
    qint16 port = ui->lineEditPort->text().toInt();

    tcpSocket->connectToHost(QHostAddress(ip),port);

    ui->ButtonSend->setEnabled(true);
    ui->ButtonDisconnect->setEnabled(true);
    ui->ButtonConnect->setEnabled(false);

    QString text = nickName;
    text.append(" 已连接服务器");
    tcpSocket->write(text.toUtf8().data());
}

void ChatClient::on_ButtonNickname_clicked()
{
    if(ui->lineEditNick->text()=="")
    {
        ui->textEditOutput->append("[INFO] 请输入昵称");
        return;
    }
    nickName = ui->lineEditNick->text();
    ui->ButtonNickname->setEnabled(false);
    ui->ButtonConnect->setEnabled(true);
}

void ChatClient::on_ButtonDisconnect_clicked()
{
    if(tcpSocket==NULL)
    {
        return;
    }

    tcpSocket->disconnectFromHost();
    tcpSocket->close();

    ui->ButtonSend->setEnabled(false);
    ui->ButtonDisconnect->setEnabled(false);
    ui->ButtonConnect->setEnabled(true);

}

void ChatClient::on_ButtonSend_clicked()
{
    QString text = ui->textEditInput->toPlainText();
    QString fulltext = nickName;
    fulltext.append(":").append(text);
    //ui->textEditOutput->append(fulltext);
    tcpSocket->write(fulltext.toUtf8().data());
    ui->textEditInput->clear();
}



