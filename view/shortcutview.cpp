// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "shortcutview.h"
#include "shortcutitem.h"

#include <DFontSizeManager>

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonParseError>
#include <QJsonObject>
#include <QByteArray>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QtMath>
#include <QDebug>
#include <QApplication>
#include <QGuiApplication>
#include <QScreen>

#define MAX_COL_COUNT 3
#define MAX_ADJUSTED_COL_COUNT 6
#define MIN_ITEM_WIDTH 250
#define MIN_COL_SPACING 10
#define PORTRAIT_COL_SPACING 20

DWIDGET_USE_NAMESPACE

ShortcutView::ShortcutView(QWidget *parent)
    : QWidget(parent)
{
    setFocus(Qt::MouseFocusReason);

    m_mainLayout = new QHBoxLayout(this);
    m_mainLayout->setSpacing(50);
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(m_mainLayout);
}

void ShortcutView::setData(const QString &data)
{
    QByteArray byteArry = data.toUtf8();

    QJsonParseError jsError;
    QJsonDocument doc = QJsonDocument::fromJson(byteArry, &jsError);
    QJsonObject obj;
    QJsonArray shortcuts;
    if (jsError.error == QJsonParseError::NoError) {
        if (doc.isObject()) {
            obj = doc.object();
            if (obj.contains("shortcut")) {
                QJsonValue value;
                value = obj.value("shortcut");
                if (value.isArray()) {
                    shortcuts = value.toArray();
                }
            }
        }
    } else {
        qWarning() << "Failed to parse shortcut data:" << jsError.errorString() << "at offset" << jsError.offset;
        return;
    }

    foreach (QJsonValue value, shortcuts) {
        QJsonObject obj = value.toObject();
        Shortcut sc;
        sc.name = obj["groupName"].toString();
        sc.value = "type";
        sc.weight = 1.5;
        m_totalWeight += 1.5;

        m_shortcutList.append(sc);
        QJsonArray items = obj["groupItems"].toArray();
        foreach (QJsonValue itemvalue, items) {
            QJsonObject item = itemvalue.toObject();
            Shortcut sc1;
            sc1.name = item["name"].toString();
            sc1.value = item["value"].toString();
            sc1.weight = 1;
            m_shortcutList.append(sc1);
            m_totalWeight++;
        }
    }

    calcColumnData();
    initUI();
}

void ShortcutView::setContentMargin(int margin)
{
    m_contentMargin = margin;
}

void ShortcutView::calcLayoutParameters(int colCount)
{
    QScreen *primaryScreen = QGuiApplication::primaryScreen();
    int screenWidth = 1920;
    bool isPortrait = false;
    if (primaryScreen) {
        QSize screenSize = primaryScreen->size();
        screenWidth = screenSize.width();
        isPortrait = screenSize.height() > screenSize.width();
    }

    int availableWidth = screenWidth - 2 * m_contentMargin;

    int fontSize { DFontSizeManager::instance()->fontPixelSize(DFontSizeManager::T6) };
    static const QMap<int, int> sizeMap {
        { 16, 310 },
        { 17, 320 },
        { 18, 330 },
        { 19, 340 },
        { 20, 350 }
    };
    int defaultItemWidth { sizeMap.value(fontSize, 300) };
    if (fontSize > 20)
        defaultItemWidth = 360;

    int defaultSpacing = isPortrait ? PORTRAIT_COL_SPACING : 50;

    m_itemWidth = defaultItemWidth;
    m_columnSpacing = defaultSpacing;
    m_adjustedColCount = colCount;

    int totalWidth = colCount * m_itemWidth + (colCount - 1) * m_columnSpacing;

    if (totalWidth <= availableWidth) {
        return;
    }

    for (int spacing = defaultSpacing; spacing >= MIN_COL_SPACING; spacing -= 5) {
        int w = colCount * m_itemWidth + (colCount - 1) * spacing;
        if (w <= availableWidth) {
            m_columnSpacing = spacing;
            totalWidth = w;
            break;
        }
    }

    if (totalWidth <= availableWidth) {
        return;
    }

    for (int width = m_itemWidth; width >= MIN_ITEM_WIDTH; width -= 10) {
        int w = colCount * width + (colCount - 1) * m_columnSpacing;
        if (w <= availableWidth) {
            m_itemWidth = width;
            totalWidth = w;
            break;
        }
    }

    if (totalWidth <= availableWidth) {
        return;
    }

    for (int col = colCount + 1; col <= MAX_ADJUSTED_COL_COUNT; ++col) {
        int w = col * MIN_ITEM_WIDTH + (col - 1) * MIN_COL_SPACING;
        if (w <= availableWidth) {
            m_adjustedColCount = col;
            m_columnSpacing = MIN_COL_SPACING;
            int calcWidth = (availableWidth - (col - 1) * MIN_COL_SPACING) / col;
            m_itemWidth = qMin(calcWidth, defaultItemWidth);
            return;
        }
    }

    m_itemWidth = MIN_ITEM_WIDTH;
    m_columnSpacing = MIN_COL_SPACING;
}

