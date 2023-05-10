#pragma once

#include <QWidget>
#include <QTextStream>

#include "mainwidget.h"

namespace Ui {
class HomeMenu;
}

/*
 * home界面。
 */
class HomeMenu : public QWidget
{
    Q_OBJECT

private:
    MainWidget *game;

    int width = 6;
    int height = 4;

public:
    explicit HomeMenu(QWidget *parent = nullptr);
    ~HomeMenu();

private:
    Ui::HomeMenu *ui;

private:
    int **initMap(int c, int r, QTextStream &ts);

private slots:
    void setWidth(QAction *action);
    void setHeight(QAction *action);
    void endGame();
    void on_startGame_clicked();
    void on_twoPlayerMode_clicked();
    void on_selectArchive_clicked();
};
