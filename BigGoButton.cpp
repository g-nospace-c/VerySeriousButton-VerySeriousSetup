// © 2014 GC <gc@grenlabs.com>
// 
// This file is part of VerySeriousSetup.
// 
// VerySeriousSetup is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// VerySeriousSetup is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with VerySeriousSetup.  If not, see <http://www.gnu.org/licenses/>.


#include <QtGui>
#include <QDesktopWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include "BigGoButton.h"

BigGoButton::BigGoButton(QWidget* parent)
    : QWidget(parent),
      goIcon(":/images/go.svg")
{
    QHBoxLayout* layout_ = new QHBoxLayout(this);
    layout_->addStretch();
    QPushButton* button = new QPushButton("Apply new configuration", this);
    QDesktopWidget desktop;
    button->setMinimumWidth((int)(2.4 * desktop.logicalDpiX()));
    button->setMinimumHeight((int)(0.5 * desktop.logicalDpiY()));
    button->setIconSize(
        QSize(0.3*desktop.logicalDpiX(), 0.3*desktop.logicalDpiY()));
    button->setIcon(goIcon);
    layout_->addWidget(button);
    layout_->addStretch();
    setLayout(layout_);
    QObject::connect(
        button, SIGNAL(clicked()), this, SLOT(handleGobuttonClicked()));
}

BigGoButton::~BigGoButton()
{}

void BigGoButton::handleGobuttonClicked()
{
    emit gobuttonClicked();
}

//#include "BigGoButton.moc"
