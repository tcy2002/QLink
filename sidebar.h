#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QWidget>
#include <QPushButton>

namespace Ui {
class SideBar;
}

/*
 * 游戏内侧边栏.
 */
class SideBar : public QWidget
{
    Q_OBJECT

public:
    explicit SideBar(bool mode = false, QWidget *parent = nullptr);
    ~SideBar();
    void setScore(int score);
    void setScore2(int score);
    void setTime(int time);
    void setText(QString str);
    void setTimeRemaining(int time);
    void enableTimeRemaining();
    void disableTimeRemaining();
    QPushButton *getButton();

private:
    Ui::SideBar *ui;
};

#endif // SIDEBAR_H
