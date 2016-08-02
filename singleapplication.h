#ifndef SINGLEAPPLICATION_H
#define SINGLEAPPLICATION_H

#include <QApplication>
#include <QLocalServer>
#include <QLocalSocket>
#include <commandlinemanager.h>
#include "./view/mainwidget.h"

class SingleApplication : public QApplication
{
    Q_OBJECT
public:
    explicit SingleApplication(int &argc, char **argv, int = ApplicationFlags);
    ~SingleApplication();
    void initConnect();

    void newClientProcess(const QString& key,QString order);
    static QString userServerName(const QString& key);
    static QString userID();
    static QString UserID;

signals:

public slots:
    bool setSingleInstance(const QString& key);
    void handleConnection();
    void readData();

private:
    QLocalServer* m_localServer;
};

#endif // SINGLEAPPLICATION_H
