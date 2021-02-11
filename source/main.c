/*
	Water Rendering demo for the Nintendo 3DS by Gek
*/

#include <3ds.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <dirent.h>
#include <time.h>
//This include a header containing definitions of our image
//#include "brew2_bgr.h"
#define CHAD_API_IMPL
#define CHAD_MATH_IMPL
#include "include/gl_helpers.h"
#include "include/tobjparse.h"
#define WIDTH 320
#define HEIGHT 240
#define SCREENBYTES (320*240*3)
#define MAX(x,y) (x>y?x:y)
#define MIN(x,y) (x<y?x:y)

#define SAMPLERATE 22050
#define SAMPLESPERBUF (SAMPLERATE / 30)
#define BYTESPERSAMPLE 4

float du[WIDTH][HEIGHT], dv[WIDTH][HEIGHT], dunew[WIDTH][HEIGHT];

vec3 campos = (vec3){.d[0]=0,.d[1]=0,.d[2]=-3};
vec3 camforw = (vec3){.d[0]=0,.d[1]=0,.d[2]=-1};
vec3 camup = (vec3){.d[0]=0,.d[1]=1,.d[2]=0};
uint wasdstate[4] = {0,0,0,0};
const float mouseratiox = 1.0/300.0f;
const float mouseratioy = 1.0/300.0f;
int mousex = 0, mousey = 0;

void rotateCamera(){
	vec3 a;
	a.d[1] = (float)mousex * mouseratiox;
	a.d[2] = (float)mousey * mouseratioy;
	//if(fabsf(camforw.d[0]) < 0.001) camforw.d[0] = 0.001;
	vec3 right = normalizev3(
					crossv3(
						normalizev3(camforw),
						normalizev3(camup)
					)
				);right.d[1]=0;
	vec3 result = rotatev3(camforw,right,-a.d[2]);
	camup = crossv3(right,camforw);
	camforw = rotatev3(result,camup,-a.d[1]);
	//if(fabsf(camforw.d[0]) < 0.001) camforw.d[0] = 0.001;
}



void initFluid(){
	for(int x = 0; x < WIDTH; x++)
	for(int y = 0; y < HEIGHT; y++)
	{
		du[x][y] = fabs(sinf(y/5.0f) + cosf(x/5.0f));
		dv[x][y] = 0.0;
		dunew[x][y] = fabs(sinf(y/5.0f) + cosf(x/5.0f));
	}
}
float get_u(int x, int y){
	if(x<0)x=0;
	if(x>=WIDTH)x=WIDTH-1;
	if(y<0)y=0;
	if(y>=HEIGHT)y=HEIGHT-1;
	return du[x][y];
}
void stepFluid(){
	for(int x = 0; x < WIDTH; x++)
		for(int y = 0; y < HEIGHT; y++)
		{
			dv[x][y] += (get_u(x-1,y) + get_u(x+1, y) + get_u(x,y+1) + get_u(x,y-1))*0.25 - get_u(x,y);
			dv[x][y] *= 0.99;
			dunew[x][y] += dv[x][y];
		}
	for(int x = 0; x < WIDTH; x++)
	for(int y = 0; y < HEIGHT; y++)
	du[x][y] = dunew[x][y];
}

//----------------------------------------------------------------------------
void fill_buffer(void *audioBuffer,size_t offset, size_t size, int frequency ) {
//----------------------------------------------------------------------------

	u32 *dest = (u32*)audioBuffer;

	for (int i=0; i<size; i++) {

		s16 sample = INT16_MAX * sin(frequency*(2*M_PI)*(offset+i)/SAMPLERATE);

		dest[i] = (sample<<16) | (sample & 0xffff);
	}

	DSP_FlushDataCache(audioBuffer,size);

}

