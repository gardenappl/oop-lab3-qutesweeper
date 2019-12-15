#ifndef GAMEWINDOW_HPP
#define GAMEWINDOW_HPP

#include "gamewindow.fwd.hpp"

#include "gamestate.hpp"
#include "qutesweeperai.hpp"
#include "rightclickablebutton.hpp"

#include <QBoxLayout>
#include <QFrame>
#include <QMainWindow>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class GameWindow; }
QT_END_NAMESPACE

class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
    GameWindow(QWidget *parent = nullptr);
    ~GameWindow();


    /*!
     * \brief Prompt the player to enter a field size and difficulty and (re-)start the game.
     * \param restart set to true if you wish to keep the current field size and difficulty
     */
    void newGameStart(bool restart = false);

    /*!
     * \brief Uncover a tile wihout a mine, if the tile has 0 mine neighbours, will keep uncovering tiles recursively.
     * \param x the x-coordinate of a tile without a mine
     * \param y the y-coordinate of a tile without a mine
     */
    void popTileSafe(MineTile& tile, int x, int y);

public slots:
    /*!
     * \brief Should get called after left-clicking on a tile button.
     * \param x button's x coordinate
     * \param y button's y coordinate
     */
    void onLeftClick(int x, int y);

    /*!
     * \brief Should get called after right-clicking on a tile button.
     * \param x button's x coordinate
     * \param y button's y coordinate
     */
    void onRightClick(int x, int y);

private:
    /*!
     * \brief The current state of the game
     */
    GameState* currentState = nullptr;

    Ui::GameWindow *ui;
    QWidget* mainWidget;
    QBoxLayout* playAreaLayout;
    QFrame* playArea;
    RightClickableButton** buttonsArray; //a static array because I don't want to invalidate pointers via std::vector

    /*!
     * An instance of QutesweeperAI. Because this takes a lot of memory, it's not initialized until
     * the player asks for it.
     */
    QutesweeperAI* ai = nullptr;

    /*!
     * \brief Gets called when the player requests an AI move.
     */
    void requestAIMove();

    /*!
     * \brief This must be called before any gameplay can happen.
     * Initializes the GameState, creates buttons for each Qutesweeper cell and arranges them in the UI, etc.
     */
    void init();

    /*!
     * \brief Clean up memory before starting a new game (or when quitting the program).
     */
    void uninit();


    /*!
     * \brief Populates the grid with mines, calcaultes neighbour counts.
     * \param startX the x coordinate of the starting tile (guaranteed to not have a mine).
     * \param startY the y coordinate of the starting tile (guaranteed to not have a mine).
     */
    void populateGrid(int startX, int startY);


    /*!
     * \brief Get a pointer to a button at the given coordinates
     * \param x x coordinate
     * \param y y coordinate
     * \return a pointer to the associated RightClickableButton
     */
    RightClickableButton*& getButton(int x, int y);
};
#endif // GAMEWINDOW_HPP
