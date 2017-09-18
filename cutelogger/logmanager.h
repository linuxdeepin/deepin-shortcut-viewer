/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include <QtCore>

class ConsoleAppender;
class RollingFileAppender;

class LogManager
{
public:
    void initConsoleAppender();
    void initRollingFileAppender();

    inline static LogManager* instance(){
        static LogManager instance;
        return &instance;
    }

    void debug_log_console_on();
    QString joinPath(const QString& path, const QString& fileName);
    QString getlogFilePath();

signals:

public slots:

private:
    QString m_format;
    QString m_logPath;
    ConsoleAppender* m_consoleAppender;
    RollingFileAppender* m_rollingFileAppender;


    explicit LogManager();
    ~LogManager();
    LogManager(const LogManager &);
    LogManager & operator = (const LogManager &);
};

#endif // LOGMANAGER_H
