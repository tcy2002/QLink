#pragma once

#include <QWidget>
#include <QQueue>

/*
 * 绘制方块消除折线。
 */
class EliminationLine : public QWidget
{
    Q_OBJECT

private:
    int blocksize;
    QPoint start;
    QPoint inf1;
    QPoint inf2;
    QPoint end;

public:
    EliminationLine(int blocksize, QPoint p1, QPoint p2, QPoint p3, QPoint p4);

private:
    void paintEvent(QPaintEvent *) override;
};
