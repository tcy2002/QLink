#include "homemenu.h"
#include "ui_homemenu.h"

#include <QMenu>
#include <QFile>
#include <QFileDialog>
#include <QByteArray>
#include <QDebug>

HomeMenu::HomeMenu(QWidget *parent) :
    QWidget(parent), game(nullptr),
    ui(new Ui::HomeMenu)
{
    ui->setupUi(this);
    ui->selectWidth->setText(tr("宽度：%1").arg(width));
    ui->selectHeight->setText(tr("高度：%1").arg(height));

    QMenu *menu;
    QAction *action;

    menu = new QMenu(this);
    ui->selectWidth->setMenu(menu);
    for (int i = 4; i <= 12; i++) {
        action = new QAction(this);
        action->setText(tr("%1").arg(i));
        action->setProperty("id", {i});
        menu->addAction(action);
    }
    connect(menu, SIGNAL(triggered(QAction *)), this, SLOT(setWidth(QAction *)));

    menu = new QMenu(this);
    ui->selectHeight->setMenu(menu);
    for (int i = 3; i <= 10; i++) {
        action = new QAction(this);
        action->setText(tr("%1").arg(i));
        action->setProperty("id", {i});
        menu->addAction(action);
    }
    connect(menu, SIGNAL(triggered(QAction *)), this, SLOT(setHeight(QAction *)));
}

HomeMenu::~HomeMenu()
{
    delete ui;
    if (game)
        delete game;
}

/*
 * 根据文件输入流初始化地图。
 * 读取文件存档时使用。
 *   c: 地图宽度；
 *   r: 地图高度；
 *   ts: 字符串输入流。
 */
int **HomeMenu::initMap(int c, int r, QTextStream &ts)
{
    int **map = new int*[c];
    for (int i = 0; i < c; i++) {
        map[i] = new int[r];
        for (int j = 0; j < r; j++)
            ts >> map[i][j];
    }
    return map;
}

/*
 * 设置游戏地图宽度。
 */
void HomeMenu::setWidth(QAction *action)
{
    width = *(int *)action->property("id").data();
    ui->selectWidth->setText(tr("宽度：%1").arg(width));
}

/*
 * 设置游戏地图高度。
 */
void HomeMenu::setHeight(QAction *action)
{
    height = *(int *)action->property("id").data();
    ui->selectHeight->setText(tr("高度：%1").arg(height));
}

/*
 * 开始游戏。
 */
void HomeMenu::on_startGame_clicked()
{
    game = new MainWidget(width, height);
    game->initMap();
    game->show();
    connect(game->menu->getButtonExit(), SIGNAL(clicked()), this, SLOT(endGame()));
    setVisible(false);
}

/*
 * 双人模式。
 */
void HomeMenu::on_twoPlayerMode_clicked()
{
    game = new MainWidget(width, height, true);
    game->initMap();
    game->show();
    connect(game->menu->getButtonExit(), SIGNAL(clicked()), this, SLOT(endGame()));
    setVisible(false);
}

/*
 * 结束游戏。
 */
void HomeMenu::endGame()
{
    setVisible(true);
    delete game;
    game = nullptr;
}

/*
 * 打开存档。
 */
void HomeMenu::on_selectArchive_clicked()
{
    QFile file;
    file.setFileName(QFileDialog::getOpenFileName(this, tr("打开存档"), tr("/"), tr("数据文件(*.dat)")));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    // 文件头信息：游戏模式，地图大小，剩余时间，人物位置，当前分数
    QString flag;
    int mode, column, row, time;
    int headc, headr, headc2, headr2;
    int score, score2;

    QByteArray data = file.readAll();
    QTextStream ts(data);
    ts >> flag;
    // 用指定字符串进行识别，防止用户打开非法存档文件
    if (flag != "MyQlink-tcy-20220731-1.0")
        return;

    ts >> mode >> column >> row >> time;
    ts >> headc >> headr >> score;
    if (mode)
        ts >> headc2 >> headr2 >> score2;

    // 初始化游戏地图
    int **mapVaild = initMap(column, row, ts);
    int **mapId = initMap(column, row, ts);
    int **mapProp = initMap(column, row, ts);

    file.close();

    // 设置游戏生剩余时间、人物位置、当前分数
    game = new MainWidget(column, row, mode);
    game->initMap(mapVaild, mapId, mapProp);
    game->setTime(time);
    game->setHeadPos(0, headc, headr);
    game->setScore(0, score);
    if (mode) {
        game->setHeadPos(1, headc2, headr2);
        game->setScore(1, score2);
    }
    game->show();
    connect(game->menu->getButtonExit(), SIGNAL(clicked()), this, SLOT(endGame()));
    setVisible(false);
}
