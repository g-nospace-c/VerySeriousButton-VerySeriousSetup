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

#include <string>
#include <stdint.h>
#include <boost/locale.hpp> //Can't use <codecvt> because C++11 support in libstdc++ isn't finished yet :-(
#include <boost/format.hpp>
#include <boost/concept_check.hpp>
#include "vsbprog.h"
#include "hidapi.h"

#define VSB_TRANSFER_SIZE (VSB_FEATREP_SIZE + 1)
#define NUM_READ_TRIES 16

//TODO: Re-order definitions (so that they match the header) to make maintenance easier

HidDeviceInfo::HidDeviceInfo(uint16_t vendorId, uint16_t productId,
                             uint16_t usagePage, uint16_t usage,
                             const std::string& manufacturer,
                             const std::string& product,
                             const std::string& serialNumber,
			     const std::string& releaseNumber,
                             const std::string& path)
    : vendorId(vendorId),
      productId(productId),
      usagePage(usagePage),
      usage(usage),
      mfrString(manufacturer),
      productString(product),
      serialNumber(serialNumber),
      releaseNumber(releaseNumber),
      path(path)
{}

void utf16BufferToUtf8String(std::string &dest, const wchar_t *src)
{
    if(!src)
    {
        dest.clear();
        return;
    }
    dest.assign(boost::locale::conv::from_utf(src, "UTF-8"));
}

HidDeviceInfos HidDevice::findDevices(
    uint16_t vendorId, uint16_t productId, const char* serialNumber,
    uint16_t usagePage, uint16_t usage)
{
    HidDeviceInfos results;
    struct hid_device_info* devs, * dev;
    devs = hid_enumerate(vendorId, productId);
    for(dev=devs; dev; dev=dev->next)
    {
        if(usagePage && usage)
            if((dev->usage_page != usagePage) || (dev->usage != usage))
                continue;
        std::string devSerialNumber;
        utf16BufferToUtf8String(devSerialNumber, dev->serial_number);
        std::string mfrString;
        utf16BufferToUtf8String(mfrString, dev->manufacturer_string);
        std::string productString;
        utf16BufferToUtf8String(productString, dev->product_string);
	uint8_t releaseHi = (dev->release_number & 0xFF00) >> 8;
	uint8_t releaseLo = dev->release_number & 0x00FF;
	std::string releaseNumber = (boost::format("%02d.%02d") % (int)releaseHi % (int)releaseLo).str();
        results.push_back(
            HidDeviceInfo(dev->vendor_id, dev->product_id, dev->usage_page,
                          dev->usage, mfrString, productString, devSerialNumber,
                          releaseNumber, dev->path));
    }
    hid_free_enumeration(devs);
    if(!serialNumber)
        return results;
    //TODO: Instead of copying into a new list, just delete from the original
    HidDeviceInfos matches;
    auto filterFn = [serialNumber](const HidDeviceInfo& devInfo)
    {
        return (!devInfo.serialNumber.compare(serialNumber));
    };
    std::copy_if(results.begin(), results.end(), std::back_inserter(matches),
                 filterFn);
    return matches;
}

HidDevice::HidDevice(const std::string& path)
    : devHandle(NULL), devPath(path)
{}

HidDevice::HidDevice(const HidDeviceInfo& devInfo)
    : HidDevice(devInfo.path.c_str())
{}

void HidDevice::open()
{
    devHandle = hid_open_path(devPath.c_str());
    if(!devHandle)
    {   
        std::string errorString;
        utf16BufferToUtf8String(errorString, hid_error(devHandle));
        throw HidDeviceError(errorString);
    }
}

bool HidDevice::isOpen()
{
    return (bool)devHandle;
}

HidDeviceError::HidDeviceError(const std::string& arg)
    : runtime_error(arg)
{}

VsbError::VsbError(const std::string& arg)
    : runtime_error(arg)
{}

ByteString HidDevice::getFeatureReport(uint8_t reportId, int maxLen)
{
    uint8_t buf[maxLen];
    buf[0] = reportId;
    int r = hid_get_feature_report(devHandle, buf, maxLen);
    if(r<0)
    {
        std::string errorString;
        utf16BufferToUtf8String(errorString, hid_error(devHandle));
        throw HidDeviceError(errorString);
    }
    return ByteString(buf+1, r-1);
}

void HidDevice::sendFeatureReport(uint8_t reportId, const ByteString& data)
{
    ByteString reportBuf = ByteString(1, reportId) + data;
    int r = hid_send_feature_report(devHandle, reportBuf.data(), reportBuf.size());
    if(r<0)
    {
        std::string errorString;
        utf16BufferToUtf8String(errorString, hid_error(devHandle));
        throw HidDeviceError(errorString);
    }
}

void HidDevice::close()
{
    if(isOpen()) {
        hid_close(devHandle);
        devHandle = NULL;
    }
}

