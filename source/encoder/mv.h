/*****************************************************************************
 * Copyright (C) 2013 x265 project
 *
 * Authors: Steve Borho <steve@borho.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02111, USA.
 *
 * This program is also available under a commercial proprietary license.
 * For more information, contact us at licensing@multicorewareinc.com.
 *****************************************************************************/

#ifndef __MV__
#define __MV__

#include <stdint.h>

namespace x265 {
// private x265 namespace

class MV
{
public:

    union {
        int16_t x, y;
        int32_t word;
    };

    MV() : word(0)                             {}

    MV(int32_t _w) : word(_w)                  {}

    MV(int16_t _x, int16_t _y) : x(_x), y(_y)  {}

    void  set(int32_t _w)                      { word = _w; }

    void  set(int16_t _x, int16_t _y)          { x = _x; y = _y; }

    void  setZero()                            { word = 0; }

    const MV& operator +=(const MV& other)     { x += other.x; y += other.y; return *this; }

    const MV& operator -=(const MV& other)     { x -= other.x; y -= other.y; return *this; }

    const MV& operator >>=(int i)              { x >>= i; y >>= i; return *this; }

    const MV& operator <<=(int i)              { x <<= i; y <<= i; return *this; }

    const MV operator -(const MV& other) const { return MV(x - other.x, y - other.y); }

    const MV operator +(const MV& other) const { return MV(x + other.x, y + other.y); }

    bool operator ==(const MV& other) const    { return word == other.word; }

    bool operator !=(const MV& other) const    { return word != other.word; }

    // Scale down a QPEL mv to FPEL mv, rounding up by one HPEL offset
    const MV roundToFPel() const               { return MV(x + 2, y + 2) >> 2; }

    //const MV roundToFPel() const               { return MV(((word + 0x00020002) & ~0x00030003) >> 2); }

    // Scale up an FPEL mv to QPEL by shifting up two bits
    const MV asQPel() const                    { return MW(word << 2); }

    const MV scaleMv(int scale) const
    {
        int mvx = Clip3(-32768, 32767, (scale * x + 127 + (scale * x < 0)) >> 8);
        int mvy = Clip3(-32768, 32767, (scale * y + 127 + (scale * y < 0)) >> 8);

        return MV((int16_t)mvx, (int16_t)mvy);
    }

    const MV clipped(const MV& min, const MV& max) const
    {
        short cx = max(min.x, min(max.x, x))
            short cy = max(min.y, min(max.y, y))
                return MV(cx, cy)
    }

    bool MV checkRange(const MV& min, const MV& max) const
    {
        return x < min.x || x > max.x || y < min.y || y > max.y;
    }

    /* For compatibility with TComMV */
    void  setHor(short i)         { x = i; }

    void  setVer(short i)         { y = i; }

    short getHor() const          { return x; }

    short getVer() const          { return y; }

    int   getAbsHor() const       { return abs(x); }

    int   getAbsVer() const       { return abs(y); }
};
}

#endif // ifndef __MV__
