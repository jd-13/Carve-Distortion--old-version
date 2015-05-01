/*
 *	File:		Carve.cpp
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

#include "Carve.h"
#include "CarveDSPUnit.h"
#include "math.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// COMPONENT_ENTRY(Carve) deprecated on MacOS X 10.7 see TN2276

AUDIOCOMPONENT_ENTRY(AUBaseProcessFactory, Carve)

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Carve::Carve
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Carve::Carve(AudioUnit component)
: AUEffectBase(component)
{
    CreateElements();
    Globals()->UseIndexedParameters(kNumberOfParameters);
    SetParameter(kParam_Mode1, kDefVal_Mode);
    SetParameter(kParam_PreGain1, kDefVal_PreGain);
    SetParameter(kParam_PostGain1, kDefVal_PostGain);
    SetParameter(kParam_Misc1, kDefVal_Misc);
    SetParameter(kParam_Mode2, kDefVal_Mode);
    SetParameter(kParam_PreGain2, kDefVal_PreGain);
    SetParameter(kParam_PostGain2, kDefVal_PostGain);
    SetParameter(kParam_Misc2, kDefVal_Misc);
    SetParameter(kParam_DryLevel, kDefVal_DryLevel);
    SetParameter(kParam_MasterVol, kDefVal_MasterVol);
    SetParameter(kParam_SignalFlow, kDefVal_SignalFlow);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Carve::Initialize
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus			Carve::Initialize()
{
    OSStatus result = AUEffectBase::Initialize();
    
    if(result == noErr )
    {

    }
    
    return result;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Carve::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus			Carve::GetParameterValueStrings(AudioUnitScope		inScope,
                                                    AudioUnitParameterID	inParameterID,
                                                    CFArrayRef *		outStrings)
{
    // for Unit 1s drop down menu
    if ((inScope == kAudioUnitScope_Global) && (inParameterID == kParam_Mode1)) {
        if (outStrings == NULL) return noErr;
        
        CFStringRef strings1 [] = {
            kMenuItem_Mode_Sine,
            kMenuItem_Mode_ParabolicSoft,
            kMenuItem_Mode_ParabolicHard,
            kMenuItem_Mode_AsymmetricSine,
            kMenuItem_Mode_Exponent
        };
        
        
        *outStrings = CFArrayCreate(NULL, (const void **) strings1, (sizeof (strings1) / sizeof(strings1 [0])), NULL);
        return noErr;
    }
    
    // for Unit 2s drop down menu
    if ((inScope == kAudioUnitScope_Global) && (inParameterID == kParam_Mode2)) {
        if (outStrings == NULL) return noErr;
        
        CFStringRef strings2 [] = {
            kMenuItem_Mode_Sine,
            kMenuItem_Mode_ParabolicSoft,
            kMenuItem_Mode_ParabolicHard,
            kMenuItem_Mode_AsymmetricSine,
            kMenuItem_Mode_Exponent
        };
        
        *outStrings = CFArrayCreate(NULL, (const void **) strings2, (sizeof(strings2) / sizeof(strings2 [0])), NULL);
        return noErr;
    }
    
    // for signal flow drop down
    if ((inScope == kAudioUnitScope_Global) && (inParameterID == kParam_SignalFlow)) {
        if (outStrings == NULL) return noErr;
        
        CFStringRef strings3 [] = {
            kMenuItem_SignalFlow_Serial,
            kMenuItem_SignalFlow_Parallel,
            kMenuItem_SignalFlow_Stereo
        };
        
        *outStrings = CFArrayCreate(NULL, (const void **) strings3, (sizeof(strings3) / sizeof(strings3 [0])), NULL);
        return noErr;
    }
    
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Carve::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus			Carve::GetParameterInfo(AudioUnitScope		inScope,
                                            AudioUnitParameterID	inParameterID,
                                            AudioUnitParameterInfo	&outParameterInfo )
{
    OSStatus result = noErr;
    
    outParameterInfo.flags = 	kAudioUnitParameterFlag_IsWritable
    |		kAudioUnitParameterFlag_IsReadable;
    
    if (inScope == kAudioUnitScope_Global) {
        switch(inParameterID)
        {
            case kParam_Mode1:
                AUBase::FillInParameterName(outParameterInfo, kParamName_Mode, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue = kMode_Sine;
                outParameterInfo.maxValue = kMode_Exponent;
                outParameterInfo.defaultValue = kDefVal_Mode;
                break;
            
            case kParam_PreGain1:
                AUBase::FillInParameterName(outParameterInfo, kParamName_PreGain, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_LinearGain;
                outParameterInfo.minValue = kMinVal_PreGain;
                outParameterInfo.maxValue = kMaxVal_PreGain;
                outParameterInfo.defaultValue = kDefVal_PreGain;
                break;
                
            case kParam_PostGain1:
                AUBase::FillInParameterName (outParameterInfo, kParamName_PostGain, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_LinearGain;
                outParameterInfo.minValue = kMinVal_PostGain;
                outParameterInfo.maxValue = kMaxVal_PostGain;
                outParameterInfo.defaultValue = kDefVal_PostGain;
                break;
            
            case kParam_Misc1:
                AUBase::FillInParameterName (outParameterInfo, kParamName_Misc, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_LinearGain;
                outParameterInfo.minValue = kMinVal_Misc;
                outParameterInfo.maxValue = kMaxVal_Misc;
                outParameterInfo.defaultValue = kDefVal_Misc;
                break;
            
            case kParam_Mode2:
                AUBase::FillInParameterName(outParameterInfo, kParamName_Mode, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue = kMode_Sine;
                outParameterInfo.maxValue = kMode_Exponent;
                outParameterInfo.defaultValue = kDefVal_Mode;
                break;
            
            case kParam_PreGain2:
                AUBase::FillInParameterName(outParameterInfo, kParamName_PreGain, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_LinearGain;
                outParameterInfo.minValue = kMinVal_PreGain;
                outParameterInfo.maxValue = kMaxVal_PreGain;
                outParameterInfo.defaultValue = kDefVal_PreGain;
                break;
            
            case kParam_PostGain2:
                AUBase::FillInParameterName (outParameterInfo, kParamName_PostGain, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_LinearGain;
                outParameterInfo.minValue = kMinVal_PostGain;
                outParameterInfo.maxValue = kMaxVal_PostGain;
                outParameterInfo.defaultValue = kDefVal_PostGain;
                break;
            
            case kParam_Misc2:
                AUBase::FillInParameterName (outParameterInfo, kParamName_Misc, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_LinearGain;
                outParameterInfo.minValue = kMinVal_Misc;
                outParameterInfo.maxValue = kMaxVal_Misc;
                outParameterInfo.defaultValue = kDefVal_Misc;
                break;
            
            case kParam_DryLevel:
                AUBase::FillInParameterName (outParameterInfo, kParamName_DryLevel, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_LinearGain;
                outParameterInfo.minValue = kMinVal_DryLevel;
                outParameterInfo.maxValue = kMaxVal_DryLevel;
                outParameterInfo.defaultValue = kDefVal_DryLevel;
                break;
            
            case kParam_MasterVol:
                AUBase::FillInParameterName (outParameterInfo, kParamName_MasterVol, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_LinearGain;
                outParameterInfo.minValue = kMinVal_MasterVol;
                outParameterInfo.maxValue = kMaxVal_MasterVol;
                outParameterInfo.defaultValue = kDefVal_MasterVol;
                break;
                
            case kParam_SignalFlow:
                AUBase::FillInParameterName(outParameterInfo, kParamName_SignalFlow, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue = kSignalFlow_Serial;
                outParameterInfo.maxValue = kSignalFlow_Stereo;
                outParameterInfo.defaultValue = kDefVal_SignalFlow;
                break;
            
            
            default:
                result = kAudioUnitErr_InvalidParameter;
                break;
        }
    } else {
        result = kAudioUnitErr_InvalidParameter;
    }
    
    
    
    return result;
}

#pragma mark ____Properties

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Carve::GetPropertyInfo
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus			Carve::GetPropertyInfo (	AudioUnitPropertyID				inID,
                                              AudioUnitScope					inScope,
                                              AudioUnitElement				inElement,
                                              UInt32 &						outDataSize,
                                              Boolean &						outWritable)
{
    if (inScope == kAudioUnitScope_Global)
    {
        switch (inID)
        {
            case kAudioUnitProperty_CocoaUI:
                outWritable = false;
                outDataSize = sizeof (AudioUnitCocoaViewInfo);
                return noErr;
                
        }
    }
    
    return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Carve::GetProperty
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus			Carve::GetProperty (	AudioUnitPropertyID 		inID,
                                          AudioUnitScope 				inScope,
                                          AudioUnitElement			inElement,
                                          void *						outData)
{
    if (inScope == kAudioUnitScope_Global)
    {
        switch (inID)
        {
                // This property allows the host application to find the UI associated with this
                // AudioUnit
                //
            case kAudioUnitProperty_CocoaUI:
            {
                // Look for a resource in the main bundle by name and type.
                CFBundleRef bundle = CFBundleGetBundleWithIdentifier( CFSTR("com.whiteelephantaudio.Carve") );
                
                if (bundle == NULL) return fnfErr;
                
                CFURLRef bundleURL = CFBundleCopyResourceURL( bundle,
                                                             CFSTR("CarveCocoaViewFactory"),	// this is the name of the cocoa bundle as specified in the CocoaViewFactory.plist
                                                             CFSTR("bundle"),			// this is the extension of the cocoa bundle
                                                             NULL);
                
                if (bundleURL == NULL) return fnfErr;
                
                CFStringRef className = CFSTR("CarveCocoaViewFactory");	// name of the main class that implements the AUCocoaUIBase protocol
                AudioUnitCocoaViewInfo cocoaInfo = { bundleURL, { className } };
                *((AudioUnitCocoaViewInfo *)outData) = cocoaInfo;
                
                return noErr;
            }
                
        }
    }
    
    // if we've gotten this far, handles the standard properties
    return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Carve::GetPresets
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/*OSStatus			Carve::GetPresets (		CFArrayRef * 		outData) const
 {
 // this is used to determine if presets are supported
 // which in this unit they are so we implement this method!
 if (outData == NULL) return noErr;
 
 CFMutableArrayRef theArray = CFArrayCreateMutable (NULL, kNumberPresets, NULL);
 for (int i = 0; i < kNumberPresets; ++i) {
 CFArrayAppendValue (theArray, &kPresets[i]);
 }
 
 *outData = (CFArrayRef)theArray;	// client is responsible for releasing the array
 return noErr;
 }*/

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	CarveKernel::CarveKernel()
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CarveKernel::CarveKernel(AUEffectBase *inAudioUnit ) : AUKernelBase(inAudioUnit),

