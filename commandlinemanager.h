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
    QString     dir();
    QPoint      pos();
    QString     jsonData();
private:
    QCommandLineParser      m_commandLineParser;
    QCommandLineOption      m_targetOption;
    QCommandLineOption      m_rectOption;
    QCommandLineOption      m_jsonDataOption;
};

#endif // COMMANDLINEMANAGER_H
