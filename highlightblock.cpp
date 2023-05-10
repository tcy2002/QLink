#include "highlightblock.h"

#include <QPainter>

/*
 * column: 位置横坐标；
 * row: 位置纵坐标；
 * size: 方块像素宽度。
 */
HighlightBlock::HighlightBlock(int column, int row, int size, QColor color)
    : column(column), row(row), blocksize(size), color(color)
{
    resize(blocksize, blocksize);
}

void HighlightBlock::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.setPen({color, 8});

    p.save();
    p.drawLine(0, 0, blocksize, 0);
    p.drawLine(blocksize, 0, blocksize, blocksize);
    p.drawLine(blocksize, blocksize, 0, blocksize);
    p.drawLine(0, blocksize, 0, 0);
    p.restore();
}
