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

#include "keyseq.h"
#include <map>
#include <stdexcept>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string/trim.hpp>
#include "bytestring.h"

const std::map<const std::string, uint8_t> allMods_ = {
    {"LCTRL", KeyMods::lCtrl},
    {"LSHIFT", KeyMods::lShift},
    {"LALT", KeyMods::lAlt},
    {"LGUI", KeyMods::lGui},
    {"RCTRL", KeyMods::rCtrl},
    {"RSHIFT", KeyMods::rShift},
    {"RALT", KeyMods::rAlt},
    {"RGUI", KeyMods::rGui},
    {"CTRL", KeyMods::lCtrl},
    {"SHIFT", KeyMods::lShift},
    {"ALT", KeyMods::lAlt},
    {"GUI", KeyMods::lGui},

};

const std::map<const std::string, uint8_t> allKeycodes_ = {
    {"A", 0x04},
    {"B", 0x05},
    {"C", 0x06},
    {"D", 0x07},
    {"E", 0x08},
    {"F", 0x09},
    {"G", 0x0A},
    {"H", 0x0B},
    {"I", 0x0C},
    {"J", 0x0D},
    {"K", 0x0E},
    {"L", 0x0F},
    {"M", 0x10},
    {"N", 0x11},
    {"O", 0x12},
    {"P", 0x13},
    {"Q", 0x14},
    {"R", 0x15},
    {"S", 0x16},
    {"T", 0x17},
    {"U", 0x18},
    {"V", 0x19},
    {"W", 0x1A},
    {"X", 0x1B},
    {"Y", 0x1C},
    {"Z", 0x1D},
    {"1", 0x1E},
    {"2", 0x1F},
    {"3", 0x20},
    {"4", 0x21},
    {"5", 0x22},
    {"6", 0x23},
    {"7", 0x24},
    {"8", 0x25},
    {"9", 0x26},
    {"0", 0x27},
    {"ENTER",  0x28},
    {"ESC",    0x29},
    {"BKSP",   0x2A},
    {"TAB",    0x2B},
    {"SPACE",  0x2C},
    {"MINUS",  0x2D},
    {"EQUALS", 0x2E},
    {"LBRACE", 0x2F},
    {"RBRACE", 0x30},
    {"BSLASH", 0x31},
    {"EUR1",   0x32},
    {"SCOLON", 0x33},
    {"QUOTE",  0x34},
    {"TILDE",  0x35},
    {"COMMA",  0x36},
    {"PERIOD", 0x37},
    {"SLASH",  0x38},
    {"CAPSLK", 0x39},
    {"F1",     0x3A},
    {"F2",     0x3B},
    {"F3",     0x3C},
    {"F4",     0x3D},
    {"F5",     0x3E},
    {"F6",     0x3F},
    {"F7",     0x40},
    {"F8",     0x41},
    {"F9",     0x42},
    {"F10",    0x43},
    {"F11",    0x44},
    {"F12",    0x45},
    {"PRTSC",  0x46},
    {"SCROLK", 0x47},
    {"PAUSE",  0x48},
    {"INSERT", 0x49},
    {"HOME",   0x4A},
    {"PGUP",   0x4B},
    {"DELETE", 0x4C},
    {"END",    0x4D},
    {"PGDN",   0x4E},
    {"RIGHT",  0x4F},
    {"LEFT",   0x50},
    {"DOWN",   0x51},
    {"UP",     0x52},
    {"NUMLK",  0x53},
    {"KPSLASH",0x54},
    {"KPAST",  0x55},
    {"KPMINUS",0x56},
    {"KPPLUS", 0x57},
    {"KPENTER",0x58},
    {"KP1",    0x59},
    {"KP2",    0x5A},
    {"KP3",    0x5B},
    {"KP4",    0x5C},
    {"KP5",    0x5D},
    {"KP6",    0x5E},
    {"KP7",    0x5F},
    {"KP8",    0x60},
    {"KP9",    0x61},
    {"KP0",    0x62},
    {"KPPER",  0x63},
    {"EUR2",   0x64},
    {"APP",    0x65},
    {"POWER",  0x66},
    {"KPEQUAL",0x67},
    {"F13",    0x68},
    {"F14",    0x69},
    {"F15",    0x6A},
    {"F16",    0x6B},
    {"F17",    0x6C},
    {"F18",    0x6D},
    {"F19",    0x6E},
    {"F20",    0x6F},
    {"F21",    0x70},
    {"F22",    0x71},
    {"F23",    0x72},
    {"F24",    0x73},
    {"HELP",   0x75},
    {"MENU",   0x76},
    {"UNDO",   0x7A},
    {"CUT",    0x7B},
    {"COPY",   0x7C},
    {"PASTE",  0x7D},
    {"FIND",   0x7E},
    {"MUTE",   0x7F},
    {"VOLUP",  0x80},
    {"VOLDN",  0x81},
    {"SYSRQ",  0x9A},
    {"LCTRL",  0xE0},
    {"LSHIFT", 0xE1},
    {"LALT",   0xE2},
    {"LGUI",   0xE3},
    {"RCTRL",  0xE4},
    {"RSHIFT", 0xE5},
    {"RALT",   0xE6},
    {"RGUI",   0xE7}
};

