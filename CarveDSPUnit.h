/*
 *	File:		CarveDSPUnit.h
 *
 *	Version:	0.01.01
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
#include "MacTypes.h"

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
    
    template<typename T>
    T boundsCheck(T param, T min, T max);
    
    Float32 processSine(Float32 inSample) const;
    
    Float32 processParabolicSoft(Float32 inSample) const;
    
    Float32 processParabolicHard(Float32 inSample) const;
    
    Float32 processAsymmetricSine(Float32 inSample) const;
    
    Float32 processExponent(Float32 inSample) const;
    
public:
    CarveDSPUnit(float preGainMin, float preGainMax, float postGainMin, float postGainMax, float miscMin, float miscMax, int modeMin, int modeMax);
    
    // set parameter methods, w/ integrated bounds checks
    void setMode(int val);
    
    void setPreGain(Float32 val);
    
    void setPostGain(Float32 val);
    
    void setMisc(Float32 val);
    
    // process methods
    Float32 process (Float32 inSample) const;
};

#endif
