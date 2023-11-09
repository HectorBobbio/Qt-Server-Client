#include "server.h"
#include "ui_server.h"

Server::Server(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Server)
{
    ui->setupUi(this);
    initServer();
    connect(ui->btn_close, &QAbstractButton::clicked, this, &QWidget::close);
    connect(serverSocket, &QTcpServer::newConnection, this, &Server::sendRandomMessage);
}

Server::~Server()
{
    delete ui;
}

void Server::initServer()
{
    qInfo() << "Initializing server";

    //Message container
    for (int i = 0; i < 50 ; i++) {
        randomMessage << "Mensaje aleatorio " + QString::number(i+1) + " enviado :)" ;
    }
    //qInfo() << randomMessage;

    // Initializing server socket
    serverSocket = new QTcpServer(this);
    if (!serverSocket->listen()){
        qInfo() << "Server Error: " + serverSocket->errorString();
        return;
    }
    qInfo() << "Server has been initialized! ^^";

    QString port = QString::number(serverSocket->serverPort(), 10);
    qInfo() << "Port: " + port;

    QString ipAddress;
    QList<QHostAddress> ipAddressList = QNetworkInterface::allAddresses();
    for (int i = 0; i < ipAddressList.size(); i++) {
        if(ipAddressList.at(i) != QHostAddress::LocalHost && ipAddressList.at(i).toIPv4Address()){
            ipAddress = ipAddressList.at(i).toString();
            qInfo() << "IP Address: " + ipAddress;
            break;
        }
    }

    QString info = "IP: " + ipAddress + "\nPort: " + port;
    ui->lbl_ip_port->setText(info);
}

void Server::sendRandomMessage()
{
    qInfo() << "sending a random message";
    QByteArray qByteArray; //channel
    QDataStream qDataStream(&qByteArray, QIODevice::WriteOnly); //stream

    QString randomMsg = randomMessage[QRandomGenerator::global()->bounded(randomMessage.size())];
    qDataStream << randomMsg;
    ui->lbl_message->setText(randomMsg);

    QTcpSocket *clientSocket = serverSocket->nextPendingConnection();
    connect(clientSocket, &QAbstractSocket::disconnected, clientSocket, &QObject::deleteLater);

    clientSocket->write(qByteArray);
    clientSocket->disconnectFromHost();

}

