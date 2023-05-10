#pragma once

#include <QPoint>
#include <queue>
#include <stack>

#include "config.h"

class CalculationMap
{
private:
    int column;
    int row;
    int **mapValid;
    int **mapId;

public:
    CalculationMap(int column, int row);
    ~CalculationMap();
    void initMap(int **&map, int num, bool positive = false);
    void setEmpty(int c, int r);
    bool isOccupied(int c, int r);
    Inflection *eliminate(int c1, int r1, int c2, int r2);

private:
    Inflection *eliminateHorizonal(int c1, int r1, int c2, int r2);
    Inflection *eliminateVertical(int c1, int r1, int c2, int r2);
};
