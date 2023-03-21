#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include"patch.h"
#include"R3BspUtil.h"
#include"jerror.h"
#include"jmalloc.h"
//#include"matrix.h"
//#include"RadUtil.h"
//#include"jmalloc.h"

//#define _MAX_WINDING_XL    15.0f	//�ִ� ũ�� ����Ʈ�� xl
//#define _MAX_WINDING_YL    15.0f	//�ִ� ũ�� ����Ʈ�� yl
#define _EPSILON	0.1/3.0

typedef struct{
	Vector3f a;	//������ ��
	Vector3f d;	//���⺤��
}_LINE_EQUAL;

static float _MAX_WINDING_XL = 10.0f;	//�ִ� ũ�� ����Ʈ�� xl
static float _MAX_WINDING_YL = 10.0f;	//�ִ� ũ�� ����Ʈ�� yl
static void SetWindingSize(float size)
{
	_MAX_WINDING_XL=size;
	_MAX_WINDING_YL=size;
}
/*
int GetEqualFaceCount(_FACE *cf,_FACE *f)
{
	int cnt=0;

	if(cf->v[0]==f->v[0])
		cnt++;
	if(cf->v[0]==f->v[1])
		cnt++;
	if(cf->v[0]==f->v[2])
		cnt++;
	if(cf->v[1]==f->v[0])
		cnt++;
	if(cf->v[1]==f->v[1])
		cnt++;
	if(cf->v[1]==f->v[2])
		cnt++;
	if(cf->v[2]==f->v[0])
		cnt++;
	if(cf->v[2]==f->v[1])
		cnt++;
	if(cf->v[2]==f->v[2])
		cnt++;
	if(cnt ==3)
	{
		return 0;
		//Error("same polys");
	}
	return cnt;
}
*/
void CrossVector( Vector3f a, Vector3f b, Vector3f r )
{
    r[0] = (a[1] * b[2]) - (a[2] * b[1]);
    r[1] = (a[2] * b[0]) - (a[0] * b[2]);
    r[2] = (a[0] * b[1]) - (a[1] * b[0]);
}
void CrossVector( Vector3d a, Vector3d b, Vector3d r )
{
    r[0] = (a[1] * b[2]) - (a[2] * b[1]);
    r[1] = (a[2] * b[0]) - (a[0] * b[2]);
    r[2] = (a[0] * b[1]) - (a[1] * b[0]);
}
/*
void CrossVector( Vector4f a, Vector3f b, Vector3f r )
{
    r[0] = (a[1] * b[2]) - (a[2] * b[1]);
    r[1] = (a[2] * b[0]) - (a[0] * b[2]);
    r[2] = (a[0] * b[1]) - (a[1] * b[0]);
}
*/
int CheckEdge(Vector3f a,Vector3f b,Vector3f v,Vector4f fv)
{

	Vector3f vv,n;
	double dp;

	vv[0] = b[0] - a[0];
	vv[1] = b[1] - a[1];
	vv[2] = b[2] - a[2];
    CrossVector(fv, vv, n);
    dp = (n[0] * ( v[0] - b[0])	+ n[1] * ( v[1] - b[1])
		+ n[2] * ( v[2] - b[2]));
    if( dp > 0 )
        return 0;
	return 1;	//  ��� 
/*
	_DVECTOR vv,n,fvec;
	double dp;

	vv[0] = b[0] - a[0];
	vv[1] = b[1] - a[1];
	vv[2] = b[2] - a[2];
	fvec[0] = fv->a;
	fvec[1] = fv->b;
	fvec[2] = fv->c;
    CrossVector(&fvec, &vv, &n);
	float div=-9999999;
	div=max(fabs(n[0]),div);
	div=max(fabs(n[1]),div);
	div=max(fabs(n[2]),div);
	n[0]/=div;
	n[1]/=div;
	n[2]/=div;
	dp = (n[0] * ( v[0] - a[0])	+ n[1] * ( v[1] - a[1])
		+ n[2] * ( v[2] - a[2]));

#define N_EPSILON 0.03
    if(  dp > N_EPSILON)
        return 0;
	return 1;	//  ��� 
*/
}
BOOL IsSameVertex(Vector3f s,Vector3f e)
{
	if( fabs(s[0] - e[0]) < _EPSILON && fabs(s[1] - e[1]) < _EPSILON && fabs(s[2] - e[2]) < _EPSILON )
		return TRUE;
	else
		return FALSE;
}

BOOL IsInsertVertex(_POLYFACE *f,Vector3f s,Vector3f e,Vector3f sm)	//���� �� �� ���ؽ��� �����ʿ������� �ش�ȸ� OK
{
	Vector3f vector;

    vector[0] = sm[0];
    vector[1] = sm[1];
    vector[2] = sm[2];

	if( IsSameVertex(s,sm) )
		return FALSE;
	if( IsSameVertex(e,sm) )
		return FALSE;

	if( CheckEdge( e,s,vector,f->f_normal) == FALSE )
		return TRUE;
	return FALSE;
}


double GetVertexLength(Vector3f v1,Vector3f v2)
{
	return sqrt( (double)(v2[0] - v1[0])*(double)(v2[0] - v1[0])
	+ (double)(v2[1] - v1[1])*(double)(v2[1] - v1[1])
	+ (double)(v2[2] - v1[2])*(double)(v2[2] - v1[2]) );
}
double GetVertexLength2(Vector3f v1,Vector3f v2)
{
	return ( (double)(v2[0] - v1[0])*(double)(v2[0] - v1[0])
	+ (double)(v2[1] - v1[1])*(double)(v2[1] - v1[1])
	+ (double)(v2[2] - v1[2])*(double)(v2[2] - v1[2]) );
}

