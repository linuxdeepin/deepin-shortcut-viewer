// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "shortcutitem.h"

#include <DPalette>
#include <DGuiApplicationHelper>
#include <DFontSizeManager>

#include <QHBoxLayout>
#include <QPainter>

DWIDGET_USE_NAMESPACE

ShortcutItem::ShortcutItem(bool isGroup, QWidget *parent)
    : QWidget(parent),
      m_isGroup(isGroup)
{
    // 默认跟随查看器自身主题，调用方可通过 setDarkTheme 用 --theme 参数覆盖，
    // 以保证弹窗文字与背景使用同一套主题（避免系统深色 + 应用浅色时文字发虚）。
    m_darkTheme = DGuiApplicationHelper::instance()->themeType() == DGuiApplicationHelper::DarkType;

    m_nameLabel = new QLabel(this);
    m_nameLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_nameLabel->setWordWrap(true);

    m_valueLabel = new QLabel(this);
    m_valueLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_valueLabel->setWordWrap(true);

    updateTextPalette();

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(m_nameLabel);
    mainLayout->addSpacerItem(new QSpacerItem(10, 10));
    mainLayout->addWidget(m_valueLabel);

    if (isGroup) {
        QFont font;
        font.setPixelSize(17);
        font.setWeight(70);

        m_nameLabel->setFont(font);
        m_valueLabel->setFont(font);

        mainLayout->setContentsMargins(10, 5, 10, 6);
    } else {
        if (qApp->devicePixelRatio() > 1.2) {
            int fontSize { DFontSizeManager::instance()->fontPixelSize(DFontSizeManager::T6) };
            QFont font { m_nameLabel->font() };
            static const QMap<int, int> fontMap {
                { 15, 14 },
                { 16, 16 },
                { 17, 16 },
                { 18, 16 },
                { 20, 18 }
            };
            int size { fontMap.value(fontSize, 14) };
            font.setPixelSize(size);
            m_nameLabel->setFont(font);
            m_valueLabel->setFont(font);
        }
        mainLayout->setContentsMargins(10, 0, 10, 0);
    }

    // 系统字号太大时，Text会超出label显示范围
    int fontSize { m_nameLabel->fontInfo().pixelSize() };
    static const QMap<int, int> sizeMap {
        { 16, 310 },
        { 17, 320 },
        { 18, 330 },
        { 19, 340 },
        { 20, 350 }
    };
    int width { sizeMap.value(fontSize, 300) };
    if (fontSize > 20)
        width = 360;

    setLayout(mainLayout);
    setFixedWidth(width);
}

void ShortcutItem::setText(const QString &name, const QString &value)
{
    m_nameLabel->setText(name);
    m_nameLabel->adjustSize();

    m_valueLabel->setText(value);
    m_valueLabel->adjustSize();
}

QString ShortcutItem::name() const
{
    return m_nameLabel->text();
}

QString ShortcutItem::value() const
{
    return m_valueLabel->text();
}

void ShortcutItem::setEnableBackground(bool enable)
{
    if (enable == m_hasBackground)
        return;

    m_hasBackground = enable;
    update();
}

bool ShortcutItem::enableBackground() const
{
    return m_hasBackground;
}

void ShortcutItem::setDarkTheme(bool dark)
{
    if (m_darkTheme == dark)
        return;

    m_darkTheme = dark;
    updateTextPalette();
}

void ShortcutItem::updateTextPalette()
{
    const QColor textColor = m_darkTheme ? QColor(192, 198, 212) : QColor(65, 77, 104);
    QPalette labelPalette;
    labelPalette.setColor(QPalette::WindowText, textColor);

    if (m_nameLabel)
        m_nameLabel->setPalette(labelPalette);
    if (m_valueLabel)
        m_valueLabel->setPalette(labelPalette);
}

void ShortcutItem::paintEvent(QPaintEvent *event)
{
    if (m_hasBackground) {
        // 分组标题背景同样使用 --theme 指定的主题，避免与弹窗背景不一致。
        DPalette dp;
        DGuiApplicationHelper::generatePalette(
            dp, m_darkTheme ? DGuiApplicationHelper::DarkType : DGuiApplicationHelper::LightType);
        QPainter p(this);
        p.setPen(Qt::NoPen);
        p.setBrush(dp.brush(DPalette::ItemBackground));
        p.drawRoundedRect(rect(), 8, 8);
    }

    return QWidget::paintEvent(event);
}
