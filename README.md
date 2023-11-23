# Emotional States Artifacts

Math Library for determining emotional state.
For calculations it is necessary to use an EEG signal.

## Description
The algorithm processes the data by a sliding window of a given length with a given frequency. If artifacts are detected on one of the bipolar channels, the artifacts on the second bipolar channel are checked, and if there are no artifacts, they are switched to that channel; in case of artifacts on both channels, the spectral values and values of mental levels are filled with previous actual values, while the counter of the number of successive artifact windows increases.

## Artifacts
When the maximum number of consecutive artifact windows is reached, `MathLibIsArtifactedSequence()` returns true, which allows you to give the user information about the need to check the position of the device. If there is no need to give notification of momentary artifacts, you can use this function as the primary for artifact notifications. Otherwise, use `MathLibIsBothSidesArtifacted()` to check for momentary artifacts, returning true for artifacts on both bipolar channels for the current window.

## Usage

### Emotional states
The estimate of emotional states (mental levels - relaxation and concentration) is available in two variants:
1. immediate assessment through alpha and beta wave intensity (and theta in the case of independent assessment). 
2. relative to the baseline calibration values of alpha and beta wave intensity

In both cases, the current intensity of the waves is defined as the average for the last N windows.

The algorithm starts processing the data after the first N seconds after connecting the device and when the minimum number of points for the spectrum calculation is accumulated.
When reading spectral and mental values an array of appropriate structures (`SpectralDataPercents` and `MindData`) of length is returned, which is determined by the number of new recorded points, signal frequency and analysis frequency. 

In this version the filters are built-in and clearly defined: 
BandStop_45_55, BandStop_55_65, BandStop_62, HighPass_10, LowPass_30
### Calibration

According to the results of calibration, the average base value of alpha and beta waves expression is determined in percent, which are further used to calculate the relative mental levels.

### Library mode
The library can operate in two modes - bipolar and multichannel. In bipolar mode only two channels are processed - left and right bipolar. In multichannel mode you can process for any number of channels using the same algorithms.

## Parameters
### Main parameters description
Structure `MathLibSettings` with fields:
1. sampling_rate - raw signal sampling frequency, Hz, integer value
2. process_win_freq - frequency of spectrum analysis and emotional levels, Hz, integer value
3. fft_window - spectrum calculation window length, integer value
4. n_first_sec_skipped - skipping the first seconds after connecting to the device, integer value
5. bipolar_mode - enabled bipolar mode, boolean value
6. channels_number - count channels for multy-channel library mode, integer value
7. channel_for_analysis - in case of multichannel mode: channel by default for computing spectral values and emotional levels, integer value

`channels_number` and `channel_for_analysis` are not used explicitly for bipolar mode, you can leave the default ones.

Separate parameters:
1. MentalEstimationMode - type of evaluation of instant mental levels - disabled by default, boolean value
2. SpectNormalizationByBandsWidth - spectrum normalization by bandwidth - disabled by default, boolean value

### Artifact detection parameters description
Structure `ArtifactDetectSetting` with fields:
1. art_bord - boundary for the long amplitude artifact, mcV, integer value
2. allowed_percent_artpoints - percent of allowed artifact points in the window, integer value
3. raw_betap_limit - boundary for spectral artifact (beta power), detection of artifacts on the spectrum occurs by checking the excess of the absolute value of the raw beta wave power, integer value
4. total_pow_border - boundary for spectral artifact (in case of assessment by total power) and for channels signal quality estimation, integer value
5. global_artwin_sec - number of seconds for an artifact sequence, the maximum number of consecutive artifact windows (on both channels) before issuing a prolonged artifact notification / device position check, integer value  
6. spect_art_by_totalp - assessment of spectral artifacts by total power, boolean value
7. hanning_win_spectrum - setting the smoothing of the spectrum calculation by Hamming, boolean value
8. hamming_win_spectrum - setting the smoothing of the spectrum calculation by Henning, boolean value
9. num_wins_for_quality_avg - number of windows for estimation of signals quality, by default = 100, which, for example, with process_win_freq=25Hz, will be equal to 4 seconds, integer value

Structure `ShortArtifactDetectSetting` with fields:
1. ampl_art_detect_win_size - the length of the sliding window segments for the detection of short-term amplitude artifacts, ms, integer value
2. ampl_art_zerod_area - signal replacement area of the previous non-artefact to the left and right of the extremum point, ms, integer value
3. ampl_art_extremum_border - boundary for the extremum considered to be artifactual, mcV, integer value

Structure `MentalAndSpectralSetting` with fields:
1. n_sec_for_instant_estimation - the number of seconds to calculate the values of mental levels, integer value
2. n_sec_for_averaging - spectrum averaging, integer value

Separate setting is the number of windows after the artifact with the previous actual value - to smooth the switching process after artifacts (`SkipWinsAfterArtifact`).

## Initialization
### Main parameters

