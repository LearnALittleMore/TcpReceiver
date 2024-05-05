#ifndef MSGRECVER_H
#define MSGRECVER_H

#include <functional>
#include <vector>
#include <string>

class MsgRecver
{
public:
    MsgRecver();

public:
    void AddRecver(std::function<void(const std::string &recver)>);

    void OnRecvMsg(const std::string &msg);

private:
    std::vector<std::function<void(const std::string &)>> m_recverlist;
};

#endif // MSGRECVER_H
