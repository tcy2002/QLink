#include "gamemenu.h"
#include "ui_gamemenu.h"

#include <QPainter>
#include <QDebug>
#include <QFileDialog>

GameMenu::GameMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameMenu)
{
    ui->setupUi(this);
}

GameMenu::~GameMenu()
{
    delete ui;
}

/*
 * 下载按钮。
 */
QPushButton *GameMenu::getButton()
{
    return ui->DownloadButton;
}

/*
 * 退出按钮。
 */
QPushButton *GameMenu::getButtonExit()
{
    return ui->HomeButton;
}

void GameMenu::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QColor color(153, 217, 234, 100);

    painter.setPen(Qt::NoPen);
    painter.setBrush(color);
    painter.drawRect(0, 0, width(), height());
}

/*
 * 打开游戏存档。
 */
void GameMenu::selectFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("打开游戏存档"), "D:", tr("文本文件(*txt)"));
    qDebug() << "fileName:" << fileName;
}
