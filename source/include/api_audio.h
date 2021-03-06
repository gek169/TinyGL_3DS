/*
 * 
Licensed under CC0, public domain software rendering toolkit.
 * 
 * */

/*
HOW TO BUILD THINGS USING THIS LIBRARY
You gotta use a makefile

Make sure to #include "api.h" into your code. You can only include it in one file.

If you aren't using mixer, I recommend disabling USE_MIXER at the top, and then you don't need to link to it.


*/
#define USE_MIXER
#define USE_MP3
//#ifdef __TINYC__
#define STBI_NO_SIMD
//#define SDL_DISABLE_IMMINTRIN_H
//#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>

//NOTE: you might need to change these depending on your project structure.

#ifdef CHAD_API_IMPL
#define CHAD_MATH_IMPL
#endif //
#include "3dMath.h"
//#include "../../include/fixedmath.h"
#ifndef c_chads_api
#define c_chads_api
typedef unsigned char uchar;

extern uint R_;
extern uint G_;
extern uint B_;
extern uint A_;


#ifdef USE_MIXER
#include<SDL/SDL_mixer.h>
void ainit();
void acleanup();

typedef Mix_Chunk samp;
typedef Mix_Music track;
samp* lwav(const char* t);
track* lmus(const char* t);
samp* qlwav(Uint8* w);
int aplay(samp* samp, int loops);
void aPos(int chan, int angle, unsigned char dist);
int mplay(track* mus,int loops, int ms);
#ifdef CHAD_API_IMPL
void ainit(){
	if (SDL_Init(SDL_INIT_AUDIO)!=0) //We only use SDL for mixer...
	{	
		fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
		exit(0);
	}
	Mix_Init(MIX_INIT_OGG | MIX_INIT_MP3);
	if(-1 == Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024)) {printf("\nAudio can't init :(");exit(2);}
}
void acleanup(){
	Mix_CloseAudio();	
	Mix_Quit(); 
	SDL_Quit();
}

samp* lwav(const char* t){return Mix_LoadWAV(t);}
track* lmus(const char* t){return Mix_LoadMUS(t);}
samp* qlwav(Uint8* w){return Mix_QuickLoad_WAV(w);}
int aplay(samp* samp, int loops){return Mix_PlayChannel(-1, samp, loops);}
void aPos(int chan, int angle, unsigned char dist){Mix_SetPosition(chan,angle,dist);}
int mplay(track* mus,int loops, int ms){return Mix_FadeInMusic(mus,loops,ms);}
//end of chad api impl
#endif
//end of USE_MIXER
#endif



#define MAX(x,y) (x>y?x:y)
#define MIN(x,y) (x<y?x:y)
#define CHAD_API_NEAR 0.0




