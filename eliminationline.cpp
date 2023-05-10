#include "eliminationline.h"

#include <QPainter>
#include <QDebug>

/*
 * blocksize: 方块像素宽度；
 * pi：折线上的第i个点，p2与p3为拐点。
 */
EliminationLine::EliminationLine(int blocksize, QPoint p1, QPoint p2, QPoint p3, QPoint p4)
    : blocksize(blocksize)
{
    QPoint tmp(blocksize * 1.5, blocksize *1.5);
    start = {p1 * blocksize + tmp};
    inf1 = {p2 * blocksize + tmp};
    inf2 = {p3 * blocksize + tmp};
    end = {p4 * blocksize + tmp};
}

void EliminationLine::paintEvent(QPaintEvent *)
{
    QColor color(255, 200, 218);
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.setPen({color, 6});

    p.save();
    p.drawLine(start, inf1);
    p.drawLine(inf1, inf2);
    p.drawLine(inf2, end);
    p.restore();
}
