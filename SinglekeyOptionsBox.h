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

#pragma once

#include <QtGui>
#include <QCheckBox>
#include <QComboBox>
#include <QGroupBox>
#include "vsbprog.h"

class ModifierKeyGrid
    : public QWidget
{
private:
    QCheckBox *lShift;
    QCheckBox *lAlt;
    QCheckBox *lGui;
    QCheckBox *lCtrl;
    QCheckBox *rShift;
    QCheckBox *rAlt;
    QCheckBox *rGui;
    QCheckBox *rCtrl;
public:
    explicit ModifierKeyGrid(QWidget* parent = 0);
    virtual ~ModifierKeyGrid();
    void setMods(uint8_t mods);
    uint8_t getMods();
};

class SinglekeyOptionsBox
    : public QGroupBox
{
    Q_OBJECT
private:
  QComboBox* keynameSelector;
  ModifierKeyGrid* modGrid;
public:
    SinglekeyOptionsBox(QWidget* parent = 0);
    void updateFromVsbInfoAndConfig(const VsbInfoAndConfig& infoAndConfig);
    void setKeystroke(const Keystroke& keystroke);
    Keystroke getKeystroke();
    virtual ~SinglekeyOptionsBox();
};
