#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <memory>
#include <atomic>

#include "epoller.h"
#include "msgrecver.h"

class Server
{
public:
    Server(int port = 8080);
    ~Server();

public:
    void Start();
    void Close();
    void AddRecver(std::function<void(const std::string &recver)>);

private:
    int InitSock(int port);
    bool IsRunning();
    void EndConn(int connfd);
    void setServStat(bool run);
    void CloseSever();

private:
    std::atomic<bool> m_run;

    int m_sockfd;
    std::unique_ptr<Epoller> m_epoller;
    std::unique_ptr<MsgRecver> m_msgRecver;
};

#endif // SERVER_H
