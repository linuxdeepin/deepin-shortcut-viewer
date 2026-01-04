// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "singleapplication.h"
#include "commandlinemanager.h"
#include "view/mainwidget.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QProcess>
#include <QDir>

QString SingleApplication::UserID = "1000";

SingleApplication::SingleApplication(int &argc, char **argv, int)
    : DApplication(argc, argv)
{
    m_localServer = new QLocalServer;
    initConnect();
}

SingleApplication::~SingleApplication()
{
    if (m_localServer) {
        m_localServer->removeServer(m_localServer->serverName());
        m_localServer->close();
    }
}

void SingleApplication::initConnect()
{
    connect(m_localServer, &QLocalServer::newConnection, this, &SingleApplication::handleConnection);
}

void SingleApplication::newClientProcess(const QString &key, const QByteArray &message)
{
    qInfo() << "Attempting to connect to existing instance with key:" << key;
    QLocalSocket *localSocket = new QLocalSocket;
    localSocket->connectToServer(userServerName(key));
    if (localSocket->waitForConnected(1000)) {
        if (localSocket->state() == QLocalSocket::ConnectedState) {
            if (localSocket->isValid()) {
                qDebug() << "Connected to existing instance, sending message";
                localSocket->write(message);
                localSocket->flush();
                localSocket->waitForBytesWritten(1000);
            }
        }
    } else {
        qWarning() << "Failed to connect to existing instance:" << localSocket->errorString();
    }
    localSocket->deleteLater();
}

QString SingleApplication::userServerName(const QString &key)
{
    QString userKey;
    if (userID() == "0") {
        userKey = QString("%1/%2").arg("/tmp", key);
    } else {
        userKey = QString("%1/%2/%3").arg("/var/run/user", userID(), key);
    }
    qDebug() << "Generated server name:" << userKey << "for user:" << userID();
    return userKey;
}

QString SingleApplication::userID()
{
    QProcess userID;
    userID.start("id", QStringList() << "-u");
    userID.waitForFinished();
    QByteArray id = userID.readAll();
    UserID = QString(id).trimmed();
    return UserID;
}

void SingleApplication::processArgs(const QStringList &list)
{
    //Command manager
    CommandLineManager cmdManager;
    cmdManager.process(list);

    QString jsonData = cmdManager.jsonData();
    QPoint pos = cmdManager.pos();

    static MainWidget *w = Q_NULLPTR;

    if (jsonData.isEmpty()) {
        qDebug() << "No JSON data to process";
        return;
    }

    if (w && cmdManager.enableBypassWindowManagerHint()) {
        w->deleteLater();
        w = Q_NULLPTR;
    }

    if (!w) {
        qDebug() << "Creating new main window";
        w = new MainWidget();
    }

    // 先隐藏窗口，设置数据后触发布局计算
    w->hide();
    w->setJsonData(jsonData);

    if (cmdManager.enableBypassWindowManagerHint()) {
        qDebug() << "Setting bypass window manager hint";
        w->setWindowFlags(w->windowFlags() | Qt::BypassWindowManagerHint);
    }

    // 强制处理事件并调整大小
    QApplication::processEvents();
    w->adjustSize();

    // 计算居中位置并移动到正确位置后显示
    pos -= QPoint(w->width() / 2, w->height() / 2);
    w->move(pos);
    w->show();
    w->setFocus();
}

bool SingleApplication::setSingleInstance(const QString &key)
{
    QString userKey = userServerName(key);

    QLocalSocket *localSocket = new QLocalSocket;
    localSocket->connectToServer(userKey);

    // if connect success, another instance is running.
    bool result = localSocket->waitForConnected(1000);
    localSocket->deleteLater();

    if (result) {
        qInfo() << "Another instance is already running";
        return false;
    }

    m_localServer->removeServer(userKey);
    bool f = m_localServer->listen(userKey);

    return f;
}

void SingleApplication::handleConnection()
{
    qDebug() << "New connection received";
    QLocalSocket *nextPendingConnection = m_localServer->nextPendingConnection();
    connect(nextPendingConnection, SIGNAL(readyRead()), this, SLOT(readData()));
}

void SingleApplication::readData()
{
    const QByteArray &message = qobject_cast<QLocalSocket *>(sender())->readAll();

    QStringList list;

    for (const QByteArray &data : message.split('\0')) {
        QString str = QString::fromLocal8Bit(data);
        if (!str.isEmpty()) {
            list << str;
        }
    }

    processArgs(list);
}
