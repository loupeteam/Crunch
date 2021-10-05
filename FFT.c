
#include <bur/plctypes.h>
#ifdef __cplusplus
	extern "C"
	{
#endif
#include "Crunch.h"
#include "fft4g.h"
#ifdef __cplusplus
	};
#endif

#include <string.h>
#include <math.h>
#ifndef NMAX
#define NMAXSQRT 64
#define MaxNumLengths 15
#endif

static unsigned long pIP[MaxNumLengths];
static unsigned long pW[MaxNumLengths];

void getCachedData(unsigned short index, unsigned long _pIP, unsigned long _pW){
	if(pIP[index]==0){		
		UDINT dataSize = sizeof(double)*pow(2,index) * 5 / 4;
		TMP_alloc(sizeof(int)*(NMAXSQRT + 2)	,&pIP[index]);
		TMP_alloc(dataSize ,&pW[index]);
		*((int*)(pIP[index]))=0;		
	}

	*(int**)_pIP =pIP[index];
	*(double**)_pW =pW[index];
}

double FFT_BinFreq(double SampleFreq, unsigned short SampleLength, unsigned short bin){
	if(SampleLength==0){
		return 0;	
	}
	return (SampleFreq/2)*bin/(SampleLength/2);
}

plcbit FFT(unsigned long Data, unsigned short Length, double SampleFrequency, unsigned long Frequencies, unsigned long Amplitude, unsigned long Phase)
{
	Length = Length>>2<<2;
	int index = log2(Length);

	if(Data==0) return 1;
	if(index > 15) return 1;
	
	int *ip;//[NMAXSQRT + 2];
	double *w;//[Length * 5 / 4];	
	double _Data[Length];	

	double *_Amplitude = (double *)Amplitude;
	double *_Phase = (double *)Phase;
	double *_Frequencies = (double *)Frequencies;
	if(_Amplitude==0){
		_Amplitude = Data;
		_Phase = &((double*)Data)[Length/2];
	}

	//Get inited table
	getCachedData(index,&ip,&w);

	//Copy data to local array so input array isn't destroyed
	memcpy(_Data,(void*)Data,sizeof(double)*Length);

	//Run real discrete fourier tranform
	rdft(Length, 1, _Data, ip, w);

	//Interpret the data
	int i;
	for(i=1;i<Length/2-1;i++){
		_Amplitude[i-1] =sqrt( _Data[i*2]*_Data[i*2] + _Data[i*2+1]*_Data[i*2+1] )/(Length/2);//Calculate the amplitude from Real and Imaginary parts
		if(_Phase!=0){_Phase[i-1]= atan2(_Data[i*2+1],_Data[i*2])*180/M_PI;}//Output the phase if it is given
		if(_Frequencies!=0){_Frequencies[i-1] =FFT_BinFreq(SampleFrequency,Length,i);}//Output the phase if it is given
	}
}