int myscandir(){
	{ 
	    struct dirent *de;  // Pointer for directory entry 
	  
	    // opendir() returns a pointer of DIR type.  
	    DIR *dr = opendir("romfs:/"); 
	  
	    if (dr == NULL)  // opendir returns NULL if couldn't open directory 
	    { 
	        printf("Could not open current directory" ); 
	        return 0; 
	    } 
	  
	    // Refer http://pubs.opengroup.org/onlinepubs/7990989775/xsh/readdir.html 
	    // for readdir() 
	    while ((de = readdir(dr)) != NULL) 
	            printf("%s\n", de->d_name); 
	  
	    closedir(dr);     
	    return 0 ; 
	} 
}


GLubyte stipplepattern[128] = {
	0xAA,0xAA,0xAA,0xAA,
	~0xAA,~0xAA,~0xAA,~0xAA,
	0xAA,0xAA,0xAA,0xAA,
	~0xAA,~0xAA,~0xAA,~0xAA,
	0xAA,0xAA,0xAA,0xAA,
	~0xAA,~0xAA,~0xAA,~0xAA,
	0xAA,0xAA,0xAA,0xAA,
	~0xAA,~0xAA,~0xAA,~0xAA,
	
	0xAA,0xAA,0xAA,0xAA,
	~0xAA,~0xAA,~0xAA,~0xAA,
	0xAA,0xAA,0xAA,0xAA,
	~0xAA,~0xAA,~0xAA,~0xAA,
	0xAA,0xAA,0xAA,0xAA,
	~0xAA,~0xAA,~0xAA,~0xAA,
	0xAA,0xAA,0xAA,0xAA,
	~0xAA,~0xAA,~0xAA,~0xAA,

	0xAA,0xAA,0xAA,0xAA,
	~0xAA,~0xAA,~0xAA,~0xAA,
	0xAA,0xAA,0xAA,0xAA,
	~0xAA,~0xAA,~0xAA,~0xAA,
	0xAA,0xAA,0xAA,0xAA,
	~0xAA,~0xAA,~0xAA,~0xAA,
	0xAA,0xAA,0xAA,0xAA,
	~0xAA,~0xAA,~0xAA,~0xAA,

	0xAA,0xAA,0xAA,0xAA,
	~0xAA,~0xAA,~0xAA,~0xAA,
	0xAA,0xAA,0xAA,0xAA,
	~0xAA,~0xAA,~0xAA,~0xAA,
	0xAA,0xAA,0xAA,0xAA,
	~0xAA,~0xAA,~0xAA,~0xAA,
	0xAA,0xAA,0xAA,0xAA,
	~0xAA,~0xAA,~0xAA,~0xAA
};


