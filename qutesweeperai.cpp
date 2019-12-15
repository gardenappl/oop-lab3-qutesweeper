#include "qutesweeperai.hpp"
#include "gamewindow.hpp"

#include <QMessageBox>
#include <iostream>
#include <stdexcept>
#include <queue>
#include <vector>
#include <map>

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

    std::vector<bool> analyzed;
    analyzed.reserve(state->width * state->height);
    for(size_t i = 0; i < state->width * state->height; i++)
        analyzed.push_back(false);

    int x, y;
    while(findEmptyTile(x, y, analyzed))
    {
        std::cout << "Empty tile at " << x << " " << y << std::endl;
        if(basicStepsRecursive(window, x, y, true, analyzed))
            return;
    }

    tankAlgorithm(window, true, analyzed);
}


/*!
 * Convenient template for accessing 2-dimensional arrays
 */
template<typename T>
T get2D(const T* array, int width, int x, int y)
{
    return array[y * width + x];
}

template<typename T>
T get2D(const std::vector<T>& array, int width, int x, int y)
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

template<typename T>
void set2D(std::vector<T>& array, int width, int x, int y, T value)
{
    array[y * width + x] = value;
}

bool QutesweeperAI::findEmptyTile(int &resultX, int &resultY,
                                  const std::vector<bool>& analyzed, int xStart, int yStart)
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
            if(yCenter + radius < state->height && !get2D(analyzed, state->width, x, yCenter + radius)
                    && state->getTile(x, yCenter + radius).uncovered)
            {
                resultX = x;
                resultY = yCenter + radius;
                return true;
            }
            std::cout << "Looking at " << x << " " << yCenter - radius << std::endl;
            if(yCenter - radius >= 0 && !get2D(analyzed, state->width, x, yCenter - radius)
                    && state->getTile(x, yCenter - radius).uncovered)
            {
                resultX = x;
                resultY = yCenter - radius;
                return true;
            }
        }
        for(int y = std::max(0, yCenter - radius + 1); y <= std::min(state->height - 1, yCenter + radius - 1); y++)
        {
            std::cout << "Looking at " << xCenter - radius << " " << y << std::endl;
            if(xCenter - radius >= 0 && !get2D(analyzed, state->width, xCenter - radius, y)
                    && state->getTile(xCenter - radius, y).uncovered)
            {
                resultX = xCenter - radius;
                resultY = y;
                return true;
            }
            std::cout << "Looking at " << xCenter + radius << " " << y << std::endl;
            if(xCenter + radius < state->width && !get2D(analyzed, state->width, xCenter + radius, y)
                    && state->getTile(xCenter + radius, y).uncovered)
            {
                resultX = xCenter + radius;
                resultY = y;
                return true;
            }
        }
    }
    std::cout << "Empty tile not found..." << std::endl;
    return false;
}

bool QutesweeperAI::basicStepsRecursive(GameWindow* window, int x, int y, bool singleStep, std::vector<bool>& analyzed)
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

void QutesweeperAI::tankAlgorithm
    (GameWindow *window, bool singleStep, std::vector<bool>& analyzed)
{
    std::vector<bool> isBoundary;
    isBoundary.reserve(state->width * state->height);
    for(size_t i = 0; i < state->width * state->height; i++)
        isBoundary.push_back(false);

    int boundaryTilesCount = 0;


    for(int x = 0; x < state->width; x++)
    {
        for(int y = 0; y < state->height; y++)
        {
            //std::cout << "Looking for boundary at " << x << " " << y << std::endl;
            //check if this is a boundary
            if(state->getTile(x, y).uncovered || state->getTile(x, y).flagged)
                continue;

            for(int xNeigh = std::max(0, x - 1); xNeigh <= std::min(state->width - 1, x + 1); xNeigh++)
            {
                for(int yNeigh = std::max(0, y - 1); yNeigh <= std::min(state->height - 1, y + 1); yNeigh++)
                {
                    MineTile& neighbourTile = state->getTile(xNeigh, yNeigh);
                    if(neighbourTile.uncovered)
                    {
                        std::cout << x << " " << y << " is a boundary" << std::endl;
                        set2D(isBoundary, state->width, x, y, true);
                        boundaryTilesCount++;

                        goto exitLoop;
                    }
                }
            }
            exitLoop:;
        }
    }

    std::cout << "Found all boundaries" << std::endl;

    const int segregationThreshold = 20; //somewhat arbitrary number

    std::vector<std::vector<std::pair<int, int>>> boundarySegments;
//    if(boundaryTilesCount > segregationThreshold)
//    {
//        //We have to check every single combination of mines among the tiles that are boundaries.
//        //But we might have a lot of boundaries, e.g. 50 tiles, which means we'll analyze 2^50 combinations.
//        //To optimize, we'll try to split these boundaries into independent regions which don't affect each other.
//        //So in our example, we'll might end up with 2^20 + 2^25 + 2^5 combinations
//    }
//    else
    {
        boundarySegments.emplace_back();
        for(int x = 0; x < state->width; x++)
        {
            for(int y = 0; y < state->height; y++)
            {
                if(get2D(isBoundary, state->width, x, y))
                    boundarySegments[0].push_back(std::make_pair(x, y));
            }
        }
        std::cout << "one segment, size: " << boundarySegments[0].size() << std::endl;
    }

    //Get tile with the least likelihood of having a mine

    double leastLikelihood = 1000.f;
    int leastLikelihoodX = -1;
    int leastLikelihoodY = -1;

    for(const auto& segment : boundarySegments)
    {
        int combinationCount = 0;
        std::vector<int> mineCount;
        mineCount.reserve(segment.size());
        for(size_t i = 0; i < segment.size(); i++)
            mineCount.push_back(0);

        countMineCombinations(segment, mineCount, combinationCount);
        for(size_t i = 0; i < segment.size(); i++)
        {
            if(mineCount[i] == 0)
            {
                window->onLeftClick(segment[i].first, segment[i].second);
                if(singleStep)
                    return;
            }
            else if((double)mineCount[i] / combinationCount < leastLikelihood)
            {
                leastLikelihood = (double)mineCount[i] / combinationCount;
                std::cout << "Likelihood at " << segment[i].first << " " << segment[i].second << ": "
                          << leastLikelihood << std::endl;
                leastLikelihoodX = segment[i].first;
                leastLikelihoodY = segment[i].second;
            }
        }
    }

    window->onLeftClick(leastLikelihoodX, leastLikelihoodY);
}

