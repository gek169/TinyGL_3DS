#include "GL/gl.h"
#include "../zbuffer.h"
//#include "GL/glu.h"
#include "3dMath.h"
#include "api.h"
//Helpers for immediate mode OpenGL rendering.
//Immediate mode has a certain elegance to it
//and with display lists, it's *almost* as fast
//as modern shaded GL...
typedef struct{
	float shininess;
	float specular[3];
} phongprops;
GLuint createModelDisplayList(
	vec3* points, uint npoints,
	vec3* colors,
	vec3* normals,
	vec3* texcoords
);
void drawModel(
	vec3* points, uint npoints,
	vec3* colors,
	vec3* normals,
	vec3* texcoords
);
void enableLighting();
void disableLighting();
void initGL(unsigned int wx, unsigned int wy);
void swapGL(void* targ);
void resizeGL(int w, int h);
void cleanGL();
void configLight(
	GLenum ln,
	vec4 ka,
	vec4 kd,
	vec4 ks,
	vec4 pos
);
GLuint loadRGBTexture(unsigned char* buf,unsigned int w, unsigned int h);
void basicGLConfig();
void clearGL();
ZBuffer *getFrameBuffer();
#define endFrame upd
