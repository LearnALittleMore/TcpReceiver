#include "server.h"

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <sys/socket.h>
#include <sys/unistd.h>
#include <sys/types.h>
#include <sys/errno.h>
#include <netinet/in.h>

#define BUFFSIZE 4048
#define MAXLINK 2048

Server::Server(int port)
{
    m_run = false;
    m_epoller = std::make_unique<Epoller>();
    m_msgRecver = std::make_unique<MsgRecver>();

    InitSock(port);
}

Server::~Server()
{
    CloseSever();
}

void Server::AddRecver(std::function<void (const std::string &)> recver)
{
    m_msgRecver->AddRecver(recver);
}

void Server::Start()
{
    int INTERVAL = 2000; //msecond
    char buffer[BUFFSIZE];

    if (-1 == listen(m_sockfd, MAXLINK))
    {
        return;
    }

    setServStat(true);

    while(true)
    {
        if (!IsRunning())
        {
            CloseSever();
            return;
        }

        int count = m_epoller->Wait(INTERVAL);

        for (int i = 0; i < count; ++i)
        {
            int fd = m_epoller->GetEvent(i);

            if (fd == m_sockfd) //new connection
            {
                int clientFd = accept(m_sockfd, NULL, NULL);
                if (clientFd > 0)
                {
                    m_epoller->AddFd(clientFd);
                }
            }
            else // receive message
            {
                recv(fd, buffer, BUFFSIZE - 1, 0);
                printf("Recv: %s\n", buffer);

                m_msgRecver->OnRecvMsg(buffer);

                EndConn(fd);
            }
        }
    }
}

void Server::Close()
{
    CloseSever();
}

int Server::InitSock(int port)
{
    m_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == m_sockfd)
    {
        printf("Create socket error(%d): %s\n", errno, strerror(errno));
        return -1;
    }

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);
    if (-1 == bind(m_sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)))
    {
        printf("Bind error(%d): %s\n", errno, strerror(errno));
        return -1;
    }

    m_epoller->AddFd(m_sockfd);

    return 0;
}

bool Server::IsRunning()
{
    return m_run.load();
}

void Server::EndConn(int connfd)
{
    close(connfd);

    m_epoller->DelFd(connfd);
}

void Server::setServStat(bool run)
{
    m_run.store(run);
}

void Server::CloseSever()
{
    setServStat(false);

    close(m_sockfd);
}


