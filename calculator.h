#pragma once

#include <QPoint>
#include <queue>
#include <stack>

#include "config.h"

/*
 *  计算模块：所有与游戏逻辑相关的计算与设置在此类中定义。
 */
class Calculator
{
private:
    int column;
    int row;

    int **mapValid;       // 方块是否存在（未消除）
    int **mapId;          // 方块类型
    int **mapProp;        // 道具类型
    Block **mapShuffle;   // 打乱矩阵
    NodeBlock **mapPath;  // 寻路算法的辅助矩阵

public:
    Calculator(int column, int row, int **valid = nullptr, int **id = nullptr, int **prop = nullptr);
    ~Calculator();

    // 初始化
    void initMap(int **&map, int num, bool positive = false);
    void initShuffle();
    void initPath();

    // 设置地图
    void setEmpty(int c, int r);
    void setId(int c, int r, int id);
    void setProp(int c, int r, int prop);

    // 获取地图
    int **getMapValid();
    int **getMapId();
    int **getMapProp();

    // 获取方块状态
    bool isOccupied(int c, int r);
    bool isReachable(int c, int r);
    int getProp(int c, int r);

    // 打乱
    QPoint getShuffle(int c, int r);
    void shuffle();

    // 寻路算法
    void resetPath();
    bool AStar(int c1, int r1, int c2, int r2);

    // 可解性判断
    Inflection *Solvable();

    // 方块消除
    Inflection *eliminate(int c1, int r1, int c2, int r2);

private:
    // 打乱算法实现函数
    void shuffle(int **&map);

    // 寻路算法辅助函数
    void limit(int &c, int &r);
    static int manhattan(int c1, int r1, int c2, int r2);
    std::queue<Node> findAdjacent(int c, int r);

    // 方块消除实现函数
    Inflection *eliminateHorizonal(int c1, int r1, int c2, int r2);
    Inflection *eliminateVertical(int c1, int r1, int c2, int r2);
};
