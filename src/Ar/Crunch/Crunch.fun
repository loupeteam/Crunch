
FUNCTION FFT : BOOL (*Run Fast Fourier Transform on Data*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		pData : UDINT; (*Pointer to LREAL[Length] data set*)
		DataLength : UINT; (*Length of data (Must be power of 2 and < 2^15) *)
		SampleFrequency : LREAL; (*Sample Frequency of the input data*)
		pBinFrequency : UDINT; (*Pointer to LREAL[Length/2]  Frequency for each Bin*)
		pAmplitude : UDINT; (*Pointer to LREAL[Length/2]  Amplitude of frequencies*)
		pPhase : UDINT; (*Pointer to LREAL[Length/2]  Phase shift of frequencies*)
	END_VAR
END_FUNCTION
