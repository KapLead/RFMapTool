#ifndef __JMALLOC_H__
#define __JMALLOC_H__




/*
int InitJmalloc(int size);
void ReleaseJmalloc(void);
unsigned char *Jmalloc(int num);	//�ܼ��� �޸𸮸� �Ҵ��� �ش�.

void SetFreePointer();				// ���������͸� �����س��´�.
void FreePointer(void *buf);		// ���õ� ���������ͷ� ������Ų��.
int GetTotalFreeJmallocSize();		// ó�� ��Ҵ� �޸��� ũ�⸦ �˷��ش�.(byte ����)
int GetNowFreeJmallocSize();		// jmalloc�� ũ�⿡�� ���� �ִ� ���� �˷��ش�.(byte ����)
int GetNowJmallocSize();			// ���� ������ �޸� ũ�⸦ �˷��ش�..(byte ����)
*/	//���� �Ⱦ���..


//---------���� �޸� ������ ���� ��.
void *Dmalloc(int size);
void Dfree(void *fptr);
int GetDmallocSize();
void *ReAlloc(void *buf,int old_size,int size);

#endif