##### C++
```cpp
MathLibSetting mathLibSetting;
mathLibSetting.sampling_rate = 250;
mathLibSetting.process_win_freq = 25;
mathLibSetting.n_first_sec_skipped = 4;
mathLibSetting.fft_window = 1000;
mathLibSetting.bipolar_mode = true;
mathLibSetting.squared_spectrum = false;
mathLibSetting.channels_number = 4;
mathLibSetting.channel_for_analysis = 3;

ArtifactDetectSetting artifactDetectSetting;
artifactDetectSetting.art_bord = 70;
artifactDetectSetting.allowed_percent_artpoints = 50;
artifactDetectSetting.total_pow_border = 3 * 1e7;
artifactDetectSetting.raw_betap_limit = 800000;
artifactDetectSetting.spect_art_by_totalp = false;
artifactDetectSetting.global_artwin_sec = 4;
artifactDetectSetting.num_wins_for_quality_avg = 125;
artifactDetectSetting.hanning_win_spectrum = true;
artifactDetectSetting.hamming_win_spectrum = false;

ShortArtifactDetectSetting shortArtifactDetectSetting;
shortArtifactDetectSetting.ampl_art_detect_win_size = 200;
shortArtifactDetectSetting.ampl_art_zerod_area = 200;
shortArtifactDetectSetting.ampl_art_extremum_border = 25;

MentalAndSpectralSetting mentalAndSpectralSetting;
mentalAndSpectralSetting.n_sec_for_averaging = 2;
mentalAndSpectralSetting.n_sec_for_instant_estimation = 2;

OpStatus opSt;
MathLib* tMathPtr = createMathLib(mathLibSetting, artifactDetectSetting, shortArtifactDetectSetting, mentalAndSpectralSetting, &opSt);
```

### Optional parameters

##### C++
```cpp
OpStatus opSt;

// setting calibration length
int calibration_length = 8;
MathLibSetCallibrationLength(tMathPtr, calibration_length, &opSt);

// type of evaluation of instant mental levels
bool independent_mental_levels = false;
MathLibSetMentalEstimationMode(tMathPtr, independent_mental_levels, &opSt);

// number of windows after the artifact with the previous actual value
int nwins_skip_after_artifact = 10;
MathLibSetSkipWinsAfterArtifact(tMathPtr, nwins_skip_after_artifact, &opSt);

// calculation of mental levels relative to calibration values
MathLibSetZeroSpectWaves(tMathPtr, true, 0, 1, 1, 1, 0, &opSt);

// spectrum normalization by bandwidth
MathLibSetSpectNormalizationByBandsWidth(tMathPtr, true);
```

## Types
#### RawChannels
Structure contains left and right bipolar values to bipolar library mode with fields:
1. LeftBipolar - left bipolar value, double value
2. RightBipolar - right bipolar value, double value

#### RawChannelsArray
Structure contains array of values of channels with field:
1. channels - double array

#### MindData
Mental levels. Struct with fields:
1. Rel_Attention - relative attention value
2. Rel_Relaxation - relative relaxation value
3. Inst_Attention - instantiate attention value
4. Inst_Relaxation - instantiate relaxation value

#### SpectralDataPercents
Relative spectral values. Struct with double fields:
1. Delta
2. Theta
3. Alpha
4. Beta
5. Gamma

#### SideType
Side of current artufact. Enum with values:
1. LEFT
2. RIGHT
3. NONE

## Usage
1. If you need calibration start calibration right after library init:
##### C++
```cpp
OpStatus opSt;
MathLibStartCalibration(tMathPtr, &opSt);
``` 

2. Adding and process data
In bipolar mode:
##### C++
```cpp
RawChannels* samples = new RawChannels[SAMPLES_COUNT];
...
MathLibPushData(tMathPtr, samples, SAMPLES_COUNT);	
MathLibProcessDataArr(tMathPtr);
``` 

In multy-channel mode:

##### C++
```cpp
RawChannelsArray* samples = new RawChannelsArray[SAMPLES_COUNT];
...
MathLibPushDataArr(tMathPtr, samples, SAMPLES_COUNT);	
MathLibProcessDataArr(tMathPtr);
``` 
2. Then check calibration status if you need to calibrate values:
##### C++
```cpp
OpStatus os;
bool calibrationFinished = false;
MathLibCalibrationFinished(tMathPtr, &calibrationFinished, &os);
// and calibration progress
int calibrationProgress = 0;
MathLibGetCallibrationPercents(tMathPtr, &calibrationProgress, &os);
``` 
3. If calibration finished (or you don't need to calibrate) read output values:
##### C++
```cpp
OpStatus opSt;

int size = 0;

// Reading mental levels in percent
MathLibReadMentalDataArrSize(tMathPtr, &size, &opSt);
MindData* mental_data = new MindData[size];
MathLibReadMentalDataArr(tMathPtr, mental_data, &size, &opSt);

// Reading relative spectral values in percent:
MathLibReadSpectralDataPercentsArrSize(tMathPtr, &size, &opSt);
SpectralDataPercents* sp_data = new SpectralDataPercents[size];
MathLibReadSpectralDataPercentsArr(tMathPtr, sp_data, &size, &opSt);
``` 
4. Check artifacts
4.1. During calibration
##### C++
```cpp
if(MathLibIsBothSidesArtifacted(tMathPtr)){
    // signal corruption
}
``` 
4.2. After (without) calibration
##### C++
```cpp
if(MathLibIsArtifactedSequence(tMathPtr)){
    // signal corruption
}
``` 
## Finishing work with the library
##### C++
```cpp
freeMathLib(tMathPtr);
```	