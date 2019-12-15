#include "qutesweeperai.hpp"
#include "gamewindow.hpp"

#include <QMessageBox>
#include <iostream>
#include <stdexcept>
#include <vector>

QutesweeperAI::QutesweeperAI(GameState* state)
    : state(state)
{

}

void QutesweeperAI::doMove(GameWindow* window)
{
    if(!state->gameStarted)
    {
        //starting from the center is your best bet
        window->onLeftClick(state->width / 2, state->height / 2);
        return;
    }

    int x, y;
    findEmptyTile(x, y);
    std::cout << "Empty tile at " << x << " " << y << std::endl;

    bool* analyzed = new bool[state->width * state->height];
    for(size_t i = 0; i < state->width * state->height; i++)
        analyzed[i] = false;
    basicStepsRecursive(window, x, y, true, analyzed);
    delete[] analyzed;
}


/*!
 * Convenient template for accessing 2-dimensional arrays
 */
template<typename T>
T get2D(T* array, int width, int x, int y)
{
    return array[y * width + x];
}

/*!
 * Convenient template for accessing 2-dimensional arrays
 */
template<typename T>
void set2D(T* array, int width, int x, int y, T value)
{
    array[y * width + x] = value;
}

void QutesweeperAI::findEmptyTile(int &resultX, int &resultY, int xStart, int yStart)
{
    int xCenter = xStart >= 0 ? xStart : state->width / 2;
    int yCenter = yStart >= 0 ? yStart : state->height / 2;
    int maxRadius = std::max(state->width, state->height);

    std::cout << "Starting at " << xCenter << " " << yCenter << std::endl;

    for(int radius = 0; radius <= maxRadius; radius++)
    {
        for(int x = std::max(0, xCenter - radius); x <= std::min(state->width - 1, xCenter + radius); x++)
        {
            std::cout << "Looking at " << x << " " << yCenter + radius << std::endl;
            if(yCenter + radius < state->height && state->getTile(x, yCenter + radius).uncovered)
            {
                resultX = x;
                resultY = yCenter + radius;
                return;
            }
            std::cout << "Looking at " << x << " " << yCenter - radius << std::endl;
            if(yCenter - radius >= 0 && state->getTile(x, yCenter - radius).uncovered)
            {
                resultX = x;
                resultY = yCenter - radius;
                return;
            }
        }
        for(int y = std::max(0, yCenter - radius + 1); y <= std::min(state->height - 1, yCenter + radius - 1); y++)
        {
            std::cout << "Looking at " << xCenter - radius << " " << y << std::endl;
            if(xCenter - radius >= 0 && state->getTile(xCenter - radius, y).uncovered)
            {
                resultX = xCenter - radius;
                resultY = y;
                return;
            }
            std::cout << "Looking at " << xCenter + radius << " " << y << std::endl;
            if(xCenter + radius < state->width && state->getTile(xCenter + radius, y).uncovered)
            {
                resultX = xCenter + radius;
                resultY = y;
                return;
            }
        }
    }
    std::cout << "Empty tile not found..." << std::endl;
    throw std::logic_error("Empty tile not found");
}

bool QutesweeperAI::basicStepsRecursive(GameWindow* window, int x, int y, bool singleStep, bool* analyzed)
{
    if(!state->getTile(x, y).uncovered || get2D(analyzed, state->width, x, y))
        return false; //has already been analyzed.

    std::cout << "Basic steps: looking at " << x << " " << y << std::endl;

    MineTile tile = state->getTile(x, y);


    if(tile.mineNeighbours > 0)
    {
        //Step 1: check if all neighbouts are mines
        int coveredCount = 0;
        //(unless they've all been flagged already)
        bool foundUnflagged = false;
        for(int xNeigh = std::max(0, x - 1); xNeigh <= std::min(state->width - 1, x + 1); xNeigh++)
        {
            for(int yNeigh = std::max(0, y - 1); yNeigh <= std::min(state->height - 1, y + 1); yNeigh++)
            {
                if(!state->getTile(xNeigh, yNeigh).uncovered)
                    coveredCount++;
                if(!state->getTile(xNeigh, yNeigh).flagged)
                    foundUnflagged = true;
            }
        }
        if(coveredCount < tile.mineNeighbours)
            throw std::logic_error("Tile has less uncovered neigbours than there are mines.");
        else if(foundUnflagged && coveredCount == tile.mineNeighbours)
        {
            for(int xNeigh = std::max(0, x - 1); xNeigh <= std::min(state->width - 1, x + 1); xNeigh++)
            {
                for(int yNeigh = std::max(0, y - 1); yNeigh <= std::min(state->height - 1, y + 1); yNeigh++)
                {
                    if(!state->getTile(xNeigh, yNeigh).uncovered && !state->getTile(xNeigh, yNeigh).flagged)
                    {
                        window->onRightClick(xNeigh, yNeigh);
                        if(singleStep)
                            return true;
                    }
                }
            }
        }

        //Step 2: check if we've flagged enough tiles
        int flaggedCount = 0;
        for(int xNeigh = std::max(0, x - 1); xNeigh <= std::min(state->width - 1, x + 1); xNeigh++)
        {
            for(int yNeigh = std::max(0, y - 1); yNeigh <= std::min(state->height - 1, y + 1); yNeigh++)
            {
                if(!state->getTile(xNeigh, yNeigh).uncovered && state->getTile(xNeigh, yNeigh).flagged)
                    flaggedCount++;
            }
        }
        if(flaggedCount > tile.mineNeighbours)
        {
            QMessageBox::warning(window, window->tr("You've played incorrectly!"),
                                 "At coordinates " + QString::number(x) + ", " + QString::number(y)
                                 + " you've flagged more tiles than there are mines!");
            if(singleStep)
                return true;
        }
        else if(flaggedCount == tile.mineNeighbours)
        {
            for(int xNeigh = std::max(0, x - 1); xNeigh <= std::min(state->width - 1, x + 1); xNeigh++)
            {
                for(int yNeigh = std::max(0, y - 1); yNeigh <= std::min(state->height - 1, y + 1); yNeigh++)
                {
                    if(!state->getTile(xNeigh, yNeigh).uncovered && !state->getTile(xNeigh, yNeigh).flagged)
                    {
                        window->onLeftClick(xNeigh, yNeigh);
                        if(singleStep)
                            return true;
                    }
                }
            }
        }
    }

    set2D(analyzed, state->width, x, y, true);
    if(x + 1 < state->width && basicStepsRecursive(window, x + 1, y, singleStep, analyzed) && singleStep)
        return true;

    if(y + 1 < state->height && basicStepsRecursive(window, x, y + 1, singleStep, analyzed) && singleStep)
        return true;

    if(x - 1 >= 0 && basicStepsRecursive(window, x - 1, y, singleStep, analyzed) && singleStep)
        return true;

    if(y - 1 >= 0 && basicStepsRecursive(window, x, y - 1, singleStep, analyzed) && singleStep)
        return true;

    return false;
}
