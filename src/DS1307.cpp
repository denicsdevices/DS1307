/*
  This Library is written for DS1307 RTC
  Author: Denics Devices (Aaditya Dengle)
  Date: 16/09/2024

*/

#include "DS1307.h"

DS1307::DS1307() {}
DS1307::DS1307(uint8_t addr)
{
    _userI2C = 0;
    _addr = addr;
}

DS1307::DS1307(uint8_t addr, uint8_t sda, uint8_t scl, TwoWire *wireInst)
{
    _userI2C = 1;
    _addr = addr;
    _sda = sda;
    _scl = scl;

    mWire = wireInst;
}

char DS1307::begin()
{

    if (_userI2C)
    {
        mWire->setSDA(_sda);
        mWire->setSCL(_scl);
    }
    mWire->begin();
    mWire->beginTransmission(_addr);
    mWire->write(0x00);
    mWire->endTransmission();

    int a = 0;
    int r = (uint8_t)mWire->requestFrom((int)_addr, 8);
    while (mWire->available())
    {
        _data[a] = mWire->read();
        a++;
    }

    if (_data[0] & 0x80)
    {
        mWire->beginTransmission(_addr);
        mWire->write(0x00);
        mWire->write(0x7F & _data[0]);
        mWire->endTransmission();
    }

    if (r)
    {
        return r;
    }
    else
    {
        return 0;
    }
}
uint8_t DS1307::fromChipFormat(uint8_t data)
{
    uint8_t result = data & 0x0f;
    result += ((data & 0xf0) >> 4) * 10;
    return result;
}
uint8_t DS1307::toChipFormat(int data)
{
    int tmp1;
    if (data > 10)
    {
        tmp1 = (int)(data / 10);
    }
    else
    {
        tmp1 = 0;
    }
    int tmp2 = data - (tmp1 * 10);

    uint8_t result = (tmp1 << 4) | tmp2;

    return result;
}
char DS1307::getTime()
{
    mWire->beginTransmission(_addr);
    mWire->write(0x00);
    mWire->endTransmission();

    int a = 0;
    int r = (uint8_t)mWire->requestFrom((int)_addr, 8);
    while (mWire->available())
    {
        _data[a] = mWire->read();
        a++;
    }
    // Serial.printf("0x%02x\n", _data[0]);
    // time
    _seconds = fromChipFormat(_data[0]); // 0 - 60 seconds
    _minutes = fromChipFormat(_data[1]); // 0 - 60 minutes

    if (_data[2] & 0x40)
    {
        _hour = fromChipFormat(_data[2] & 0x1F);
        _ampm = ((_data[2] & 0x20) >> 5) & 1;
        _hour12 = 12;
    }
    else
    {
        _hour = fromChipFormat(_data[2] & 0x3F);
        _hour12 = 24;
    }

    // date
    _day = _data[3];                          // 1-7  day of week
    _date = fromChipFormat(_data[4]);         // 1-31 date
    _month = fromChipFormat(_data[5] & 0x3f); // 1- 12 month
    _year = fromChipFormat(_data[6]);         // 0- 99 year

    return r;
}

uint8_t DS1307::getSeconds()
{
    return _seconds;
}

uint8_t DS1307::getMinute()
{
    return _minutes;
}

uint8_t DS1307::getHour()
{
    return _hour;
}

char *DS1307::getDay()
{
    return _weekDays[_day];
}

uint8_t DS1307::getDate()
{
    return _date;
}

uint8_t DS1307::getMonth()
{
    return _month;
}

uint8_t DS1307::getYear()
{
    return _year;
}

uint8_t DS1307::getAMPM()
{
    return _ampm;
}

uint8_t DS1307::getFormat()
{
    return _hour12;
}

void DS1307::setFormat(uint8_t fmt)
{
    getTime();

    if (fmt == 12)
    {
        _data[2] |= 0x40;
        mWire->beginTransmission(_addr);
        mWire->write(0x02);
        mWire->write(_data[2]);
        mWire->endTransmission();
        _hour12 = 12;
    }
    else
    {
        _data[2] &= 0x3f;
        mWire->beginTransmission(_addr);
        mWire->write(0x02);
        mWire->write(_data[2]);
        mWire->endTransmission();
        _hour12 = 24;
    }
}

void DS1307::setAMPM(uint8_t ampm)
{
    getTime();

    if (ampm == 1)
    {
        _data[2] |= 0x20;
        mWire->beginTransmission(_addr);
        mWire->write(0x02);
        mWire->write(_data[2]);
        mWire->endTransmission();
        _ampm = 1;
    }
    else
    {
        _data[2] &= 0x5f;
        mWire->beginTransmission(_addr);
        mWire->write(0x02);
        mWire->write(_data[2]);
        mWire->endTransmission();
        _ampm = 0;
    }
}
void DS1307::setTime(const char *t)
{
    int sec;
    int min;
    int hour;
    sscanf(t, "%d:%d:%d", &hour, &min, &sec);

    _data[0] = toChipFormat(sec);
    _data[1] = toChipFormat(min);
    _data[2] &= 0x60;
    _data[2] |= toChipFormat(hour);

    mWire->beginTransmission(_addr);
    mWire->write(0x00);
    mWire->write(_data[0]);
    mWire->write(_data[1]);
    mWire->write(_data[2]);
    mWire->endTransmission();
}
void DS1307::setDate(const char *d)
{
    int date;
    int mon;
    int year;

    sscanf(d, "%d/%d/%d", &mon, &date, &year);

    _data[4] = toChipFormat(date);
    _data[5] &= 0x80;
    _data[5] = toChipFormat(mon);
    _data[6] = toChipFormat(year);

    mWire->beginTransmission(_addr);
    mWire->write(0x04);
    mWire->write(_data[4]);
    mWire->write(_data[5]);
    mWire->write(_data[6]);
    mWire->endTransmission();
}
void DS1307::setDay(uint8_t d)
{
    _data[2] = toChipFormat(d);
}
