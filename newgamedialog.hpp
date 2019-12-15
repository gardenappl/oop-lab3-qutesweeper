#ifndef NEWGAMEWINDOW_HPP
#define NEWGAMEWINDOW_HPP

#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLabel>
#include <QSpinBox>

/*!
 * The NewGameWindow class shows a pop-up window where the user can select their difficulty.
 */
class NewGameDialog : public QDialog
{
    Q_OBJECT

public:
    NewGameDialog(int width = 16, int height = 16, int mineCount = 40, QWidget *parent = nullptr);
    ~NewGameDialog();

    /*!
     * \brief Get the user's selected values.
     * \param x width of the field
     * \param y height of the field
     * \param mineCount amount of mines
     */
    void getOptions(int& x, int& y, int& mineCount);

private:
    QVBoxLayout* layout;
    QWidget* formWidget;
    QFormLayout* formLayout;
    QLabel* widthLabel;
    QLabel* heightLabel;
    QLabel* minesCountLabel;
    QSpinBox* widthBox;
    QSpinBox* heightBox;
    QSpinBox* minesCountBox;

    QDialogButtonBox *buttonBox;
};

#endif // NEWGAMEWINDOW_HPP