void ShortcutView::initUI()
{
    m_mainLayout->setSpacing(m_columnSpacing);

    int spacing { itemSpacing() };
    for (const auto &col : m_colDatas) {
        QVBoxLayout *colLayout = new QVBoxLayout(this);
        colLayout->setSpacing(spacing);
        colLayout->setContentsMargins(0, 0, 0, 0);
        for (const auto &sc : col) {
            ShortcutItem *item = nullptr;
            if (sc.value == "type") {
                item = new ShortcutItem(true, this);
                item->setText(sc.name, "");
                item->setEnableBackground(true);
                colLayout->addSpacing(4);
            } else {
                item = new ShortcutItem(false, this);
                item->setText(sc.name, sc.value);
            }

            item->setItemWidth(m_itemWidth);
            colLayout->addWidget(item);
        }

        colLayout->addSpacerItem(new QSpacerItem(10, 0, QSizePolicy::Expanding, QSizePolicy::Expanding));
        m_mainLayout->addLayout(colLayout);
    }
}

void ShortcutView::calcColumnData()
{
    if (m_shortcutList.size() <= 12) {
        m_colDatas << m_shortcutList;
        calcLayoutParameters(1);
        return;
    }

    int col = qCeil(m_totalWeight / 12);
    if (col > MAX_COL_COUNT) {
        col = MAX_COL_COUNT;
    }

    int res = qCeil(m_totalWeight / col);
    if (res > 20) {
        col = MAX_COL_COUNT + 1;
        res = qCeil(m_totalWeight / col);
    }

    calcLayoutParameters(col);
    if (m_adjustedColCount > col) {
        col = m_adjustedColCount;
        res = qCeil(m_totalWeight / col);
    }

    double subSum = 0;
    int offset = 0;
    int colCount = 1;
    for (int i = 0; i < m_shortcutList.size(); ++i) {
        subSum += m_shortcutList[i].weight;
        if (res <= subSum) {
            m_colDatas << m_shortcutList.mid(offset, i - offset + 1);
            offset = i + 1;
            subSum = 0;

            if (++colCount == col) {
                m_colDatas << m_shortcutList.mid(offset, m_shortcutList.size() - i);
                break;
            }
        }
    }
}

int ShortcutView::itemSpacing()
{
    QLocale systemLocale = QLocale::system();
    bool isNormal { systemLocale.language() == QLocale::Chinese };
    if (isNormal && qApp->devicePixelRatio() > 1.2)
        isNormal = false;

    int fontSize { DFontSizeManager::instance()->fontPixelSize(DFontSizeManager::T6) };
    static const QMap<int, int> spacingMap {
        { 14, isNormal ? 16 : 8 },
        { 15, isNormal ? 14 : 7 },
        { 16, isNormal ? 12 : 6 },
        { 17, isNormal ? 10 : 5 },
        { 18, isNormal ? 8 : 4 },
        { 19, isNormal ? 6 : 3 },
        { 20, isNormal ? 4 : 2 }
    };
    return spacingMap.value(fontSize, 16);
}