void ConvertVectorFromVertex(Vector3f vec,_VERTEX_UV *v)
{
	vec[0] = v->local[0];
	vec[1] = v->local[1];
	vec[2] = v->local[2];
}
void VectorScale(Vector3f s,Vector3f e,float t,Vector3f get)
{
	get[0] = ( e[0] - s[0] )*t + s[0];
	get[1] = ( e[1] - s[1] )*t + s[1];
	get[2] = ( e[2] - s[2] )*t + s[2];
}
void VectorSubtract(Vector3f sou1,Vector3f sou2,Vector3f tar)
{
    tar[0]=sou1[0]-sou2[0];
    tar[1]=sou1[1]-sou2[1];
    tar[2]=sou1[2]-sou2[2];
}
float VectorLength(Vector3f sou1)
{
    return(sqrtf((sou1[0])*(sou1[0])+(sou1[1])*(sou1[1])+(sou1[2])*(sou1[2])));
}
void VectorPlus(Vector3f sou1,Vector3f sou2,Vector3f tar)
{
    tar[0]=sou1[0]+sou2[0];
    tar[1]=sou1[1]+sou2[1];
    tar[2]=sou1[2]+sou2[2];
}



void GetLineEqual(_LINE_EQUAL *line,Vector3f s,Vector3f e)	//������ �������� ���Ѵ�.
{
	line->d[0] = (e[0] - s[0]);
	line->d[1] = (e[1] - s[1]);
	line->d[2] = (e[2] - s[2]);
	line->a[0] = s[0];
	line->a[1] = s[1];
	line->a[2] = s[2];
}
//���ϴ� ����� ���� ���� 2-2�� 185page�� �ִ�.
void GetCrossLineEqual(_LINE_EQUAL *get_line,_LINE_EQUAL *line,Vector3f v)	//x�� ������ �Ǵ� ������ �������� ���Ѵ�.
{
	double t,a,b;
	double x,y,z;

	a = (double)line->d[0]*line->d[0] + (double)line->d[1]*line->d[1] + (double)line->d[2]*line->d[2];
	b = (double)line->a[0]*line->d[0]+(double)line->a[1]*line->d[1]+(double)line->a[2]*line->d[2]
		-(double)line->d[0]*v[0]-(double)line->d[1]*v[1]-(double)line->d[2]*v[2];
	t = -b/a;	//�Ű� ���� t�� 

	x = (double)line->d[0]*t + (double)line->a[0];
	y = (double)line->d[1]*t + (double)line->a[1];
	z = (double)line->d[2]*t + (double)line->a[2];
	Vector3f e;
	e[0] = (float)x;
	e[1] = (float)y;
	e[2] = (float)z;
	GetLineEqual(get_line,e,v);	//������ �������� ���Ѵ�.
}

#define _MAX_VERTICES		20
//_VERTEX Best_list_v[_MAX_VERTICES];

