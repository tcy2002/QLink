#include "calculator.h"

#include <QtCore/qmath.h>
#include <QTime>
#include <QDebug>

/*
 * 支持给定地图参数（从外部文件导入）。
 *   column: 地图宽度；
 *   row: 地图高度；
 *   valid: 方块是否存在；
 *   id: 方块类型；
 *   prop: 道具类型。
 */
Calculator::Calculator(int column, int row, int **valid, int **id, int **prop)
    : column(column), row(row)
{
    mapValid = valid;
    mapId = id;
    mapProp = prop;
    if (mapValid == nullptr)
        initMap(mapValid, 0, true);
    if (mapId == nullptr)
        initMap(mapId, 5);
    if (mapProp == nullptr)
        initMap(mapProp, 40);
    initShuffle();
    initPath();
}

Calculator::~Calculator()
{
    DELETE_MAP(mapValid, column)
    DELETE_MAP(mapId, column)
    DELETE_MAP(mapProp, column)
    DELETE_MAP(mapShuffle, column)
    DELETE_MAP(mapPath, column + 2)
}

/*
 * 根据num与positive对地图进行随机初始化。
 *   num: 参数范围；
 *   positive: 参数是否为恒定值。
 */
void Calculator::initMap(int **&map, int num, bool positive)
{
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));

    map = new int*[column];
    for (int i = 0; i < column; i++) {
        map[i] = new int[row];
        for (int j = 0; j < row; j++)
            map[i][j] = positive ? 1 : qrand() % num;
    }
}

/*
 * 初始化打乱矩阵。
 */
void Calculator::initShuffle()
{
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));

    mapShuffle = new Block*[column];
    for (int i = 0; i < column; i++)
        mapShuffle[i] = new Block[row]{};

    // 判断纵向坐标是否重复的lambda
    auto isDupVeitical = [this](int i, int j) {
        for (int k = 0; k < j; k++) {
            if (mapShuffle[i][k].r == mapShuffle[i][j].r)
                return true;
        }
        return false;
    };

    // 设置纵向坐标
    for (int i = 0; i < column; i++)
        for (int j = 0; j < row; j++) {
            mapShuffle[i][j].r = qrand() % row;
            while (isDupVeitical(i, j))
                mapShuffle[i][j].r = (mapShuffle[i][j].r + 1) % row;
        }

    // 判断横向坐标是否重复的lambda
    auto isDupHorizonal = [this](int i, int j) {
        for (int k = 0; k < i; k++)
            if (mapShuffle[k][j].c == mapShuffle[i][j].c)
                return true;
        return false;
    };

    // 设置横向坐标
    for (int i = 0; i < row; i++) {
        int tmp[column];
        for (int j = 0; j < column; j++)
            tmp[j] = mapShuffle[j][i].r;
        for (int j = 0; j < column; j++) {
            mapShuffle[j][i].c = qrand() % column;
            while (isDupHorizonal(j, i))
                mapShuffle[j][i].c = (mapShuffle[j][i].c + 1) % column;
            mapShuffle[j][i].r = tmp[mapShuffle[j][i].c];
        }
    }
}

/*
 * 初始化寻路算法的辅助矩阵。
 */
void Calculator::initPath()
{
    mapPath = new NodeBlock *[column + 2];
    for (int i = 0; i < column + 2; i++) {
        mapPath[i] = new NodeBlock[row + 2];
        for (int j = 0; j < row + 2; j++) {
            mapPath[i][j].c = i - 1;
            mapPath[i][j].r = j - 1;
        }
    }
}

/*
 * 消除方块。
 *   c: 方块横坐标；
 *   r: 方块纵坐标。
 */
void Calculator::setEmpty(int c, int r)
{
    mapValid[c][r] = false;
}

/*
 * 设置方块类型。
 *   c: 方块横坐标；
 *   r: 方块纵坐标；
 *   id: 方块类型。
 */
void Calculator::setId(int c, int r, int id)
{
    mapId[c][r] = id;
}

/*
 * 设置道具类型。
 *   c: 方块横坐标
 *   r: 方块纵坐标
 *   prop: 道具类型
 */
void Calculator::setProp(int c, int r, int prop)
{
    mapProp[c][r] = prop;
}

/*
 * 获取地图的方块消除信息。
 * 返回消除信息矩阵。
 */
int **Calculator::getMapValid()
{
    return mapValid;
}

/*
 * 获取地图的方块类型信息。
 * 返回类型信息矩阵。
 */
int **Calculator::getMapId()
{
    return mapId;
}

/*
 * 获取地图的道具信息。
 * 返回道具信息矩阵。
 */
