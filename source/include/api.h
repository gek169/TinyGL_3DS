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
//#define USE_MP3
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
#define STB_IMAGE_IMPLEMENTATION
#endif

#include "stb_image.h"
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
	if(-1 == Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024)) {printf("\nAudio can't init :(");exit(2);}
}
void acleanup(){Mix_CloseAudio();}

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
typedef struct{
	uchar* d; //data
	int w,h; //width and height
} sprite;
typedef struct{
	sprite** d;
	uint w, h;
	int xoff, yoff;
	uint sprw, sprh;
} tilemap;
typedef struct{uint defs[256-' '];} palette;
typedef struct{
	float* d;
	size_t npoints; //npoints * 3 is the number of floats for an uncolored model.
} pmod;
typedef int (*shiftfunc)(int);
//extern SDL_Surface* sdlsurf;
//extern SDL_Renderer* ren;
typedef struct{
	int w, h, pitch;
	void* pixels;
} api_surface;
extern api_surface* surf;
//extern SDL_GLContext glcontext;
extern float* dbuff; //depth buffer. used to render sprites and geometry with Z depths.
//extern SDL_Texture* tex;
extern sprite charsprites[256]; //For loading characters
void clean();
void flipVertical();
void initTileMap(tilemap* t, uint w, uint h, uint sprw, uint sprh);
void freeTileMap(tilemap* t);
void init();
#ifdef USE_API_GL
//void cWinGL(const char* name, int x, int y, int w, int h, int resizeable);
//void swapGL();
//void deleteGL();
#endif
void makeCurrent();
void upd();
void ev(void (*f)(SDL_Event* e));
//Allocate a blank sprite, all transparent.
void nullsp(sprite* s);
void initsp(sprite* s, uint w, uint h);
//Initialize a sprite from a given palette and a string representing it. Allows 
void initspfromstr(sprite* s, const char* str, palette* pal);
void scalespr(sprite* s, sprite* d, uint nw, uint nh);
void lspr(sprite* s, const char* filename);
void freesp(sprite* s);

void initFont(const char* filename);
void freeFont();
void resetdbuff(float f);
void line2d(int x0, int y0, 
			int x1, int y1, 
			uchar r, uchar g, uchar b);
typedef void (*APIFS)(int x, int y, float z, uint col, float mix0, float mix1, float mix2);
//these structs and functions are only used internally in api.h
//typedef struct{int x_0, x_1, y_0, y_1;float z_0, z_1;}api_Edge;
//void drawSpansBetweenEdges(api_Edge e0, api_Edge e1,uchar r, uchar g, uchar b);
//void drawSpansBetweenEdgesND(api_Edge e0, api_Edge e1,uchar r, uchar g, uchar b);
void tri2d(int x0, int y0, float z0,
			int x1, int y1, float z1,
			int x2, int y2, float z2,
			uchar r, uchar g, uchar b);
void tri2dFS(int x0, int y0, float z0_,
			int x1, int y1, float z1_,
			int x2, int y2, float z2_,
			uchar r, uchar g, uchar b,
			APIFS fragshader);
void tri2dND(int x0, int y0,
			int x1, int y1, 
			int x2, int y2, 
			uchar r, uchar g, uchar b);

//Renders 3D models using lines.
void linemodel(pmod* mod, uchar r, uchar g, uchar b);
void rensp(sprite* s, int x, int y);
void renspadv(sprite* s, int x, int y, uint flips);
//Used for tinting pure black and white sprites to the correct rgb.
//Used for the text renderer.
void rensptint(sprite* s, int x, int y,uchar r, uchar g, uchar b);
void drawText(int xleft, int ytop, const char* text, uchar r, uchar g, uchar b);
void renspdbuff(sprite* s, int x, int y,float z);
void renback(sprite* s, uint x, uint y);
void blitback(sprite* s, uint x, uint y);
void blitsp(sprite* s, int x, int y);
void renTileMap(tilemap* t);
void hshift(shiftfunc f);
void vshift(shiftfunc f);
void lSPal(palette* p);
void cWin(int screenmode);
void captMouse();
void noCaptMouse();
void setFullscreen(int mode);
void cRend();
void cSurf(int w, int h);
void clear(uchar a);
void uTx();














#ifdef CHAD_API_IMPL
uint R_=0;
uint G_=1;
uint B_=2;
uint A_=3;
api_surface actualSurf = {0,0,0,0};
api_surface* surf = NULL;
float* dbuff = NULL; //depth buffer. used to render sprites and geometry with Z depths.
sprite charsprites[256]; //For loading characters
void initTileMap(tilemap* t, uint w, uint h, uint sprw, uint sprh)
{
	t->d=calloc(w * h, sizeof(sprite*));
	t->w = w; t->h = h;
	t->xoff = 0; t->yoff = 0;
	t->sprw = sprw; t->sprh = sprh;
}
void freeTileMap(tilemap* t){if(t->d)free(t->d);}


void init(){
	if (SDL_Init(SDL_INIT_AUDIO)!=0) //We only use SDL for mixer...
	{	
		fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
		exit(0);
	}
	Mix_Init(MIX_INIT_OGG | MIX_INIT_MP3);
}


