/*
 *	File:		CarveDSPUnit.h
 *
 *	Version:	0.01.00
 *
 *	Created:	17/01/2015
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

#include "math.h"

#ifndef Carve_CarveDSPUnit_h
#define Carve_CarveDSPUnit_h




class CarveDSPUnit {
private:
    float   kMinVal_PreGain,
            kMaxVal_PreGain,
            kMinVal_PostGain,
            kMaxVal_PostGain,
            kMinVal_Misc,
            kMaxVal_Misc,
            kMinVal_Mode,
            kMaxVal_Mode;
    
    Float32 preGain,
            postGain,
            misc;
    
    int mode;
    
    // TODO: check if an int bounds check method is necessary of if float works for both
    float boundsCheck(float param, float min, float max) {
        if (param < min) param = min;
        if (param > max) param = max;
        
        return param;
    }
    
    Float32 processSine(Float32 inSample) {
        return sin(M_PI * inSample * preGain + (misc * M_PI)) * postGain;
    }
    
    Float32 processParabolicSoft(Float32 inSample) {
        return (inSample * ((4 * misc) - sqrt((4 - preGain) * pow(inSample, 2))) * 0.5) * postGain;
    }
    
    Float32 processParabolicHard(Float32 inSample) {
        return (atan(preGain * 4 * inSample + (misc * M_PI)) / 1.5) * postGain;
    }
    
    Float32 processAsymmetricSine(Float32 inSample) {
        return (cos(inSample * (misc + 1)) * atan(4 * inSample * preGain)) * postGain;
    }
    
    Float32 processExponent(Float32 inSample) {
        return (sin(misc * pow(M_E, (inSample + preGain)))) * postGain;
    }
    
public:
    CarveDSPUnit(float preGainMin, float preGainMax, float postGainMin, float postGainMax, float miscMin, float miscMax, int modeMin, int modeMax) {
        
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
    void setMode(int val) {
        mode = val;
        mode = boundsCheck(mode, kMinVal_Mode, kMaxVal_Mode);
    }
    
    void setPreGain(Float32 val) {
        preGain = val;
        preGain = boundsCheck(preGain, kMinVal_PreGain, kMaxVal_PreGain);
    }
    
    void setPostGain(Float32 val) {
        postGain = val;
        postGain = boundsCheck(postGain, kMinVal_PostGain, kMaxVal_PostGain);
    }
    
    void setMisc(Float32 val) {
        misc = val;
        misc = boundsCheck(misc, kMinVal_Misc, kMaxVal_Misc);
    }
    
    // process methods
    Float32 process(Float32 inSample) {
        if (mode < 0.4) { return processSine(inSample); }
        if (mode > 0.5 && mode < 1.4) { return processParabolicSoft(inSample); }
        if (mode > 1.5 && mode < 2.4) { return processParabolicHard(inSample); }
        if (mode > 2.5 && mode < 3.4) { return processAsymmetricSine(inSample); }
        if (mode > 3.5 && mode < 4.4) { return processExponent(inSample); }
        
        return 0.0;
    }
};

#endif
