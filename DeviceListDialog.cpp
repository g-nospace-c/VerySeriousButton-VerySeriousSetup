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


#include <QtCore>
#include <QtGui>
#include <QComboBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVariant>
#include <boost/foreach.hpp>
#include "DeviceListDialog.h"

#include <iostream> //DEBUG

DeviceListDialog::DeviceListDialog()
{
    //selectButton = new QPushButton("Select", this);
    buttonBox = new QDialogButtonBox(this);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel | QDialogButtonBox::Ok);
    buttonBox->button(QDialogButtonBox::Ok)->setText("Select");
    listBox = new QComboBox(this);
    //TODO: Make list box multi-line, or use a table thing instead
    setLayout(new QVBoxLayout(this));
    layout()->addWidget(new QLabel("Available devices:",this));
    layout()->addWidget(listBox);
    layout()->addWidget(buttonBox);
    setWindowTitle("Select device");
    QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

void DeviceListDialog::setDeviceList(const std::vector<HidDeviceInfo>& newDevInfos)
{
    devInfos = newDevInfos;
    listBox->clear();
    for(int i = 0; i < devInfos.size(); ++i)
    {
        listBox->addItem((devInfos[i].productString + " #" + devInfos[i].serialNumber).c_str(), QVariant(i));
        listBox->setEnabled(true);
        buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    }
    if(!listBox->count())
    {
        listBox->addItem("(no devices connected?)", QVariant(-1));
        listBox->setEnabled(false);
        buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    }
}

boost::optional<HidDeviceInfo> DeviceListDialog::selectedDevice()
{
    if((listBox->currentIndex() < 0) || (listBox->itemData(listBox->currentIndex()).toInt() < 0))
        return boost::none;
    return devInfos[listBox->currentIndex()];
}


//#include "DeviceListDialog.moc"
