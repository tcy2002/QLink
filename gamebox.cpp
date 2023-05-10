#include "gamebox.h"

#include <QDebug>

/*
 * column: 地图宽度；
 * row: 地图高度；
 * blocksize: 方块像素宽度。
 */
GameBox::GameBox(int column, int row, int blocksize, QWidget *parent)
    : QWidget(parent), column(column), row(row), blocksize(blocksize)
{
    layoutsize.setWidth(blocksize * column);
    layoutsize.setHeight(blocksize * row);
    resize(layoutsize.rwidth(), layoutsize.rheight());

    box = new QGridLayout();
    box->setGeometry(QRect(0, 0, layoutsize.rwidth(), layoutsize.rheight()));
    box->setSpacing(0);
    box->setMargin(0);

    lbs = new QLabel**[row];
    for (int i = 0; i < row; i++) {
        lbs[i] = new QLabel*[column];
        for (int j = 0; j < column; j++) {
            QLabel *lb = new QLabel();
            lb->resize(blocksize, blocksize);
            box->addWidget(lb, i, j);
            lbs[i][j] = lb;
        }
    }

    setLayout(box);
}

GameBox::~GameBox()
{
    delete box;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++)
            delete lbs[i][j];
        delete []lbs[i];
    }
    delete []lbs;
}

/*
 * 从地图界面中删除指定方块。
 *   c: 方块横坐标；
 *   r: 方块纵坐标。
 */
void GameBox::removeBlock(int c, int r)
{
    lbs[r][c]->setPixmap(QPixmap());
}

/*
 * 更新地图。
 *   map: 新的地图；
 *   images: 贴图；
 *   num: 方块种类数；
 *   size: 贴图宽度。
 */
void GameBox::updateBlocks(int **map, const QString images[], int num, int size)
{
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            if (map[j][i] < num)
                lbs[i][j]->setPixmap(QPixmap(images[map[j][i]]).scaled(size, size));
            else
                lbs[i][j]->setPixmap(QPixmap());
        }
    }
}
