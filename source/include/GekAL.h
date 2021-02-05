#ifndef GEKAL
#define GEKAL


#include <AL/al.h>
#include <AL/alc.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "3dMath.h"

const char* ErrorCheck(ALenum error) {
	if (error == AL_INVALID_NAME) {
		return "\nInvalid name";
	} else if (error == AL_INVALID_ENUM) {
		return "\nInvalid enum ";
	} else if (error == AL_INVALID_VALUE) {
		return "\nInvalid value ";
	} else if (error == AL_INVALID_OPERATION) {
		return "\nInvalid operation ";
	} else if (error == AL_OUT_OF_MEMORY) {
		return "\nOut of memory. Wowza.";
	}
	return "\nDon't know ";
}

// START GekAL.h
int isBigEndian() {
	int a = 1;
	return !((char*)&a)[0];
}
typedef struct {
	ALuint sounds[64];
	ALuint streams[3];
} sourcemanager;
void initsourcemanager(sourcemanager* s){
	for(uint i = 0; i < 64; i++)
		s->sounds[i] = 0;
	for(uint i = 0; i < 3; i++)
		s->streams[i] = 0;
	alGenSources(64, s->sounds);
	alGenSources(3, s->streams);
	ALenum error = alGetError();
	if (error != AL_NO_ERROR) {
		puts("\nOPENAL ERROR!");
		puts(ErrorCheck(error));
	}
}
void freesourcemanager(sourcemanager* s){
	alDeleteSources(64, s->sounds);
	alDeleteSources(3, s->streams);
}


ALuint playSound(sourcemanager* sm,
	ALuint soundbuffer, vec3 loc, vec3 vel, 
	ALuint source, float gain, float pitch, 
	float mingain, float maxgain, float rolloff_factor, 
	float max_distance, bool looping) 
{
	int state;
	bool specifiedSource = alIsSource(source);
	if (!specifiedSource) // Means we gotta find one
		for (int i = 0; i < 64; i++){
			ALuint s = sm->sources[i];
			if (alIsSource(s)) // Dont use an invalid one
			{
				alGetSourcei(s, AL_SOURCE_STATE, &state);
				if (state == AL_INITIAL || state == AL_STOPPED) {
					source = s;
					break;
				}
			}
		}
	if (alIsBuffer(soundbuffer) && alIsSource(source))
		alSourcei(source, AL_BUFFER, soundbuffer);
	if (alIsSource(source)) {
		alSource3f(source, AL_POSITION, loc.d[0], loc.d[1], loc.d[2]);
		alSource3f(source, AL_VELOCITY, vel.d[0], vel.d[1], vel.d[2]);
		alSourcef(source, AL_GAIN, gain);
		alSourcef(source, AL_PITCH, pitch);
		alSourcef(source, AL_MIN_GAIN, mingain);
		alSourcef(source, AL_MAX_GAIN, maxgain);
		alSourcef(source, AL_ROLLOFF_FACTOR, rolloff_factor);
		alSourcef(source, AL_MAX_DISTANCE, max_distance);
		alSourcei(source, AL_LOOPING, (looping) ? AL_TRUE : AL_FALSE);
	}
	if (alIsBuffer(soundbuffer) && alIsSource(source))
		alSourcePlay(source);
	return source;
}
ALuint QueueAndPlayStream(
sourcemanager* sm,
ALuint soundbuffer, 
vec3 loc, vec3 vel, ALuint source, float gain, 
float pitch, float mingain,
float maxgain, float rolloff_factor,
float max_distance, bool looping) {
	int state;
	bool specifiedSource = alIsSource(source);
	if (!specifiedSource) // Means we gotta find one
		for (int i = 0; i < 3; i++){
			ALuint s = sm->streams[i];
			if (alIsSource(s)) // Dont use an invalid one
			{
				alGetSourcei(s, AL_SOURCE_STATE, &state);
				if (state == AL_INITIAL || state == AL_STOPPED) {
					source = s;
					break;
				}
			}
		}
	if (alIsBuffer(soundbuffer) && alIsSource(source))
		alSourceQueueBuffers(source, 1, &soundbuffer);
	if (alIsSource(source)) {
		alSource3f(source, AL_POSITION, loc.d[0], loc.d[1], loc.d[2]);
		alSource3f(source, AL_VELOCITY, vel.d[0], vel.d[1], vel.d[2]);
		alSourcef(source, AL_GAIN, gain);
		alSourcef(source, AL_PITCH, pitch);
		alSourcef(source, AL_MIN_GAIN, mingain);
		alSourcef(source, AL_MAX_GAIN, maxgain);
		alSourcef(source, AL_ROLLOFF_FACTOR, rolloff_factor);
		alSourcef(source, AL_MAX_DISTANCE, max_distance);
		alSourcei(source, AL_LOOPING, (looping) ? AL_TRUE : AL_FALSE);
	}
	alGetSourcei(source, AL_SOURCE_STATE, &state);
	if (alIsBuffer(soundbuffer) && alIsSource(source) && !(state == AL_PLAYING))
		alSourcePlay(source);
	return source;
}


void initOpenAL(
	ALCdevice** dev,
	ALCcontext** cont
){
	*dev = alcOpenDevice(NULL);
	if(*dev){
		*cont = alcCreateContext(*dev, 0);
		alcMakeContextCurrent(*cont)
	} else {
		fprintf(stderr,"\n(ERROR)Failed to init AL device.\n");
	}
}
void cleanOpenAL(ALCDevice* dev){
	alcCloseDevice(dev);
}