//Allocate a blank sprite, all transparent.
void nullsp(sprite* s){
	s->d = NULL; s->w = 0; s->h = 0;
}
void initsp(sprite* s, uint w, uint h){
	s->d = calloc(w * h, 4);s->w = w; s->h = h;
}
//Initialize a sprite from a given palette and a string representing it. Allows 
void initspfromstr(sprite* s, const char* str, palette* pal){
	if(!str)return;
	int w = 0,x=0,y=0, h = 1;uint i = 0;
	for(int cw=0;str[i]!='\0';i++,cw++)
		if(str[i]=='\n'){h++;w=cw>w?cw:w;cw=-1;}
	//printf("\n%u by %u",w,h);
	initsp(s,w,h);
	for(i=0,x=0,y=0;str[i]!='\0';i++){
		uchar v = str[i];
		if(v=='\n'){
			for(;x<w;x++)//fill in the rest of the row with pal->defs[0]
				*((uint*)(s->d+(x+y*s->w)*4)) = pal->defs[0];
			y++;x=0;
		} else {
			((uint*)(s->d+(x+y*w)*4))[0] = pal->defs[v-(uchar)' '];
			x++;
		}
	}
}
void scalespr(sprite* s, sprite* d, uint nw, uint nh){
	if(nw * nh == 0){
		d->d = NULL;d->w = 0; d->h = 0; return;
	}
	d->d = malloc(nw * nh * 4);
	d->w = nw;
	d->h = nh;
	for(uint x = 0; x < nw; x++)
	for(uint y = 0; y < nh; y++){
		uint sx = (float)x/(float)nw * s->w;
		uint sy = (float)y/(float)nh * s->h;
		((uint*)(d->d))[x + y * nw] = 
		((uint*)(s->d))[sx + sy * s->w];
	}
}
void lspr(sprite* s, const char* filename){
	int sw = 0, sh = 0, sc = 0; //sc goes unused.
	uchar* source_data = stbi_load(filename, &sw, &sh, &sc, 4);
	if(!source_data) {
		printf("\nError loading font %s\n",filename);
		return;
	}
	s->d = source_data;
	s->w = sw;
	s->h = sh;
}
void freesp(sprite* s){
	if(!s->d) return;
	free(s->d); s->w = 0; s->h = 0;
	s->d = NULL;
}

void initFont(const char* filename){
	int sw = 0, sh = 0, sc = 0; //sc goes unused.
	int charwidth, charheight;
	for(int i = 0; i < 256; i++)
		{	charsprites[i].d = NULL;
			charsprites[i].w = 0;
			charsprites[i].h = 0;
		}
	uchar* source_data = stbi_load(filename, &sw, &sh, &sc, 4);
	if(!source_data) {
		printf("\nError loading font %s\n",filename);
		return;
	}
	//quickly move the red channel to the green,blue, and alpha channels.
	
	for(int i = 0; i < sw * sh; i++){ //NOTE: if we have to change R_ shit, don't touch this!!!
		uchar r = source_data[i * 4];
		source_data[i * 4] = r?255:0;
		source_data[i * 4 + 1] = r?255:0;
		source_data[i * 4 + 2] = r?255:0;
		source_data[i * 4 + 3] = r?255:0;
	}
	charwidth = sw / 8; charheight = charwidth;
	for(uchar i = '!'; i < '~'+1; i++){
		int xc = i%8;int yc = i/8;
		int xoff = xc * charwidth; int yoff = yc * charheight;
		if(yc * charheight > sh) {
		puts("\nSafety check 1 failed.\n");
		break;} //Impossible!
		uchar* dat = malloc(4 * charwidth * charheight);	if(!dat) break;
		//Copy over the character data into dat. 
		for(int row = 0; row < charheight; row++){
			memcpy(	dat+row*charwidth*4,
					source_data+(xoff + (yoff+row) * sw)*4,
					charwidth*4);
		}
		charsprites[i].d = dat;
		charsprites[i].w = charwidth;
		charsprites[i].h = charheight;
	}
	free(source_data);
}
void freeFont(){
	for(int i = 0; i < 256; i++)
	{	
		freesp(charsprites+i);
	}
}

