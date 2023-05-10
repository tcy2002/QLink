#include "mainwidget.h"

#include <QKeyEvent>
#include <QPainter>
#include <QTimer>
#include <QTime>
#include <QPushButton>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <cmath>

/*
 * column: 地图宽度；
 * row: 地图高度；
 * twoplayers: 是否为双人模式。
 */
MainWidget::MainWidget(int column, int row, bool twoplayers, QWidget *parent)
    : QWidget(parent), map(nullptr), mode(twoplayers), column(column), row(row)
{
    setFixedSize(960, 600);
    setFocusPolicy(Qt::StrongFocus);
    setWindowIcon(QIcon(":/images/3.jpg"));

    int w = 400 / column, h = 300 / row;
    blocksize = (w + h) / 2;
    layoutsize = {column * blocksize, row * blocksize};
    layoutpos = {(800 - layoutsize.rwidth()) / 2, (600 - layoutsize.rheight()) / 2};

    // 初始化地图
    gamebox = new GameBox(column, row, blocksize, this);
    propbox = new GameBox(column, row, blocksize, this);

    // 初始化人物
    head = new Head(0, blocksize, 0, -1, this);
    moveBlock(head, 0, -1, 0, 0);
    if (mode) {
        head2 = new Head(1, blocksize, -1, 0, this);
        moveBlock(head2, -1, 0, 0, 0);
    }

    // 侧边栏
    bar = new SideBar(mode, this);
    bar->move(800, 0);

    // 暂停界面
    menu = new GameMenu(this);
    menu->setVisible(false);

    // 初始化计时器
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(reduceTime()));
    timer->start(1000);

    connect(bar->getButton(), SIGNAL(clicked()), this, SLOT(toggleMenu()));
    connect(menu->getButton(), SIGNAL(clicked()), this, SLOT(download()));
}

MainWidget::~MainWidget()
{
    delete map;
}

/*
 * 初始化游戏地图。
 *   mapVaild: 方块状态矩阵；
 *   mapId: 方块类型矩阵；
 *   mapProp: 道具矩阵。
 */
void MainWidget::initMap(int **mapValid, int **mapId, int **mapProp)
{
    map = new Calculator(column, row, mapValid, mapId, mapProp);

    gamebox->updateBlocks(map->getMapId(), images, 5, blocksize);
    moveBlock(gamebox, 0, 0, 0, 0);

    propbox->updateBlocks(map->getMapProp(), props, mode ? 3 : 4, blocksize / 3.2);
    moveBlock(propbox, 0, 0, 5, (int)(-blocksize / 3.2));

    if (!map->Solvable())
        gameOver();
}

void MainWidget::keyPressEvent(QKeyEvent *event)
{
    if (!enableMove)
        return;

    switch (event->key()) {
    case Qt::Key_D:
        if (mode)
            moveHead(head2, RIGHT);
        break;
    case Qt::Key_Right:
        moveHead(head, RIGHT);
        break;
    case Qt::Key_A:
        if (mode)
            moveHead(head2, LEFT);
        break;
    case Qt::Key_Left:
        moveHead(head, LEFT);
        break;
    case Qt::Key_W:
        if (mode)
            moveHead(head2, UP);
        break;
    case Qt::Key_Up:
        moveHead(head, UP);
        break;
    case Qt::Key_S:
        if (mode)
            moveHead(head2, DOWN);
        break;
    case Qt::Key_Down:
        moveHead(head, DOWN);
        break;
    default:
        break;
    }
}

void MainWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (!enableFlash)
        return;

    if (event->button() == Qt::LeftButton) {
        int x, y, x2, y2;
        x = head->getHeadcolumn();
        y = head->getHeadrow();
        x2 = floor(1.0 * (event->x() - layoutpos.rx()) / blocksize);
        y2 = floor(1.0 * (event->y() - layoutpos.ry()) / blocksize);

        if (!map->isOccupied(x2, y2)) {
            if (map->AStar(x, y, x2, y2))
                head->moveTo(x2, y2);
        } else {
            if (!map->isOccupied(x2 - 1, y2) && map->AStar(x, y, x2 - 1, y2)) {
                head->moveTo(x2 - 1, y2);
                activateOne(0, x2, y2);
            } else if (!map->isOccupied(x2 + 1, y2) && map->AStar(x, y, x2 + 1, y2)) {
                head->moveTo(x2 + 1, y2);
                activateOne(0, x2, y2);
            } else if (!map->isOccupied(x2, y2 - 1) && map->AStar(x, y, x2, y2 - 1)) {
                head->moveTo(x2, y2 - 1);
                activateOne(0, x2, y2);
            } else if (!map->isOccupied(x2, y2 + 1) && map->AStar(x, y, x2, y2 + 1)) {
                head->moveTo(x2, y2 + 1);
                activateOne(0, x2, y2);
            }
        }
    }
}

void MainWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, width(), height(), QPixmap(":/images/background.png"));
}

/*
 * 向指定方向移动人物。
 *   h: 人物实例；
 *   DIR: 移动方向。
 */
void MainWidget::moveHead(Head *h, Direction DIR)
{
    int c = h->getHeadcolumn();
    int r = h->getHeadrow();
    if (map->isOccupied(c, r) || !map->AStar(c, r, -1, -1)) {
        executeMove(h, DIR);
        return;
    }

    int currentcolumn, currentrow;
    switch (DIR) {
    case RIGHT:
        currentcolumn = c + 1;
        currentrow = r;
        break;
    case LEFT:
        currentcolumn = c - 1;
        currentrow = r;
        break;
    case UP:
        currentcolumn = c;
        currentrow = r - 1;
        break;
    case DOWN:
        currentcolumn = c;
        currentrow = r + 1;
        break;
    default:
        break;
    }

    // 若移动方向上存在方块，则启东激活方块的逻辑，人物不移动
    if (map->isOccupied(currentcolumn, currentrow)) {
        activateOne(h->getId(), currentcolumn, currentrow);
        return;
    }

    executeMove(h, DIR);
}

void MainWidget::moveBlock(QWidget *b, int c, int r, int coffset, int roffset)
{
    b->move(layoutpos.rx() + blocksize * c + coffset,
            layoutpos.ry() + blocksize * r + roffset);
}

/*
 * 激活方块。
 *   id: 人物ID；
 *   c: 方块横坐标；
 *   r: 方块纵坐标。
 */
void MainWidget::activateOne(int id, int c, int r)
{
    if (!enableActiavte)
        return;

    HighlightBlock *&block = (id == 0 ? activate : activate2);
    HighlightBlock *&another = (id == 1 ? activate : activate2);

    // 若需要激活的方块已经被另一个玩家激活，则当前玩家无法激活
    if (another && another->column == c && another->row == r)
        return;

    if (!block)
        // 激活的方块是第一个
        highlight(block, c, r, {255, 200, 218});
    else {
        // 激活的方块是第二个
        int c2 = block->column;
        int r2 = block->row;

        // 判断是否可以消除
        Inflection *inf = map->eliminate(c, r, c2, r2);
        if (inf != nullptr) {
            // 绘制消除折线
            drawLines({c, r}, inf->p1, inf->p2, {c2, r2});
            delete inf;

            map->setEmpty(c, r);
            map->setEmpty(c2, r2);

            // 若消除方块包含道具，则执行道具
            if (map->getProp(c, r) == 1 || map->getProp(c2, r2) == 1)
                enableActiavte = false;

            // 定时消除方块（0.5s时间显示消除折线）
            QTimer *timer = new QTimer(this);
            timer->singleShot(500, [this, c, r, c2, r2]{eliminate(c, r, c2, r2);});

            // 计分
            if (id == 0) {
                score++;
                bar->setScore(score);
            } else {
                score2++;
                bar->setScore2(score2);
            }
        }

        delete block;
        block = nullptr;
    }
}

/*
 * 绘制折线。
 *   pi: 折线上的第i个点，p2与p3为拐点。
 */
void MainWidget::drawLines(QPoint p1, QPoint p2, QPoint p3, QPoint p4)
{
    EliminationLine *line = new EliminationLine(blocksize, p1, p2, p3, p4);
    line->setParent(this);
    moveBlock(line, -1, -1, 0, 0);
    line->show();
    lines.enqueue(line);
}

/*
 * 游戏结束。
 */
void MainWidget::gameOver()
{
    enableMove = false;
    if (!mode)
        bar->setText("游戏结束");
    else {
        if (score > score2)
            bar->setText("玩家1获胜");
        else if (score < score2)
            bar->setText("玩家2获胜");
        else
            bar->setText("达成平局");
    }
    if (activate)
        delete activate;
    if (activate2)
        delete activate2;
}

/*
 * 当执行打乱道具时，提示道具正在使用中，需要对高亮方框也进行位置变换。
 *   block: 需要打乱的高亮方块。
 */
