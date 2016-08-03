#ifndef COMMANDLINEMANAGER_H
#define COMMANDLINEMANAGER_H
#include <QCommandLineParser>
#include <QApplication>
#include <QPoint>

class CommandLineManager
{
public:
    CommandLineManager();
    void process(QApplication &app);
//    QStringList args();
    QString dir();
    QPoint pos();
private:
    QCommandLineParser m_commandLineParser;
    QCommandLineOption m_targetOption;
    QCommandLineOption m_rectOption;
//    QStringList m_strls;
//    QString m_dir="";
};

#endif // COMMANDLINEMANAGER_H
