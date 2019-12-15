#include "gamewindow.hpp"
#include "./ui_gamewindow.h"

#include <iostream>

#include <QPushButton>

GameWindow::GameWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GameWindow)
{
    ui->setupUi(this);
    currentState = GameState(10, 10);
    mainWidget = new QWidget();
    setCentralWidget(mainWidget);
    populateButtons();
}

void GameWindow::populateButtons()
{
    buttonsArray = new QPushButton*[currentState.width * currentState.height];

    const int buttonSize = 32;
    const int buttonMargin = 2;

    playAreaLayout = new QBoxLayout(QBoxLayout::LeftToRight, centralWidget());
    playAreaLayout->setContentsMargins(5, 5, 5, 5);
    playArea = new QFrame(centralWidget());
    playArea->setFrameShape(QFrame::Shape::Box);
    playArea->setFrameShadow(QFrame::Shadow::Sunken);
    playArea->setMinimumSize(2 * buttonMargin + currentState.width * buttonSize,
                             2 * buttonMargin + currentState.width * buttonSize);
    playArea->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    playAreaLayout->addWidget(playArea, Qt::AlignCenter);

    for(int i = 0; i < currentState.width; i++)
    {
        for(int j = 0; j < currentState.height; j++)
        {
            getButton(i, j) = new QPushButton("", playArea);
            getButton(i, j)->setGeometry(buttonMargin + i * buttonSize, buttonMargin + j * buttonSize,
                                         buttonSize, buttonSize);
        }
    }
}

QPushButton*& GameWindow::getButton(int x, int y)
{
    return buttonsArray[y * currentState.width + x];
}

GameWindow::~GameWindow()
{
    delete ui;
    for(int x = 0; x < currentState.width; x++)
    {
        for(int y = 0; y < currentState.height; y++)
        {
            delete getButton(x, y);
        }
    }
    delete[] buttonsArray;
    delete playArea;
    delete playAreaLayout;
}