void GetSquarePointAndUV(Vector4f v_normal,int v_num,_VERTEX_UV v[],Vector3f l_up,Vector3f r_up,Vector3f l_down,Vector3f r_down)
{
	int i;
	float min_xleng,max_xleng,min_yleng,max_yleng;
	D3DMATRIX trans,atrans,btrans,invtrans;

	max_xleng=-9999999,max_yleng=-9999999,min_xleng=9999999,min_yleng=9999999;
	int max_x_i,min_x_i,min_y_i,max_y_i;
	float front,back,fronty,backy;
	float tr_y;

	Vector3f v_s,v_t;
	if( fabs(v_normal[1]) > 0.98 )	//���� �ٴڿ� �������� �����ϰ�
	{
		v_s[0] = 1.0;	v_s[1]=0;	v_s[2]=0;
		Vector3fCross(v_s,v_normal,v_t);

		for(i=0; i<v_num; i++)
		{
			v[i].lu=Vector3fDot(v_s,v[i].local);
			v[i].lv=Vector3fDot(v_t,v[i].local);
		}
	}
	else
	{
//		v_s[0] = -v_normal[1];	v_s[1]=v_normal[0];	v_s[2]=0;
//		Normalize(v_s);
//		Vector3fCross(v_s,v_normal,v_t);
//		v_s[0] = v_normal[0];	v_s[1]=0;	v_s[2]=v_normal[2];
/*
		v_s[0] = v_normal[0];	v_s[1]=v_normal[1];	v_s[2]=0;
		Normalize(v_s);
		Vector3fCross(v_s,v_normal,v_t);
*/
		Vector3f stand={0,1,0},garo={-1,0,0},axis,garo_axis;
		float a,b,c,d;

		Vector3fCross(stand,v_normal,axis);
		Normalize(axis);
		float Theta = (float)acos(Vector3fDot(stand,v_normal));

		if( fabs( Vector3fDot(axis,garo) ) == 1)
		{
			garo_axis[0]=1;
			garo_axis[1]=0;
			garo_axis[2]=0;
		}
		else
		{
			Vector3fCross(garo,axis,garo_axis);
			Normalize(garo_axis);
		}
		float Pheta = (float)acos(Vector3fDot(axis,garo));
		D3DMath_QuaternionFromRotation(a, b, c, d, (_D3DVECTOR &)axis[0], Theta);
		D3DMath_MatrixFromQuaternion(atrans, a, b, c, d );

		D3DMath_QuaternionFromRotation(a, b, c, d, (_D3DVECTOR &)garo_axis[0], Pheta);
		D3DMath_MatrixFromQuaternion(btrans, a, b, c, d );
		D3DMath_MatrixMultiply(trans,btrans,atrans);
		D3DMath_MatrixInvert(invtrans,trans);
		

		for(i=0; i<v_num; i++)
		{
			v[i].lu=v[i].local[0]*trans._11+v[i].local[1]*trans._21+v[i].local[2]*trans._31;
			v[i].lv=v[i].local[0]*trans._13+v[i].local[1]*trans._23+v[i].local[2]*trans._33;
			tr_y = v[i].local[0]*trans._12+v[i].local[1]*trans._22+v[i].local[2]*trans._32;

		}
	}
	for(i=0; i<v_num; i++)
	{
		if(v[i].lu > max_xleng )
		{
			max_xleng = v[i].lu;
			max_x_i=i;
		}
		if(v[i].lu < min_xleng )
		{
			min_xleng = v[i].lu;
			min_x_i=i;
		}
		if(v[i].lv > max_yleng )
		{
			max_yleng = v[i].lv;
			max_y_i=i;
		}
		if(v[i].lv < min_yleng )
		{
			min_yleng = v[i].lv;
			min_y_i=i;
		}
	}
//	float patch_size = GetIniFile()->PatchSize;	//���Ĵ���..
	
	if( fabs(v_normal[1]) > 0.98 )	//���� �ٴڿ� �������� �����ϰ�
	{
		l_up[2] = v[min_y_i].local[2];	//���� ���� v���� ���� y��ǥ�� ����.
		l_up[0] = v[min_x_i].local[0];	//x��ǥ�� ���Ѵ�.
		fronty =(v[min_x_i].local[2]-100);
		backy =(v[min_x_i].local[2]+100);

        front=v_normal[0]*l_up[0]+v_normal[1]*fronty+v_normal[2]*l_up[2] -v_normal[3];
        back= v_normal[0]*l_up[0] +v_normal[1]*backy +v_normal[2]*l_up[2]  -v_normal[3];
		if((front-back) ==0)
			l_up[1] =v[min_x_i].local[1];
		else
		{
			front = front / (front-back);
			l_up[1] = fronty + front*(backy-fronty);
		}

		r_up[2] = v[min_y_i].local[2];	//����  ���� v���� ���� y��ǥ�� ����.
		r_up[0] = v[max_x_i].local[0];	//x��ǥ�� ���Ѵ�.
		fronty =(v[min_x_i].local[2]-100);
		backy =(v[min_x_i].local[2]+100);
        front=v_normal[0]*r_up[0]+v_normal[1]*fronty+v_normal[2]*r_up[2] -v_normal[3];
        back= v_normal[0]*r_up[0] +v_normal[1]*backy +v_normal[2]*r_up[2]  -v_normal[3];
		if((front-back) ==0)
			r_up[1] =v[min_x_i].local[1];
		else
		{
			front = front / (front-back);
			r_up[1] = fronty + front*(backy-fronty);
		}

		l_down[2] = v[max_y_i].local[2];	//���� ū v���� ���� y��ǥ�� ����.
		l_down[0] = v[min_x_i].local[0];	//x��ǥ�� ���Ѵ�.
		fronty =(v[min_x_i].local[2]-100);
		backy =(v[min_x_i].local[2]+100);
        front=v_normal[0]*l_down[0]+v_normal[1]*fronty+v_normal[2]*l_down[2] -v_normal[3];
        back= v_normal[0]*l_down[0] +v_normal[1]*backy +v_normal[2]*l_down[2]  -v_normal[3];
		if((front-back) ==0)
			l_down[1] =v[min_x_i].local[1];
		else
		{
			front = front / (front-back);
			l_down[1] = fronty + front*(backy-fronty);
		}

		r_down[2] = v[max_y_i].local[2];	//����  ���� v���� ���� y��ǥ�� ����.
		r_down[0] = v[max_x_i].local[0];	//x��ǥ�� ���Ѵ�.
		fronty =(v[min_x_i].local[2]-100);
		backy =(v[min_x_i].local[2]+100);
        front=v_normal[0]*r_down[0]+v_normal[1]*fronty+v_normal[2]*r_down[2] -v_normal[3];
        back= v_normal[0]*r_down[0] +v_normal[1]*backy +v_normal[2]*r_down[2]  -v_normal[3];
		if((front-back) ==0)
			r_down[1] =v[min_x_i].local[1];
		else
		{
			front = front / (front-back);
			r_down[1] = fronty + front*(backy-fronty);
		}
	}
	else
	{
/*
		float t;
		Vector3f tem;
		Vector3fSub(v[0].local,v[1].local,tem);
		float dist1=Vector3fDist(tem);
		Vector3fSub(v[1].local,v[2].local,tem);
		float tests2=Vector3fDist(tem);
*/
		l_up[1] = v[min_y_i].local[1];	//���� ���� v���� ���� y��ǥ�� ����.
		l_up[2] = v[min_x_i].lu*invtrans._13+ tr_y*invtrans._23+v[min_y_i].lv*invtrans._33;
		l_up[0] = v[min_x_i].lu*invtrans._11+ tr_y*invtrans._21+v[min_y_i].lv*invtrans._31;

		r_up[1] = v[min_y_i].local[1];	//����  ���� v���� ���� y��ǥ�� ����.
		r_up[2] = v[max_x_i].lu*invtrans._13+ tr_y*invtrans._23+v[min_y_i].lv*invtrans._33;
		r_up[0] = v[max_x_i].lu*invtrans._11+ tr_y*invtrans._21+v[min_y_i].lv*invtrans._31;

		l_down[1] = v[max_y_i].local[1];	//���� ū v���� ���� y��ǥ�� ����.
		l_down[2] = v[min_x_i].lu*invtrans._13+ tr_y*invtrans._23+v[max_y_i].lv*invtrans._33;
		l_down[0] = v[min_x_i].lu*invtrans._11+ tr_y*invtrans._21+v[max_y_i].lv*invtrans._31;

		r_down[1] = v[max_y_i].local[1];	//����  ���� v���� ���� y��ǥ�� ����.
		r_down[2] = v[max_x_i].lu*invtrans._13+ tr_y*invtrans._23+v[max_y_i].lv*invtrans._33;
		r_down[0] = v[max_x_i].lu*invtrans._11+ tr_y*invtrans._21+v[max_y_i].lv*invtrans._31;

	}

	for(i=0; i<v_num; i++)
	{
		v[i].lv=(v[i].lv-min_yleng)/(max_yleng - min_yleng);
		v[i].lu=(v[i].lu-min_xleng)/(max_xleng - min_xleng);
		v[i].lu = 1.0f-v[i].lu;	//������ �𸣰ڴµ� u�� ��������....
//		v[i].lv = 1.0f-v[i].lv;	//������ �𸣰ڴµ� u�� ��������....
	}
}




