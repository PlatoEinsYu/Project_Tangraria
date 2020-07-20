#include "updaterserver.h"
#include "ui_updaterserver.h"

UpdaterServer::UpdaterServer(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UpdaterServer)
{
    ui->setupUi(this);

    tcpServer = new QTcpServer(this);
    tcpServer->listen(QHostAddress::Any,31415);//监听8888端口所有p频段
    setWindowTitle("服务器端口为：31415");

    connect(tcpServer,&QTcpServer::newConnection,
            [=]()
        {
            tcpSocket = tcpServer->nextPendingConnection();
            //通信套接字

            QString ip = tcpSocket->peerAddress().toString();
            qint16 port = tcpSocket->peerPort();

            QString str = QString("[%1:%2]成功连接").arg(ip).arg(port);
            ui->textEditRead->setText(str);//显示成功连接


            //这里还有一些操作

            //这时候将 通信套接字的信号绑定

            connect(tcpSocket,&QTcpSocket::disconnected,
                [=]()
            {
                ui->textEditRead->append("客户端断开连接");
                tcpSocket = NULL;
            }

                    );

            connect(tcpSocket,&QTcpSocket::readyRead,
                    [=]()
            {
                //由一个信号激活
                QString text = tcpSocket->readAll();

                if(QString::compare(text,"Download") == 0)
                {
                    file.open(QIODevice::ReadOnly);
                    qDebug()<<"确认了信号";
                    //发送文件头信息
                    QString head = QString("%1##%2").arg(fileName).arg(fileSize);
                    //发送头部信息
                    qint64 len =  tcpSocket->write(head.toUtf8());

                    if(len > 0)
                    {
                        //发送真正的文件
                        //防止tcp粘包问题
                        timer.start(20);
                    }
                    else
                    {
                        qDebug()<<"头部信息发送失败 110";
                        file.close();
                    }


                    connect(&timer,&QTimer::timeout,
                          [=]()
                        {
                                timer.stop();
                                //发送文件
                                sendData();
                        }
                             );

                }

            }
                    );


        }
            );
}



UpdaterServer::~UpdaterServer()
{
    delete ui;
}


void UpdaterServer::sendData()
{
    qint64 len = 0;
    do
    {
        //每次发送大小
        char buf[4*1024] = {0};
        len = 0;
        //读数据
        len = file.read(buf,sizeof(buf));
        //发送数据 读多少发多少
        len = tcpSocket->write(buf,len);

        //发送数据累计
        sendSize += len;
    } while(len > 0);

    //是否发送文件完毕
    if(sendSize == fileSize)
    {
        ui->textEditRead->append("文件发送完毕");
        file.close();
//        关闭文件
        tcpSocket->disconnectFromHost();
        sendSize = 0;
    }

}

//下面要选择文件



//这里是单独选择某一个文件
void UpdaterServer::on_ButtonSelect_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this,"选择文件","../");
    if(false == filePath.isEmpty())//文件路径有效
    {
        fileName.clear();
        fileSize = 0;
        //获取文件信息

        //只读方式打开
        QFileInfo info(filePath);
        fileName = info.fileName();//文件名称
        fileSize = info.size();//获取文件大小

        sendSize = 0;

        file.setFileName(filePath);
        //打开文件
        bool flag = file.open(QIODevice::ReadOnly);

        if(flag == false)
        {
            qDebug()<<"只读方式打开失败 77";
        }

        ui->textEditRead->append(filePath);

    }
    else
    {
        qDebug()<<"选择文件路径出错 62";
    }

}
