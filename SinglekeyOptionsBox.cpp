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
#include <QCheckBox>
#include <QEvent>
#include <QGridLayout>
#include <QLabel>
#include <utility>
#include <boost/foreach.hpp>
#include "SinglekeyOptionsBox.h"

ModifierKeyGrid::ModifierKeyGrid(QWidget* parent)
    : QWidget(parent)
{
    lShift = new QCheckBox("Shift", this);
    lAlt = new QCheckBox("Alt", this);
    lGui = new QCheckBox("Gui", this);
    lCtrl = new QCheckBox("Ctrl", this);
    rShift = new QCheckBox("Shift", this);
    rAlt = new QCheckBox("Alt", this);
    rGui = new QCheckBox("Gui", this);
    rCtrl = new QCheckBox("Ctrl", this);
    QGridLayout* layout_ = new QGridLayout(this);
    QLabel *leftLabel = new QLabel("(left)",this);
    QLabel *rightLabel = new QLabel("(right)",this);
    QFont font = leftLabel->font();
    font.setPointSizeF(font.pointSizeF()*.75);
    leftLabel->setFont(font);
    rightLabel->setFont(font);//FIXME:
    layout_->addWidget(leftLabel, 0, 0);
    layout_->addWidget(lShift, 0, 1);
    layout_->addWidget(lAlt, 0, 2);
    layout_->addWidget(lGui, 0, 3);
    layout_->addWidget(lCtrl, 0, 4);
    layout_->addWidget(rightLabel, 1, 0);
    layout_->addWidget(rShift, 1, 1);
    layout_->addWidget(rAlt, 1, 2);
    layout_->addWidget(rGui, 1, 3);
    layout_->addWidget(rCtrl, 1, 4);
    setLayout(layout_);
}

ModifierKeyGrid::~ModifierKeyGrid()
{}

void ModifierKeyGrid::setMods(uint8_t mods)
{
    lShift->setChecked(mods & Keystroke::modFromString("LSHIFT"));
    lCtrl->setChecked(mods & Keystroke::modFromString("LCTRL"));
    lAlt->setChecked(mods & Keystroke::modFromString("LALT"));
    lGui->setChecked(mods & Keystroke::modFromString("LGUI"));
    rShift->setChecked(mods & Keystroke::modFromString("RSHIFT"));
    rCtrl->setChecked(mods & Keystroke::modFromString("RCTRL"));
    rAlt->setChecked(mods & Keystroke::modFromString("RALT"));
    rGui->setChecked(mods & Keystroke::modFromString("RGUI"));
} //FIXME: Not exactly the most efficient way... instead, maybe do those lookups in the constructor and store them as consts?

uint8_t ModifierKeyGrid::getMods()
{
    return
        (lShift->isChecked() ? Keystroke::modFromString("LSHIFT") : 0)
        | (lCtrl->isChecked() ? Keystroke::modFromString("LCTRL") : 0)
        | (lAlt->isChecked() ? Keystroke::modFromString("LALT") : 0)
        | (lGui->isChecked() ? Keystroke::modFromString("LGUI") : 0)
        | (rShift->isChecked() ? Keystroke::modFromString("RSHIFT") : 0)
        | (rCtrl->isChecked() ? Keystroke::modFromString("RCTRL") : 0)
        | (rAlt->isChecked() ? Keystroke::modFromString("RALT") : 0)
        | (rGui->isChecked() ? Keystroke::modFromString("RGUI") : 0);
}

SinglekeyOptionsBox::SinglekeyOptionsBox(QWidget* parent)
    : QGroupBox("Options for single key mode", parent)
{
    keynameSelector = new QComboBox(this);
    modGrid = new ModifierKeyGrid(this);
    typedef std::pair<const std::string ,int> entry_t; //The comma screws up the BOOST_FOREACH macro
    BOOST_FOREACH(entry_t entry, Keystroke::allKeycodes())
    {
        keynameSelector->addItem(entry.first.c_str());
    }

    QGridLayout* layout_ = new QGridLayout(this);
    layout_->addWidget(new QLabel("Key name", this), 0, 0, Qt::AlignLeft);
    layout_->addWidget(keynameSelector, 0, 1, Qt::AlignLeft);
    layout_->addWidget(new QLabel("Modifiers", this), 1, 0, Qt::AlignLeft);
    layout_->addWidget(modGrid, 1, 1, Qt::AlignLeft);
    layout_->setColumnStretch(0,0);
    layout_->setColumnStretch(1,1);
    setLayout(layout_);
}

SinglekeyOptionsBox::~SinglekeyOptionsBox()
{}

void SinglekeyOptionsBox::updateFromVsbInfoAndConfig(
    const VsbInfoAndConfig& infoAndConfig)
{
    setKeystroke(infoAndConfig.config.singlekeyKeys);
}

void SinglekeyOptionsBox::setKeystroke(const Keystroke& keystroke) //FIXME: Handle multi-key keystrokes sensibly somehow
{
    if(keystroke.numKeys())
    {
        std::string keyName = keystroke.keyName(0);
        keynameSelector->setCurrentIndex(
            keynameSelector->findText(keyName.c_str()));
        modGrid->setMods(keystroke.mods());
    }
}

Keystroke SinglekeyOptionsBox::getKeystroke()
{
    return Keystroke::fromString(
               keynameSelector->currentText().toStdString(),
               modGrid->getMods());
}