//���� ������ ������ �ִ� ����ȣ�� �����ϰ� lu,lv���� ���Ѵ�.�׸��� �簢���� ������� ��ǥ�� �����ش�.
int GetBestPointAndUV(int v_num,_VERTEX_UV v[],Vector3f l_up,Vector3f r_up,Vector3f l_down,Vector3f r_down)	
{
	int i,j,best_v;
	float lu[_MAX_VERTICES][_MAX_VERTICES],lv[_MAX_VERTICES][_MAX_VERTICES],best_squre=9999999;
	float max_xleng[_MAX_VERTICES],max_yleng[_MAX_VERTICES],min_xleng[_MAX_VERTICES],min_yleng[_MAX_VERTICES];
//	Vector3f normal[_MAX_VERTICES][_MAX_VERTICES];

	int is_square=-1;	//��� ������ �������� ����Ʈ�� �켱�̵ȴ�.
	for(i=0; i<v_num; i++)
	{
		double d2,b2,c,x,b,d;
		lu[i][i] = 0;	//������ �Ǵ� ���� ��ǥ�� 0,0�̴�.
		lv[i][i] = 0;

		lu[i][(i+1)%v_num] = (float)GetVertexLength(v[i].local,v[(i+1)%v_num].local);
		c=lu[i][(i+1)%v_num];
		lv[i][(i+1)%v_num] = 0;
		if( fabs(v[i].local[0] - v[(i+1)%v_num].local[0]) < _EPSILON
		|| fabs(v[i].local[1] - v[(i+1)%v_num].local[1]) < _EPSILON
		|| fabs(v[i].local[2] - v[(i+1)%v_num].local[2]) < _EPSILON )
		{
			is_square = 1;
		}
		for(j=(i+2)%v_num; j<((i+2)%v_num)+v_num-2; j++)
		{
			d2 = GetVertexLength2(v[i].local,v[j%v_num].local);
			b2 = GetVertexLength2(v[(i+1)%v_num].local,v[j%v_num].local);				
			d = sqrt(d2);
			b = sqrt(b2);
			lu[i][j%v_num] = (float)((d2-b2+c*c) / (2*c) );
			x = lu[i][j%v_num];
			lv[i][j%v_num] = (float)sqrt((d2-x*x));
		}		
		max_xleng[i]=-9999999,max_yleng[i]=-9999999,min_xleng[i]=9999999,min_yleng[i]=9999999;
		for(j=0; j<v_num; j++)
		{
			max_xleng[i]=max(lu[i][j],max_xleng[i]);
			min_xleng[i]=min(lu[i][j],min_xleng[i]);
			max_yleng[i]=max(lv[i][j],max_yleng[i]);
			min_yleng[i]=min(lv[i][j],min_yleng[i]);
		}
		/*
		for(j=0; j<v_num; j++)	//�����ǥ�� �ٲ��ش�.
		{
			lu[i][j]-=min_xleng;
			lv[i][j]-=min_yleng;
		}*/
		if( (max_xleng[i]-min_xleng[i])*(max_yleng[i]-min_yleng[i]) < best_squre || is_square == 1)
		{
			best_squre=(max_xleng[i]-min_xleng[i])*(max_yleng[i]-min_yleng[i]);
			best_v=i;
		}
		if( is_square == 1 )
			break;
	}

	for(i=0; i<v_num; i++)	//����Ʈ�� ��ǥ�� �־��ش�.
	{
		v[i].lu = lu[best_v][i];
		v[i].lv = lv[best_v][i];
	}

	_VERTEX_UV *bv[40];
	if( v_num > 3 )
	{
		Vector3f tts,tte;
		float min_int=4;
		float min_int2=4;
		int best_point;
		for(i=2; i<v_num; i++)	//����Ʈ ��ǥ�� �߽�����.. 
		{
			Vector3fSub(v[(best_v+1)%v_num].local,v[(best_v+0)%v_num].local,tts);
			Vector3fSub(v[(best_v+i)%v_num].local,v[(best_v+0)%v_num].local,tte);
			Normalize(tts);
			Normalize(tte);
			
			float temp_int= tts[0]*tte[0]+tts[1]*tte[1]+tts[2]*tte[2];
			//if(temp_int < min_int )
			if( fabs(temp_int) < fabs(min_int) )
			{
				min_int = temp_int;
				best_point=i;
			}
		}
		bv[0] = &v[(best_v+0)%v_num];
		bv[1] = &v[(best_v+1)%v_num];
		bv[2] = &v[(best_v+best_point)%v_num];
	}
	else
	{
		for(i=0; i<v_num; i++)	//����Ʈ ��ǥ�� �߽�����.. 
		{
			bv[i] = &v[(best_v+i)%v_num];
		}
	}
//------------------- �簢���� �� ���� ��ǥ�� ���Ѵ�.	
	float t;
	Vector3f s,e;

	//������ ���� 3D ��ǥ�� ���Ѵ�.
	ConvertVectorFromVertex(s,bv[0]);
	ConvertVectorFromVertex(e,bv[1]);
	t=max_xleng[best_v]/bv[1]->lu;
	VectorScale(s,e,t,r_up);		

	//���� ���� 3D ��ǥ�� ���Ѵ�.
	t=(-min_xleng[best_v]+(float)GetVertexLength(bv[0]->local,bv[1]->local))/bv[1]->lu;
	VectorScale(e,s,t,l_up);

	//�Ʒ���ǥ�� ���ϱ����� �غ�
	_LINE_EQUAL line,get_line;

	GetLineEqual(&line,bv[0]->local,bv[1]->local);
	GetCrossLineEqual(&get_line,&line,bv[2]->local);	//x������ ������ �Ǵ� ������ �������� ���Ѵ�.

	//������ �Ʒ� 3D ��ǥ�� ���Ѵ�.
	Vector3f sp,ep,dp,temp_v;
	sp[0] = get_line.a[0];
	sp[1] = get_line.a[1];
	sp[2] = get_line.a[2];
	ConvertVectorFromVertex(ep,bv[2]);
	VectorSubtract(ep,sp,dp);
	VectorPlus(dp,r_up,temp_v);
	t = (max_yleng[best_v]-min_yleng[best_v])/VectorLength(dp);
	VectorScale(r_up,temp_v,t,r_down);

	//���� �Ʒ� 3D ��ǥ�� ���Ѵ�.
	VectorPlus(dp,l_up,temp_v);
	VectorScale(l_up,temp_v,t,l_down);

	for(i=0; i<v_num; i++)	//���� ��� ����Ʈ�� ��ǥ�� �־��ش�.
	{
		lu[best_v][i]-=min_xleng[best_v];
		lv[best_v][i]-=min_yleng[best_v];
		v[i].lu = lu[best_v][i]/(max_xleng[best_v]-min_xleng[best_v]);
		v[i].lv = lv[best_v][i]/(max_yleng[best_v]-min_yleng[best_v]);
		v[i].lu = 1.0f-v[i].lu;	//������ �𸣰ڴµ� u�� ��������....

		v[i].lu=((int)(v[i].lu*1000))/1000.0f;	//�Ҽ��ڸ� ����
		v[i].lv=((int)(v[i].lv*1000))/1000.0f;
	}
	return best_v;
}


