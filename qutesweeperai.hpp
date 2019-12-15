#ifndef QUTESWEEPERAI_HPP
#define QUTESWEEPERAI_HPP

#include "gamestate.hpp"
#include "gamewindow.fwd.hpp"

class QutesweeperAI
{
public:
    QutesweeperAI(GameState* state);

    void doMove(GameWindow* window);

private:
    GameState* state;

    /*!
     * \brief Find an empty tile, starting from center or from the start coordinates if they're specified.
     * \param x x start coordinate
     * \param y y start coordinate
     * \throw std::logic_error if there are no empty tiles
     */
    void findEmptyTile(int& resultX, int& resultY, int xStart = -1, int yStart = -1);

    /*!
     * Performs initial checks, where we're 100% sure about whether or not we have mines
     * based on available information. This is recursive and will be called for all tiles in the
     * surrounding uncovered area.
     * \param window a pointer to the window
     * \param x starting x-coordinate (must be uncovered)
     * \param y starting y-coordinate (must be uncovered)
     * \param singleStep should we return after a single step?
     * \param analyzed an array where "true" means that the tile at those coordinates has already been analyzed.
     * \return returns true if a single step has been completed, false if no "basic steps" are available.
     */
    bool basicStepsRecursive(GameWindow* window, int x, int y, bool singleStep, bool* analyzed);

    /*!
     * Tank algorithm:
     * 1. analyze all possible combinations of mines in "boundary" locations
     * 2. see how frequently does a particular tile contain mine
     * 3. pick tile with least frequency.
     * \param window a pointer to the window
     * \param x starting x-coordinate (must be uncovered)
     * \param y starting y-coordinate (must be uncovered)
     * \param singleStep should we return after a single step?
     * \param analyzed an array where "true" means that the tile at those coordinates has already been analyzed.
     */
    void statisticalPredictionsRecursive(GameWindow* window, int x, int y, bool singleStep, bool* analyzed);
};

#endif // QUTESWEEPERAI_HPP