void resetdbuff(float f){
	for(int i = 0; i < surf->w * surf->h; i++)
		dbuff[i] = f;
}
void line2d(int x0, int y0, 
			int x1, int y1, 
			uchar r, uchar g, uchar b)
{
	uint color; 
	{	
		uchar* dat = (uchar*)&color;
		dat[R_]=r;dat[G_]=g;dat[B_]=b;dat[A_]=255;
	}
	uint* targ = (uint*)surf->pixels;
	int dx = x1 - x0, dy = y1 - y0;
	//float dydx = (float)dy/(float)dx;
	if(dx == 0 && dy == 0)return;//Not a line.
	float Steps;
	if(abs(dx) > abs(dy))
		Steps = abs(dx);
	else
		Steps = abs(dy);
	float xinc = (float)dx / Steps, yinc = (float)dy / Steps;
	float x = x0, y = y0;
	for(int i = 0; i < Steps; i++){
		x += xinc; y += yinc;
		if(x > 0 && x < surf->w &&
			y > 0 && y < surf->h)
		*(targ +(uint)x +(uint)y*surf->w) = color;
	}
}
typedef struct{int x_0, x_1, y_0, y_1;float z_0, z_1;}api_Edge;
typedef struct{int x_0, x_1, y_0, y_1;float z_0, z_1;float mix0_0, mix1_0, mix2_0, mix0_1, mix1_1, mix2_1;}api_Edge_ID;
void drawSpansBetweenEdges(api_Edge e0, api_Edge e1,uchar r, uchar g, uchar b){
	uint col; uchar* dat; dat = (uchar*)&col;
	dat[R_] = r; dat[G_] = g; dat[B_] = b; dat[A_] = 255;
	uint* datu = (uint*)surf->pixels;
	float e0ydiff = (float)(e0.y_1 - e0.y_0);
	if(e0ydiff == 0) return;
	float e1ydiff = (float)(e1.y_1 - e1.y_0);
	if(e1ydiff == 0) return;
	float e0xdiff = (e0.x_1 - e0.x_0);
	float e1xdiff = (e1.x_1 - e1.x_0);
	float factor0 = ((float)(e1.y_0 - e0.y_0)/e0ydiff);
	float factor0init = factor0;
	float factorstep0 = (1.0f / e0ydiff);
	float factor1 = (0.0);
	float factor1init = factor1;
	float factorstep1 = (1.0f / e1ydiff);
	//We also have to handle the Z component.
	//We have to linearly interpolate.
	//For each step (there will be e0.y_1 - e0.y_0 of them)
	//We have to move the Z closer to its endpoint from z_0 to z_1.
	float e0zdiff =(e0.z_1 - e0.z_0);
	float e1zdiff =(e1.z_1 - e1.z_0);
	
	struct{int x_0, x_1;float z_0, z_1;} s;
	for(int y = MAX(e1.y_0,0); y < e1.y_1 && y < surf->h; y++){
			factor0 = factor0init + factorstep0 * (y-e1.y_0);
			factor1 = factor1init + factorstep1 * (y-e1.y_0);
		{
			int xA = e0.x_0 + e0xdiff*factor0;
			int xB = e1.x_0 + e1xdiff*factor1;
			float zA = e0.z_0 + e0zdiff*factor0;
			float zB = e1.z_0 + e1zdiff * factor1;
			if(xA < xB){
				s.x_0 = xA;s.x_1 = xB;
				s.z_0 = zA;s.z_1 = zB;
			} else {
				s.x_0 = xB;s.x_1 = xA;
				s.z_0 = zB;s.z_1 = zA;
			}
			if(s.x_1 - s.x_0 != 0){
				float z = s.z_0; float zincr = (s.z_1 - s.z_0)/(s.x_1 - s.x_0);
				int end = MIN(s.x_1,surf->w);
				int begin = MAX(s.x_0,0);
				for(int j = begin; j < end; j++){
					if(j > 0 && j < surf->w && y > 0 && y < surf->h){
						float zsamp = dbuff[j + y * surf->w];
						if(z < zsamp && z > (CHAD_API_NEAR) )
							{
								datu[j+y*surf->w] = col;
								dbuff[j + y * surf->w] = z;
							}
						
					}
					z+=zincr;
				}
			}
		}
		//factor0 += factorstep0;
		//factor1 += factorstep1;
	}
}

