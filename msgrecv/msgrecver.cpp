#include "msgrecver.h"

using std::function;
using std::vector;
using std::string;

MsgRecver::MsgRecver() {}

void MsgRecver::AddRecver(function<void (const string &)> recver)
{
    m_recverlist.push_back(recver);
}

void MsgRecver::OnRecvMsg(const string &msg)
{
    for (function<void (const string &)> &func : m_recverlist)
    {
        func(msg);
    }
}

