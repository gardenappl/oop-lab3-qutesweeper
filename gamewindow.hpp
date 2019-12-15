#ifndef GAMEWINDOW_HPP
#define GAMEWINDOW_HPP

#include "gamestate.hpp"

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
     * \brief This must be called before any gameplay can happen.
     * Initializes the GameState, creates buttons for each Qutesweeper cell and arranges them in the UI, etc.
     */
    void init();


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
     * \return a pointer to the associated QPushButton
     */
    QPushButton*& getButton(int x, int y);

private slots:
    /*!
     * \brief Should get called after left-clicking on a tile button.
     * \param x button's x coordinate
     * \param y button's y coordinate
     */
    void onClick(int x, int y);

private:
    /*!
     * \brief The current state of the game
     */
    GameState currentState;

    Ui::GameWindow *ui;
    QWidget* mainWidget;
    QBoxLayout* playAreaLayout;
    QFrame* playArea;
    QPushButton** buttonsArray; //a static array because I don't want to invalidate pointers via std::vector
};
#endif // GAMEWINDOW_HPP
