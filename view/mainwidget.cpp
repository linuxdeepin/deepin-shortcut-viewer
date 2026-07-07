// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "mainwidget.h"
#include "shortcutview.h"

#include <DPlatformWindowHandle>
#include <DApplication>
#include <DGuiApplicationHelper>
#include <DFontSizeManager>

#include <QProcessEnvironment>
#include <QTimer>
#include <QPainter>
#include <QPalette>
#include <QKeyEvent>
#include <QPainterPath>
#include <QRegion>
#include <QScreen>

DWIDGET_USE_NAMESPACE

MainWidget::MainWidget(QWidget *parent)
    : DWidget(parent)
{
    initUI();
}

void MainWidget::setJsonData(const QString &data)
{
    if (m_mainView) {
        m_mainLayout->removeWidget(m_mainView);
        m_mainView->deleteLater();
    }

    m_mainView = new ShortcutView(this);
    m_mainView->setObjectName("MainView");
    m_mainView->setAttribute(Qt::WA_TranslucentBackground);
    m_mainLayout->addWidget(m_mainView);
    m_mainView->setData(data);
    adjustSize();
}

void MainWidget::setThemeName(const QString &themeName)
{
    if (m_themeName == themeName)
        return;

    m_themeName = themeName;
    update();
}

void MainWidget::initUI()
{
    if (qApp->platformName() == "wayland") {
        setWindowFlags(Qt::WindowStaysOnTopHint | Qt::Dialog);
        setWindowFlag(Qt::FramelessWindowHint);
    } else {
        setWindowFlags(Qt::WindowStaysOnTopHint | Qt::Popup);
    }

    m_mainLayout = new QVBoxLayout;
    m_mainLayout->setMargin(0);

    setLayout(m_mainLayout);
    initMargins();
    setAutoFillBackground(false);
    setAttribute(Qt::WA_TranslucentBackground);
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
            this, QOverload<>::of(&MainWidget::update));

    if (DApplication::isDXcbPlatform()) {
        DPlatformWindowHandle handle(this);

        handle.setBorderWidth(2);
        handle.setBorderColor(QColor(255, 255, 255, static_cast<int>(255 * 0.15)));
    }
}

void MainWidget::initMargins()
{

    QLocale systemLocale = QLocale::system();
    bool isNormal { systemLocale.language() == QLocale::Chinese };
    if (isNormal && qApp->devicePixelRatio() > 1.2)
        isNormal = false;
    // 高分屏不减少margin
    QScreen *primaryScreen = QGuiApplication::primaryScreen();
    if (primaryScreen) {
        QSize screenSize = primaryScreen->size();
        if (screenSize.width() > 2000)
            isNormal = true;
    }
    int margin { CONTENT_MARGINS };
    if (!isNormal)
        margin = 10;

    setContentsMargins(margin, margin, margin, margin);
}

void MainWidget::mousePressEvent(QMouseEvent *e)
{
    hide();
    DWidget::mousePressEvent(e);
}

void MainWidget::keyReleaseEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Control || e->key() == Qt::Key_Shift) {
        releaseKeyboard();
        DWidget::keyReleaseEvent(e);
        hide();
    }
}

void MainWidget::focusInEvent(QFocusEvent *e)
{
    grabKeyboard();
    DWidget::focusInEvent(e);
}

void MainWidget::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape || e->key() == Qt::Key_Back)
        hide();
}

void MainWidget::showEvent(QShowEvent *e)
{
    DWidget::showEvent(e);

    setFocus(Qt::MouseFocusReason);
    grabKeyboard();

    QTimer::singleShot(500, this, [this]() {
        if (DApplication::queryKeyboardModifiers() != (Qt::ShiftModifier | Qt::ControlModifier))
            hide();
    });
    emit resizeOver();
}

void MainWidget::hideEvent(QHideEvent *e)
{
    DWidget::hideEvent(e);
}

void MainWidget::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);

    const auto themeType = DGuiApplicationHelper::instance()->themeType();
    const QColor backgroundColor = DGuiApplicationHelper::instance()->applicationPalette().color(QPalette::Background);
    bool darkTheme = themeType == DGuiApplicationHelper::DarkType || backgroundColor.lightness() < 128;
    if (m_themeName == "dark")
        darkTheme = true;
    else if (m_themeName == "light")
        darkTheme = false;

    QPainterPath path;
    path.addRoundedRect(rect(), 18, 18);
    painter.setBrush(darkTheme ? QColor(32, 32, 32) : QColor(255, 255, 255));
    painter.drawPath(path);
}

void MainWidget::resizeEvent(QResizeEvent *e)
{
    DWidget::resizeEvent(e);

    QPainterPath path;
    path.addRoundedRect(rect(), 18, 18);
    setMask(QRegion(path.toFillPolygon().toPolygon()));

    // 当widget真正resize时，发送高度变化信号
    // 因为它在实际几何变化后触发
    emit resizeOver();
}
