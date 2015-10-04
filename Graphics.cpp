#include "Globals.h"
#include "Graphics.h"

void GetWorldPoint(long x,long y,Vector2* hit)
{
	double model[16],projection[16];
	int viewport[4];

	glGetIntegerv(GL_VIEWPORT,viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX,model);
	glGetDoublev(GL_PROJECTION_MATRIX,projection);

	double x0,y0,z0;
	double x1,y1,z1;

	gluUnProject(x,viewport[3] - y,0.0f,model,projection,viewport,&x0,&y0,&z0);
	gluUnProject(x,viewport[3] - y,1.0f,model,projection,viewport,&x1,&y1,&z1);

	x1 -= x0;
	y1 -= y0;
	z1 -= z0;

	// Find the intersection with xOy
	double k = -z0 / z1;

	*hit = Vector2((float)(x0 + k * x1),(float)(y0 + k * y1));
}