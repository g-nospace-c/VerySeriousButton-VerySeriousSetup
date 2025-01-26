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


#include <QApplication>
#include <iostream>
#include "VerySeriousSetup.h"
#include "hidapi.h"
#include "vsbprog.h"
#include "keyseq.h"

class MyApplication
    : public QApplication
{
public:
    MyApplication(int& argc, char** argv);
    virtual bool notify(QObject* receiver, QEvent* event);
};

MyApplication::MyApplication(int& argc, char** argv)
    : QApplication(argc, argv)
{}

bool MyApplication::notify(QObject* receiver, QEvent* event)
{
    bool done = true;
    try
    {
        done = QApplication::notify(receiver, event);
    }
    catch(VsbError err)
    {
        std::cerr << "Unhandled VsbError: " << err.what() << std::endl;
    }
    catch(InvalidKeycode err)
    {
        std::cerr << "Unhandled InvalidKeycode: " << err.what() << std::endl;
    }
    catch(InvalidModName err)
    {
        std::cerr << "Unhandled InvalidModName: " << err.what() << std::endl;
    }
    catch(InvalidKeyName err)
    {
        std::cerr << "Unhandled InvalidKeyname: " << err.what() << std::endl;
    }
    catch(const std::exception& ex)
    {
        std::cerr << "Unhandled exception: " << ex.what() << std::endl;
    }
    return done;
}

int main(int argc, char** argv)
{
    hid_init();
    MyApplication app(argc, argv);
    Q_INIT_RESOURCE(images);
    VssMainWindow foo;
    foo.show();
    return app.exec();
}