DSPUnit1(kMinVal_PreGain, kMaxVal_PreGain, kMinVal_PostGain, kMaxVal_PostGain, kMinVal_Misc, kMaxVal_Misc, kMode_Sine, kMode_Exponent),
DSPUnit2(kMinVal_PreGain, kMaxVal_PreGain, kMinVal_PostGain, kMaxVal_PostGain, kMinVal_Misc, kMaxVal_Misc, kMode_Sine, kMode_Exponent)
{
    Reset();
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	CarveKernel::~CarveKernel()
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CarveKernel::~CarveKernel()
{
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Carve::CarveKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		CarveKernel::Reset()
{
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Carve::CarveKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void CarveKernel::Process(	const Float32 	*inSourceP,
                          Float32		 	*inDestP,
                          UInt32 			inSamplesToProcess,
                          UInt32			inNumChannels, // for version 2 AudioUnits inNumChannels is always 1
                          bool			&ioSilence )
{
    if (!ioSilence) {
        const Float32 *sourceP = inSourceP;
        
        Float32 *destP = inDestP,
        inputSample,
        outputSample,
        
        dryLevel,
        masterVol,
        signalFlow;
        
        DSPUnit1.setMode(GetParameter(kParam_Mode1));
        DSPUnit1.setPreGain(GetParameter(kParam_PreGain1));
        DSPUnit1.setPostGain(GetParameter(kParam_PostGain1));
        DSPUnit1.setMisc(GetParameter(kParam_Misc1));
        
        DSPUnit2.setMode(GetParameter(kParam_Mode2));
        DSPUnit2.setPreGain(GetParameter(kParam_PreGain2));
        DSPUnit2.setPostGain(GetParameter(kParam_PostGain2));
        DSPUnit2.setMisc(GetParameter(kParam_Misc2));
        
        dryLevel = GetParameter(kParam_DryLevel);
        masterVol = GetParameter(kParam_MasterVol);
        signalFlow = GetParameter(kParam_SignalFlow);
        
        // TODO: bounds checks
        
        for (int iii = inSamplesToProcess; iii > 0; --iii) {
            inputSample = *sourceP;
            
            // process the signal differently depending on signalFlow
            if (signalFlow == 0) {
                outputSample = (DSPUnit2.process(DSPUnit1.process(inputSample)) + inputSample * dryLevel) * masterVol;
            } else if (signalFlow == 1) {
                outputSample = (DSPUnit1.process(inputSample) + DSPUnit2.process(inputSample) + inputSample * dryLevel) * masterVol;
            } else {
                // TODO: change this to do stereo processing
                outputSample = (DSPUnit1.process(inputSample) + DSPUnit2.process(inputSample) + inputSample * dryLevel) * masterVol;
            }
            
            *destP = outputSample;
            
            sourceP += 1;
            destP += 1;
        }
        
    }
}