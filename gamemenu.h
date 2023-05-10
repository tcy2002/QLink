#pragma once

#include <QWidget>
#include <QPushButton>

namespace Ui {
class GameMenu;
}

/*
 * 游戏界面的侧边栏。
 */
class GameMenu : public QWidget
{
    Q_OBJECT

public:
    explicit GameMenu(QWidget *parent = nullptr);
    ~GameMenu();
    QPushButton *getButton();
    QPushButton *getButtonExit();

private:
    void paintEvent(QPaintEvent *) override;

private:
    Ui::GameMenu *ui;

private slots:
    void selectFile();
};