int WhatAddVertex(int ev[3],int sv[3])	//����� ������?
{
	if( sv[0] == ev[0] ||  sv[0] == ev[1] || sv[0] == ev[2] )
	{
	}
	else
		return 0;
	if( sv[1] == ev[0] || sv[1] == ev[1] || sv[1] == ev[2] )
	{
	}
	else
		return 1;
	if( sv[2] == ev[0] || sv[2] == ev[1] || sv[2] == ev[2] )
	{
	}
	else
		return 2;

	Error("�̷�����!","");
	return -1;
}


int AdjustTwoMultiply(int leng)
{
#define _MUL	4		//������ ��Ƴ����� ���� ���������� �־...


	if(leng >= 2048  - 2)
		leng=2048 - 2;
	return leng;
/*
	int r_leng;
	if(leng >= 2048 - 1024/_MUL)
		r_leng=2048 + 0;
	else
	if(leng >= 1024 - 512/_MUL)
		r_leng=1024 + 0;
	else
	if(leng >= 512 - 256/_MUL)
		r_leng=512 + 0;
	else
	if(leng >= 256 - 128/_MUL)
		r_leng=256 + 0;
	else
	if(leng >= 128 - 64/_MUL)
		r_leng=128 + 0;
	else
	if(leng >= 64 - 32/_MUL)
		r_leng=64 + 0;
	else
	if(leng >= 32 - 16/_MUL)
		r_leng=32 + 0;
	else
	if(leng >= 16 - 8/_MUL)
		r_leng=16 + 0;
	else
	if(leng >= 8 - 4/_MUL)
		r_leng=8 + 0;
	else
	if(leng >= 4)
		r_leng=4 + 0;
	else
	if(leng >= 2)
		r_leng=2 + 0;
	else
		r_leng=1;

	return r_leng;
*/
}

static void MakeWinding(_WINDING *winding,int w_xl,int w_yl,float w_one_fxl,float w_one_fyl
			,Vector3f l_up,Vector3f r_up,Vector3f l_down,Vector3f r_down)
{
	Vector3f l[2560],r[2560];

	for(int i=0; i<w_yl; i++)
	{
		float t = ((float)i+0.5f)/(float)w_yl;
		VectorScale(l_up,l_down,t,l[i]);
		VectorScale(r_up,r_down,t,r[i]);
	}
	int cnt=0;
	for( i=0; i<w_yl; i++)
	{
		for( int j=0; j<w_xl; j++)
		{
			float t = ((float)j+0.5f)/(float)w_xl;
			VectorScale(r[i],l[i],t,winding[cnt++].pos);
		}
	}
}

void MakeOneWiding(_WINDING *winding,_VERTEX_UV v[],int vertex_num)
{
	winding->pos[0]=0;
	winding->pos[1]=0;
	winding->pos[2]=0;
	for(int i=0; i<vertex_num; i++)
	{
		winding->pos[0] += v[i].local[0];
		winding->pos[1] += v[i].local[1];
		winding->pos[2] += v[i].local[2];
	}
	winding->pos[0]/=(float)vertex_num;
	winding->pos[1]/=(float)vertex_num;
	winding->pos[2]/=(float)vertex_num;
}
void MakeIsOutWinding(_POLYFACE *f,_PATCH *patch,_VERTEX_UV v[],int vertex_num)
{
	for(int i=0; i<patch->xl*patch->yl; i++)
	{
		Vector3f sm;
		sm[0] = patch->winding[i].pos[0];
		sm[1] = patch->winding[i].pos[1];
		sm[2] = patch->winding[i].pos[2];
		for(int j=0; j<vertex_num; j++)
		{
			if( IsInsertVertex(f,v[(j+1)%vertex_num].local,v[j].local,sm) )
//			if( IsInsertVertex(f,v[j].local,v[(j+1)%vertex_num].local,sm) )
			{
				patch->winding[i].is_out_face=1;
				break;
			}
		}
	}
}

BOOL IsLongPoly(int vertex_num,_VERTEX_UV v[])
{
#define _LONG_RATIO	(1/8.0f)

	int i;
	float sec_dist,max_dist=-600000,min_dist=600000;
	Vector3f temp;

	for(i=0; i<vertex_num; i++)
	{
		Vector3fSub(v[i].local,v[(i+1)%vertex_num].local,temp);
		float t_dist=Vector3fDist(temp);
		if(	min_dist > t_dist)
		{
			sec_dist=min_dist;
			min_dist = t_dist;
		}
		if(	max_dist < t_dist)
		{
			max_dist = t_dist;
		}
	}
	if( vertex_num ==3 )
	{
		if( min_dist/max_dist < _LONG_RATIO )
			return TRUE;
		else
			return FALSE;
	}
	else
	{
		if( sec_dist/max_dist < _LONG_RATIO )
			return TRUE;
		else
			return FALSE;
	}
}

