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
#include <QMainWindow>
#include <boost/optional.hpp>
#include "vsbprog.h"
#include "VsbDeviceSelector.h"
#include "VsbModeSelector.h"
#include "SinglekeyOptionsBox.h"
#include "KeyseqOptionsBox.h"
#include "BigGoButton.h"

class VssMainWindow
    : public QMainWindow
{
    Q_OBJECT
private:
    static const std::string swVersion;
    QAction *refreshAction;
    QAction *devinfoAction;
    bool controlsLocked_;
    int whichControlsEnabled_;
    const QIcon appIcon;
    SelectedDeviceBox *deviceSelector;
    VsbModeSelector *modeSelector;
    SinglekeyOptionsBox *singlekeyOptionsBox;
    KeyseqOptionsBox *keyseqOptionsBox;
    BigGoButton *bigGoButton;
    void setAllControlsLocked(bool y);
    void setAllControlsLocked();
    void setWhichControlsEnabled();
    void updateConfigFromDevice(const HidDeviceInfo& devInfo);
    boost::optional<HidDeviceInfo> selectedDevice;
    void setSelectedDevice(const HidDeviceInfo& devInfo);
    void unsetSelectedDevice();
    void showError(std::string summary, std::string msgBoxText);
private slots:
    void setWhichControlsEnabled(int v);
    void applyNewConfig();
    void handleSelectButtonClicked();
    void updateConfigFromDevice();
    void autoSelectDevice();
    void showAboutDialog();
    void showDevinfoDialog();
public:
    VssMainWindow();
    virtual ~VssMainWindow();
public slots:

};

