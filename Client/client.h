#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QTcpServer>
#include <QtNetwork>

QT_BEGIN_NAMESPACE
namespace Ui { class Client; }
QT_END_NAMESPACE

class Client : public QWidget
{
    Q_OBJECT

public:
    Client(QWidget *parent = nullptr);
    ~Client();

private slots:
    void requestMessage();
    void readMessage();

private:
    Ui::Client *ui;
    QTcpSocket *clientSocket = nullptr;
    QDataStream in;

};
#endif // CLIENT_H
