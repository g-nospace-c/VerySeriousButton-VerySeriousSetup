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
#include <QPushButton>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QDesktopWidget>
#include <boost/foreach.hpp>
#include <boost/format/format_fwd.hpp>
#include "VsbDeviceSelector.h"

DeviceIconWidget::DeviceIconWidget(QWidget* parent)
    : QPushButton(parent),
      nodevIcon(":/images/dev_none.svg"),
      vsbIcon(":/images/dev_vsb.svg")
{
    setFlat(true);
    QDesktopWidget desktop;
    setIconSize(QSizeF(
                    0.4 * desktop.logicalDpiX(),
                    0.4 * desktop.logicalDpiY()
                ).toSize());
    setIconToNodev();
}

void DeviceIconWidget::setIconToVsb()
{
    setIcon(vsbIcon);
}

void DeviceIconWidget::setIconToNodev()
{
    setIcon(nodevIcon);
}

DeviceSelectorInfoWidget::DeviceSelectorInfoWidget(QWidget* parent)
    : QLabel(parent)
{
    clearInfo();
}

void DeviceSelectorInfoWidget::setInfo(const std::string& serialNumber)
{
    setText(QString("<b>Very Serious Button</b><br>Serial number: ") + QString(serialNumber.c_str()));
}

void DeviceSelectorInfoWidget::clearInfo()
{
    setText("(no device selected)");
}

SelectedDeviceBox::SelectedDeviceBox(QWidget* parent)
    : QGroupBox("Select device", parent),
      chooseIcon(":/images/choose.svg")
{
    iconWidget = new DeviceIconWidget(this);
    infoWidget = new DeviceSelectorInfoWidget(this);
    selectButton = new QPushButton("Choose...", this);
    selectButton->setIcon(chooseIcon);
    QHBoxLayout* layout_ = new QHBoxLayout(this);
    layout_->addWidget(iconWidget, 0, Qt::AlignLeft | Qt::AlignVCenter);
    layout_->addWidget(infoWidget, 1);
    layout_->addWidget(selectButton, 0, Qt::AlignRight | Qt::AlignVCenter);
    QObject::connect(selectButton, SIGNAL(clicked()), this, SLOT(handleSelectClicked()));
}

void SelectedDeviceBox::setInfo(const std::string& serialNumber)
{
    infoWidget->setInfo(serialNumber);
    iconWidget->setIconToVsb();
}

void SelectedDeviceBox::clearInfo()
{
    infoWidget->clearInfo();
    iconWidget->setIconToNodev();
}

void SelectedDeviceBox::handleSelectClicked()
{
    emit selectButtonClicked();
}

//#include "VsbDeviceSelector.moc"