//���ε� ����� ���Ѵ�.
void MakeWindingNormal(_POLYFACE *f,_PATCH *patch,_VERTEX_UV v[],int vertex_num
			,Vector3f l_up,Vector3f r_up,Vector3f l_down,Vector3f r_down)
{
	Vector2f uv[40];	//uv��ǥ�� �����Ұ�.
//	float xl,yl;
	Vector3f lpos[4096];
	Vector3f rpos[4096];
	float u_lpos[4096];
	float u_rpos[4096];
	int i,j;

	//������� ����� ���ٸ� 
	for(i=0; i<vertex_num; i++)
	{
		if( v[0].normal[0] == v[i].normal[0] 
		&& v[0].normal[1] == v[i].normal[1] 
		&& v[0].normal[2] == v[i].normal[2] )
		{
		}
		else
			break;
	}
	if( i == vertex_num )
	{
		for( i=0; i<patch->xl*patch->yl; i++)
		{
			patch->winding[i].normal[0]=v[0].normal[0];
			patch->winding[i].normal[1]=v[0].normal[1];
			patch->winding[i].normal[2]=v[0].normal[2];
		}
		return;
	}

	for(i=0; i<vertex_num; i++)
	{
		uv[i][0]=v[i].lu;
		uv[i][1]=v[i].lv;
	}
int _CAL_YL=1024;

	for(j=0; j<vertex_num; j++)
	{
		if( uv[(j+1)%vertex_num][1]-uv[j][1]>= 0)
		{
			for(float f = uv[j][1]*(float)_CAL_YL+.5f; f<uv[(j+1)%vertex_num][1]*(float)_CAL_YL+.5f ;f+=1.0f)
			{
				float one=(f-uv[j][1]*(float)_CAL_YL)/(float)(_CAL_YL*(uv[(j+1)%vertex_num][1]-uv[j][1]));

				u_rpos[(int)(f)] = (uv[(j+1)%vertex_num][0]-uv[j][0])*one + uv[j][0];
				rpos[(int)(f)][0] = (v[(j+1)%vertex_num].normal[0]-v[j].normal[0])*one + v[j].normal[0];
				rpos[(int)(f)][1] = (v[(j+1)%vertex_num].normal[1]-v[j].normal[1])*one + v[j].normal[1];
				rpos[(int)(f)][2] = (v[(j+1)%vertex_num].normal[2]-v[j].normal[2])*one + v[j].normal[2];
			}
		}
		else
		{
			for(float f = uv[(j+1)%vertex_num][1]*(float)_CAL_YL+.5f; f<uv[j][1]*(float)_CAL_YL+.5f ;f+=1.0f)
			{
				float one=(f-uv[(j+1)%vertex_num][1]*(float)_CAL_YL)/(float)(_CAL_YL*(uv[j][1]-uv[(j+1)%vertex_num][1]));

				u_lpos[(int)(f)] = (uv[j][0]-uv[(j+1)%vertex_num][0])*one + uv[(j+1)%vertex_num][0];
				lpos[(int)(f)][0] = (v[j].normal[0]-v[(j+1)%vertex_num].normal[0])*one + v[(j+1)%vertex_num].normal[0];
				lpos[(int)(f)][1] = (v[j].normal[1]-v[(j+1)%vertex_num].normal[1])*one + v[(j+1)%vertex_num].normal[1];
				lpos[(int)(f)][2] = (v[j].normal[2]-v[(j+1)%vertex_num].normal[2])*one + v[(j+1)%vertex_num].normal[2];
			}
		}
	}
	for(i=0; i<patch->yl; i++)
	{
		int lpoint = (int)(((i+0.5f)*_CAL_YL)/(float)patch->yl);
		for(j=0; j<patch->xl; j++)
		{
			Vector3f final;
			float slope;
			if(u_rpos[lpoint]-u_lpos[lpoint] < 0.1 )
				slope = 0;
			else
				slope = ((j+0.5f)/(float)patch->xl-u_lpos[lpoint])/(u_rpos[lpoint]-u_lpos[lpoint]);
			slope = min(slope,1.0f);
			slope = max(slope,0.0f);
			final[0] = (rpos[lpoint][0]-lpos[lpoint][0])*slope + lpos[lpoint][0];
			final[1] = (rpos[lpoint][1]-lpos[lpoint][1])*slope + lpos[lpoint][1];
			final[2] = (rpos[lpoint][2]-lpos[lpoint][2])*slope + lpos[lpoint][2];
			Normalize(final);

			patch->winding[(i)*patch->xl+j].normal[0] = final[0];
			patch->winding[(i)*patch->xl+j].normal[1] = final[1];
			patch->winding[(i)*patch->xl+j].normal[2] = final[2];
		}
	}
/*
	for(j=0; j<vertex_num; j++)
	{
		if( uv[(j+1)%vertex_num][1]-uv[j][1]>= 0)
		{
			for(float f = uv[j][1]*(float)patch->yl+.5f; f<uv[(j+1)%vertex_num][1]*(float)patch->yl+.5f ;f+=1.0f)
			{
				float one=f/(float)(patch->yl);

				u_rpos[(int)(f)] = (uv[(j+1)%vertex_num][0]-uv[j][0])*one + uv[j][0];
				rpos[(int)(f)][0] = (v[(j+1)%vertex_num].normal[0]-v[j].normal[0])*one + v[j].normal[0];
				rpos[(int)(f)][1] = (v[(j+1)%vertex_num].normal[1]-v[j].normal[1])*one + v[j].normal[1];
				rpos[(int)(f)][2] = (v[(j+1)%vertex_num].normal[2]-v[j].normal[2])*one + v[j].normal[2];
			}
		}
		else
		{
			for(float f = uv[(j+1)%vertex_num][1]*(float)patch->yl+.5f; f<uv[j][1]*(float)patch->yl+.5f ;f+=1.0f)
			{
				float one=f/(float)(patch->yl);

				u_lpos[(int)(f)] = (uv[j][0]-uv[(j+1)%vertex_num][0])*one + uv[(j+1)%vertex_num][0];
				lpos[(int)(f)][0] = (v[j].normal[0]-v[(j+1)%vertex_num].normal[0])*one + v[(j+1)%vertex_num].normal[0];
				lpos[(int)(f)][1] = (v[j].normal[1]-v[(j+1)%vertex_num].normal[1])*one + v[(j+1)%vertex_num].normal[1];
				lpos[(int)(f)][2] = (v[j].normal[2]-v[(j+1)%vertex_num].normal[2])*one + v[(j+1)%vertex_num].normal[2];
			}
		}
	}
	for(i=0; i<patch->yl; i++)
	{
		for(j=0; j<patch->xl; j++)
		{
			Vector3f final;
			float slope = ((j+0.5f)/(float)patch->xl-u_lpos[i])/(u_rpos[i]-u_lpos[i]);
//			slope = min(slope,1.0f);
//			slope = max(slope,0.0f);
			final[0] = (rpos[i][0]-lpos[i][0])*slope + lpos[i][0];
			final[1] = (rpos[i][1]-lpos[i][1])*slope + lpos[i][1];
			final[2] = (rpos[i][2]-lpos[i][2])*slope + lpos[i][2];
			Normalize(final);

			patch->winding[(i)*patch->xl+j].normal[0] = final[0];
			patch->winding[(i)*patch->xl+j].normal[1] = final[1];
			patch->winding[(i)*patch->xl+j].normal[2] = final[2];
		}
	}
*/
}