void drawSpansBetweenEdgesFS(api_Edge_ID e0, api_Edge_ID e1,uchar r, uchar g, uchar b, APIFS fragshader){
	uint col; uchar* dat; dat = (uchar*)&col;
	dat[R_] = r; dat[G_] = g; dat[B_] = b; dat[A_] = 255;
	//uint* datu = (uint*)surf->pixels;
	float e0ydiff = (float)(e0.y_1 - e0.y_0);
	if(e0ydiff == 0) return;
	float e1ydiff = (float)(e1.y_1 - e1.y_0);
	if(e1ydiff == 0) return;
	float e0xdiff = (e0.x_1 - e0.x_0);
	float e1xdiff = (e1.x_1 - e1.x_0);
	float factor0 = ((float)(e1.y_0 - e0.y_0)/e0ydiff);
	float factor0init = factor0;
	float factorstep0 = (1.0f / e0ydiff);
	float factor1 = (0.0);
	float factor1init = factor1;
	float factorstep1 = (1.0f / e1ydiff);
	//We also have to handle the Z component.
	//We have to linearly interpolate.
	//For each step (there will be e0.y_1 - e0.y_0 of them)
	//We have to move the Z closer to its endpoint from z_0 to z_1.
	float e0zdiff =(e0.z_1 - e0.z_0);
	float e1zdiff =(e1.z_1 - e1.z_0);
	float e0mix0diff = e0.mix0_1 - e0.mix0_0;
	float e1mix0diff = e1.mix0_1 - e1.mix0_0;

	float e0mix1diff = e0.mix1_1 - e0.mix1_0;
	float e1mix1diff = e1.mix1_1 - e1.mix1_0;

	float e0mix2diff = e0.mix2_1 - e0.mix2_0;
	float e1mix2diff = e1.mix2_1 - e1.mix2_0;
	struct{int x_0, x_1;float z_0, z_1;float mix0_0, mix0_1, mix1_0, mix1_1, mix2_0, mix2_1;} s;
	for(int y = MAX(e1.y_0,0); y < e1.y_1 && y < surf->h; y++){
			factor0 = factor0init + factorstep0 * (y-e1.y_0);
			factor1 = factor1init + factorstep1 * (y-e1.y_0);
		{
			int xA = e0.x_0 + ( (e0xdiff*factor0));
			int xB = e1.x_0 + ( (e1xdiff*factor1));
			float zA = e0.z_0 +  (e0zdiff*factor0);
			float zB = e1.z_0 +  (e1zdiff*factor1);
			float mix0A = e0.mix0_0 +  (e0mix0diff*factor0);
			float mix0B = e1.mix0_0 +  (e1mix0diff*factor1);

			float mix1A = e0.mix1_0 +  (e0mix1diff*factor0);
			float mix1B = e1.mix1_0 +  (e1mix1diff*factor1);

			float mix2A = e0.mix2_0 +  (e0mix2diff*factor0);
			float mix2B = e1.mix2_0 +  (e1mix2diff*factor1);
			if(xA < xB){
				s.x_0 = xA;		s.x_1 = xB;
				s.z_0 = zA;		s.z_1 = zB;
				s.mix0_0 = mix0A;s.mix0_1 = mix0B;
				s.mix1_0 = mix1A;s.mix1_1 = mix1B;
				s.mix2_0 = mix2A;s.mix2_1 = mix2B;
			} else {
				s.x_0 = xB;s.x_1 = xA;
				s.z_0 = zB;s.z_1 = zA;
				s.mix0_0 = mix0B;s.mix0_1 = mix0A;
				s.mix1_0 = mix1B;s.mix1_1 = mix1A;
				s.mix2_0 = mix2B;s.mix2_1 = mix2A;
			}
			if(y > 0 && (s.x_1 - s.x_0 != 0)){
				float z = s.z_0; float zincr = (s.z_1 - s.z_0)/(s.x_1 - s.x_0);
				float 	mix0 = s.mix0_0, mix0incr = (s.mix0_1-s.mix0_0)/(s.x_1 - s.x_0), 
						mix1 = s.mix1_0, mix1incr = (s.mix1_1-s.mix1_0)/(s.x_1 - s.x_0), 
						mix2 = s.mix2_0, mix2incr = (s.mix2_1-s.mix2_0)/(s.x_1 - s.x_0);
				int end = MIN(s.x_1,surf->w);
				int begin = MAX(s.x_0,0);
				for(int j = begin; j < end; j++){
					if(j > 0 && j < surf->w && y > 0 && y < surf->h){
						float zsamp = dbuff[j + y * surf->w];
						if(z < zsamp && z > (CHAD_API_NEAR) )
							{
								//datu[j+y*surf->w] = col;
								fragshader(j,y,z,col,mix0,mix1,mix2);
								//dbuff[j + y * surf->w] = z;
							}
						
					}
					z+=zincr;
					mix0 += mix0incr;
					mix1 += mix1incr;
					mix2 += mix2incr;
				}
			}
		}
		//factor0 += factorstep0;
		//factor1 += factorstep1;
	}
}