int **Calculator::getMapProp()
{
    return mapProp;
}

/*
 * 方块是否存在（未消除）。
 *   c: 方块横坐标；
 *   r: 方块纵坐标；
 *   存在时返回true， 否则返回false。
 */
bool Calculator::isOccupied(int c, int r)
{
    if (c < 0 || c >= column || r < 0 || r >= row)
        return false;
    return mapValid[c][r];
}

/*
 * 方块是否可到达（存在一条到达（-1, -1）的路径）。
 *   c: 方块横坐标；
 *   r: 方块纵坐标；
 *   可到达时返回true，否则返回false。
 */
bool Calculator::isReachable(int c, int r)
{
    if (!mapValid[c][r])
        return false;
    return AStar(c, r, -1, -1);
}

/*
 * 获取指定方块的道具类型。
 *   c: 方块横坐标；
 *   r: 方块纵坐标；
 *   返回道具类型。
 */
int Calculator::getProp(int c, int r)
{
    return mapProp[c][r];
}

/*
 * 获取指定方块的打乱位置。
 *   c: 方块横坐标；
 *   r: 放块纵坐标;
 *   返回打乱后的位置坐标。
 */
QPoint Calculator::getShuffle(int c, int r)
{
    for (int i = 0; i < column; i++)
        for (int j = 0; j < row; j++)
            if (mapShuffle[i][j].c == c && mapShuffle[i][j].r == r)
                return {i, j};
    return {-1, -1};
}

/*
 * 打乱。
 */
void Calculator::shuffle()
{
    shuffle(mapValid);
    shuffle(mapId);
    shuffle(mapProp);
}

/*
 * 重置寻路算法的辅助矩阵。
 */
void Calculator::resetPath()
{
    for (int i = 0; i < column + 2; i++)
        for (int j = 0; j < row + 2; j++) {
            mapPath[i][j].g = 0;
            mapPath[i][j].h = 0;
            mapPath[i][j].state = 0;
        }
}

/*
 * A*算法：一种高效的寻路算法，采用估值预测模式，利用辅助矩阵进行标记。
 * 基本思路：自起点开始，逐次寻找相邻节点中估值最优的节点（一般为与终点距离最近的节点），
 * 若陷入死胡同，则说明没有路径。
 *   c1: 起点横坐标；
 *   r1: 起点纵坐标；
 *   c2: 终点横坐标；
 *   r2: 终点纵坐标；
 *   返回是否存在路径。
 */
bool Calculator::AStar(int c1, int r1, int c2, int r2)
{
    // 不考虑终点方块未消除的情况
    if (isOccupied(c2, r2))
        return false;

    // 限制坐标在地图坐标的合法范围内，未在范围内的位置将设置为最近的空闲方块
    limit(c1, r1);
    limit(c2, r2);

    // 重置辅助矩阵
    resetPath();

    NodeBlock *current, *child;
    std::priority_queue<Node> open; // 顺序排列被估值的节点

    // 设置起点为当前节点
    current = &mapPath[c1 + 1][r1 + 1];
    current->g = 0;
    current->h = manhattan(c1, r1, c2, r2);
    current->state = 1;
    open.emplace(current);

    while (true) {
        // 若open列表为空，说明没有可到达的路径
        if (open.empty())
            return false;
        // 若终点被访问到，说明已经找到一条最短路径
        if (mapPath[c2 + 1][r2 + 1].state == 1)
            return true;

        // 将open列表中估值最优的节点设置为当前节点，继续下一层搜索
        current = open.top().b;
        open.pop();

        // 设置当前节已经被访问
        current->state = 2;

        // 搜索当前节点的子节点
        std::queue<Node> children{findAdjacent(current->c, current->r)};
        while (!children.empty()) {
            child = children.front().b;
            children.pop();

            if (child->state == 2)
                // 子节点已经被访问过，则忽略
                continue;
            else if (child->state == 1) {
                // 子节点在open列表中，则根据当前节点更新子节点的估值
                if (child->g > current->g + 1) {
                    child->g = current->g + 1;
                }
            } else {
                // 子节点不再open列表中，则计算估值并加入open列表
                child->state = 1;
                child->g = current->g + 1;
                child->h = manhattan(child->r, child->c, c2, r2);
                open.emplace(child);
            }
        }
    }
}

/*
 * 地图可解性判断。
 * 若可解，则返回一组可被消除的方块坐标， 否则返回NULL。
 */