void VsbDevice::open()
{
    try
    {
        HidDevice::open();
    }
    catch(HidDeviceError err)
    {
        throw VsbError(std::string("Failed to open device!\t") + err.what());
    }
    info = new VsbInfo(getInfo());
}

VsbDevice::~VsbDevice()
{
    if(info)
        delete info;
}

bool VsbDevice::isOpen()
{
    return HidDevice::isOpen();
}

void VsbDevice::close()
{
    HidDevice::close();
}

VsbRawInfo VsbDevice::getRawInfo()
{
    ByteString data = doQuery(VSB_CMD_GETDEVINFO);
    VsbRawInfo rawInfo = {
        .singlekeyNumKeys = data[0],
        .keyseqNumKeys = data[1],
        .keyseqPageSize = data[2],
        .keyseqNumPages = data[3]
    };
    return rawInfo;
}

ByteString VsbDevice::doQuery(uint8_t cmdId, ByteString data)
{
    ByteString cmdBuf(VSB_TRANSFER_SIZE, 0);
    cmdBuf[0] = cmdId;
    std::copy(data.begin(), data.end(), cmdBuf.begin()+2);
    sendFeatureReport(VSB_REPORTID_VSB, cmdBuf);
    bool dataOk = false;
    ByteString resp;
    for(int i = 0; i < NUM_READ_TRIES; ++i)
    {
        try
        {
            resp = getFeatureReport(VSB_REPORTID_VSB, VSB_TRANSFER_SIZE);
        }
        catch(HidDeviceError err)
        {
            if(i < NUM_READ_TRIES-1)
                continue;
            throw VsbError(std::string("HID subsystem error: ") + err.what());
        }
        if(resp.size() < 2)
            continue; //Probably shouldn't ever happen, but let's not crash if it does
        if(!resp[0])
            continue; //This also shouldn't happen...
        //FIXME: Implement a proper exception class to use here
        if(resp[0] != cmdId)
            throw std::runtime_error(boost::str(boost::format(
                "Got the wrong command ID back! (%d)") % (int)resp[1]));
        if(resp[1] == VSB_RESP_BUSY)
        {
            //TODO: Insert a small time delay
            continue;
        }
        else if(resp[1] == VSB_RESP_NULL)
            throw VsbError("Null response code?");
        else if(resp[1] == VSB_RESP_BADCMD)
            throw VsbError("Device reported bad command ID");
        else if(resp[1] == VSB_RESP_BADCS)
            throw VsbError("Device reported stored configuration is corrupt/invalid");
        else if(resp[1] == VSB_RESP_BADIDX)
            throw VsbError("Device reported bad keyseq page number");
        else if(resp[1] == VSB_RESP_ERR)
            throw VsbError("Device reported general error");
        else if(resp[1] == VSB_RESP_OK)
        {
            dataOk = true;
            break;
        }
        else
            throw VsbError("Got invalid response code");
    }
    if(!dataOk)
        throw VsbError("Device failed to respond in time");
    return ByteString(resp, 2);
}

VsbRawConfig VsbDevice::getRawConfig()
{
    ByteString data = doQuery(VSB_CMD_GETCFG);
    VsbRawConfig rawConfig =
    {
        .mode = data[0],
        .mods = data[1],
        .keycodes = ByteString(data, 2, getInfo().singlekeyNumKeys),
        .keyseqLen = data[8]
    };
    return rawConfig;
}

void VsbDevice::setRawConfig(const VsbRawConfig& rawCfg)
{
    ByteString cmdData(3+info->singlekeyNumKeys, 0);
    cmdData[0] = rawCfg.mode;
    cmdData[1] = rawCfg.mods;
    cmdData[cmdData.size()-1] = rawCfg.keyseqLen;
    for(int i=0; (i<rawCfg.keycodes.size()) && (i < cmdData.size()-1); ++i)
        cmdData[i+2] = rawCfg.keycodes[i];
    doQuery(VSB_CMD_SETCFG, cmdData);
}

void VsbDevice::setConfig(const VsbConfig& cfg)
{
    VsbRawConfig rawCfg =
    {
        .mode = (uint8_t)cfg.mode,
        .mods = cfg.singlekeyKeys.mods(),
        .keycodes = cfg.singlekeyKeys.keycodes(),
        .keyseqLen = (uint8_t)cfg.keyseqKeyseq.size()
    };
    if(cfg.mode == VsbMode::keyseq)
    {
        storeKeyseq(cfg.keyseqKeyseq);
    }
    setRawConfig(rawCfg);
}

void VsbDevice::storeConfig()
{
    doQuery(VSB_CMD_SAVECFG);
}

void VsbDevice::loadConfig()
{
    doQuery(VSB_CMD_LOADCFG);
}

void VsbDevice::storeKeyseq(const KeystrokeSequence& seq)
{
    if(seq.size() > info->keyseqNumPages)
        throw VsbError("Key sequence too long");
    for(int i=0; i < seq.size(); ++i)
        storeKeyseqPage(i, seq[i]);
}

