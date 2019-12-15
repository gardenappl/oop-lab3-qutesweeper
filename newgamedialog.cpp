#include "newgamedialog.hpp"
#include "ui_newgamedialog.h"

#include <iostream>

NewGameDialog::NewGameDialog(int width, int height, int mineCount, QWidget *parent) :
    QDialog(parent)
{
    std::cout << "Dialog" << std::endl;
    layout = new QVBoxLayout(this);

    formWidget = new QWidget(this);
    layout->addWidget(formWidget);
    formLayout = new QFormLayout(formWidget);

    widthLabel = new QLabel(tr("Width: "), formWidget);
    heightLabel = new QLabel(tr("Height: "), formWidget);
    minesCountLabel = new QLabel(tr("Mines Count: "), formWidget);

    widthBox = new QSpinBox(formWidget);
    widthBox->setRange(9, 24);
    widthBox->setValue(width);
    heightBox = new QSpinBox(formWidget);
    heightBox->setRange(9, 24);
    heightBox->setValue(height);
    minesCountBox = new QSpinBox(formWidget);
    minesCountBox->setRange(0, 24 * 24 - 1);
    minesCountBox->setValue(mineCount);

    formLayout->addRow(widthLabel, widthBox);
    formLayout->addRow(heightLabel, heightBox);
    formLayout->addRow(minesCountLabel, minesCountBox);

    connect(widthBox, QOverload<int>::of(&QSpinBox::valueChanged), this,
            [=](int newValue) { minesCountBox->setMaximum(heightBox->value() * newValue - 1); });
    connect(heightBox, QOverload<int>::of(&QSpinBox::valueChanged), this,
            [=](int newValue) { minesCountBox->setMaximum(widthBox->value() * newValue - 1); });

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    layout->addWidget(buttonBox);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    std::cout << "Dialog created" << std::endl;
}

void NewGameDialog::getOptions(int &x, int &y, int &mineCount)
{
    x = widthBox->value();
    y = heightBox->value();
    mineCount = minesCountBox->value();
}

NewGameDialog::~NewGameDialog()
{
    delete layout;
//    delete formWidget;
//    delete widthLabel;
//    delete heightLabel;
//    delete minesCountLabel;
//    delete widthBox;
//    delete heightBox;
//    delete minesCountLabel;
    delete formLayout;
//    delete buttonBox;
}
