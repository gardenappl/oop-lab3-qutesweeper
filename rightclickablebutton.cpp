#include "rightclickablebutton.hpp"

#include <QMouseEvent>

RightClickableButton::RightClickableButton(QWidget* parent)
    : QPushButton(parent)
{

}

void RightClickableButton::mousePressEvent(QMouseEvent* e)
{
    if(e->button() == Qt::RightButton)
        emit rightClicked();
    this->QPushButton::mousePressEvent(e);
}