void MainWidget::shuffleHint(HighlightBlock *block)
{
    QPoint p(map->getShuffle(block->column, block->row));
    block->column = p.rx();
    block->row = p.ry();
    moveBlock(block, p.rx(), p.ry(), 0, 0);
}

/*
 * 提示时间结束时，删除高亮方框。
 */
void MainWidget::removeHint()
{
    delete hintblock[0];
    delete hintblock[1];
    hintblock[0] = nullptr;
    hintblock[1] = nullptr;
}

/*
 * 执行人物移动。
 *   h: 人物实例；
 *   DIR: 移动方向。
 */
void MainWidget::executeMove(Head *h, Direction DIR)
{
    switch (DIR) {
    case RIGHT:
        h->moveRight();
        break;
    case LEFT:
        h->moveLeft();
        break;
    case UP:
        h->moveUp();
        break;
    case DOWN:
        h->moveDown();
        break;
    default:
        break;
    }
}

/*
 * 执行游戏道具。
 *   prop: 道具类型。
 */
void MainWidget::executeProps(int prop)
{
    switch (prop) {
    case 0:
        increaseTime();  // 增加时间30s
        break;
    case 1:
        shuffle();       // 打乱地图
        break;
    case 2:
        hint();          // 提示
        break;
    case 3:
        if (mode)        // 双人模式不支持鼠标道具
            break;
        flash();         // 鼠标
        break;
    default:
        break;
    }
}

/*
 * 生成高亮方框。
 *   block: 高亮方框保存位置；
 *   c: 位置横坐标；
 *   r: 位置纵坐标；
 *   color: 方框颜色。
 */
void MainWidget::highlight(HighlightBlock *&block, int c, int r, QColor color)
{
    block = new HighlightBlock(c, r, blocksize, color);
    block->setParent(this);
    moveBlock(block, c, r, 0, 0);
    block->show();
}

/*
 * 设置人物位置。
 *   id: 人物ID；
 *   c: 位置横坐标；
 *   r: 位置纵坐标。
 */
void MainWidget::setHeadPos(int id, int c, int r)
{
    if (id == 0)
        head->moveTo(c, r);
    else if (head2)
        head2->moveTo(c, r);
}

/*
 * 设置分数。
 *   id: 人物ID；
 *   sc: 新的分数。
 */
void MainWidget::setScore(int id, int sc)
{
    if (id == 0) {
        score = sc;
        bar->setScore(score);
    }
    else {
        score2 = sc;
        bar->setScore2(score2);
    }
}

/*
 * 设置游戏剩余时间。
 *   t: 新的剩余时间。
 */
void MainWidget::setTime(int t)
{
    time = t;
    bar->setTime(time);
}

/*
 * 游戏暂停。
 * 暂停时禁止移动人物、时间更新。
 */
void MainWidget::Suspend()
{
    enableMove = false;
    if (enableHint > 0)
        timerhint->stop();
    if (timeremaining > 0)
        timerflash->stop();
}

/*
 * 游戏继续。
 * 恢复人物移动以及时间更新。
 */
void MainWidget::Continue()
{
    if (map->Solvable()) {
        enableMove = true;
        if (enableHint > 0)
            timerhint->start();
        if (timeremaining > 0)
            timerflash->start();
    }
}

/*
 * 道具：增加时间30s
 */
void MainWidget::increaseTime()
{
    time += 30;
    bar->setTime(time);
}

/*
 * 道具：打乱。
 */
void MainWidget::shuffle()
{
    map->shuffle();
    gamebox->updateBlocks(map->getMapId(), images, 5, blocksize);
    propbox->updateBlocks(map->getMapProp(), props, 4, blocksize / 3.2);

    // 若存在提示方框，则一并打乱。
    if (hintblock[0] && hintblock[1]) {
        shuffleHint(hintblock[0]);
        shuffleHint(hintblock[1]);
        if (map->eliminate(hintblock[0]->column, hintblock[0]->row,
                           hintblock[1]->column, hintblock[1]->row) == nullptr) {
            removeHint();
        }
    }

    // 若剩余地图无解，则游戏结束。
    if (map->Solvable() == nullptr)
        gameOver();
}

/*
 * 道具：提示。
 */
void MainWidget::hint()
{
    if (enableHint > 0)
        timerhint->stop();
    enableHint = 10;

    timerhint = new QTimer();
    connect(timerhint, SIGNAL(timeout()), this, SLOT(reduceHintTime()));
    timerhint->start(1000);
}

/*
 * 道具：鼠标。
 * 仅支持单人模式。
 */
