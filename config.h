#pragma once

/*
 * 此文件定义程序运算所需的常量、参数、宏与工具类。
 */

#include <QPoint>
#include <QString>

// 方块贴图
const QString images[] = {
    ":/images/1.jpg",
    ":/images/2.jpg",
    ":/images/3.jpg",
    ":/images/4.jpg",
    ":/images/5.jpg"
};

// 道具贴图
const QString props[] = {
    ":/images/1s.png",
    ":/images/shuffle.png",
    ":/images/hint.png",
    ":/images/flash.png"
};

// 保存地图数据至文件
#define SAVE_MAP(map, file) \
    for (int i = 0; i < column; i++) \
        for (int j = 0; j < row; j++) \
            (file).write(tr("%1 ").arg((map)[i][j]).toUtf8()); \
    (file).write("\n");

// 删除地图
#define DELETE_MAP(map, c) \
    for (int i = 0; i < (c); i++) \
        delete[] (map)[i]; \
    delete[] (map);

// 消除折线的中间点
struct Inflection {
    QPoint p1;
    QPoint p2;
    Inflection(QPoint p1, QPoint p2): p1(p1), p2(p2) {}
};

// 取向
enum Pose {
    HORIZONAL, VERTICAL
};

// 取向
enum Direction {
    UP, DOWN, LEFT, RIGHT
};

// 方块
struct Block {
    int c;
    int r;
    Block(): c(0), r(0) {}
};

// 寻路算法内层节点
struct NodeBlock {
    int c;
    int r;
    int g;
    int h;
    int state;
    NodeBlock(): c(0), r(0), g(INT32_MAX), h(INT32_MAX), state(0) {}
};

// 寻路算法外层节点
struct Node {
    NodeBlock *b;
    explicit Node(NodeBlock *b = nullptr): b(b) {}
    friend bool operator <(Node m, Node n) {
        return m.b->g + m.b->h > n.b->g + n.b->h;
    }
};
