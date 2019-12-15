#ifndef RIGHTCLICKABLEBUTTON_HPP
#define RIGHTCLICKABLEBUTTON_HPP

#include <QPushButton>



class RightClickableButton : public QPushButton
{
    Q_OBJECT

public:
    explicit RightClickableButton(QWidget* parent = nullptr);

private slots:
    void mousePressEvent(QMouseEvent* e);

signals:
    void rightClicked();
};

#endif // RIGHTCLICKABLEBUTTON_HPP
