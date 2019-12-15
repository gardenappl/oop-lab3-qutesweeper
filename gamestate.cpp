#include "gamestate.hpp"

#include <stdexcept>

GameState::GameState(int width, int height, int mineCount)
    : width(width), height(height), mineCount(mineCount)
{
    tiles = new MineTile[width * height];
}

MineTile& GameState::getTile(int x, int y)
{
    if(x < 0 || y < 0 || x >= width || y >= height)
        throw std::out_of_range("Invalid coordinates.");
    return tiles[y * width + x];
}

GameState::~GameState()
{
    delete[] tiles;
}

MineTile::MineTile(bool isMine)
    : mineNeighbours(isMine ? -1 : 0)
{

}

bool MineTile::isMine() const
{
    return mineNeighbours == -1;
}
