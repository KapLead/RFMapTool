#ifndef __MOVE_CAMERA_H__
#define __MOVE_CAMERA_H__

typedef struct{
	float pos[3],point[3];
	float th,ph,rho;
	float speed;
	float step;
	BOOL move_camera;	//true�� �̵��Ҽ� �ִ� ī�޶�
	BOOL is_free;		//true�� ����ī�޶�.. false�� Ÿ��ī�޶�..
	float tr_matrix[4][4];	//view
}_MOVE_CAMERA;

void CalculateMoveCamera(_MOVE_CAMERA *camera);
void ReMoveCamera(_MOVE_CAMERA *cam);


#endif