_LFACE **Real_lf;

_LFACE **GetLFace( void )//���� ����Ʈ�� ���̽� ����ü�����͸� ����
{
	return Real_lf;
}
void MakePatchAndWinding(_MESH *mesh,int option)	// ��Ƽ ������Ʈ �޽��� ���ڷ�....
{
	int i,j,k,cnt,m,l;
	_PATCH *patch;
	_POLYFACE *f;
	//*cf;
//	Vector4f cn,n;

	_LFACE **l_lf=(_LFACE **)Dmalloc(sizeof(_LFACE*)*mesh->objectnum);
	for(k=0; k<mesh->objectnum; k++)
	{
		l_lf[k]=(_LFACE *)Dmalloc(sizeof(_LFACE)*mesh->object[k].facenum);
		memset(l_lf[k],0,sizeof(_LFACE)*mesh->object[k].facenum);
	}

	Real_lf = l_lf;

	_INIFILE *ini=GetIniFile();
	SetWindingSize(ini->PatchSize);	//ȯ�����Ͽ� �ִ� ���ε� ������� �Ѵ�.

	for(k=0; k<mesh->objectnum; k++)
	{
		if( mesh->object[k].type & _OBJECT_TYPE_COMPOSITE )	//�̸� ������ ����Ʈ����...
		{
			for(i=0;i<mesh->object[k].facenum;i++)
				mesh->object[k].p_face[i].l_mtlid = (SHORT)mesh->object[k].composite_id;
		}
		else
		{
			for(i=0;i<mesh->object[k].facenum;i++)
				mesh->object[k].p_face[i].l_mtlid=-1;
		}
	}

//---��ġ�� �����Ѵ�.
	cnt=0;
	for(k=0; k<mesh->objectnum; k++)
	{
		for( i=0; i<mesh->object[k].facenum; i++)
		{
	//		if(mesh->object[k].p_face[i].mtlid==-1)	//������ �ȵȰ��� ����.
	//			continue;

			patch = (_PATCH *)Dmalloc(sizeof(_PATCH));
			memset(patch,0,sizeof(_PATCH));

			patch->mtlid = mesh->object[k].p_face[i].mtlid;

			memcpy(patch->fnormal,mesh->object[k].p_face[i].f_normal,sizeof(Vector4f));
			l_lf[k][i].patch = patch;				//patch ���� 
		}
	}

//----���ε��� ���� �� �����Ѵ�.
	for(k=0; k<mesh->objectnum; k++)
	{
		if( mesh->object[k].type & _OBJECT_TYPE_COMPOSITE )	//�̸� ������ ����Ʈ����...
		{
			for( i=0; i<mesh->object[k].facenum; i++)
			{
				l_lf[k][i].patch->xl=0;	//��� �����.
				l_lf[k][i].patch->yl=0;
				l_lf[k][i].patch->winding=NULL;
			}
			continue;
		}
		if( mesh->object[k].type & _SHADOW_RECEIVE_OFF )	//����Ʈ���� ������ ���ؽ�����Ʈ
		{
			int vertex_num=0;

			for( i=0; i<mesh->object[k].facenum; i++)
			{
				f = &mesh->object[k].p_face[i];
				vertex_num=mesh->object[k].p_face[i].vertex_num;

				// ���ε������� ���ѵ� ������ ���ε���ǥ�� ������ش�.
				int winding_xl,winding_yl;
				
				winding_xl=vertex_num;
				winding_yl=1;

				l_lf[k][i].patch->xl=winding_xl;	//���ε� ������ ���ߴ�.
				l_lf[k][i].patch->yl=winding_yl;
				l_lf[k][i].patch->winding=(_WINDING *)Dmalloc(sizeof(_WINDING)*winding_xl*winding_yl);
				memset(l_lf[k][i].patch->winding,0,sizeof(_WINDING)*winding_xl*winding_yl);

				Vector3f all_n;
				for(j=0; j<vertex_num; j++)
				{
					memcpy(l_lf[k][i].patch->winding[j].pos,&mesh->object[k].vertex[f->vertex_id[j]].local,sizeof(Vector3f));
					all_n[0]=0;
					all_n[1]=0;
					all_n[2]=0;
					for( l=0; l<mesh->object[k].facenum; l++)
					{
						_POLYFACE *pf = &mesh->object[k].p_face[l];
						for(m=0; m<vertex_num; m++)
						{
							if( mesh->object[k].p_face[i].vertex_id[j] == pf->vertex_id[m])
							{
								all_n[0]+=pf->f_normal[0];
								all_n[1]+=pf->f_normal[1];
								all_n[2]+=pf->f_normal[2];
							}
						}
					}
					if( all_n[0]==0 && all_n[1]==0 && all_n[2]==0 )	//���� �̷�����������..
						all_n[2]=1;
					Normalize(all_n);
					Vector3fCopy(all_n,l_lf[k][i].patch->winding[j].normal);
				}
			}
			continue;
		}
		for( i=0; i<mesh->object[k].facenum; i++)
		{
			_VERTEX_UV v[_MAX_VERTICES];
			int vertex_num=0;

			f = &mesh->object[k].p_face[i];
			vertex_num=mesh->object[k].p_face[i].vertex_num;

			for(j=0; j<vertex_num; j++)
			{
				v[j].local[0]=mesh->object[k].vertex[mesh->object[k].p_face[i].vertex_id[j]].local[0];
				v[j].local[1]=mesh->object[k].vertex[mesh->object[k].p_face[i].vertex_id[j]].local[1];
				v[j].local[2]=mesh->object[k].vertex[mesh->object[k].p_face[i].vertex_id[j]].local[2];
				v[j].lu=0;
				v[j].lv=0;
				v[j].normal[0] = mesh->object[k].p_face[i].v_normal[j][0];
				v[j].normal[1] = mesh->object[k].p_face[i].v_normal[j][1];
				v[j].normal[2] = mesh->object[k].p_face[i].v_normal[j][2];
			}	
			//���� ������ ������ �ִ� ����ȣ�� �����ϰ� lu,lv���� ���Ѵ�,2D���� �簢������ 3D ��ǥ�� ���Ѵ�.
			float l_up[6],r_up[6],l_down[6],r_down[6],temp_v[6];	//3,4,5���ٰ� ��� �־����...

			if( IsLongPoly(vertex_num,v) )	//�������� ���������ΰ�?
				GetBestPointAndUV(vertex_num,v,l_up,r_up,l_down,r_down);	
			else
				GetSquarePointAndUV(f->f_normal,vertex_num,v,l_up,r_up,l_down,r_down);	

			// face lu,lv�� �ִ´�.
			for(j=0; j<vertex_num; j++)
			{
				mesh->object[k].p_face[i].lu[j]=v[j].lu;
				mesh->object[k].p_face[i].lv[j]=v[j].lv;
			}
			// ���ε������� ���ѵ� ������ ���ε���ǥ�� ������ش�.
			int winding_xl,winding_yl;
			float winding_one_fxl,winding_one_fyl;
			
			VectorSubtract(r_up,l_up,temp_v);
			winding_xl=(int)(VectorLength(temp_v)/_MAX_WINDING_XL+0.999999f);	//xl�� ���Ѵ�.
			winding_xl=AdjustTwoMultiply(winding_xl);	//2�� ���� ����� ����
			winding_one_fxl=VectorLength(temp_v)/(float)winding_xl;

			VectorSubtract(r_up,r_down,temp_v);
			winding_yl=(int)(VectorLength(temp_v)/_MAX_WINDING_YL+0.999999f);	//yl�� ���Ѵ�.
			winding_yl=AdjustTwoMultiply(winding_yl);	//2�� ���� ����� ����
			winding_one_fyl=VectorLength(temp_v)/(float)winding_yl;

			if( winding_xl > 1024 || winding_yl > 1024)
				int dfddfd=0;
			if( option & _NO_SPLIT )
			{
				winding_xl=1;
				winding_yl=1;
			}
			l_lf[k][i].patch->xl=winding_xl;	//���ε� ������ ���ߴ�.
			l_lf[k][i].patch->yl=winding_yl;
//			l_lf[k][i].patch->sqare	= winding_one_fxl*winding_one_fyl;
			l_lf[k][i].patch->winding=(_WINDING *)Dmalloc(sizeof(_WINDING)*winding_xl*winding_yl);		
			memset(l_lf[k][i].patch->winding,0,sizeof(_WINDING)*winding_xl*winding_yl);

			MakeWinding(l_lf[k][i].patch->winding,winding_xl,winding_yl,winding_one_fxl,winding_one_fyl
				,l_up,r_up,l_down,r_down);	//���� ���ε� ����.
			if( winding_xl == 1 && winding_yl == 1 )	//���ε��� �Ѱ� �϶��� �׸��� �߾��̴�.
				MakeOneWiding(l_lf[k][i].patch->winding,v,vertex_num);
			MakeIsOutWinding(f,l_lf[k][i].patch,v,vertex_num);	//������ ���ε��� ���� ����ִ°�...
			MakeWindingNormal(f,l_lf[k][i].patch,v,vertex_num,l_up,r_up,l_down,r_down);	//������ ���ε��� ����� ���Ѵ�.
			cnt++;
		}
	}
}

