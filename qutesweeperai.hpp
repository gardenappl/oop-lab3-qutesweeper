#ifndef QUTESWEEPERAI_HPP
#define QUTESWEEPERAI_HPP

#include "gamestate.hpp"
#include "gamewindow.fwd.hpp"

#include <vector>
#include <utility>

class QutesweeperAI
{
public:
    QutesweeperAI(GameState* state);

    void doMove(GameWindow* window);

private:
    GameState* state;

    /*!
     * Find an empty tile, starting from center or from the start coordinates if they're specified.
     * If the analyzed array is not empty, already analyzed tiles will be ignored.
     * \param x x start coordinate
     * \param y y start coordinate
     * \param analyzed a 2D array of tiles that have been already looked at.
     * \return true if an empty tile has been found that's not present in the anlayzed array
     */
    bool findEmptyTile(int& resultX, int& resultY, const std::vector<bool>& analyzed, int xStart = -1, int yStart = -1);

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
    bool basicStepsRecursive(GameWindow* window, int x, int y, bool singleStep, std::vector<bool>& analyzed);

    /*!
     * Tank algorithm:
     * 1. analyze all possible combinations of mines in "boundary" locations
     * 2. see how frequently does a particular tile contain mine
     * 3. pick tile with least frequency.
     * \param window a pointer to the window
     * \param singleStep should we return after a single step?
     * \param analyzed an array where "true" means that the tile at those coordinates has already been analyzed.
     */
    void tankAlgorithm(GameWindow* window, bool singleStep, std::vector<bool>& analyzed);

    /*!
     * A helper function for the Tank solver. Arranges "boundary" into regions, which are independent of each other.
     * \param boundaryTiles
     */
    std::vector<std::vector<std::pair<int, int>>> splitBoundarySegments(bool* isBoundary);

    /*!
     * \brief Step 2 of the Tank solver: check how many possible combinations contain a mine.
     * \param segment a region of boundary tiles to check
     * \param mineCount how many combinations had a mine at a particular location
     * \param combinationCount how many valid combinations were there in total?
     */
    void countMineCombinations(const std::vector<std::pair<int, int>>& segment, std::vector<int>& mineCount, int& combinationCount);

    /*!
     * \brief Step 2 of the Tank solver: check how many possible combinations contain a mine.
     * This function analyzes a particular combination of mines in a boundary, and then tries the next combination.
     * \param segment a region of boundary tiles to check
     * \isMine a 2D array which indicates whether or not this tile is (hypothetically) a mine in this combination
     * \param mineCount an array which indicates how many combinations had a mine at a particular index of "region" vector
     * \param combinationCount how many valid combinations were there in total?
     * \param currentTile this is a recursive function; which tile in the region array are we modifying now?
     */
    void countMineCombinationsRecursive(const std::vector<std::pair<int, int>>& segment, const std::vector<bool>& isMine,
                                        std::vector<int>& mineCount, int& combinationCount, int currentTile);
};

#endif // QUTESWEEPERAI_HPP
