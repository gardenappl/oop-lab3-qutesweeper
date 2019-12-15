#include "gamewindow.hpp"
#include "./ui_gamewindow.h"
#include "newgamedialog.hpp"

#include <iostream>
#include <random>

#include <QMessageBox>
#include <QPushButton>

GameWindow::GameWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GameWindow)
{
    ui->setupUi(this);
    currentState = new GameState(10, 10, 15);
    mainWidget = new QWidget();
    setCentralWidget(mainWidget);

    connect(ui->actionStart_New_Game, &QAction::triggered, this, &GameWindow::newGameStart);
    connect(ui->actionDo_one_move, &QAction::triggered, this, &GameWindow::requestAIMove);

    init();
}

void GameWindow::init()
{
    buttonsArray = new RightClickableButton*[currentState->width * currentState->height];

    const int buttonSize = 32;
    const int buttonMargin = 2;

    playAreaLayout = new QBoxLayout(QBoxLayout::LeftToRight, centralWidget());
    playAreaLayout->setContentsMargins(5, 5, 5, 5);
    playArea = new QFrame(centralWidget());
    playArea->setFrameShape(QFrame::Shape::Box);
    playArea->setFrameShadow(QFrame::Shadow::Sunken);
    playArea->setMinimumSize(2 * buttonMargin + currentState->width * buttonSize,
                             2 * buttonMargin + currentState->height * buttonSize);
    playArea->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    playAreaLayout->addWidget(playArea, Qt::AlignCenter);

    for(int i = 0; i < currentState->width; i++)
    {
        for(int j = 0; j < currentState->height; j++)
        {
            RightClickableButton* button = new RightClickableButton(playArea);
            button->setGeometry(buttonMargin + i * buttonSize, buttonMargin + j * buttonSize,
                                         buttonSize, buttonSize);
            connect(button, &RightClickableButton::clicked, this,
                    [=](bool checked){ this->onLeftClick(i, j); });
            connect(button, &RightClickableButton::rightClicked, this,
                    [=](){ this->onRightClick(i, j); });
            getButton(i, j) = button;
        }
    }
}

void GameWindow::uninit()
{
    //delete ui;
    if(currentState == nullptr)
        return;
    for(int x = 0; x < currentState->width; x++)
    {
        for(int y = 0; y < currentState->height; y++)
        {
            delete getButton(x, y);
        }
    }
    delete[] buttonsArray;
    delete playArea;
    delete playAreaLayout;
    delete currentState;
    delete ai;
    ai = nullptr;
    currentState = nullptr;
}

RightClickableButton*& GameWindow::getButton(int x, int y)
{
    return buttonsArray[y * currentState->width + x];
}

void GameWindow::populateGrid(int startX, int startY)
{
    static std::default_random_engine rng;
    std::uniform_int_distribution<int> xDist(0, currentState->width - 1);
    std::uniform_int_distribution<int> yDist(0, currentState->height - 1);
    for(int i = 0; i < currentState->mineCount; i++)
    {
        int x, y;
        do
        {
            x = xDist(rng);
            y = yDist(rng);
        }
        while(currentState->getTile(x, y).isMine() || (x == startX && y == startY));
        MineTile& tile = currentState->getTile(x, y);
        tile.mineNeighbours = -1;
        for(int xNeigh = std::max(0, x - 1); xNeigh <= std::min(currentState->width - 1, x + 1); xNeigh++)
        {
            for(int yNeigh = std::max(0, y - 1); yNeigh <= std::min(currentState->height - 1, y + 1); yNeigh++)
            {
                MineTile& neighbourTile = currentState->getTile(xNeigh, yNeigh);
                if(!neighbourTile.isMine())
                    neighbourTile.mineNeighbours++;
            }
        }
        std::cout << "Put mine at " << x << " " << y << std::endl;
    }
}

void GameWindow::newGameStart(bool restart)
{
    NewGameDialog* dialog;
    if(restart)
        dialog = new NewGameDialog(currentState->width, currentState->height, currentState->mineCount);
    else
        dialog = new NewGameDialog();
    //dialog->setModal(true);
    dialog->exec();

    int x, y, mineCount;
    std::cout << "Creating game state" << std::endl;
    dialog->getOptions(x, y, mineCount);
    delete dialog;
    uninit();
    currentState = new GameState(x, y, mineCount);
    init();
}

void GameWindow::onLeftClick(int i, int j)
{
    //The game is initialized on the first click, to guarantee that the first tile is not a mine.
    if(!currentState->gameStarted)
    {
        populateGrid(i, j);
        currentState->gameStarted = true;
    }

    std::cout << i << ' ' << j << std::endl;
    MineTile& tile = currentState->getTile(i, j);
    if(tile.isMine())
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Game over!"));
        msgBox.setInformativeText(tr("Would you like to try again?"));
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Yes);
        int choice = msgBox.exec();
        if(choice == QMessageBox::No)
        {
            uninit();
            this->close();
        }
        else
            newGameStart(true);
    }
    else
    {
        popTileSafe(tile, i, j);
    }
}

void GameWindow::onRightClick(int i, int j)
{
    MineTile& tile = currentState->getTile(i, j);
    RightClickableButton* button = getButton(i, j);
    if(tile.flagged)
    {
        tile.flagged = false;
        button->setIcon(QIcon());
    }
    else
    {
        tile.flagged = true;
        button->setIcon(QIcon(":/resources/flag.png"));
    }
}

void GameWindow::popTileSafe(MineTile& tile, int x, int y)
{
    tile.uncovered = true;
    getButton(x, y)->setDisabled(true);
    getButton(x, y)->setFlat(true);
    if(tile.mineNeighbours > 0)
        getButton(x, y)->setText(QString::number(tile.mineNeighbours));

    currentState->uncoveredCount++;
    if(currentState->uncoveredCount >= currentState->width * currentState->height - currentState->mineCount)
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Congratulations!"));
        msgBox.setInformativeText(tr("Would you like to try again?"));
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Yes);
        int choice = msgBox.exec();
        if(choice == QMessageBox::No)
        {
            uninit();
            this->close();
        }
        else
            newGameStart(true);
    }

    if(tile.mineNeighbours == 0)
    {
        //TO-DO: turn this loop into a function? E.g. forEachNeighbourDo(&funcName);
        for(int xNeigh = std::max(0, x - 1); xNeigh <= std::min(currentState->width - 1, x + 1); xNeigh++)
        {
            for(int yNeigh = std::max(0, y - 1); yNeigh <= std::min(currentState->height - 1, y + 1); yNeigh++)
            {
                MineTile& tileNeigh = currentState->getTile(xNeigh, yNeigh);
                if(!tileNeigh.uncovered)
                    popTileSafe(tileNeigh, xNeigh, yNeigh);
            }
        }
    }
}

void GameWindow::requestAIMove()
{
    if(ai == nullptr)
        ai = new QutesweeperAI(currentState);
    ai->doMove(this);
}

GameWindow::~GameWindow()
{
    uninit();
    delete ui;
    delete mainWidget;
}

