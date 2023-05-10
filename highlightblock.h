#pragma once

#include <QWidget>

/*
 * 绘制提示方框（使用提示道具时）
 */
class HighlightBlock : public QWidget
{
    Q_OBJECT

public:
    int column;
    int row;

private:
    int blocksize;
    QColor color;

public:
    HighlightBlock(int column, int row, int size, QColor color);

private:
    void paintEvent(QPaintEvent *) override;
};
