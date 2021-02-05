#include "include/gl_helpers.h"
ZBuffer *frameBuffer;

ZBuffer *getFrameBuffer(){return frameBuffer;}
void initGL(unsigned int wx, unsigned int wy){
	frameBuffer = ZB_open( wx, wy, ZB_MODE_RGBA, 0, 0, 0, 0);
	glInit( frameBuffer );
}
void resizeGL(int w, int h){
	ZB_resize(frameBuffer, NULL, w, h);
}
void basicGLConfig(){
	
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
//	glEnable(GL_TEXTURE_2D);
	glEnable(GL_COLOR_MATERIAL);
//	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,1);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,0);
	glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
	glClearColor(0,0,0,0);
	glClearDepth(1.0f);
	//glClearStencil(0);
	//glDepthFunc(GL_LEQUAL);
}
void enableLighting(){
	glEnable(GL_LIGHTING);
}
void cleanGL(){
	ZB_close(frameBuffer);
}
void disableLighting(){
	glDisable(GL_LIGHTING);
}
void configLight( //THIS FUNCTION ALSO DEPENDS ON
//f_ being FLOAT!!!
	GLenum ln,
	vec4 ka,
	vec4 kd,
	vec4 ks,
	vec4 pos
){
	glLightfv(ln,GL_AMBIENT,ka.d);
	glLightfv(ln,GL_DIFFUSE,kd.d);
	glLightfv(ln,GL_SPECULAR,ks.d);
	glLightfv(ln,GL_POSITION,pos.d);
}


void drawModel(
//HUGE important note! these depend on the math library using 
//f_ as float and not double!
//Remember that!
	vec3* points, uint npoints,
	vec3* colors,
	vec3* normals,
	vec3* texcoords
){
	if(!points)return;
	glBegin(GL_TRIANGLES);
	for(uint i = 0; i < npoints;i++){
		if(colors){ //Fix for TinyGL color interpolation.
			glColor3f(colors[i].d[0],colors[i].d[1],colors[i].d[2]);
		}
		if(texcoords)
			glTexCoord2f(texcoords[i].d[0],texcoords[i].d[1]);
		if(normals)
			glNormal3f(normals[i].d[0],normals[i].d[1],normals[i].d[2]);
		glVertex3f(points[i].d[0],points[i].d[1],points[i].d[2]);
	}
	glEnd();
}




GLuint createModelDisplayList(
//HUGE important note! these depend on the math library using 
//f_ as float and not double!
//Remember that!
	vec3* points, uint npoints,
	vec3* colors,
	vec3* normals,
	vec3* texcoords
){
	GLuint ret = 0;
	if(!points)return 0;
	ret = glGenLists(1);
	glNewList(ret, GL_COMPILE);
		glBegin(GL_TRIANGLES);
		for(uint i = 0; i < npoints; i++){
			if(colors){
				glColor3f(colors[i].d[0],colors[i].d[1],colors[i].d[2]);
			}
			if(texcoords)
				glTexCoord2f(texcoords[i].d[0],texcoords[i].d[1]);
			if(normals)
				glNormal3f(normals[i].d[0],normals[i].d[1],normals[i].d[2]);
			glVertex3f(points[i].d[0],points[i].d[1],points[i].d[2]);
		}
		//printf("\ncreateModelDisplayList is not the problem.\n");
		glEnd();
	glEndList();
	return ret;
}



void swapGL(void* targ){
	ZB_copyFrameBuffer(frameBuffer, targ, 240 * 4); //4 for RGBA, 2 for 16 bit (internal!!!). 24 bit is broken.
	/*
	for(int i = 0; i < surf->w * surf->h; i++)
	{
		uint col = ((uint*)surf->pixels)[i];
		unsigned char r = ((unsigned char*)(&col))[0];
		((unsigned char*)(&col))[0] = ((unsigned char*)(&col))[2];
		((unsigned char*)(&col))[2] = r;
		((uint*)surf->pixels)[i] = col;
	}
	*/
}
void clearGL(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}
GLuint loadRGBTexture(unsigned char* buf, unsigned int w, unsigned int h){
	GLuint t = 0;
	glGenTextures(1, &t);
	// for(unsigned int i = 0; i < w * h; i++)
		// {
			// unsigned char t = 0;
			// unsigned char* r = buf + i*3;
			// // unsigned char* g = buf + i*3+1;
			// unsigned char* b = buf + i*3+2;
			// t = *r;
			// *r = *b;
			// *b = t;
		// }
	glBindTexture(GL_TEXTURE_2D,t);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D,0,3,w,h,0,GL_RGB,GL_UNSIGNED_BYTE,buf);
	return t;
}
