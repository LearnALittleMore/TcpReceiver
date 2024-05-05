#include "widget.h"

#include <QVBoxLayout>
#include <QToolBar>
#include <QLabel>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    initwidget();
    initconnect();
    this->setWindowIcon(QIcon(":/icon/tcp_recerver.ico"));
    this->resize(800, 500);
}

Widget::~Widget(){}

void Widget::onRecvMsg(const std::string &msg)
{
    emit sgMsg(QString::fromStdString(msg)); //modify ui data only in the main thread
}

void Widget::initwidget()
{
    QVBoxLayout *pLayout = new QVBoxLayout(this);
    this->setLayout(pLayout);

    QToolBar *toobar = new QToolBar(this);
    toobar->setStyleSheet(tr("background-color:white"));
    pLayout->addWidget(toobar);

    m_startAct = new QAction("start", this);toobar->addAction(m_startAct);
    m_cleanAct = new QAction("clean", this);toobar->addAction(m_cleanAct);
    QLabel *portLab = new QLabel("port", this); toobar->addWidget(portLab);
    m_portEdit = new QLineEdit(this); m_portEdit->setFixedWidth(100);
    toobar->addWidget(m_portEdit);

    m_textedit = new QTextEdit(this);
    pLayout->addWidget(m_textedit);
}

void Widget::initconnect()
{
    connect(m_startAct, SIGNAL(triggered(bool)), this, SLOT(slStartActTriggered()));
    connect(m_cleanAct, SIGNAL(triggered(bool)), this, SLOT(slCleanActTriggered()));
    connect(this, SIGNAL(sgMsg(QString)), this, SLOT(onMsg(QString)));
}

void Widget::slStartActTriggered()
{
    if (m_startAct->text() == "start")
    {
        m_server = std::make_unique<Server>(m_portEdit->text().toInt());
        m_server->AddRecver([this](const std::string &msg)
                            {
                                this->onRecvMsg(msg);
                            });

        std::thread subThread([this]()
        {
            m_server->Start();
        });

        subThread.detach();

        m_startAct->setText("stop");
    }
    else
    {
        m_server->Close();

        m_server.release();

        m_startAct->setText("start");
    }
}

void Widget::slCleanActTriggered()
{
    m_textedit->clear();
}

void Widget::onMsg(const QString &msg)
{
    m_textedit->setText(msg);
}