const char* Keystroke::sepStr = "+";
const char* KeystrokeSequence::sepStr = " ";

bool Keystroke::empty() const
{
    return keyCodes_.empty();
}

std::string Keystroke::toString() const
{
    std::vector<std::string> pieces;
    typedef std::pair<const std::string, uint8_t> entry_t;
    uint8_t mods = 0;
    BOOST_FOREACH(entry_t entry, allMods_)
    {
        if(hasMod(entry.second) and !(entry.second & mods))
        {
            pieces.push_back(entry.first);
            mods |= entry.second;
        }
    }
    BOOST_FOREACH(uint8_t kc, keyCodes_)
    {
        if(kc)
            pieces.push_back(keycodeToString(kc));
    }
    return boost::algorithm::join(pieces, sepStr);
}

Keystroke::Keystroke(const ByteString& keyCodes, uint8_t mods)
    : mods_(mods),
      keyCodes_(keyCodes)
{}

KeystrokeSequence KeystrokeSequence::fromString(const std::string& s)
{
    KeystrokeSequence seq;
    std::vector<std::string> tokens;
    boost::regex whitespace("[[:space:]]+", boost::regex_constants::egrep);
    std::string despaced = boost::regex_replace(s, whitespace, " ");
    boost::trim(despaced);
    boost::split(tokens, despaced, boost::is_space()); //TODO: use a separator string defined in the class
    BOOST_FOREACH(const std::string& tok, tokens)
    {
        if(tok.empty())
            continue;
        Keystroke ks = Keystroke::fromString(tok);
        if(ks.empty())
            continue;
        seq.push_back(ks);
        //TODO: Handle invalid tokens sensibly
    }
    return seq;
}

const std::map<const std::string, uint8_t>& Keystroke::allKeycodes()
{
    return allKeycodes_;
}

const std::map<const std::string, uint8_t>& Keystroke::allMods()
{
    return allMods_;
}

bool Keystroke::hasMod(uint8_t mod) const
{
    return mods_ & mod;
}

uint8_t Keystroke::mods() const
{
    return mods_;
}

const ByteString& Keystroke::keycodes() const
{
    return keyCodes_;
}

int Keystroke::numKeys() const
{
    return keyCodes_.size();
}

uint8_t Keystroke::keycode(int i) const
{
    return keyCodes_[i];
    //TODO: Perhaps return 0 for out-of-range i?
}

std::string Keystroke::keyName(int i) const
{
    return keycodeToString(keycode(i));
}

std::string Keystroke::keycodeToString(uint8_t keycode)
{
//FIXME: Wow, this is inefficient. Maybe try using a bidirectional map from Boost for the dictionary, or just keep an inverted copy around?
    typedef std::pair<const std::string, uint8_t> Entry;
    BOOST_FOREACH(Entry e, allKeycodes_)
    {
        if(e.second == keycode)
            return std::string(e.first);
    }
    throw InvalidKeycode((boost::format("%d") % (int)keycode).str());
}

Keystroke Keystroke::fromString(const std::string& s, uint8_t extraMods)
{
    std::vector<std::string> tokens;
    boost::split(tokens, s, boost::is_any_of("+")); //FIXME: Use the sepChar string
    int mods = 0;
    ByteString keyCodes;
    for(int i=0; i<tokens.size(); ++i)
    {
        if(tokens[i].empty())
            continue;
        int mod = -1;
        try
        {
            mod = modFromString(tokens[i]);
        }
        catch(InvalidModName err) {}
        if((mod>=0) && !(i==tokens.size()-1) && !(mods & mod))
        {
            mods |= mod;
        }
        else
        {
            uint8_t keyCode = Keystroke::keycodeFromString(tokens[i]);
            keyCodes.push_back(keyCode);
        }
    }
    return Keystroke(keyCodes, mods | extraMods);
}

uint8_t Keystroke::modFromString(const std::string inp)
{
    std::string keyStr = boost::trim_copy(inp);
    boost::to_upper(keyStr);
    try
    {
        return allMods_.at(keyStr.c_str());
    }
    catch(std::out_of_range)
    {
        throw InvalidModName(keyStr);
    }
}

uint8_t Keystroke::keycodeFromString(const std::string inp)
{
    std::string keyStr = boost::trim_copy(inp);
    boost::to_upper(keyStr);
    try
    {
        return allKeycodes_.at(keyStr.c_str());
    }
    catch(std::out_of_range)
    {
        throw InvalidKeyName(keyStr);
    }
}

std::string KeystrokeSequence::toString() const
{
    std::vector<std::string> pieces;
    BOOST_FOREACH(const Keystroke& ks, *this)
    {
        pieces.push_back(ks.toString());
    }
    return boost::algorithm::join(pieces, sepStr);
}

InvalidModName::InvalidModName(const std::string& __arg)
    :runtime_error(__arg)
{}

InvalidKeyName::InvalidKeyName(const std::string& __arg)
    :runtime_error(__arg)
{}

InvalidKeycode::InvalidKeycode(const std::string& __arg)
    : runtime_error(__arg)
{}

