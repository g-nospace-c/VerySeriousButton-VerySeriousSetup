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

#include <vector>
#include <stdexcept>
#include "hidapi.h"
#include "keyseq.h"
#include "bytestring.h"
#include "vsb_config.h"

namespace VsbMode
{
const uint8_t inactive = 0;
const uint8_t joystick = 1;
const uint8_t singlekey = 2;
const uint8_t keyseq = 3;
};

struct HidDeviceInfo
{
    uint16_t vendorId;
    uint16_t productId;
    uint16_t usagePage;
    uint16_t usage;
    std::string mfrString;
    std::string productString;
    std::string serialNumber;
    std::string releaseNumber;
    std::string path;
    HidDeviceInfo(uint16_t vendorId, uint16_t productId, uint16_t usagePage,
		  uint16_t usage, const std::string& manufacturer,
		  const std::string& product, const std::string& serialNumber,
		  const std::string& releaseNumber, const std::string& path);
};

typedef std::vector<HidDeviceInfo> HidDeviceInfos;

class HidDeviceError : public std::runtime_error
{
public:
    HidDeviceError(const std::string&);
};

class VsbError : public std::runtime_error
{
public:
    VsbError(const std::string&);
};

class HidDevice {
private:
    hid_device *devHandle;
    const std::string& devPath;
public:
    static HidDeviceInfos findDevices(
        uint16_t vendorId = 0, uint16_t productId = 0,
        const char* serialNumber = 0,
	uint16_t usagePage = 0, uint16_t usage = 0);
    HidDevice(const std::string& path);
    HidDevice(const HidDeviceInfo &devInfo);
    void sendFeatureReport(uint8_t reportId, const ByteString& data);
    ByteString getFeatureReport(uint8_t reportId, int maxLen);
    bool isOpen();
    void open();
    void close();
};


class VsbInfo
{
public:
    std::string serialNumber;
    std::string fwVersion;
    int singlekeyNumKeys;
    int keyseqNumKeys;
    int keyseqNumPages;
    VsbInfo(const std::string& serialNumber, const std::string& fwVersion,
            int singlekeyNumKeys, int keyseqNumKeys, int keyseqNumPages);
};

struct VsbConfig
{
    int mode;
    Keystroke singlekeyKeys;
    KeystrokeSequence keyseqKeyseq;
};

struct VsbRawInfo
{
    uint8_t singlekeyNumKeys;
    uint8_t keyseqNumKeys;
    uint8_t keyseqPageSize;
    uint8_t keyseqNumPages;
};

struct VsbRawConfig
{
    uint8_t mode;
    uint8_t mods;
    ByteString keycodes;
    uint8_t keyseqLen;
};

class VsbDevice
    : private HidDevice
{
private:
    VsbInfo *info;
public:
    static HidDeviceInfos findDevices(const char* serialNumber = 0);
    VsbDevice(const std::string& path);
    VsbDevice(const HidDeviceInfo &devInfo);
    ~VsbDevice();
    VsbRawInfo getRawInfo();
    Keystroke readKeyseqPage(uint8_t i);
    VsbRawConfig getRawConfig();
    void setRawConfig(const VsbRawConfig& rawCfg);
    void setConfig(const VsbConfig& cfg);
    void storeConfig();
    void storeKeyseq(const KeystrokeSequence& seq);
    void storeKeyseqPage(uint8_t i, const Keystroke& keys);
    KeystrokeSequence readKeyseq(const VsbRawConfig& rawCfg);
    KeystrokeSequence readKeyseq();
    void loadConfig();
    void open();
    bool isOpen();
    void close();
    VsbConfig getConfig();
    VsbInfo getInfo();
    ByteString doQuery(uint8_t cmdId, ByteString data = ByteString());
};

struct VsbInfoAndConfig
{
    VsbInfo info;
    VsbConfig config;
};

namespace VsbProg
{
::VsbInfo getInfo(const std::string& path);
::VsbConfig getConfig(const std::string& path);
::VsbInfoAndConfig getInfoAndConfig(const std::string& path);
void setKeyseqMode(const std::string& path, const KeystrokeSequence& seq);
void setJoystickMode(const std::string& path);
void setSinglekeyMode(const std::string& path, const Keystroke& keys);
};
