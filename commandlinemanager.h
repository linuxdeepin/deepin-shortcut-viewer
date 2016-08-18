#ifndef COMMANDLINEMANAGER_H
#define COMMANDLINEMANAGER_H
#include <QCommandLineParser>
#include <QApplication>
#include <QPoint>

class CommandLineManager
{
public:
    CommandLineManager();
    void        process(QApplication &app);
    QPoint      pos();
    QString     jsonData();
private:
    QCommandLineParser      m_commandLineParser;
    QCommandLineOption      m_posOption;
    QCommandLineOption      m_jsonDataOption;
};

#endif // COMMANDLINEMANAGER_H