Inflection *Calculator::Solvable()
{
    // 从第一个方块开始搜索
    // 思路：当两个方块的状态均为可到达时，则可消除，返回第一组可消除的方块
    for (int i = 0; i < column; i++)
        for (int j = 0; j < row; j++) {
            if (!isReachable(i, j))
                continue;
            for (int k = j + 1; k < row; k++)
                if (isReachable(i, k) &&
                        mapId[i][k] == mapId[i][j] &&
                        eliminate(i, k, i, j) != nullptr)
                    return new Inflection({i, k}, {i, j});
            for (int m = i + 1; m < column; m++)
                for (int n = 0; n < row; n++)
                    if (isReachable(m, n) &&
                            mapId[m][n] == mapId[i][j] &&
                            eliminate(m, n, i, j) != nullptr)
                        return new Inflection({m, n}, {i, j});
        }
    return nullptr;
}

/*
 * 消除方块。
 *   c1: 起点横坐标；
 *   r1: 起点纵坐标；
 *   c2: 终点横坐标；
 *   r2: 终点纵坐标；
 *   若可以消除，则返回折线的两个拐点坐标，否则返回NULL。
 */
Inflection *Calculator::eliminate(int c1, int r1, int c2, int r2)
{
    if ((c1 == c2 && r1 == r2) || mapId[c1][r1] != mapId[c2][r2])
        return nullptr;

    Inflection *inf = eliminateHorizonal(c1, r1, c2, r2);
    if (inf != nullptr)
        return inf;

    return eliminateVertical(c1, r1, c2, r2);
}

/*
 * 横向消除。
 *   c1: 起点横坐标；
 *   r1: 起点纵坐标；
 *   c2: 终点横坐标；
 *   r2: 终点纵坐标；
 *   若可以消除，则返回折线的两个拐点坐标，否则返回NULL。
 */
Inflection *Calculator::eliminateHorizonal(int c1, int r1, int c2, int r2)
{
    // 分别计算两个方块横向可以到达的位置
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

    // 判断两个方块的横向可到达点是否可以被连接
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

/*
 * 纵向消除。
 *   c1: 起点横坐标；
 *   r1: 起点纵坐标；
 *   c2: 终点横坐标；
 *   r2: 终点纵坐标；
 *   若可以消除，则返回折线的两个拐点坐标，否则返回NULL。
 */
Inflection *Calculator::eliminateVertical(int c1, int r1, int c2, int r2)
{
    // 分别计算两个方块纵向可以到达的位置
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

    // 判断两个方块的纵向可到达点是否可以被连接
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

/*
 * 打乱算法实现：根据创建地图时初始化的打乱矩阵打乱矩阵对地图进行打乱
 *   map: 需要打乱的地图
 */
void Calculator::shuffle(int **&map)
{
    int **tmp = map;
    map = new int*[column];
    for (int i = 0; i < column; i++)
        map[i] = new int[row];

    for (int i = 0; i < column; i++)
        for (int j = 0; j < row; j++)
            map[i][j] = tmp[mapShuffle[i][j].c][mapShuffle[i][j].r];

    for (int i = 0; i < column; i++)
        delete[] tmp[i];
    delete[] tmp;
}

/*
 * 坐标限制函数：保证坐标的合法性。
 *   c: 方块横坐标；
 *   r: 放块纵坐标。
 */
void Calculator::limit(int &c, int &r)
{
    c = c < -1 ? -1 : c;
    r = r < -1 ? -1 : r;
    c = c > column ? column : c;
    r = r > row ? row : r;
}

/*
 * 计算两个节点的曼哈顿距离，用于A*算法hi节点估值。
 *   c1: 当前点横坐标
 *   r1: 当前点纵坐标
 *   c2: 终点横坐标
 *   r2: 终点纵坐标
 *   返回当前点到终点的曼哈顿距离.
 */
int Calculator::manhattan(int c1, int r1, int c2, int r2)
{
    return std::abs(c2 - c1) + std::abs(r2 - r1);
}

/*
 * 寻找节点的相邻空闲节点，用于A*算法寻找子节点.
 *   c: 方块横坐标；
 *   r: 放块纵坐标。
 */
std::queue<Node> Calculator::findAdjacent(int c, int r)
{
    std::queue<Node> result;
    if (c > -1 && !isOccupied(c - 1, r))
        result.emplace(&mapPath[c][r + 1]);
    if (c < column && !isOccupied(c + 1, r))
        result.emplace(&mapPath[c + 2][r + 1]);
    if (r > -1 && !isOccupied(c, r - 1))
        result.emplace(&mapPath[c + 1][r]);
    if (r < row && !isOccupied(c, r + 1))
        result.emplace(&mapPath[c + 1][r + 2]);
    return result;
}
