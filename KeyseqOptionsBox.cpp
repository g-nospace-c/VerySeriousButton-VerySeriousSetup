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
#include <QVBoxLayout>
#include <QLabel>
#include "KeyseqOptionsBox.h"

InvalidKeyseqString::InvalidKeyseqString(const std::string& arg)
    : std::runtime_error(arg)
{}

KeyseqOptionsBox::KeyseqOptionsBox(QWidget* parent)
    : QGroupBox("Options for key sequence mode", parent)
{
    textEdit = new QTextEdit(this);
    textEdit->setAcceptRichText(false);
    textEdit->setTabChangesFocus(false);
    QGridLayout *layout_ = new QGridLayout(this);
    layout_->addWidget(new QLabel("Key sequence",this), 0, 0);
    layout_->addWidget(textEdit, 0, 1);
    setLayout(layout_);
}

KeyseqOptionsBox::~KeyseqOptionsBox()
{}

void KeyseqOptionsBox::updateFromVsbInfoAndConfig(const VsbInfoAndConfig& infoAndConfig)
{
    setKeystrokeSequence(infoAndConfig.config.keyseqKeyseq);
}

void KeyseqOptionsBox::setKeystrokeSequence(const KeystrokeSequence& seq)
{
    textEdit->setPlainText(seq.toString().c_str());
}

KeystrokeSequence KeyseqOptionsBox::getKeystrokeSequence()
{
    try
    {
        return KeystrokeSequence::fromString(textEdit->toPlainText().toStdString());
    }
    catch(InvalidKeyName err)
    {
        throw InvalidKeyseqString(std::string("'") + err.what() + "' is not a valid key name");
    }
    catch(InvalidModName err)
    {
        throw InvalidKeyseqString(std::string("'") + err.what() + "' is not a valid modifier name");
    }
}