int main(int argc, char **argv)
{
	uint RR_=3;
	uint GG_=2;
	uint BB_=1;
	uint AA_=0;
	GLuint modelDisplayList = 0; int dlExists = 0;
	char* modelName = "romfs:/extrude.obj";
	track* mytrack = NULL;
	//gfxInitDefault();
	gfxInit(GSP_RGBA8_OES,GSP_RGBA8_OES,false);
	fsInit();
	romfsInit();
	init();ainit();
	consoleInit(GFX_TOP, NULL);
	myscandir();
	//mytrack = lmus("romfs:/WWGW.wav");
	mytrack = lmus("romfs:/Strongest.mp3");
		if(!mytrack){
			printf("\nError loading sounds\n");
		}
	if(mytrack)
		mplay(mytrack, -1, 1000);
	//Initialize console on top screen. Using NULL as the second argument tells the console library to use the internal console structure as current one
	
	initFluid();
	printf("\nBitmap CPU Rendering demo by Gek!\nThanks Evie!");

	printf("\n\n<Start>: Exit program.\n");

	gfxSetDoubleBuffering(GFX_BOTTOM, false);
	
	

	//Get the bottom screen's frame buffer
	u8* fb = gfxGetFramebuffer(GFX_BOTTOM, GFX_LEFT, NULL, NULL);
	u8* ifb = linearAlloc(320 * 240 * 4); //Enough for the bottom screen.
	for(int i = 0; i < 320 * 240; i++)
		{
			ifb[i*4+RR_] = rand() % 255;//R
		}
	if(!ifb) return 1;
	//Copy our image in the bottom screen's frame buffer
	//memcpy(fb, brew2_bgr, brew2_bgr_size);


	srand(time(NULL));
		initGL(WIDTH,HEIGHT);
		basicGLConfig();
		glClearColor(0,0,0.3,0);
		glDisable(GL_TEXTURE_2D);
		
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glShadeModel(GL_SMOOTH);
		//glDisable(GL_DEPTH_TEST);
		double t = 0;
		glViewport(0,0,WIDTH,HEIGHT);

		glEnable(GL_POLYGON_STIPPLE);
			
		glPolygonStipple(stipplepattern);

	{
		objraw omodel; model m = initmodel();
		omodel = tobj_load(modelName);
		
		if(!omodel.positions) {puts("\nERROR! No positions in model. Aborting...\n");} else {
		m = tobj_tomodel(&omodel);
		printf("\nHas %d points.\n",m.npoints); 
		modelDisplayList = createModelDisplayList(
			m.d, m.npoints,
			m.c,
			m.n,
			m.t
		);dlExists = 1;
		freemodel(&m);
		}
		freeobjraw(&omodel);
	}

	GLuint tex = 0;
		{
			int sw = 0, sh = 0, sc = 0; //sc goes unused.
			uchar* source_data = stbi_load("romfs:/tex.jpg", &sw, &sh, &sc, 3);
			if(source_data){
				tex = loadRGBTexture(source_data, sw, sh);
				free(source_data);
			}
		//tex = 
		}
	glDisable(GL_LIGHTING);
	// Main loop
	while (aptMainLoop())
	{
		t+=0.016666f;
		//Scan all the inputs. This should be done once for each frame
		hidScanInput();

		//hidKeysDown returns information about which buttons have been just pressed (and they weren't in the previous frame)
		u32 kDown = hidKeysDown();
		u32 kHeld = hidKeysHeld();
		if (kDown & KEY_START) break; // break in order to return to hbmenu
		wasdstate[0] = (kHeld & KEY_DUP)   ?1:0;
		wasdstate[1] = (kHeld & KEY_DLEFT) ?1:0;
		wasdstate[3] = (kHeld & KEY_DRIGHT)?1:0;
		wasdstate[2] = (kHeld & KEY_DDOWN) ?1:0;
		mousex = (kHeld & KEY_Y)?-30:0 + (kHeld & KEY_A)?30:0;
		mousey = (kHeld & KEY_X)?-30:0 + (kHeld & KEY_B)?30:0;
		
		fb = gfxGetFramebuffer(GFX_BOTTOM, GFX_LEFT, NULL, NULL);
		
		touchPosition touch;
		//Read the touch screen coordinates
		
		if (kHeld & KEY_TOUCH)
		{
			hidTouchRead(&touch);	
			/*
			float amt = rand()%100>50?-50:50;
			touch.py = HEIGHT - touch.py;
			dunew[MIN(touch.px,WIDTH-1)][MIN(touch.py,HEIGHT-1)] += amt;
			du[MIN(touch.px,WIDTH-1)][MIN(touch.py,HEIGHT-1)] += amt;
			*/
			printf("Detected Touch!\n");
		}
		//Copy our image in the bottom screen's frame buffer
		//memcpy(fb, ifb, 320*240*3);
		//memcpy(fb, brew2_bgr, brew2_bgr_size);
		/*
			stepFluid();
			for(int x = 0; x < WIDTH; x++)
			for(int y = 0; y < HEIGHT; y++)
			{
				u8* datum = ifb;
				float valr = MIN(255,80+du[x][y]*50);
				float valg = MIN(255,80+du[x][y]*50);
				float valb = MIN(255,80+du[x][y]*100);
				datum[4 * (y + x * HEIGHT)+RR_] = (u8)valr;
				datum[4 * (y + x * HEIGHT)+GG_] = (u8)valg;
				datum[4 * (y + x * HEIGHT)+BB_] = (u8)valb;
			}
		*/
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		//gluPerspective(70,(float)WIDTH/(float)HEIGHT,1,512);
		mat4 matrix = perspective(70,(float)WIDTH/(float)HEIGHT,1,512);
		glLoadMatrixf(matrix.d);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glPushMatrix(); //Pushing on the LookAt Matrix.
		/*
		gluLookAt(
		campos.d[0],campos.d[1],campos.d[2],
		campos.d[0] + camforw.d[0],
		campos.d[1] + camforw.d[1],
		campos.d[2] + camforw.d[2],
		camup.d[0],camup.d[1],camup.d[2]
		);
		*/
		
		vec3 right = normalizev3(
					crossv3(
						normalizev3(camforw),
						normalizev3(camup)
					)
				);right.d[1]=0;
		matrix = (lookAt(campos,addv3(campos,camforw),camup)); //Using right vector to correct for screen rotation.
		glLoadMatrixf(matrix.d);
		if(wasdstate[0])
			campos = addv3(campos,
				scalev3(0.1,
					camforw
				)
			);
		if(wasdstate[2])
			campos = addv3(campos,
				scalev3(-0.1,
					camforw
				)
			);
		if(wasdstate[1])
			campos = addv3(campos,
				scalev3(-0.1,
					right
				)
			);
		if(wasdstate[3])
			campos = addv3(campos,
				scalev3(0.1,
					right
				)
			);
		clearGL();
		glEnable(GL_DEPTH_TEST);
		glBindTexture(GL_TEXTURE_2D,tex);
		//glDisable(GL_BLEND);
		//glDisable(GL_TEXTURE_2D);
		//printf("\nNew triangle!\n");
		if(!dlExists){
			glDisable(GL_TEXTURE_2D);
			glBegin(GL_TRIANGLES);
				//glColor3f(0,0,1);
				glColor3f(1,0,0);glTexCoord2f(0,0);glVertex3f(-1,-1,-10); 
				glColor3f(0,1,0);glTexCoord2f(1,0);glVertex3f(1,-1,-10);
				glColor3f(0,0,1);glTexCoord2f(0.5,1);glVertex3f(0,1,-10);
				//glColor3f(0,1,0);
			glEnd();
		} else {
			glEnable(GL_TEXTURE_2D);
			//glDisable(GL_TEXTURE_2D);
			glEnable(GL_POLYGON_STIPPLE);
			//glDisable(GL_COLOR_MATERIAL);
			for(int i = 0; i < 20; i++){
				glPushMatrix();
				glTranslatef((float)(i%10) * 8.0,(float)(i/10) * 8.0,-10);
				glCallList(modelDisplayList);
				// drawModel(
					// m.d, m.npoints,
					// m.c,
					// m.n,
					// m.t
				// );
				glPopMatrix();
			}
			glDisable(GL_POLYGON_STIPPLE);
			glDisable(GL_TEXTURE_2D);
		}
		glEnable(GL_BLEND);
		glPopMatrix(); //The view transform.
		//puts("\nafter GL code");
		for(int i = 0; i < WIDTH * HEIGHT * 4; i++)
			ifb[i] = 0;
		glDrawText("Blitting to the screen!\n\n\nWritten by GEK!",0,0,0x00FFffFF); //Should be solid white.
		swapGL(ifb);
		rotateCamera();
		/*
		for(int x = 0; x < WIDTH; x++)
		for(int y = 0; y < HEIGHT; y++)
		{
			u8* src = getFrameBuffer()->pbuf;
			ifb[4 * (x + y * HEIGHT)+RR_] = src[4 * (x + y * HEIGHT)+0];
			ifb[4 * (x + y * HEIGHT)+GG_] = src[4 * (x + y * HEIGHT)+1];
			datum[4 * (x + y * HEIGHT)+BB_] = src[4 * (x + y * HEIGHT)+2];
		}*/
		
		memcpy(fb, ifb, 320*240*4);
		// Flush and swap framebuffers
		gfxFlushBuffers();
		gfxSwapBuffers();

		//Wait for VBlank
		gspWaitForVBlank();
	}
	if(ifb) linearFree(ifb);
	
	// Exit services
	
	Mix_Quit();
	SDL_Quit();
	gfxExit();
	return 0;
}
