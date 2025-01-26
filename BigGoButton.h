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
#include <QWidget>

class BigGoButton
    : public QWidget
{
    Q_OBJECT
private:
    const QIcon goIcon;
private slots:
    void handleGobuttonClicked();
public:
    explicit BigGoButton(QWidget* parent = 0);
    virtual ~BigGoButton();
signals:
    void gobuttonClicked();
};
