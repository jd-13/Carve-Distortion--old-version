/*
 *	File:		Carve.h
 *
 *	Version:	0.01.00
 *
 *	Created:	05/01/2015
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

#include "AUEffectBase.h"
#include "CarveVersion.h"
#include "CarveDSPUnit.h"

#if AU_DEBUG_DISPATCHER
#include "AUDebugDispatcher.h"
#endif


#ifndef __Carve_h__
#define __Carve_h__

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Constants for parameters and factory presets
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#pragma mark ____Carve Parameters
static CFStringRef kParamName_Mode = CFSTR("Mode");
static const int kMode_Sine = 0;
static const int kMode_ParabolicSoft = 1;
static const int kMode_ParabolicHard = 2;
static const int kMode_AsymmetricSine = 3;
static const int kMode_Exponent = 4;
static const int kDefVal_Mode = kMode_Sine;

static CFStringRef kParamName_PreGain = CFSTR("Pre Gain");
static const float kDefVal_PreGain = 1;
static const float kMinVal_PreGain = 0;
static const float kMaxVal_PreGain = 2;

static CFStringRef kParamName_PostGain = CFSTR("Post Gain");
static const float kDefVal_PostGain = 0.5;
static const float kMinVal_PostGain = 0;
static const float kMaxVal_PostGain = 2;

static CFStringRef kParamName_Misc = CFSTR("Misc");
static const float kDefVal_Misc = 0;
static const float kMinVal_Misc = -1;
static const float kMaxVal_Misc = 1;

static const CFStringRef kParamName_DryLevel = CFSTR("Dry Level");
static const float kDefVal_DryLevel = 0;
static const float kMinVal_DryLevel = 0;
static const float kMaxVal_DryLevel = 2;

static const CFStringRef kParamName_MasterVol = CFSTR("Master Vol");
static const float kDefVal_MasterVol = 1;
static const float kMinVal_MasterVol = 0;
static const float kMaxVal_MasterVol = 2;

static const CFStringRef kParamName_SignalFlow = CFSTR("Signal Flow");
static const int kSignalFlow_Serial = 0;
static const int kSignalFlow_Parallel = 1;
static const int kSignalFlow_Stereo = 2;
static const int kDefVal_SignalFlow = kSignalFlow_Serial;

static CFStringRef kMenuItem_Mode_Sine = CFSTR("Sine");
static CFStringRef kMenuItem_Mode_ParabolicSoft = CFSTR("Parabolic Soft");
static CFStringRef kMenuItem_Mode_ParabolicHard = CFSTR("Parabolic Hard");
static CFStringRef kMenuItem_Mode_AsymmetricSine = CFSTR("Asymmetric Sine");
static CFStringRef kMenuItem_Mode_Exponent = CFSTR("Exponent");

static CFStringRef kMenuItem_SignalFlow_Serial = CFSTR("Serial");
static CFStringRef kMenuItem_SignalFlow_Parallel = CFSTR("Parallel");
static CFStringRef kMenuItem_SignalFlow_Stereo = CFSTR("Stereo");



// any changes to the following enum must be reflected in CarveCocoaView.mm
enum {
    kParam_Mode1 = 0,
    kParam_PreGain1 = 1,
    kParam_PostGain1 = 2,
    kParam_Misc1 = 3,
    kParam_Mode2 = 4,
    kParam_PreGain2 = 5,
    kParam_PostGain2 = 6,
    kParam_Misc2 = 7,
    kParam_DryLevel = 8,
    kParam_MasterVol = 9,
    kParam_SignalFlow = 10,
    kNumberOfParameters = 11
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Carve class
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#pragma mark ____CarveKernel

class CarveKernel : public AUKernelBase {
public:
    CarveKernel(AUEffectBase *inAudioUnit);
    virtual ~CarveKernel();
    
    virtual void Process(const Float32  *inSourceP,
                         Float32        *inDestP,
                         UInt32         inFramesToProcess,
                         UInt32         inNumChannels,
                         bool           &ioSilence);
    
    virtual void Reset();
    
    CarveDSPUnit DSPUnit1;
    CarveDSPUnit DSPUnit2;
    
    //private: state variables
};

#pragma mark ____Carve

class Carve : public AUEffectBase {
public:
    Carve(AudioUnit component);
    
    virtual OSStatus Version() { return kCarveVersion; }
    
    virtual OSStatus Initialize();
    
    virtual AUKernelBase *NewKernel() { return new CarveKernel(this); }
    
    virtual OSStatus GetPropertyInfo(AudioUnitPropertyID    inID,
                                     AudioUnitScope         inScope,
                                     AudioUnitElement       inElement,
                                     UInt32                 &outDataSize,
                                     Boolean                 &outWritable);
    
    virtual OSStatus GetProperty(AudioUnitPropertyID        inID,
                                 AudioUnitScope             inScope,
                                 AudioUnitElement           inElement,
                                 void                       *outData);
    
    virtual OSStatus GetParameterInfo(AudioUnitScope        inScope,
                                      AudioUnitParameterID  inParameterID,
                                      AudioUnitParameterInfo &outParameterInfo);
    
    virtual OSStatus GetParameterValueStrings(AudioUnitScope        inScope,
                                              AudioUnitParameterID  inParameterID,
                                              CFArrayRef            *outStrings);
    
    
    //virtual OSStatus GetPresets(CFArrayRef *outData) const;
    
    //virtual OSStatus NewFactoryPresetSet(const AUPreset &inNewFactoryPreset);
    
    virtual bool SupportsTail() { return true; }
    virtual Float64 GetTailTime() { return 0.001; }
    virtual Float64 GetLatency() { return 0.0; }
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#endif