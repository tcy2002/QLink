#include "sidebar.h"
#include "ui_sidebar.h"

#include <QPainter>
#include <QtDebug>

/*
 * mode: 游戏模式（false: 单人/true: 双人）
 */
SideBar::SideBar(bool mode, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SideBar)
{
    ui->setupUi(this);
    setTime(60);
    setScore(0);
    setScore2(0);
    disableTimeRemaining();

    // 双人模式以下内容不显示
    if (!mode) {
        ui->LCDScore2->setVisible(false);
        ui->label->setVisible(false);
        ui->label2->setVisible(false);
    }
}

SideBar::~SideBar()
{
    delete ui;
}

/*
 * 设置玩家1分数。
 */
void SideBar::setScore(int score)
{
    ui->LCDScore->display(score);
}

/*
 * 设置玩家2分数。
 */
void SideBar::setScore2(int score)
{
    ui->LCDScore2->display(score);
}

/*
 * 设置剩余时间。
 */
void SideBar::setTime(int time)
{
    ui->LCDTime->display(time);
}

/*
 * 设置显示文本。
 */
void SideBar::setText(QString str)
{
    ui->GameOverLabel->setText(str);
}

/*
 * 设置道具的剩余时间。
 */
void SideBar::setTimeRemaining(int time)
{
    ui->LCDTimeRemaining->display(time);
}

/*
 * 启用道具。
 */
void SideBar::enableTimeRemaining()
{
    ui->LCDTimeRemaining->setVisible(true);
}

/*
 * 禁用道具。
 */
void SideBar::disableTimeRemaining()
{
    ui->LCDTimeRemaining->setVisible(false);
}

/*
 * 获取按钮实例。
 */
QPushButton *SideBar::getButton()
{
    return ui->SuspendButton;
}
