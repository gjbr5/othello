#ifndef NETWORKCLIENT_H
#define NETWORKCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <map>
#include <string>

typedef std::map<std::string, std::string> DataMap;

class NetworkClient : public QObject {
    Q_OBJECT

private:
    constexpr static size_t BUFFER_SIZE = 1024;
    QTcpSocket socket;
    void send(std::string data);

public:
    NetworkClient(uint32_t ip, uint16_t port);
    ~NetworkClient() override;

    void sendMove(size_t move, const std::string& token);
    DataMap recv();
};

#endif // NETWORKCLIENT_H
