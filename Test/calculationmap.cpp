#include "calculationmap.h"

#include <QtCore/qmath.h>
#include <QTime>
#include <QDebug>

CalculationMap::CalculationMap(int column, int row)
    : column(column), row(row)
{
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    initMap(mapValid, 1, true);
    initMap(mapId, 1);
}

CalculationMap::~CalculationMap()
{
    DELETE_MAP(mapValid, column)
    DELETE_MAP(mapId, column)
}

void CalculationMap::initMap(int **&map, int num, bool positive)
{
    map = new int*[column];
    for (int i = 0; i < column; i++) {
        map[i] = new int[row];
        for (int j = 0; j < row; j++)
            map[i][j] = qrand() % num + positive;
    }

    if (num == 40)
        map[0][0] = 3;
}

void CalculationMap::setEmpty(int c, int r)
{
    mapValid[c][r] = false;
}

bool CalculationMap::isOccupied(int c, int r)
{
    if (c < 0 || c >= column || r < 0 || r >= row)
        return false;
    return mapValid[c][r];
}

Inflection *CalculationMap::eliminate(int c1, int r1, int c2, int r2)
{
    if ((c1 == c2 && r1 == r2) || mapId[c1][r1] != mapId[c2][r2])
        return nullptr;

    Inflection *inf = eliminateHorizonal(c1, r1, c2, r2);
    if (inf != nullptr)
        return inf;

    return eliminateVertical(c1, r1, c2, r2);
}

Inflection *CalculationMap::eliminateHorizonal(int c1, int r1, int c2, int r2)
{
    int xmax1, xmin1, xmax2, xmin2;
    for (xmax1 = c1 + 1; xmax1 < column && !mapValid[xmax1][r1]; xmax1++);
    for (xmin1 = c1 - 1; xmin1 > -1 && !mapValid[xmin1][r1]; xmin1--);
    for (xmax2 = c2 + 1; xmax2 < column && !mapValid[xmax2][r2]; xmax2++);
    for (xmin2 = c2 - 1; xmin2 > -1 && !mapValid[xmin2][r2]; xmin2--);

    int xmin, xmax;
    xmin = qMax(xmin1, xmin2);
    xmax = qMin(xmax1, xmax2);
    if (xmax - xmin < 2)
        return nullptr; // 1

    int rmin, rmax, xpos, ypos;
    rmax = qMax(r1, r2);
    rmin = qMin(r1, r2);
    for (xpos = xmin + 1; xpos < xmax; xpos++) {
        for (ypos = rmin + 1; ypos < rmax && !mapValid[xpos][ypos]; ypos++);
        if (ypos == rmax)
            return new Inflection({xpos, r1}, {xpos, r2}); // 2
    }

    if (xmin1 == xmin2 && xmin1 == -1)
        return new Inflection({xmin1, r1}, {xmin1, r2}); // 3
    if (xmax1 == xmax2 && xmax1 == column)
        return new Inflection({xmax1, r1}, {xmax1, r2}); // 4

    return nullptr; // 5
}

Inflection *CalculationMap::eliminateVertical(int c1, int r1, int c2, int r2)
{
    int ymax1, ymin1, ymax2, ymin2;
    for (ymax1 = r1 + 1; ymax1 < row && !mapValid[c1][ymax1]; ymax1++);
    for (ymin1 = r1 - 1; ymin1 > -1 && !mapValid[c1][ymin1]; ymin1--);
    for (ymax2 = r2 + 1; ymax2 < row && !mapValid[c2][ymax2]; ymax2++);
    for (ymin2 = r2 - 1; ymin2 > -1 && !mapValid[c2][ymin2]; ymin2--);

    int ymin, ymax;
    ymin = qMax(ymin1, ymin2);
    ymax = qMin(ymax1, ymax2);
    if (ymax - ymin < 2)
        return nullptr; // 1

    int cmin, cmax, xpos, ypos;
    cmax = qMax(c1, c2);
    cmin = qMin(c1, c2);
    for (ypos = ymin + 1; ypos < ymax; ypos++) {
        for (xpos = cmin + 1; xpos < cmax && !mapValid[xpos][ypos]; xpos++);
        if (xpos == cmax)
            return new Inflection({c1, ypos}, {c2, ypos}); // 2
    }

    if (ymin1 == ymin2 && ymin1 == -1)
        return new Inflection({c1, ymin1}, {c2, ymin1}); // 3
    if (ymax1 == ymax2 && ymax1 == row)
        return new Inflection({c1, ymax1}, {c2, ymax1}); // 4

    return nullptr; // 5
}
