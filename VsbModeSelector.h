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
#include <QToolButton>
#include <QGroupBox>

class VsbModeButton : public QToolButton
{
    Q_OBJECT
public:
    VsbModeButton(const QIcon& icon, const QString labelText, QWidget* parent = 0);
    ~VsbModeButton();
};

class VsbModeSelector
    : public QGroupBox
{
    Q_OBJECT
private:
    const QIcon keyseqModeIcon;
    const QIcon singlekeyModeIcon;
    const QIcon joystickModeIcon;
    VsbModeButton *keyseqButton;
    VsbModeButton *singlekeyButton;
    VsbModeButton *joystickButton;
    QButtonGroup *buttonGroup;
public:
    static const int joystickModeId = 1;
    static const int singlekeyModeId = 2;
    static const int keyseqModeId = 3;
    VsbModeSelector(QWidget* parent = 0);
    virtual ~VsbModeSelector();
    int selectedMode();
    void setSelectedMode(int x);
private slots:
    void handleButtonPressed(int x);
signals:
    void newSelectedMode(int x);
};
