#ifndef COMMON_API_H
#define COMMON_API_H

#include "lib_export.h"
#include <stdint.h>
#include <stddef.h>


typedef struct _MathLibSetting
{
	uint32_t sampling_rate;
	uint32_t process_win_freq;
	uint32_t fft_window;
	uint32_t n_first_sec_skipped;
	uint8_t bipolar_mode;
	uint8_t squared_spectrum;
	uint32_t channels_number;
	uint32_t channel_for_analysis;
} MathLibSetting;


typedef struct _ArtifactDetectSetting
{
	uint32_t art_bord;
	uint32_t allowed_percent_artpoints;
	uint32_t raw_betap_limit;
	uint32_t total_pow_border;
	uint32_t global_artwin_sec;
	uint8_t spect_art_by_totalp;
	uint8_t hanning_win_spectrum;
	uint8_t hamming_win_spectrum;
	uint32_t num_wins_for_quality_avg;
}ArtifactDetectSetting;

typedef struct _ShortArtifactDetectSetting
{
	int32_t ampl_art_detect_win_size;
	int32_t ampl_art_zerod_area;
	int32_t ampl_art_extremum_border;
}ShortArtifactDetectSetting;

typedef struct _MentalAndSpectralSetting
{
	int32_t n_sec_for_instant_estimation;
	int32_t n_sec_for_averaging;
} MentalAndSpectralSetting;


typedef struct _OutStatus
{
	uint8_t Success;
	uint32_t Error;
	char ErrorMsg[512];
} EMOpStatus;

typedef enum _SideType
{
	LEFT,
	RIGHT,
	NONE
} SideType;

typedef struct _MindData {
	double Rel_Attention;
	double Rel_Relaxation;
	double Inst_Attention;
	double Inst_Relaxation;
} MindData;

typedef struct _SpectralDataPercents {
	double Delta;
	double Theta;
	double Alpha;
	double Beta;
	double Gamma;
} SpectralDataPercents;

typedef struct _RawChannels {
	double left_bipolar;
	double right_bipolar;
} RawChannels;

typedef struct _RawChannelsArray
{
	double* channels;
} RawChannelsArray;

typedef struct _RawSpectVals {
	double alpha;
	double beta;
} RawSpectVals;

typedef struct _MathLib MathLib;

