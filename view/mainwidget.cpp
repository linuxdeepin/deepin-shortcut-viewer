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

#include "mainwidget.h"

#include <DPlatformWindowHandle>
#include <DApplication>

#include <QProcessEnvironment>
#include <QTimer>

MainWidget::MainWidget(QWidget *parent)
    : DAbstractDialog(false, parent)
{
    m_checkTimer = new QTimer(this);
    m_checkTimer->setInterval(500);
    connect(m_checkTimer, &QTimer::timeout, this, &MainWidget::checkKeyboard);

    initUI();
}

void MainWidget::setJsonData(const QString &data, int flag)
{
    if (m_scene)
        m_scene->deleteLater();

    auto e = QProcessEnvironment::systemEnvironment();
    QString str_output = e.value(QStringLiteral("LANG"));

    m_scene = new ShortcutScene(this, data, flag);
    m_mainView->setScene(m_scene);

    if (str_output == "zh_CN.UTF-8") {
        m_mainView->resize(static_cast<int>(m_scene->sceneRect().width() + 88), static_cast<int>(m_scene->sceneRect().height() + 80));
        setFixedSize(m_mainView->size().width() + CONTENT_MARGINS * 2, m_mainView->size().height() + CONTENT_MARGINS * 2);
    }

    else {
        m_mainView->resize(static_cast<int>(m_scene->sceneRect().width() + 44), static_cast<int>(m_scene->sceneRect().height() + 40));
        setFixedSize(m_mainView->size().width() + CONTENT_MARGINS * 2, m_mainView->size().height() + CONTENT_MARGINS * 2);
    }
}

void MainWidget::startCheckKeyboard()
{
    m_checkTimer->start();
}

void MainWidget::initUI()
{

    setWindowFlag(Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);

    m_mainLayout = new QVBoxLayout;
    m_mainLayout->setMargin(0);
    m_mainView = new QGraphicsView(this);

    m_mainView->setStyleSheet("background: transparent;");
    setWindowFlag(Qt::FramelessWindowHint);
    m_mainView->setObjectName("MainView");
    m_mainLayout->addWidget(m_mainView);

//    m_scene=new ShortcutScene(this);
//    m_scene->loadFile(m_url);

    setLayout(m_mainLayout);
    setContentsMargins(CONTENT_MARGINS, CONTENT_MARGINS, CONTENT_MARGINS, CONTENT_MARGINS);

    if (DApplication::isDXcbPlatform()) {
        DPlatformWindowHandle handle(this);

        handle.setBorderWidth(2);
        handle.setBorderColor(QColor(255, 255, 255, static_cast<int>(255 * 0.15)));
    }
}

void MainWidget::checkKeyboard()
{
    if (DApplication::queryKeyboardModifiers() != (Qt::ShiftModifier | Qt::ControlModifier))
        hide();
}

void MainWidget::mousePressEvent(QMouseEvent *e)
{
    hide();
    DAbstractDialog::mousePressEvent(e);
}

void MainWidget::keyReleaseEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Control || e->key() == Qt::Key_Shift) {
        releaseKeyboard();
        DAbstractDialog::keyReleaseEvent(e);
        hide();
    }
}

void MainWidget::focusInEvent(QFocusEvent *e)
{
    grabKeyboard();
    DAbstractDialog::focusInEvent(e);
}

void MainWidget::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape || e->key() == Qt::Key_Back)
        hide();
}

void MainWidget::showEvent(QShowEvent *e)
{
    DAbstractDialog::showEvent(e);

    setFocus(Qt::MouseFocusReason);
    grabKeyboard();
}

void MainWidget::hideEvent(QHideEvent *e)
{
    m_checkTimer->stop();

    DAbstractDialog::hideEvent(e);
}

void MainWidget::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)
    QPainter pa(this);
    pa.fillRect(rect(), QColor(0, 0, 0, static_cast<int>(255 * 0.7)));
}
