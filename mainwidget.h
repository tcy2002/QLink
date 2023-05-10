#pragma once

#include <QWidget>

#include "gamebox.h"
#include "sidebar.h"
#include "head.h"
#include "highlightblock.h"
#include "calculator.h"
#include "eliminationline.h"
#include "gamemenu.h"

/*
 * 主模块：游戏的主要逻辑的实现在此类中完成。
 */
class MainWidget : public QWidget
{
    Q_OBJECT

private:
    GameBox *gamebox;    // 地图
    GameBox *propbox;    // 道具
    SideBar *bar;        // 侧边栏

    Calculator *map;     // 逻辑计算类

    int score{};         // 玩家1分数
    int score2{};        // 玩家2分数
    Head *head{};        // 玩家1
    Head *head2{};       // 玩家2
    HighlightBlock *activate{};         // 玩家1激活方块
    HighlightBlock *activate2{};        // 玩家2激活方块

    HighlightBlock *hintblock[2]{};     // 提示方块
    QQueue<EliminationLine *> lines{};  // 消除曲线

    QTimer *timerhint{};    // 提示道具计时器
    QTimer *timerflash{};   // 鼠标道具计时器

    bool mode;                  // 游戏模式（单人/双人）
    int column;                 // 地图宽度
    int row;                    // 地图高度
    int blocksize;              // 方块大小
    int time{10};               // 游戏剩余时间
    int timeremaining{};        // 提示道具剩余时间
    bool enableMove{true};      // 人物是否可以移动
    bool enableActiavte{true};  // 方块是否可以被激活
    int enableHint{};           // 提示道具
    bool enableFlash{};         // 鼠标道具
    QSize layoutsize;           // 地图界面大小
    QPoint layoutpos;           // 地图界面在主界面的位置

public:
    GameMenu *menu;

public:
    MainWidget(int column, int row, bool twoplayers = false, QWidget *parent = nullptr);
    ~MainWidget();

    // 初始化地图
    void initMap(int **mapValid = nullptr, int **mapId = nullptr, int **mapProp = nullptr);

    // 游戏参数设置
    void setHeadPos(int id, int c, int r);
    void setScore(int id, int sc);
    void setTime(int t);

private:
    // Qt事件函数
    void keyPressEvent(QKeyEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *) override;

    // 绘制相关
    void moveHead(Head *h, Direction DIR);
    void moveBlock(QWidget *b, int c, int r, int coffset, int roffset);
    void activateOne(int id, int c, int r);
    void drawLines(QPoint p1, QPoint p2, QPoint p3, QPoint p4);
    void gameOver();
    void shuffleHint(HighlightBlock *block);
    void removeHint();
    void executeMove(Head *h, Direction DIR);
    void executeProps(int prop);
    void highlight(HighlightBlock *&block, int c, int r, QColor color);
    void eliminate(int c1, int r1, int c2, int r2);

    // 游戏暂停与继续
    void Suspend();
    void Continue();

    // 道具
    void increaseTime();
    void shuffle();
    void hint();
    void flash();

private slots:
    void reduceTime();
    void reduceHintTime();
    void reduceTimeremaining();
    void toggleMenu();
    void download();
};
