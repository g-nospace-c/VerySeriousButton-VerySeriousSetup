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
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <vector>
#include "vsbprog.h"

class DeviceIconWidget
    : public QPushButton
{
private:
    const QIcon nodevIcon;
    const QIcon vsbIcon;
public:
    explicit DeviceIconWidget(QWidget* parent = 0);
    void setIconToVsb();
    void setIconToNodev();
};

class DeviceSelectorInfoWidget
    : public QLabel
{
public:
    explicit DeviceSelectorInfoWidget(QWidget* parent = 0);
    void setInfo(const std::string& serialNumber);
    void clearInfo();
};

class SelectedDeviceBox
    : public QGroupBox
{
    Q_OBJECT
private:
    const QIcon chooseIcon;
    DeviceIconWidget *iconWidget;
    DeviceSelectorInfoWidget *infoWidget;
    QPushButton *selectButton;
private slots:
    void handleSelectClicked();
signals:
    void selectButtonClicked();
public:
    SelectedDeviceBox(QWidget* parent = 0);
    void setInfo(const std::string& serialNumber);
    void clearInfo();
};
