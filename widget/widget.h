#ifndef WIDGET_H
#define WIDGET_H

#include "server/server.h"
#include <thread>
#include <memory>
#include <string>

#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QAction>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

public:
    void onRecvMsg(const std::string &msg);

private:
    void initwidget();
    void initconnect();

private:
    std::thread *m_sevThread;
    std::unique_ptr<Server> m_server;

    QLineEdit *m_portEdit;
    QTextEdit *m_textedit;
    QAction *m_startAct;
    QAction *m_cleanAct;

signals:
    void sgMsg(const QString &msg);

private slots:
    void slStartActTriggered();
    void slCleanActTriggered();
    void onMsg(const QString &msg);
};

#endif // WIDGET_H
