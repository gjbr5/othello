#include "networkclient.h"
#include <QHostAddress>
#include <QTimer>
#include <sstream>
#include <string>

NetworkClient::NetworkClient(uint32_t ip, uint16_t port)
{
    // Connect
    socket.connectToHost(QHostAddress(ip), port);
    qInfo().noquote().nospace() << "Server: " << QHostAddress(ip).toString() << ":" << port;
    qInfo() << "Server Connecting...";
    if (!socket.waitForConnected()) {
        qInfo().noquote() << socket.errorString();
        throw socket.error();
    }
    qInfo() << "Successfully Connected.";
}

NetworkClient::~NetworkClient()
{
    socket.disconnectFromHost();
    socket.close();
}

void NetworkClient::send(std::string data)
{
    qDebug().noquote() << QString::fromStdString(data);

    // Send
    socket.write(data.c_str(), data.size());
    if (!socket.waitForBytesWritten()) {
        qInfo() << socket.errorString();
        throw socket.error();
    }
}

void NetworkClient::sendMove(size_t move, const std::string& token)
{
    std::ostringstream data;

    // Make Move Message
    data << "move\nmove:" << move / 8 + 1 << static_cast<char>(move % 8 + 'A') << "\ntoken:" << token;

    send(data.str());
}

DataMap NetworkClient::recv()
{
    DataMap data;

    char buffer[BUFFER_SIZE] = { 0 };

    socket.waitForReadyRead();
    socket.read(buffer, BUFFER_SIZE);

    qDebug() << buffer;

    // Parse
    std::stringstream stream(buffer);
    std::string line;

    // Code
    std::getline(stream, line, '\n');
    data["code"] = line;

    // Data
    while (std::getline(stream, line, '\n')) {
        size_t pos = line.find(":");
        data[line.substr(0, pos)] = line.substr(pos + 1);
    }
    return data;
}
