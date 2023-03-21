#ifndef __MESHUTIL_H__
#define __MESHUTIL_H__


void SetWorldMesh(_MESH *mesh,_MATERIAL *material);
_MESH *ConvMeshDX8Optimize(_MESH *mesh,_MATERIAL *material);	//���͸��� �������� ���� ���ο� 

_MESH *ConvMeshTriToPoly(_MESH *mesh);	//��¥ ������Ʈ�� �ﰢ�� �޽��� ������ �޽��� ��ȯ
void SetMeshFaceNormal(_MESH *mesh,_MESH *org_mesh);
_MATERIAL *ConvOneMaterial(_MATERIAL *material);	//�������� ���͸����� ������ ��¥ ���͸����� ���´�.
void AdjustMeshMaterial(_MESH *mesh,_MATERIAL *material);	//�Ⱦ��� ���͸���� ���̾��� �ؽ��ĸ� ����...
void CalculateMeshSmooth(_MESH *mesh); // �������׷��� ����Ѵ�.
void GetInvScaleMatrix(_MESH *mesh);	//�ι��� ������ ��Ʈ������ ���Ѵ�.
void SetLocalCoodinate(_OBJECT *object);	//������ǥ�� �ٲ۴�.
void SetWorldCoodinate(_OBJECT *object);	//������ǥ�� �ٲ۴�.

DWORD GetFaceNumFromMesh(_MESH *mesh);	//�޽��κ��� �鰹���� ����.
_MESH *ConvOneMesh(_MESH *mesh,_MATERIAL *material);	//�ϳ��� ������Ʈ�� ����Ʈ,���͸����� ��������...
_MATERIAL *MakeVolumeFogObject(_MESH *mesh,_MATERIAL *mat);	//���� ���׿����� ��������� �����Ѵ�.

void GetCompositeMat(_MATERIAL *one_mat,_MATERIAL **composite_mat);	// ���͸��󿡼� composite���͸���� �׳� ���͸�����  �����س���...
void SetMeshCompositeMat(_MESH *mesh, _MATERIAL *org_mat);	//Composite������Ʈ�� ���. �� ������ ������Ʈ�� �Ӽ��� �־��ش�.

#endif