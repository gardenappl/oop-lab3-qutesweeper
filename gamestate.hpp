#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

/*!
 * \brief The GameState class is responsible for storing data about the currently running game.
 */
class GameState
{
public:
    /*!
     * \brief the width of the current grid
     */
    int width;
    /*!
     * \brief the height of the current grid
     */
    int height;

    GameState() = default;
    GameState(int width, int height);
};

#endif // GAMESTATE_HPP
