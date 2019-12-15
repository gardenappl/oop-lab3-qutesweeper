#include "gamewindow.hpp"
#include "./ui_gamewindow.h"

#include <iostream>
#include <random>

#include <QPushButton>

GameWindow::GameWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GameWindow)
{
    ui->setupUi(this);
    currentState = GameState(10, 10, 20);
    mainWidget = new QWidget();
    setCentralWidget(mainWidget);
    init();

}

void GameWindow::init()
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
            QPushButton* button = new QPushButton("", playArea);
            button->setGeometry(buttonMargin + i * buttonSize, buttonMargin + j * buttonSize,
                                         buttonSize, buttonSize);
            connect(button, &QPushButton::clicked, this,
                    [=](bool checked){ this->onClick(i, j); });
            getButton(i, j) = button;
        }
    }
}

QPushButton*& GameWindow::getButton(int x, int y)
{
    return buttonsArray[y * currentState.width + x];
}

void GameWindow::populateGrid(int startX, int startY)
{
    static std::default_random_engine rng;
    std::uniform_int_distribution<int> xDist(0, currentState.width - 1);
    std::uniform_int_distribution<int> yDist(0, currentState.height - 1);
    for(int i = 0; i < currentState.mineCount; i++)
    {
        int x, y;
        do
        {
            x = xDist(rng);
            y = yDist(rng);
        }
        while(currentState.getTile(x, y).isMine() || (x == startX && y == startY));
        MineTile& tile = currentState.getTile(x, y);
        tile.mineNeighbours = -1;
        for(int xNeigh = std::max(0, x - 1); xNeigh <= std::min(currentState.width - 1, x + 1); xNeigh++)
        {
            for(int yNeigh = std::max(0, x - 1); yNeigh <= std::min(currentState.width - 1, x + 1); yNeigh++)
            {
                MineTile& neighbourTile = currentState.getTile(xNeigh, yNeigh);
                if(!neighbourTile.isMine())
                    neighbourTile.mineNeighbours++;
            }
        }
    }
}

void GameWindow::onClick(int i, int j)
{
    std::cout << i << ' ' << j << std::endl;
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
    delete mainWidget;
}

