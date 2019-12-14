#include "gamewindow.hpp"
#include "./ui_gamewindow.h"

#include <QPushButton>

GameWindow::GameWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GameWindow)
{
    ui->setupUi(this);
    currentState = GameState(10, 10);
    populateButtons();
}

void GameWindow::populateButtons()
{
    buttonsArray = new QPushButton[currentState.width * currentState.height];
    for(int i = 0; i < currentState.width; i++)
    {
        for(int j = 0; j < currentState.height; j++)
        {
            QPushButton* button = new QPushButton("", centralWidget());
            button->setGeometry(i * 32, j * 32, 30, 30);
            buttonsLayout.addWidget(button, i, j);
        }
    }
    centralWidget()->setLayout(&buttonsLayout);
}

QPushButton* GameWindow::getButton(int x, int y)
{
    return &buttonsArray[y * currentState.width + x];
}

GameWindow::~GameWindow()
{
    delete ui;
}

