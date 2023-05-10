#include "head.h"

/*
 * id: 人物ID；
 * size: 方块像素宽度；
 * hc: 位置横坐标；
 * hr: 位置纵坐标；
 */
Head::Head(int id, int size, int hc, int hr, QWidget *parent)
    :QWidget(parent), id(id), headsize(size), headcolumn(hc), headrow(hr)
{
    headimage = new QLabel();
    headimage->setParent(this);
    headimage->setPixmap(QPixmap(":/images/head.png").scaled(headsize, headsize));
}

Head::~Head()
{
    delete headimage;
}

/*
 * 获取位置横坐标。
 */
int Head::getHeadcolumn()
{
    return headcolumn;
}

/*
 * 获取位置纵坐标。
 */
int Head::getHeadrow()
{
    return headrow;
}

/*
 * 获取人物ID。
 */
int Head::getId()
{
    return id;
}

/*
 * 设置位置横坐标。
 */
void Head::setHeadcolumn(int c)
{
    headcolumn = c;
}

/*
 * 设置位置纵坐标。
 */
void Head::setHeadrow(int r)
{
    headrow = r;
}

/*
 * 右移一个单位。
 */
void Head::moveRight()
{
    headcolumn += 1;
    move(x() + headsize, y());
}

/*
 * 左移一个单位。
 */
void Head::moveLeft()
{
    headcolumn -= 1;
    move(x() - headsize, y());
}

/*
 * 上移一个单位。
 */
void Head::moveUp()
{
    headrow -= 1;
    move(x(), y() - headsize);
}

/*
 * 下移一个单位。
 */
void Head::moveDown()
{
    headrow += 1;
    move(x(), y() + headsize);
}

/*
 * 移至指定位置。
 *   c: 位置横坐标；
 *   r: 位置纵坐标。
 */
void Head::moveTo(int c, int r)
{
    move(x() + headsize * (c - headcolumn), y() + headsize * (r - headrow));
    headcolumn = c;
    headrow = r;
}
