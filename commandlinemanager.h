/*
 * Copyright (C) 2016 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     dragondjf<dingjiangfeng@deepin.com>
 *
 * Maintainer: dragondjf<dingjiangfeng@deepin.com>
 *             Tangtong<tangtong@deepin.com>
 *
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

#ifndef COMMANDLINEMANAGER_H
#define COMMANDLINEMANAGER_H
#include <QCommandLineParser>
#include <QApplication>
#include <QPoint>

class CommandLineManager
{
public:
    CommandLineManager();
    void process(const QCoreApplication &app);
    void process(const QStringList &list);
    QPoint pos();
    QString jsonData();

    bool enableBypassWindowManagerHint() const;

private:
    QCommandLineParser m_commandLineParser;
    QCommandLineOption m_posOption;
    QCommandLineOption m_jsonDataOption;
};

#endif // COMMANDLINEMANAGER_H