#ifdef __cplusplus
	extern "C"
	{
#endif

extern const ArtifactDetectSetting ArtifactDetectSettingDefault;
extern const MathLibSetting MathLibSettingDefault;
extern const ShortArtifactDetectSetting ShortArtifactDetectSettingDefault;
extern const MentalAndSpectralSetting MentalAndSpectralSettingDefault;


	SDK_SHARED MathLib* createMathLib(MathLibSetting lib_setting, ArtifactDetectSetting art_setting, ShortArtifactDetectSetting short_art_setting, MentalAndSpectralSetting mental_spectral_setting, EMOpStatus* opResult);
	SDK_SHARED uint8_t freeMathLib(MathLib*, EMOpStatus* opResult);

	SDK_SHARED uint8_t MathLibSetMentalEstimationMode(MathLib* mathLibPtr, bool independent, EMOpStatus* opResult);
	SDK_SHARED uint8_t MathLibSetHanningWinSpect(MathLib* mathLibPtr, EMOpStatus* opResult);
	SDK_SHARED uint8_t MathLibSetHammingWinSpect(MathLib* mathLibPtr, EMOpStatus* opResult);
	SDK_SHARED uint8_t MathLibSetCallibrationLength(MathLib* mathLibPtr, int s, EMOpStatus* opResult);
	SDK_SHARED uint8_t MathLibSetSkipWinsAfterArtifact(MathLib* mathLibPtr, int nwins, EMOpStatus* opResult);

	SDK_SHARED uint8_t MathLibPushData(MathLib* mathLibPtr, RawChannels* samples, size_t samplesCount, EMOpStatus* opResult);
	SDK_SHARED uint8_t MathLibPushDataArr(MathLib* mathLibPtr, RawChannelsArray* samples, size_t samplesCount, EMOpStatus* opResult);

	SDK_SHARED uint8_t MathLibProcessWindow(MathLib* mathLibPtr, EMOpStatus* opResult);
	SDK_SHARED uint8_t MathLibProcessData(MathLib* mathLibPtr, SideType side, EMOpStatus* opResult);
	SDK_SHARED uint8_t MathLibProcessDataArr(MathLib* mathLibPtr, EMOpStatus* opResult);
	
	SDK_SHARED uint8_t MathLibSetPrioritySide(MathLib* mathLibPtr, SideType side, EMOpStatus* opResult);
	SDK_SHARED uint8_t MathLibStartCalibration(MathLib* mathLibPtr, EMOpStatus* opResult);
	SDK_SHARED uint8_t MathLibCalibrationFinished(MathLib* mathLibPtr, bool* result, EMOpStatus* opResult);
	
	SDK_SHARED uint8_t MathLibIsArtifactedWin(MathLib* mathLibPtr, SideType side, bool print_info, bool* result, EMOpStatus* opResult);
	SDK_SHARED uint8_t MathLibIsArtifactedSequence(MathLib* mathLibPtr,  bool* result, EMOpStatus* opResult);
	SDK_SHARED uint8_t MathLibIsBothSidesArtifacted(MathLib* mathLibPtr, bool* result, EMOpStatus* opResult);

	SDK_SHARED uint8_t MathLibReadMentalDataArrSize(MathLib* mathLibPtr, int* arr_size, EMOpStatus* opResult);
    SDK_SHARED uint8_t MathLibReadMentalDataArr(MathLib* mathLibPtr, MindData* mindd, int* arr_size, EMOpStatus* opResult);

	SDK_SHARED uint8_t MathLibReadAverageMentalData(MathLib* mathLibPtr, int n_lastwins_toaverage, MindData* minddatavals, EMOpStatus* opResult);

    SDK_SHARED uint8_t MathLibReadSpectralDataPercentsArrSize(MathLib* mathLibPtr, int* arr_size, EMOpStatus* opResult);
	SDK_SHARED uint8_t MathLibReadSpectralDataPercentsArr(MathLib* mathLibPtr, SpectralDataPercents* spectraldata_arr, int* arr_size, EMOpStatus* opResult);
	SDK_SHARED uint8_t MathLibReadRawSpectralVals(MathLib* mathLibPtr, RawSpectVals* raw_spectral_vals, EMOpStatus* opResult);

	SDK_SHARED uint8_t MathLibSetZeroSpectWaves(MathLib* mathLibPtr, bool active, int delta, int theta, int alpha, int beta, int gamma, EMOpStatus* opResult);
	SDK_SHARED uint8_t MathLibSetWeightsForSpectra(MathLib* mathLibPtr, double delta_c, double theta_c, double alpha_c, double beta_c, double gamma_c, EMOpStatus* opResult);
	SDK_SHARED uint8_t MathLibSetSpectNormalizationByBandsWidth(MathLib* mathLibPtr, bool fl, EMOpStatus* opResult);
	SDK_SHARED uint8_t MathLibSetSpectNormalizationByCoeffs(MathLib* mathLibPtr, bool fl, EMOpStatus* opResult);
    SDK_SHARED uint8_t MathLibGetCallibrationPercents(MathLib* mathLibPtr, int* outPercents, EMOpStatus* opResult);


	#ifdef _DEBUG

	SDK_SHARED void MathLibCheckArtifactsOnLeftChannel(MathLib* mathLibPtr, int* type, double* location, bool* start, EMOpStatus* opResult);
	SDK_SHARED void MathLibCheckArtifactsOnRightChannel(MathLib* mathLibPtr, int* type, double* location, bool* start, EMOpStatus* opResult);

	SDK_SHARED double MathLibReadLeftChannel(MathLib* mathLibPtr, EMOpStatus* opResult);
	SDK_SHARED double MathLibReadRightChannel(MathLib* mathLibPtr, EMOpStatus* opResult);

	#endif
	
#ifdef __cplusplus
}
#endif

#endif
