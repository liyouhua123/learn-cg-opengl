// Util.cpp: implementation of the Util class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GLTest001.h"
#include "Util.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static int Message(LPCTSTR lpText,LPCTSTR lpCaption=NULL){
	return MessageBox(0,lpText,lpCaption,MB_OK);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
void GLPoint::operator =(const GLPoint &glpt){
	x=glpt.x;
	y=glpt.y;
	z=glpt.z;
}

BOOL GLPoint::operator ==(const GLPoint &glpt){
	return (x==glpt.x&&y==glpt.y&&z==glpt.z);
}


Util::Util()
{

}

Util::~Util()
{

}

void Polygon3DBegin(GLenum type){
	glEnd();
	glBegin(type);
	//Message("000000");
}
void Polygon3DVertex(const GLvoid* data){
	const GLdouble * ptr=(const GLdouble*)data;
	glVertex3dv(ptr);
	//Message("111111");
}
void Polygon3DEnd(){
	glEnd();
	//Message("2222");
}
BOOL Util::PolygonTesslator(CArray<GLPoint,GLPoint> &m_Point_Array){
	int pt_num = m_Point_Array.GetSize();
	if(pt_num<3) return FALSE;

	GLPoint pt;
	GLdouble (*quad)[3];
	quad=new GLdouble[pt_num][3];
	for(int i=0;i<pt_num;i++){
		pt=m_Point_Array.GetAt(i);
		quad[i][0]=pt.x;
		quad[i][1]=pt.y;
		quad[i][2]=pt.z;
	}
	GLUtesselator *tess =gluNewTess();
	gluTessCallback(tess,GLU_TESS_BEGIN,(void (CALLBACK*)())&Polygon3DBegin);
	gluTessCallback(tess,GLU_TESS_VERTEX,(void (CALLBACK *)())&Polygon3DVertex);
	gluTessCallback(tess,GLU_TESS_END,(void (CALLBACK*)())&Polygon3DEnd);
	gluTessBeginPolygon(tess,NULL);		//开始网格化
		gluTessBeginContour(tess);		//外环，内环一样
		for(int j=0;j<pt_num;j++){
			gluTessVertex(tess,quad[j],quad[j]);	//
		}
		gluTessEndContour(tess);
	gluTessEndPolygon(tess);
	gluDeleteTess(tess);		//删除网格化器
	delete[] quad;
	return TRUE;
}