void MainWidget::flash()
{
    enableFlash = true;
    if (timeremaining > 0)
        timerflash->stop();

    timeremaining = 5;
    bar->enableTimeRemaining();
    bar->setTimeRemaining(timeremaining);

    timerflash = new QTimer();
    connect(timerflash, SIGNAL(timeout()), this, SLOT(reduceTimeremaining()));
    timerflash->start(1000);
}

/*
 * 方块消除。
 *   c1: 起点横坐标；
 *   r1: 起点纵坐标；
 *   c2: 终点横坐标；
 *   r2: 终点纵坐标.
 */
void MainWidget::eliminate(int c1, int r1, int c2, int r2)
{
    delete lines.dequeue();

    // 删除地图方块
    gamebox->removeBlock(c1, r1);
    gamebox->removeBlock(c2, r2);
    map->setId(c1, r1, 5);
    map->setId(c2, r2, 5);

    // 删除道具方块
    propbox->removeBlock(c1, r1);
    propbox->removeBlock(c2, r2);
    int prop1 = map->getProp(c1, r1);
    int prop2 = map->getProp(c2, r2);
    map->setProp(c1, r1, 4);
    map->setProp(c2, r2, 4);

    // 更新高亮方框
    if (hintblock[0] && hintblock[1] &&
            ((hintblock[0]->column == c1 && hintblock[0]->row == r1) ||
            (hintblock[0]->column == c2 && hintblock[0]->row == r2) ||
            ((hintblock[1]->column == c1 && hintblock[1]->row == r1) ||
            (hintblock[1]->column == c2 && hintblock[1]->row == r2)))) {
        removeHint();
    }

    // 执行道具
    executeProps(prop1);
    executeProps(prop2);

    // 若剩余方块无解，则游戏结束。
    auto inf = map->Solvable();
    if (inf == nullptr)
        gameOver();
    else {
        if (enableHint > 0 && !hintblock[0] && !hintblock[1]) {
            // 绘制提示道具的高亮方框
            highlight(hintblock[0], inf->p1.rx(), inf->p1.ry(), {215, 215, 215});
            highlight(hintblock[1], inf->p2.rx(), inf->p2.ry(), {215, 215, 215});
        }
        delete inf;
    }

    if (!enableActiavte)
        enableActiavte = true;
}

/*
 * 减少游戏剩余时间，用于定时器。
 */
void MainWidget::reduceTime()
{
    if (!enableMove || time <= 0)
        return;
    time--;
    if (time == 0)
        gameOver();
    bar->setTime(time);
}

/*
 * 减少提示道具剩余时间，用于定时器。
 */
void MainWidget::reduceHintTime()
{
    enableHint--;
    if (enableHint <= 0 && hintblock[0] && hintblock[1]) {
        timerhint->stop();
        delete hintblock[0];
        delete hintblock[1];
        hintblock[0] = nullptr;
        hintblock[1] = nullptr;
    }
}

/*
 * 减少鼠标道具时间，用于定时器。
 */
void MainWidget::reduceTimeremaining()
{
    timeremaining--;
    if (timeremaining <= 0) {
        enableFlash = false;
        bar->disableTimeRemaining();
        timerflash->stop();
    }
    bar->setTimeRemaining(timeremaining);
}

/*
 * 打开菜单。
 */
void MainWidget::toggleMenu()
{
    QPushButton *button = bar->getButton();
    if (button->isChecked()) {
        menu->setVisible(true);
        Suspend();
    }
    else {
        menu->setVisible(false);
        Continue();
    }
}

/*
 * 保存游戏存档。
 */
void MainWidget::download()
{
    int **mapValid = map->getMapValid();
    int **mapId = map->getMapId();
    int **mapProp = map->getMapProp();

    QFile file;
    file.setFileName(QFileDialog::getSaveFileName(this, tr("保存路径"), tr("/archive"), tr("数据文件(*.dat)")));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    // 写入文件头
    file.write("MyQlink-tcy-20220731-1.0\n");
    file.write(tr("%1 %2 %3 %4\n").arg(int(mode)).arg(column).arg(row).arg(time).toUtf8());
    file.write(tr("%1 %2 %3\n").arg(head->getHeadcolumn()).arg(head->getHeadrow()).arg(score).toUtf8());
    if (mode)
        file.write(tr("%1 %2 %3n").arg(head2->getHeadcolumn()).arg(head2->getHeadrow()).arg(score2).toUtf8());

    // 写入地图信息
    SAVE_MAP(mapValid, file)
    SAVE_MAP(mapId, file)
    SAVE_MAP(mapProp, file)

    file.close();
}
