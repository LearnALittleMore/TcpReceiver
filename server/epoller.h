#ifndef EPOLLER_H
#define EPOLLER_H

#include <sys/epoll.h>
#include <vector>

class Epoller
{
public:
    Epoller(int maxEvents = 1024);
    ~Epoller();

public:
    int Wait(int timeoutMs);
    int GetEvent(int index);

    int AddFd(int fd);
    int DelFd(int fd);

private:
    int m_epollfd;

    std::vector<struct epoll_event> m_events;
};

#endif // EPOLLER_H
