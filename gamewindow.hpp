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
     * \brief Create buttons for each Qutesweeper cell and arrange them in the UI.
     */
    void populateButtons();

    /*!
     * \brief Get a pointer to a button at the given coordinates
     * \param x x coordinate
     * \param y y coordinate
     * \return a pointer to the associated QPushButton
     */
    QPushButton*& getButton(int x, int y);

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
