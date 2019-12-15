#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

/*!
 * \brief The MineTile struct contains information about a single Qutesweeper tile.
 */
struct MineTile
{
    /*!
     * \brief Whether or not the mine was flagged by the player.
     */
    bool flagged = false;
    /*!
     * \brief Whether or not the mine has been uncovered by the player.
     */
    bool uncovered = false;
    /*!
     * \brief The amount of neigbouring tiles which also contain mines, or -1 if the tile itself is a mine.
     */
    int mineNeighbours;

    /*!
     * Constructs a tile with or without a mine. This assumes that the game has only just begun,
     * meaning that the tile will be uncovered and unflagged.
     * \param isMine whether or not this is a mine
     */
    MineTile(bool isMine = false);

    /*!
     * \brief Checks if this is a mine.
     * \return true if it's a mine, false otherwise
     */
    bool isMine() const;
};


/*!
 * \brief The GameState class is responsible for storing data about the currently running game.
 */
class GameState
{
public:
    /*!
     * \brief The width of the current grid
     */
    int width;
    /*!
     * \brief The height of the current grid
     */
    int height;

    /*!
     * \brief How many mines there are on the grid in total
     */
    int mineCount;

    /*!
     * \brief How many mines have been uncovered
     */
    int uncoveredCount = 0;

    /*!
     * \brief Whether or not the game has been started (that is, the grid is populated, the timer is running, etc.)
     */
    bool gameStarted = false;

    GameState() = default;
    GameState(int width, int height, int mineCount);
    ~GameState();

    /*!
     * \brief Get the tile at the given x, y coordinates
     * \param x x coordinate
     * \param y y coordinate
     * \return reference to the contained tile
     * \throw std::out_of_range if the x, y coordinates are out of bounds
     */
    MineTile& getTile(int x, int y);

private:
    MineTile* tiles;
};
#endif // GAMESTATE_HPP
