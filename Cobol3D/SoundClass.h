#ifndef __SOUNDCLASS_H__
#define __SOUNDCLASS_H__

#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")
 
 
#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <stdio.h>

namespace Cobol {
	class SoundClass {
	
		struct WaveHeaderType
		{
			char chunkId[4];
			unsigned long chunkSize;
			char format[4];
			char subChunkId[4];
			unsigned long subChunkSize;
			unsigned short audioFormat;
			unsigned short numChannels;
			unsigned long sampleRate;
			unsigned long bytesPerSecond;
			unsigned short blockAlign;
			unsigned short bitsPerSample;
			char dataChunkId[4];
			unsigned long dataSize;
		};
	public:
		SoundClass();
		~SoundClass();

		bool Init(HWND hwnd);
		void Shut();

		
	private:
		bool InitDirectSound(HWND hwnd);
		void ShutDirectSound();
		bool LoadWaveFile(const char * filename, IDirectSoundBuffer8**);
		void DeleteWaveFile(IDirectSoundBuffer8**);
		
		bool PlayWaveFile();
		SoundClass(const SoundClass&);
		IDirectSound8* mDirectSound;
		IDirectSoundBuffer* mPrimaryBuffer;
		IDirectSoundBuffer8* mSecondaryBuffer1;

	};
};

#endif