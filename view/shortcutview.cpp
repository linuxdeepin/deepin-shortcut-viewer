// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "shortcutview.h"
#include "shortcutitem.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonParseError>
#include <QJsonObject>
#include <QByteArray>
#include <QVBoxLayout>
#include <QtMath>
#include <QDebug>

#define MAX_COL_COUNT 3

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
    QByteArray byteArry;
    byteArry.append(data);

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
        qDebug() << "Json data parsing error:" << jsError.error << jsError.errorString();
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

void ShortcutView::initUI()
{
    for (const auto &col : m_colDatas) {
        QVBoxLayout *colLayout = new QVBoxLayout(this);
        colLayout->setSpacing(16);
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
    } else {
        int col = qCeil(m_totalWeight / 12);
        if (col > MAX_COL_COUNT) {
            col = MAX_COL_COUNT;
        }

        int res = qCeil(m_totalWeight / col);
        if (res > 20) {
            col = MAX_COL_COUNT + 1;
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
}
