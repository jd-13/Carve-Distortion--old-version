/*
 *	File:		CarveDSPUnit.cpp
 *
 *	Version:	0.01.00
 *
 *	Created:	21/07/2015
 *
 *	This file is part of Carve.
 *
 *  Carve is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Carve is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Carve.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "CarveDSPUnit.h"

template<typename T>
T CarveDSPUnit::boundsCheck(T param, T min, T max) {
    if (param < min) param = min;
    if (param > max) param = max;
    
    return param;
}

inline Float32 CarveDSPUnit::processSine(Float32 inSample) const {
    return sin(M_PI * inSample * preGain + (misc * M_PI)) * postGain;
}

inline Float32 CarveDSPUnit::processParabolicSoft(Float32 inSample) const {
    return (inSample * ((4 * misc) - sqrt((4 - preGain) * pow(inSample, 2))) * 0.5) * postGain;
}

inline Float32 CarveDSPUnit::processParabolicHard(Float32 inSample) const {
    return (atan(preGain * 4 * inSample + (misc * M_PI)) / 1.5) * postGain;
}

inline Float32 CarveDSPUnit::processAsymmetricSine(Float32 inSample) const {
    return (cos(inSample * (misc + 1)) * atan(4 * inSample * preGain)) * postGain;
}

inline Float32 CarveDSPUnit::processExponent(Float32 inSample) const {
    return (sin(misc * pow(M_E, (inSample + preGain)))) * postGain;
}

CarveDSPUnit::CarveDSPUnit(float preGainMin, float preGainMax, float postGainMin, float postGainMax, float miscMin, float miscMax, int modeMin, int modeMax) {
    
    kMinVal_PreGain = preGainMin;
    kMaxVal_PreGain = preGainMax;
    
    kMinVal_PostGain = postGainMin;
    kMaxVal_PostGain = postGainMax;
    
    kMinVal_Misc = miscMin;
    kMaxVal_Misc = miscMax;
    
    kMinVal_Mode = modeMin;
    kMaxVal_Mode = modeMax;
}

// set parameter methods, w/ integrated bounds checks
void CarveDSPUnit::setMode(int val) {
    mode = val;
    mode = boundsCheck<int>(mode, kMinVal_Mode, kMaxVal_Mode);
}

void CarveDSPUnit::setPreGain(Float32 val) {
    preGain = val;
    preGain = boundsCheck(preGain, kMinVal_PreGain, kMaxVal_PreGain);
}

void CarveDSPUnit::setPostGain(Float32 val) {
    postGain = val;
    postGain = boundsCheck(postGain, kMinVal_PostGain, kMaxVal_PostGain);
}

void CarveDSPUnit::setMisc(Float32 val) {
    misc = val;
    misc = boundsCheck(misc, kMinVal_Misc, kMaxVal_Misc);
}

// process methods
Float32 CarveDSPUnit::process (Float32 inSample) const {
    if (mode < 0.4) { return processSine(inSample); }
    if (mode > 0.5 && mode < 1.4) { return processParabolicSoft(inSample); }
    if (mode > 1.5 && mode < 2.4) { return processParabolicHard(inSample); }
    if (mode > 2.5 && mode < 3.4) { return processAsymmetricSine(inSample); }
    if (mode > 3.5 && mode < 4.4) { return processExponent(inSample); }
    
    return 0.0;
}