#include "r3blockmat.h"
extern R3BlockBsp *TempCBlockBsp;


void MakeVertexPatchAndWinding(_MESH *mesh)	// ���ڸ޽��� ���ڷ�.... ��ī�� �ڽ��� ���Ѱ�...
{
	int i,j,k;
	_PATCH *patch;
	_POLYFACE *f;

	_LFACE **lf=(_LFACE **)Dmalloc(sizeof(_LFACE*)*mesh->objectnum);
	Real_lf = lf;

//---��ġ�� �����Ѵ�.
	for(k=0; k<mesh->objectnum; k++)
	{
		lf[k]=(_LFACE *)Dmalloc(sizeof(_LFACE)*mesh->object[k].facenum);
		memset(lf[k],0,sizeof(_LFACE)*mesh->object[k].facenum);
		for( i=0; i<mesh->object[k].facenum; i++)
		{
			patch = (_PATCH *)Dmalloc(sizeof(_PATCH));
			memset(patch,0,sizeof(_PATCH));
	//		patch->emission[0] = 0.0;
	//		patch->emission[1] = 0.0;
	//		patch->emission[2] = 0.0;
			patch->mtlid = mesh->object[k].p_face[i].mtlid;
			memcpy(patch->fnormal,mesh->object[k].p_face[i].f_normal,sizeof(Vector4f));
			lf[k][i].patch = patch;				//patch ���� 
		}
	}

//----���ε��� ���� �� �����Ѵ�.
	for(k=0; k<mesh->objectnum; k++)
	{
		for( i=0; i<mesh->object[k].facenum; i++)
		{
			int vertex_num=0;

			f = &mesh->object[k].p_face[i];
			vertex_num=mesh->object[k].p_face[i].vertex_num;

			// ���ε������� ���ѵ� ������ ���ε���ǥ�� ������ش�.
			int winding_xl,winding_yl;
			
			winding_xl=vertex_num;
			winding_yl=1;

			lf[k][i].patch->xl=winding_xl;	//���ε� ������ ���ߴ�.
			lf[k][i].patch->yl=winding_yl;
			lf[k][i].patch->winding=(_WINDING *)Dmalloc(sizeof(_WINDING)*winding_xl*winding_yl);		
			memset(lf[k][i].patch->winding,0,sizeof(_WINDING)*winding_xl*winding_yl);
			for(j=0; j<vertex_num; j++)
			{
				memcpy(lf[k][i].patch->winding[j].pos,&mesh->object[k].vertex[f->vertex_id[j]].local,sizeof(Vector3f));
				Vector3fCopy(f->f_normal,lf[k][i].patch->winding[j].normal);
			}
		}
	}
}