void drawSpansBetweenEdgesND(api_Edge e0, api_Edge e1,uchar r, uchar g, uchar b){
	uint col; uchar* dat; dat = (uchar*)&col;
	dat[R_] = r; dat[G_] = g; dat[B_] = b; dat[A_] = 255;
	uint* datu = (uint*)surf->pixels;
	float e0ydiff = (float)(e0.y_1 - e0.y_0);
	if(e0ydiff == 0) return;
	float e1ydiff = (float)(e1.y_1 - e1.y_0);
	if(e1ydiff == 0) return;
	float e0xdiff = (e0.x_1 - e0.x_0);
	float e1xdiff = (e1.x_1 - e1.x_0);
	float factor0 = ((float)(e1.y_0 - e0.y_0)/e0ydiff);
	float factor0init = factor0;
	float factorstep0 = (1.0f / e0ydiff);
	float factor1 = (0.0);
	float factor1init = factor1;
	float factorstep1 = (1.0f / e1ydiff);
		
	//We also have to handle the Z component.
	//We have to linearly interpolate.
	//For each step (there will be e0.y_1 - e0.y_0 of them)
	//We have to move the Z closer to its endpoint from z_0 to z_1.
	struct{int x_0, x_1;} s;
	for(int y = MAX(e1.y_0,0); y < e1.y_1 && y < surf->h; y++){
			factor0 = factor0init + factorstep0 * (y-e1.y_0);
			factor1 = factor1init + factorstep1 * (y-e1.y_0);
		{
			int xA = e0.x_0 + (e0xdiff*factor0);
			int xB = e1.x_0 + (e1xdiff*factor1);
			if(xA < xB){
				s.x_0 = xA;s.x_1 = xB;
			} else {
				s.x_0 = xB;s.x_1 = xA;
			}
			int end = MIN(s.x_1,surf->w);
			int begin = MAX(s.x_0,0);
			for(int j = begin; j < end; j++)
				if(j > 0 && j < surf->w)
					datu[j+y*surf->w] = col;
		}
	}
}
void tri2d(int x0, int y0, float z0,
			int x1, int y1, float z1,
			int x2, int y2, float z2,
			uchar r, uchar g, uchar b){
	api_Edge edges[3];
	//fixed z0 = FLOATTOFIXED(z0_);
	//fixed z1 = FLOATTOFIXED(z1_);
	//fixed z2 = FLOATTOFIXED(z2_);
	/*
	initEdge(x0,y0,z0,x1,y1,z1,edges);
	initEdge(x1,y1,z1,x2,y2,z2,edges+1);
	initEdge(x2,y2,z2,x0,y0,z0,edges+2);
	*/
	if(y0 < y1){
		edges[0].x_0 = x0;
		edges[0].y_0 = y0;
		edges[0].z_0 = z0;
		//edges[0].id_0 = id0;
		edges[0].x_1 = x1;
		edges[0].y_1 = y1;
		edges[0].z_1 = z1;
		//edges[0].id_1 = id1;
	} else {
		edges[0].x_0 = x1;
		edges[0].y_0 = y1;
		edges[0].z_0 = z1;
		//edges[0].id_0 = id1;
		edges[0].x_1 = x0;
		edges[0].y_1 = y0;
		edges[0].z_1 = z0;
		//edges[0].id_1 = id0;
	}
	if(y2 < y1){
		edges[1].x_0 = x2;
		edges[1].y_0 = y2;
		edges[1].z_0 = z2;
		//edges[1].id_0 = id2;
		edges[1].x_1 = x1;
		edges[1].y_1 = y1;
		edges[1].z_1 = z1;
		//edges[1].id_1 = id1;
	} else {
		edges[1].x_0 = x1;
		edges[1].y_0 = y1;
		edges[1].z_0 = z1;
		//edges[1].id_0 = id1;
		edges[1].x_1 = x2;
		edges[1].y_1 = y2;
		edges[1].z_1 = z2;
		//edges[1].id_1 = id2;
	}
	if(y2 < y0){
		edges[2].x_0 = x2;
		edges[2].y_0 = y2;
		edges[2].z_0 = z2;
		//edges[2].id_0 = id2;
		edges[2].x_1 = x0;
		edges[2].y_1 = y0;
		edges[2].z_1 = z0;
		//edges[2].id_0 = id0;
	} else {
		edges[2].x_0 = x0;
		edges[2].y_0 = y0;
		edges[2].z_0 = z0;
		//edges[2].id_0 = id0;
		edges[2].x_1 = x2;
		edges[2].y_1 = y2;
		edges[2].z_1 = z2;
		//edges[2].id_1 = id2;
	}
	int maxLength = 0, longEdge = 0;
	for(int i = 0; i < 3; i++){
		int length = (edges[i].y_1-edges[i].y_0);
		if(length > maxLength){
			maxLength = length;
			longEdge = i;
		}
	}
	int shortEdge1 = (longEdge+1)%3;
	int shortEdge2 = (longEdge+2)%3;
	drawSpansBetweenEdges(edges[longEdge],edges[shortEdge1],r,g,b);
	drawSpansBetweenEdges(edges[longEdge],edges[shortEdge2],r,g,b);
}
void tri2dFS(int x0, int y0, float z0,
			int x1, int y1, float z1,
			int x2, int y2, float z2,
			uchar r, uchar g, uchar b,
			APIFS fragshader){
	api_Edge_ID edges[3];
	//fixed z0 = FLOATTOFIXED(z0_);
	//fixed z1 = FLOATTOFIXED(z1_);
	//fixed z2 = FLOATTOFIXED(z2_);
	const float mix00 = 1;
	const float mix11 = 1;
	const float mix22 = 1;
	if(y0 < y1){
		edges[0].x_0 = x0;
		edges[0].y_0 = y0;
		edges[0].z_0 = z0;
		edges[0].mix0_0=mix00; //assigning point 0 here!
		edges[0].mix1_0=0;
		edges[0].mix2_0=0;
		edges[0].x_1 = x1;
		edges[0].y_1 = y1;
		edges[0].z_1 = z1;
		edges[0].mix0_1=0;
		edges[0].mix1_1=mix11; //assigning point 1 here!
		edges[0].mix2_1=0;
	} else {
		edges[0].x_0 = x1;
		edges[0].y_0 = y1;
		edges[0].z_0 = z1;
		edges[0].mix0_0=0;
		edges[0].mix1_0=mix11;//p1
		edges[0].mix2_0=0;
		edges[0].x_1 = x0;
		edges[0].y_1 = y0;
		edges[0].z_1 = z0;
		edges[0].mix0_1=mix00;//p0
		edges[0].mix1_1=0;
		edges[0].mix2_1=0;
	}
	if(y2 < y1){
		edges[1].x_0 = x2;
		edges[1].y_0 = y2;
		edges[1].z_0 = z2;
		edges[1].mix0_0=0;
		edges[1].mix1_0=0;
		edges[1].mix2_0=mix22;//p2
		edges[1].x_1 = x1;
		edges[1].y_1 = y1;
		edges[1].z_1 = z1;
		edges[1].mix0_1=0;
		edges[1].mix1_1=mix11;//p1
		edges[1].mix2_1=0;
	} else {
		edges[1].x_0 = x1;
		edges[1].y_0 = y1;
		edges[1].z_0 = z1;
		edges[1].mix0_0=0;
		edges[1].mix1_0=mix11;//p1
		edges[1].mix2_0=0;
		edges[1].x_1 = x2;
		edges[1].y_1 = y2;
		edges[1].z_1 = z2;
		edges[1].mix0_1=0;
		edges[1].mix1_1=0;
		edges[1].mix2_1=mix22;//p2
	}
	if(y2 < y0){
		edges[2].x_0 = x2;
		edges[2].y_0 = y2;
		edges[2].z_0 = z2;
		edges[2].mix0_0=0;
		edges[2].mix1_0=0;
		edges[2].mix2_0=mix22;//p2
		edges[2].x_1 = x0;
		edges[2].y_1 = y0;
		edges[2].z_1 = z0;
		edges[2].mix0_1=mix00;//p0
		edges[2].mix1_1=0;
		edges[2].mix2_1=0;
	} else {
		edges[2].x_0 = x0;
		edges[2].y_0 = y0;
		edges[2].z_0 = z0;
		edges[2].mix0_0=mix00; //Assigning point 0 to this one
		edges[2].mix1_0=0;
		edges[2].mix2_0=0;
		edges[2].x_1 = x2;
		edges[2].y_1 = y2;
		edges[2].z_1 = z2;
		edges[2].mix0_1=0;
		edges[2].mix1_1=0;
		edges[2].mix2_1=mix22; //Assigning point 2 to this one
	}
	int maxLength = 0, longEdge = 0;
	for(int i = 0; i < 3; i++){
		int length = (edges[i].y_1-edges[i].y_0);
		if(length > maxLength){
			maxLength = length;
			longEdge = i;
		}
	}
	int shortEdge1 = (longEdge+1)%3;
	int shortEdge2 = (longEdge+2)%3;
	drawSpansBetweenEdgesFS(edges[longEdge],edges[shortEdge1],r,g,b,fragshader);
	drawSpansBetweenEdgesFS(edges[longEdge],edges[shortEdge2],r,g,b,fragshader);
}
void tri2dND(int x0, int y0,
			int x1, int y1, 
			int x2, int y2, 
			uchar r, uchar g, uchar b){
	api_Edge edges[3];
	const float z0=0, z1=0, z2=0;
	if(y0 < y1){
			edges[0].x_0 = x0;
			edges[0].y_0 = y0;
			//edges[0].z_0 = z0;
			
			edges[0].x_1 = x1;
			edges[0].y_1 = y1;
			//edges[0].z_1 = z1;
		} else {
			edges[0].x_0 = x1;
			edges[0].y_0 = y1;
			//edges[0].z_0 = z1;
			
			edges[0].x_1 = x0;
			edges[0].y_1 = y0;
			//edges[0].z_1 = z0;
		}
	if(y2 < y1){
		edges[1].x_0 = x2;
		edges[1].y_0 = y2;
		//edges[1].z_0 = z2;
		
		edges[1].x_1 = x1;
		edges[1].y_1 = y1;
		//edges[1].z_1 = z1;
	} else {
		edges[1].x_0 = x1;
		edges[1].y_0 = y1;
		//edges[1].z_0 = z1;
		
		edges[1].x_1 = x2;
		edges[1].y_1 = y2;
		//edges[1].z_1 = z2;
	}
	if(y2 < y0){
		edges[2].x_0 = x2;
		edges[2].y_0 = y2;
		//edges[2].z_0 = z2;
		
		edges[2].x_1 = x0;
		edges[2].y_1 = y0;
		//edges[2].z_1 = z0;
	} else {
		edges[2].x_0 = x0;
		edges[2].y_0 = y0;
		//edges[2].z_0 = z0;
		
		edges[2].x_1 = x2;
		edges[2].y_1 = y2;
		//edges[2].z_1 = z2;
	}
	int maxLength = 0, longEdge = 0;
	for(int i = 0; i < 3; i++){
		int length = (edges[i].y_1-edges[i].y_0);
		if(length > maxLength){
			maxLength = length;
			longEdge = i;
		}
	}
	int shortEdge1 = (longEdge+1)%3;
	int shortEdge2 = (longEdge+2)%3;
	drawSpansBetweenEdgesND(edges[longEdge],edges[shortEdge1],r,g,b);
	drawSpansBetweenEdgesND(edges[longEdge],edges[shortEdge2],r,g,b);
}