int convertToInt(char* buffer, int len) {
	int a = 0;
	if (!isBigEndian())
		for (int i = 0; i < len; i++)
			((char*)&a)[i] = buffer[i];
	else
		for (int i = 0; i < len; i++)
			((char*)&a)[3 - i] = buffer[i];
	return a;
}

// WAV File Loader
char* loadWAV(const char* fn, int chan, int samplerate, int bps, int size) {
	char buffer[4];
	int incrementer = 0; // for the crawler
	//std::ifstream in(fn, std::ios::binary);
	FILE* in = fopen(fn,"rb");
	//in.read(buffer, 4);
	fread(buffer,1,4,in);
	if (strncmp(buffer, "RIFF", 4) != 0) {
		//std::cout << "this is not a valid WAVE file" << std::endl;
		puts("\nthis is not a valid WAVE file\n");
		return NULL;
	}
	/*
	in.read(buffer, 4);
	in.read(buffer, 4); // WAVE
	in.read(buffer, 4); // fmt
	in.read(buffer, 4); // 16
	in.read(buffer, 2); // 1
	in.read(buffer, 2);
	*/
	fread(buffer,1,4,in);
	fread(buffer,1,4,in); //WAVE
	fread(buffer,1,4,in); //fmt
	fread(buffer,1,4,in); //16
	fread(buffer,1,2,in); //1
	fread(buffer,1,2,in);
	chan = convertToInt(buffer, 2);
	fread(buffer,1,4,in);
	samplerate = convertToInt(buffer, 4);
	fread(buffer,1,4,in);
	fread(buffer,1,2,in);
	fread(buffer,1,2,in);
	bps = convertToInt(buffer, 2);
	fread(buffer,1,4,in); // data
	if (strncmp(buffer, "data", 4) == 0) {
		fread(buffer,1,4,in);
		size = convertToInt(buffer, 4);
		char* data = malloc(size);// char[size];
		//in.read(data, size);
		fread(data,1,size,in);
		// std::cout << "\n USING DEFAULT LOADING METHOD";
		return data;
	} else {
		char crawler = 'F';
		int foundD = 0;
		int foundDA = 0;
		int foundDAT = 0;
		int foundDATA = 0;
		// crawl to the data
		while (!foundDATA && incrementer < 300) {
			//in.read(&crawler, 1);
			fread(&crawler,1,1,in);
			if (foundDAT && crawler == 'a') {
				foundDATA = 1;
				foundDAT = 0;
				foundDA = 0;
				foundD = 0;
			} else if (foundDAT) {
				foundDATA = 0;
				foundDAT = 0;
				foundDA = 0;
				foundD = 0;
			}

			if (foundDA && crawler == 't') {
				foundDAT = 1;
				foundDA = 0;
				foundD = 0;
			} else if (foundDA) {
				foundDATA = 0;
				foundDAT = 0;
				foundDA = 0;
				foundD = 0;
			}

			if (foundD && crawler == 'a') {
				foundDA = 1;
				foundD = 0;
			} else if (foundD) {
				foundDATA = 0;
				foundDAT = 0;
				foundDA = 0;
				foundD = 0;
			}
			if (crawler == 'd') {
				foundD = 1;
			}
			incrementer++;
		}
		if (foundDATA) {
			// std::cout << "\nFOUND DATA!!!";
			fread(buffer,1,4,in);
			size = convertToInt(buffer, 4);
			// std::cout << "\nSize is " << size;
			char* data = malloc(size);
			//in.read(data, size);
			fread(data,1,size,in);
			return data;
		} else {
			//std::cout << "\nUH OH!";
			puts("\nUH OH!");
			return NULL;
		}
	}
}

inline ALuint loadWAVintoALBuffer(const char* fn) {
	ALuint return_val = 0;

	// OpenAL Loading
	int channel, sampleRate, bps, size;
	ALuint format = 0;
	alGenBuffers(1, &return_val);
	// Loading TONE.WAV
	char* TONE_WAV_DATA = NULL;
	TONE_WAV_DATA = loadWAV(fn, channel, sampleRate, bps, size);
	// std::cout << "\nSAMPLE RATE: " << sampleRate;
	if (channel == 1) {
		if (bps == 8) {
			format = AL_FORMAT_MONO8;
			// std::cout << "\nMONO8 FORMAT";
		} else {
			format = AL_FORMAT_MONO16;
			// std::cout << "\nMONO16 FORMAT";
		}
	} else {
		if (bps == 8) {
			format = AL_FORMAT_STEREO8;
			// std::cout << "\nSTEREO8 FORMAT";
		} else {
			format = AL_FORMAT_STEREO16;
			// std::cout << "\nSTEREO16 FORMAT";
		}
	}
	alBufferData(return_val, format, TONE_WAV_DATA, size, sampleRate);

	if (TONE_WAV_DATA) // Gotta free what we malloc
		free(TONE_WAV_DATA);
	// std::cout << ErrorCheck(algetError());
	return return_val;
}

void syncALListener(vec3 pos, vec3 forw, vec3 up, vec3 vel) {
	ALfloat listenerOri[] = {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f};
	alListener3f(AL_POSITION, pos.d[0], pos.d[1], pos.d[2]);
	alListener3f(AL_VELOCITY, vel.d[0], vel.d[1], vel.d[2]);

	listenerOri[0] = forw.d[0];
	listenerOri[1] = forw.d[1];
	listenerOri[2] = forw.d[2];

	listenerOri[3] = up.d[0];
	listenerOri[4] = up.d[1];
	listenerOri[5] = up.d[2];
	alListenerfv(AL_ORIENTATION, listenerOri);
}
// End GekAL.h

// OpenAL reference link
// https://www.openal.org/documentation/openal-1.1-specification.pdf

#endif
