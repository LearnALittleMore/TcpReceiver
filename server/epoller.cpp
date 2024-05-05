#include "epoller.h"

#include "unistd.h"
#include "string.h"

Epoller::Epoller(int maxEvents)
{
    m_epollfd = epoll_create1(0);
    m_events.resize(maxEvents);
}

Epoller::~Epoller()
{
    close(m_epollfd);
}

int Epoller::Wait(int timeoutMs)
{
    return epoll_wait(m_epollfd, &m_events[0], m_events.size(), timeoutMs); //return count of events
}

int Epoller::GetEvent(int index)
{
    return m_events[index].data.fd;
}

int Epoller::AddFd(int fd)
{
    struct epoll_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.events = EPOLLIN;
    ev.data.fd = fd;

    if (epoll_ctl(m_epollfd, EPOLL_CTL_ADD, fd, &ev) < 0)
    {
        return -1;
    }

    return 0;
}

int Epoller::DelFd(int fd)
{
    struct epoll_event ev;
    if (epoll_ctl(m_epollfd, EPOLL_CTL_DEL, fd, &ev) < 0)
    {
        return -1;
    }

    return 0;
}
