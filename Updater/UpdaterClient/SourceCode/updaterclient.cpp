#include "updaterclient.h"
#include "ui_updaterclient.h"

UpdaterClient::UpdaterClient(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UpdaterClient)
{
    ui->setupUi(this);

    setWindowTitle("Tangraria更新器V0.1");

    tcpSocket=NULL;

    tcpSocket = new QTcpSocket(this);

    ui->ButtonUpdate->setEnabled(false);
    ui->ButtonDisconnect->setEnabled(false);
    //不能点击更新按钮

    ui->DownloadProgress->setRange(0,100);
    ui->DownloadProgress->setValue(0);

    connect(tcpSocket,&QTcpSocket::connected,
            [=]()
                {
                    ui->textEditRead->setText("成功连接服务器");
                    ui->ButtonUpdate->setEnabled(true);
                    ui->ButtonDisconnect->setEnabled(true);
                }
            );

    connect(tcpSocket,&QTcpSocket::disconnected,
            [=]()
                {
                    ui->textEditRead->append("与服务器断开连接");

                    ui->ButtonUpdate->setEnabled(false);
                    ui->ButtonDisconnect->setEnabled(false);
                    ui->ButtonConnect->setEnabled(true);
                }

            );

    isHead = true;//为文件头

    //下面进行下载任务
    connect(tcpSocket,&QTcpSocket::readyRead,
            [=]()

    {

        //取出所有的buf
         QByteArray buf = tcpSocket->readAll();

         if(isHead == true)
         {
             isHead = false;
             //解析文件头
             fileName = QString(buf).section("##",0,0);
             fileSize = QString(buf).section("##",1,1).toInt();
             //接收到的文件大小目前为0;
             recvSize = 0;
             //打开文件
             file.setFileName(fileName);
             //在本目录中写文件

             qDebug()<<fileName;

             bool flag = file.open(QIODevice::WriteOnly);

             if(flag == false)
             {
                 qDebug()<<"打开文件错误"<<endl;//这里应该拒绝接受了
             }

         }
         else
         {
             qDebug()<<"走到了这里";
             qint64 len = file.write(buf);
             recvSize += len;
             int progress = (int)(100*recvSize/fileSize);
             qDebug()<<progress;
             ui->DownloadProgress->setValue(progress);

             if(recvSize == fileSize)
             {
                 file.close();
                 ui->textEditRead->append("更新完毕");
                //文件接收完毕
                 tcpSocket->disconnectFromHost();
                 ui->ButtonUpdate->setEnabled(false);
                 ui->ButtonDisconnect->setEnabled(false);
                 ui->DownloadProgress->setValue(0);


             }
         }

    }
            );


}


UpdaterClient::~UpdaterClient()
{
    delete ui;
}


void UpdaterClient::on_ButtonConnect_clicked()
{
    isHead = true;
    if(tcpSocket == NULL)
    {
        tcpSocket = new QTcpSocket(this);
    }


    QString ip = ui->lineEditIP->text();
    qint16 port = ui->lineEditPort->text().toInt();

    //主动与服务器建立连接
    tcpSocket->connectToHost(QHostAddress(ip),port);

    ui->ButtonUpdate->setEnabled(true);
}

void UpdaterClient::on_ButtonDisconnect_clicked()
{
    if(tcpSocket == NULL)
    {
        return;
    }

    tcpSocket->disconnectFromHost();
    tcpSocket->close();
    tcpSocket = NULL;
}

void UpdaterClient::on_ButtonUpdate_clicked()
{
    QString text = "Download";
    //随便写了一句 目的是更新服务端的readready
    tcpSocket->write(text.toUtf8().data());

    ui->ButtonConnect->setEnabled(false);
    ui->ButtonDisconnect->setEnabled(false);

}