void VsbDevice::storeKeyseqPage(uint8_t i, const Keystroke& keys)
{
    ByteString cmdData;
    cmdData.push_back(i);
    cmdData.push_back(keys.mods());
    cmdData.append(keys.keycodes(), 0, info->keyseqNumKeys);
    doQuery(VSB_CMD_WRITEPAGE, cmdData);
}

Keystroke VsbDevice::readKeyseqPage(uint8_t i)
{
    ByteString cmdData;
    cmdData.push_back(i);
    ByteString resp = doQuery(VSB_CMD_READPAGE, cmdData);
    if(resp[0] != i)
        throw VsbError("Device returned wrong page ID when reading keyseq page");
    return Keystroke(ByteString(resp, 2, info->keyseqNumKeys), resp[1]);
}

KeystrokeSequence VsbDevice::readKeyseq(const VsbRawConfig& rawCfg)
{
    KeystrokeSequence keySeq;
    for(int i=0; i < rawCfg.keyseqLen; ++i)
        keySeq.push_back(readKeyseqPage(i));
    return keySeq;
}

KeystrokeSequence VsbDevice::readKeyseq()
{
    return readKeyseq(getRawConfig());
}


VsbConfig VsbDevice::getConfig()
{
    VsbRawConfig rawConfig = getRawConfig();
    VsbConfig config =
    {
        .mode = rawConfig.mode,
        .singlekeyKeys = Keystroke(rawConfig.keycodes, rawConfig.mods),
        .keyseqKeyseq = readKeyseq(rawConfig)
    };
    return config;
}

VsbInfo VsbDevice::getInfo()
{
    VsbRawInfo rawInfo = getRawInfo();
    VsbInfo info
    (
        std::string(),
        std::string(),
        rawInfo.singlekeyNumKeys,
        rawInfo.keyseqNumKeys,
        rawInfo.keyseqNumPages
    );
    return info;
}

VsbDevice::VsbDevice(const std::string& path)
    : HidDevice(path), info(NULL)
{
}

HidDeviceInfos VsbDevice::findDevices(const char* serialNumber)
{
    return HidDevice::findDevices(VSB_VENDORID, VSB_PRODID, serialNumber,
                                  0xFF01, 0x0001);
}

VsbDevice::VsbDevice(const HidDeviceInfo& devInfo)
    : VsbDevice(devInfo.path)
{}

VsbInfo::VsbInfo(const std::string& serialNumber, const std::string& fwVersion,
                 int singlekeyNumKeys, int keyseqNumKeys, int keyseqNumPages)
    : serialNumber(serialNumber), fwVersion(fwVersion),
      singlekeyNumKeys(singlekeyNumKeys), keyseqNumKeys(keyseqNumKeys),
      keyseqNumPages(keyseqNumPages)
{}

VsbInfo VsbProg::getInfo(const std::string& path)
{
    VsbDevice dev(path);
    dev.open();
    VsbInfo info = dev.getInfo();
    dev.close();
    return info;
}

VsbConfig VsbProg::getConfig(const std::string& path)
{
    VsbDevice dev(path);
    dev.open();
    VsbConfig config = dev.getConfig();
    dev.close();
    return config;
}

VsbInfoAndConfig VsbProg::getInfoAndConfig(const std::string& path)
{
    boost::optional<VsbInfoAndConfig> infoAndConfig = boost::none;
    VsbDevice dev(path);
    dev.open();
    try
    {
        infoAndConfig.emplace(VsbInfoAndConfig{
            .info = dev.getInfo(),
            .config = dev.getConfig()
            });
    }
    catch(...)
    {
        dev.close();
        throw;
    }
    dev.close();
    return *infoAndConfig;
}

void VsbProg::setJoystickMode(const std::string& path)
{
    VsbDevice dev(path);
    dev.open();
    VsbConfig config;
    try
    {
        config = dev.getConfig();
    }
    catch(...)
    {
        dev.close();
        throw;
    }
    config.mode = VsbMode::joystick;
    try
    {
        dev.setConfig(config);
        dev.storeConfig();
    }
    catch(...)
    {
        dev.close();
        throw;
    }
    dev.close();
}

void VsbProg::setSinglekeyMode(const std::string& path, const Keystroke& keys)
{
    VsbDevice dev(path);
    dev.open();
    VsbConfig config;
    try
    {
        config = dev.getConfig();
    }
    catch(...)
    {
        dev.close();
        throw;
    }
    config.mode = VsbMode::singlekey;
    config.singlekeyKeys = keys;
    try
    {
        dev.setConfig(config);
        dev.storeConfig();
    }
    catch(...)
    {
        dev.close();
        throw;
    }
    dev.close();
}

void VsbProg::setKeyseqMode(const std::string& path, const KeystrokeSequence& seq)
{
    VsbDevice dev(path);
    dev.open();
    VsbConfig config = dev.getConfig();
    config.mode = VsbMode::keyseq;
    config.keyseqKeyseq = seq;
    dev.setConfig(config);
    dev.storeConfig();
    dev.close();
}
