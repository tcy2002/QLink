#pragma once

#include <QWidget>
#include <QLabel>
#include <QGridLayout>

/*
 * 游戏地图界面。
 */
class GameBox : public QWidget
{
    Q_OBJECT

private:
    QGridLayout *box;
    QLabel ***lbs;

    int column;
    int row;
    int blocksize;
    QSize layoutsize;

public:
    GameBox(int column, int row, int blocksize, QWidget *parent = nullptr);
    ~GameBox();
    void removeBlock(int c, int r);
    void updateBlocks(int **map, const QString images[], int num, int size);

private:
    void deleteLbs();
};
