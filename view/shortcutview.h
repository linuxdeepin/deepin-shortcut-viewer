// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef SHORTCUTVIEW_H
#define SHORTCUTVIEW_H

#include <QWidget>
#include <QHBoxLayout>
#include <QScreen>

class ShortcutView : public QWidget
{
    Q_OBJECT
public:
    struct Shortcut
    {
        QString name;
        QString value;
        double weight;
    };

    explicit ShortcutView(QWidget *parent = nullptr);
    void setData(const QString &data);
    void setContentMargin(int margin);

private:
    void initUI();
    void calcColumnData();
    void calcLayoutParameters(int colCount);
    int itemSpacing();

private:
    QHBoxLayout *m_mainLayout = nullptr;

    using ColData = QList<Shortcut>;
    ColData m_shortcutList;
    QList<ColData> m_colDatas;
    double m_totalWeight = 0;
    int m_contentMargin = 30;
    int m_itemWidth = 0;
    int m_columnSpacing = 50;
    int m_adjustedColCount = 0;
};

#endif   // SHORTCUTVIEW_H