//Renders 3D models using lines.
void linemodel(pmod* mod, uchar r, uchar g, uchar b){
	for(size_t i = 0; i+1 < mod->npoints; i+=2){//operates on pairs of points.
		line2d(	mod->d[i*3],mod->d[i*3+1],
				mod->d[(i+1)*3],mod->d[(i+1)*3+1],
				r,g,b);
	}
}
void renspadv(sprite* s, int x, int y, uint flips){
	sprite temp;
	temp = *s;
	unsigned int spritedata[s->w * s->h];
	temp.d = (unsigned char*)spritedata;
	int startsx=0, startsy=0, endsx=s->w, endsy=s->h, stepsx = 1, stepsy = 1;
	// int tx = 0, ty = 0;
	if(flips & 1){ //Flip horizontal
		startsx = s->w-1;
		endsx = -1;
		stepsx = -1;
	}
	if(flips & 2){ //Flip horizontal
		startsy = s->h-1;
		endsy = -1;
		stepsy = -1;
	}
	for(int x = startsx, tx = 0; x != endsx; x+=stepsx, tx++)
	for(int y = startsy, ty = 0; y != endsy; y+=stepsy, ty++)
		spritedata[tx+ty*s->w] = ((uint*)s->d)[x + y*s->w];
	rensp(&temp,x,y);
}
void rensp(sprite* s, int x, int y){
	if(	x > surf->w || 
		y > surf->h || 
		x+s->w < 0 ||
		y+s->h < 0) return; //None of this will draw.
	//
	int offx = (x>0)?0:-x;
	int offy = (y>0)?0:-y;
	for(int sx = offx; (sx < s->w) && (x+sx < surf->w); sx++)
	for(int sy = offy; (sy < s->h) && (y+sy < surf->h); sy++)
	{
		int tx = sx+x;int ty = sy+y;
		unsigned char* datum = surf->pixels; 
		datum += (tx + ty * surf->w)*4;
		uint dest = *((uint*)datum); 
		unsigned char destA = *(datum+A_);
		uint src = *((uint*)(s->d+(sx + sy * s->w)*4)); 
		unsigned char srcA = *((s->d+(sx + sy * s->w)*4)+A_);
		uint result;
		float srcmult = srcA/255.0f;
		float destmult = (255-srcA)/255.0f;
		((unsigned char*)&result)[R_] = ((unsigned char*)&src)[R_] * srcmult + ((unsigned char*)&dest)[R_] * destmult;
		((unsigned char*)&result)[G_] = ((unsigned char*)&src)[G_] * srcmult + ((unsigned char*)&dest)[G_] * destmult;
		((unsigned char*)&result)[B_] = ((unsigned char*)&src)[B_] * srcmult + ((unsigned char*)&dest)[B_] * destmult;
		((unsigned char*)&result)[A_] = destA;
		*((uint*)datum) = result;
	}
}
//Used for tinting pure black and white sprites to the correct rgb.
//Used for the text renderer.
void rensptint(sprite* s, int x, int y,uchar r, uchar g, uchar b){
	if(	x > surf->w || 
		y > surf->h || 
		x+s->w < 0 ||
		y+s->h < 0) return;
	int offx = (x>0)?0:-x;
	int offy = (y>0)?0:-y;
	for(int sx = offx; (sx < s->w) && (x+sx < surf->w); sx++)
	for(int sy = offy; (sy < s->h) && (y+sy < surf->h); sy++)
	{
		int tx = sx+x;int ty = sy+y;
		char* datum = surf->pixels; 
		datum += (tx + ty * surf->w)*4;
		uint sd = ((uint*)s->d)[(sx + sy * s->w)];
		((uchar*)&sd)[R_] &= r;
		((uchar*)&sd)[G_] &= g;
		((uchar*)&sd)[B_] &= b;
		if(((uchar*)&sd)[A_] > 128)
		*((uint*)datum) = sd;
	}
}
void drawText(int xleft, int ytop, const char* text, uchar r, uchar g, uchar b){
	int x = xleft;
	for(size_t i = 0; i < strlen(text);i++){
		if(text[i] == '\n')
			{ytop+=charsprites['!'].h;//HEHE
			x = xleft;
		} else { 
			rensptint(charsprites + ((uchar*)text)[i],x,ytop,r,g,b);
			x+=charsprites['!'].w;
		}
	}
}
void renspdbuff(sprite* s, int x, int y,float z){
//	fixed z = FLOATTOFIXED(z_);
	if(	x > surf->w || 
		y > surf->h || 
		x+s->w < 0 ||
		y+s->h < 0) return; //None of this will draw.
	int offx = (x>0)?0:-x;int offy = (y>0)?0:-y;
	for(int sx = offx; (sx < s->w) && (x+sx < surf->w); sx++)
	for(int sy = offy; (sy < s->h) && (y+sy < surf->h); sy++)
	{
		int tx = sx+x;int ty = sy+y;
		float zsamp = dbuff[tx + ty * surf->w];
		char* datum = surf->pixels; 
		datum += (tx + ty * surf->w)*4;
		if(s->d[(sx + sy * s->w)*4+A_] > 128 && z < zsamp){
			*((uint*)datum) = *((uint*)(s->d+(sx + sy * s->w)*4));//write color
			dbuff[tx + ty * surf->w] = z;//write z
		}
	}
}
void renback(sprite* s, uint x, uint y){
	for(int i = 0; i < surf->w; i++)
	for(int j = 0; j < surf->h; j++)
	{
		uint ix = (i + x)%s->w;
		uint jy = (j + y)%s->h;
		char* dat = (char*)surf->pixels + 	(i + j*surf->w)*4; 
		char* src = (char*)(s->d) + 		(ix+ jy*s->w)*4;
		if(((uchar)src[4])>128)
		*(uint*)dat = *(uint*)src;
	}
}
void blitback(sprite* s, uint x, uint y){
	uint j = 0;
	for(uint i = 0; i < (uint)surf->w; i++)
	for(j = 0; j < (uint)s->h && j < (uint)surf->h ; j++)
	{
		uint ix = (i + x)%s->w;
		uint jy = (j + y)%s->h;
		char* dat = (char*)surf->pixels + 	(i + j*surf->w)*4; 
		char* src = (char*)(s->d) + 		(ix+ jy*s->w)*4;
		*(uint*)dat = *(uint*)src;
	}
	//for the remaining rows...
	for(;j < (uint)surf->h;j++){
		int sj = j%s->h;
		char* dat = (char*)surf->pixels + (j*surf->w)*4;
		char* src = (char*)surf->pixels + (sj*surf->w)*4;
		memcpy(dat,src,surf->w*4);
	}
}
void blitsp(sprite* s, int x, int y){
	if(	x > surf->w || 
		y > surf->h || 
		x+s->w < 0 ||
		y+s->h < 0) return; //None of this will draw.
	//
	int offsx = (x>0)?0:-x;
	int offtx = (x<0)?0:x;
	int offsy = (y>0)?0:-y;
	int endsx = (x+s->w > surf->w)?	surf->w-x:	s->w;
	int endsy = (y+s->h > surf->h)?	surf->h-y:	s->h;
	char* datum = surf->pixels;
	for(int sy = offsy; sy < endsy; sy++)
	memcpy(
		datum + (offtx+(sy+y)*surf->w)*4,
		s->d  + (offsx + sy*s->w)*4,
		4*(endsx-offsx)
	);
}
void renTileMap(tilemap* t){
	for(uint x = 0; x < t->w; x++)
	for(uint y = 0; y < t->h; y++)
	{
		int ind = x + y * t->w;
		sprite* s = t->d[ind];
		if(!s)continue;
		int xd = -t->xoff + x * t->sprw;
		int yd = -t->yoff + y * t->sprh;
		blitsp(s,xd,yd);
	}
}
void hshift(shiftfunc f){
	//uchar* row = malloc(4 * surf->w * 3);
	uchar row[surf->w * 12]; //Variable length array
	uchar* dat = surf->pixels;
	for(int i = 0; i < surf->h; i++){
		int s = MIN(MAX(f(i),-1*(int)surf->w),surf->w); //Shift Offset for the row.
		memcpy(row, 		dat+4*surf->w*i,	4*surf->w);
		memcpy(row + 4*surf->w, dat+4*surf->w*i, 4*surf->w);
		memcpy(row + 8*surf->w, dat+4*surf->w*i, 4*surf->w);
		memcpy(dat+4*surf->w*i, row + 4*surf->w+s*4, 4*surf->w);
	}
	//free(row);
}
void vshift(shiftfunc f){
	uchar* dat = surf->pixels;
	//uchar* column = malloc(4 * surf->h * 3);
	uchar column[surf->h * 12];
	for(int i = 0; i < surf->w; i++) //for every column...
	{	
		int s = MIN(MAX(f(i),-1*(int)surf->h),surf->h); //Shift Offset for the row.
		for(int j = 0; j < surf->h*4; j++)
			column[j] = dat[(i + j/4 * surf->w)*4+j%4];
		memcpy(column+4*surf->h, column, 4*surf->h);
		memcpy(column+8*surf->h, column, 4*surf->h);
		for(int j = 0; j < surf->h*4; j++)
			dat[(i + j/4 * surf->w)*4+j%4] = column[j+4*surf->h+4*s];
	}
}
void lSPal(palette* p){ //Load the standard palette. 0,0,0 to 255,255,255 as 0,0,0 to 3,3,3 (64 colors)
	uint memory = 0; //4 bytes!
	uchar* mem = (uchar*)&memory;
	p->defs[0] = 0;
	mem[A_]=255;
	for(uchar i = 1; i < 65;i++){
		mem[B_] = i-1;//B_ is now the number between 0 and 63 representing what portion of 255 we want to assign to each.
		mem[R_] = mem[B_]/16; mem[B_]-=mem[R_]*16; //Highest digit in base 4.
		mem[G_] = mem[B_]/4; mem[B_]-=mem[G_]*4; //Middle digit in base 4. mem[B_] is now the blue portion.
		mem[R_] *= 85;mem[G_] *= 85;mem[B_] *= 85; //0 - 3 times 
		p->defs[i] = memory;
	}
}
void cWin(int screenmode){
	int w = 400;
	int h = 240;
	if(dbuff)free(dbuff);dbuff=NULL;
	if(actualSurf.pixels)free(actualSurf.pixels);
	actualSurf.pixels = malloc(w*h*4);
	actualSurf.w = w;
	actualSurf.h = h;
	surf = &actualSurf;
    dbuff = malloc(4 * w * h);
    //TODO: open the graphics output for 3DS.
}

