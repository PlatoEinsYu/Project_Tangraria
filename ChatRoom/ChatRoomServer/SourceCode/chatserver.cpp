#include "chatserver.h"
#include "ui_chatserver.h"

ChatServer::ChatServer(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChatServer)
{
    ui->setupUi(this);
    setWindowTitle("Tangraria聊天室 端口:31415");

    OnlineNumber = 0 ;

    tcpServer = new QTcpServer;

    tcpServer->listen(QHostAddress::Any,31415);
    //监听套接字 监听任何源 端口8888

    connect(tcpServer,&QTcpServer::newConnection,
            [=]()
    {
        if(OnlineNumber>=MAXNUM)
        {
            //如果人数已满 返回refuse
            QTcpSocket * refuseTcp = new QTcpSocket;
            refuseTcp = tcpServer->nextPendingConnection();
            QString info = QString("服务器人数已满！");
            refuseTcp->write(info.toUtf8().data());
            refuseTcp->disconnect();
            refuseTcp->close();
        }
        else
        {
            //如果人数没有满
            int FirstNull = getFirstNullSocket();
            tcpSocket[FirstNull] = new QTcpSocket;
            //套接字获取,连接上
            tcpSocket[FirstNull] = tcpServer->nextPendingConnection();
            //显示连接信息
            QString temp = QString("[%1:%2]成功连接").arg(tcpSocket[FirstNull]->peerAddress().toString()).arg(tcpSocket[FirstNull]->peerPort());
            QString info = QString("占用用户位[%1]位置").arg(FirstNull);
            ui->textEditOutput->append(info);
            ui->textEditOutput->append(temp);

            OnlineNumber++;

            connect(tcpSocket[FirstNull],&QTcpSocket::readyRead,
                    [=]()
            {
                QByteArray array = tcpSocket[FirstNull]->readAll();
                ui->textEditOutput->append(array);

                //向所有非NULL的tcpsocket发送此消息
                int i;
                for(i=0;i<MAXNUM;i++)
                {
                    if(tcpSocket[i]!=NULL)
                    {
                        tcpSocket[i]->write(array);
                    }
                }
            }
                    );


            connect(tcpSocket[FirstNull],&QTcpSocket::disconnected,
                    [=]()
            {
                QString text = QString("[%1:%2]已断开连接").arg(tcpSocket[FirstNull]->peerAddress().toString()).arg(tcpSocket[FirstNull]->peerPort());int i;
                for(i=0;i<MAXNUM;i++)
                {
                    if(tcpSocket[i]!=NULL)
                    {
                        tcpSocket[i]->write(text.toUtf8().data());
                    }
                }
                ui->textEditOutput->append(text);
                tcpSocket[FirstNull] = NULL;
                OnlineNumber--;
            }
                    );

        }


    }
            );




}



ChatServer::~ChatServer()
{
    delete ui;
}


int ChatServer::getFirstNullSocket()
{
    int i;
    for(i=0;i<MAXNUM;i++)
    {
        if(tcpSocket[i]==NULL)
        {
            return i;
        }
    }
    return -1;
}
