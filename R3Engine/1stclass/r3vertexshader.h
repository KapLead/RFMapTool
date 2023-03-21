#ifndef _R3VERTEXSHADER_H
#define _R3VERTEXSHADER_H

//--------------------------------------------------------------------------------------
//���ؽ� ���̴� �������� inupt ����.   ���̴����� �Ⱦ��� ���� �ʴ°� �⺻...
// c0-c3 ��Ʈ���� 1 
// c4-c7 ��Ʈ���� 2
// c8	����
// c12- ���ؽ� ���̴��� ��Ȳ�� ���� ����.

BOOL LoadVertexShaderList();
void SetWorldViewMatrixVS(float world_mat[4][4]);	//������Ʈ������ ��ƼƼ����� �Ѹ������� ������ϹǷ�.
void SetGrassVS(float add_frame,float factor[2],float alpha,DWORD map_color);
void ReleaseVertexShaderList();


#endif