void cRend(){
	//do nothing.
}
void cSurf(int w, int h){
	//Do Nothing.
}
void clear(uchar a){
	uint* dat = surf->pixels;
	uint col = 0; ((uchar*)&col)[A_] = a;
	for(int i = 0; i < surf->w; i++)
		dat[i] = col;
	for(int i = 1; i < surf->h; i++)
	{
		memcpy(dat+i*surf->w,dat,surf->w*4);
	}
}
void uTx(){
	//do nothing
}
void clean(){
	//if(tex)SDL_DestroyTexture(tex);
	if(actualSurf.pixels)free(actualSurf.pixels); actualSurf.pixels = NULL;
	if(dbuff)free(dbuff);
	//if(ren)SDL_DestroyRenderer(ren);
	surf = NULL;
	dbuff = NULL;
	Mix_Quit();
	SDL_Quit();
}
void flipVertical(){
	uint s = 4 * surf->w;
	unsigned char* dat = surf->pixels;
	unsigned char row[s]; //variable length array
	for(int i = 0; i < surf->h/2; i++)
	{
		uint rs = surf->h-1-i; //The other row.
		memcpy(row,dat + i*s,s);
		memcpy(dat+i*s,dat+s*rs,s);
		memcpy(dat+rs*s,row,s);
	}
	//free(row);
}
#endif
#endif