std::vector<std::vector<std::pair<int, int>>> QutesweeperAI::splitBoundarySegments(bool* isBoundary)
{
    for(int x = 0; x < state->width; x++)
    {
        for(int y = 0; y < state->height; y++)
        {
            //TO-DO
        }
    }
}

void QutesweeperAI::countMineCombinations(const std::vector<std::pair<int, int> > &segment,
                                          std::vector<int> &mineCount, int &combinationCount)
{
    std::vector<bool> isMine;
    isMine.reserve(state->width * state->height);
    for(size_t i = 0; i < state->width * state->height; i++)
        isMine.push_back(false);
    std::vector<bool> isMineCopy(isMine);
    countMineCombinationsRecursive(segment, isMineCopy, mineCount, combinationCount, 0);

    isMineCopy.clear();
    isMineCopy = std::vector<bool>(isMine);
    set2D(isMineCopy, state->width, segment[0].first, segment[0].second, true);
    countMineCombinationsRecursive(segment, isMineCopy, mineCount, combinationCount, 0);
}

void QutesweeperAI::countMineCombinationsRecursive(const std::vector<std::pair<int, int>>& segment,
                                                   const std::vector<bool>& isMine, std::vector<int>& mineCount,
                                                   int& combinationCount, int currentTile)
{
    //std::cout << "Current tile: " << currentTile << std::endl;
    //std::cout << segment.size() << std::endl;
    bool isValid = true;
    for(const auto& point : segment)
    {
        int x = point.first;
        int y = point.second;

        for(int xNeigh = std::max(0, x - 1); xNeigh <= std::min(state->width - 1, x + 1); xNeigh++)
        {
            for(int yNeigh = std::max(0, y - 1); yNeigh <= std::min(state->height - 1, y + 1); yNeigh++)
            {
                //check if neighbour is in valid position
                MineTile currentTile = state->getTile(xNeigh, yNeigh);

                if(currentTile.uncovered)
                {
                   // std::cout << "Mine neighbours: " << currentTile.mineNeighbours << std::endl;
                    int mineCount = 0;
                    for(int xNeigh2 = std::max(0, xNeigh - 1); xNeigh2 <= std::min(state->width - 1, xNeigh + 1); xNeigh2++)
                    {
                        for(int yNeigh2 = std::max(0, yNeigh - 1); yNeigh2 <= std::min(state->height - 1, yNeigh + 1); yNeigh2++)
                        {
                            if(state->getTile(xNeigh2, yNeigh2).flagged ||
                                    get2D(isMine, state->width, xNeigh2, yNeigh2))
                            {
                                mineCount++;
                            }
                            if(mineCount > currentTile.mineNeighbours)
                            {
                                //std::cout << "Mine count: >" << mineCount << std::endl;
                                isValid = false;
                                goto exitLoop;
                            }
                        }
                    }
                    if(mineCount != currentTile.mineNeighbours)
                    {
                        //std::cout << "Mine count: " << mineCount << std::endl;
                        isValid = false;
                        goto exitLoop;
                    }
                }
            }
        }
    }
    exitLoop:
    //std::cout << "is valid? " << std::boolalpha << isValid << std::endl;
    if(isValid)
    {
        combinationCount++;
        for(size_t i = 0; i < segment.size(); i++)
        {
            int x = segment[i].first;
            int y = segment[i].second;
            if(get2D(isMine, state->width, x, y))
                mineCount[i]++;
        }
    }
    if(currentTile + 1 < segment.size())
    {
        std::vector<bool> isMineCopy(isMine);
        countMineCombinationsRecursive(segment, isMineCopy, mineCount, combinationCount, currentTile + 1);

        isMineCopy.clear();
        isMineCopy = std::vector<bool>(isMine);
        set2D(isMineCopy, state->width, segment[currentTile + 1].first, segment[currentTile + 1].second, true);
        countMineCombinationsRecursive(segment, isMineCopy, mineCount, combinationCount, currentTile + 1);
    }
}
