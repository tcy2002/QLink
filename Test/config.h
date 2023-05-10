#pragma once

#include <QPoint>
#include <QString>

const QString images[] = {
    ":/images/1.jpg",
    ":/images/2.jpg",
    ":/images/3.jpg",
    ":/images/4.jpg",
    ":/images/5.jpg"
};

const QString props[] = {
    ":/images/1s.png",
    ":/images/shuffle.png",
    ":/images/hint.png",
    ":/images/flash.png"
};

#define DELETE_MAP(map, c) \
    for (int i = 0; i < (c); i++) \
        delete[] (map)[i]; \
    delete[] (map);

struct Inflection {
    QPoint p1;
    QPoint p2;
    Inflection(QPoint p1, QPoint p2): p1(p1), p2(p2) {}
};

enum Pose {
    HORIZONAL, VERTICAL
};

enum Direction {
    UP, DOWN, LEFT, RIGHT
};

struct Block {
    int c;
    int r;
    Block(): c(0), r(0) {}
};

struct NodeBlock {
    int c;
    int r;
    int g;
    int h;
    int state;
    NodeBlock(): c(0), r(0), g(INT32_MAX), h(INT32_MAX), state(0) {}
};

struct Node {
    NodeBlock *b;
    explicit Node(NodeBlock *b = nullptr): b(b) {}
    friend bool operator <(Node m, Node n) {
        return m.b->g + m.b->h > n.b->g + n.b->h;
    }
};
