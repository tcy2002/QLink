#pragma once

#include <QWidget>
#include <QLabel>

/*
 * 游戏人物（可移动方块）
 */
class Head : public QWidget
{
    Q_OBJECT

private:
    QLabel *headimage;
    int id;
    int headsize;
    int headcolumn;
    int headrow;

public:
    Head(int id, int size, int hc, int hr, QWidget *parent = nullptr);
    ~Head();

    // 设置参数
    int getHeadcolumn();
    int getHeadrow();
    int getId();

    // 读取参数
    void setHeadcolumn(int c);
    void setHeadrow(int r);

    // 移动
    void moveRight();
    void moveLeft();
    void moveUp();
    void moveDown();
    void moveTo(int c, int r);
};
