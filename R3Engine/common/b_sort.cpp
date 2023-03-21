#include "b_sort.h"

WORD Sortcount[260];
//sort���� �������� �ӽ����庯��
/*
#define _MAXPOLYNUM 20000
DWORD Onearray[_MAXPOLYNUM];
// �������ȣ�� ��� �����Һ���
short Twoarray[_MAXPOLYNUM];
*/

void ByteSortForShort(DWORD sortface,DWORD *facesort,short *facesortz,DWORD *Onearray,short *Twoarray)
{
	DWORD i;

    memset((void *)Sortcount,0,260*2);
    for(i=0 ;i<sortface; i++)
	{
        Sortcount[ (facesortz[i]&255)+1 ]+=1;
	}
	for(i=0; i<256; i++)
	{
        Sortcount[i+1] += Sortcount[i];
	}
    for(i=0; i<sortface; i++)
	{
        Onearray[ Sortcount[ facesortz[i]&0xff ]] = facesort[i];
        Twoarray[ Sortcount[ facesortz[i]&0xff ]] = facesortz[i];
        Sortcount[ facesortz[i]&0xff ]+=1;
	}
    memset((void *)Sortcount,0,260*2);
    for(i=0 ;i<sortface; i++)
	{
        Sortcount[ ((facesortz[i]&0xff00)>>8) +1] += 1;
	}
	for(i=0; i<256; i++)
	{
        Sortcount[i+1] += Sortcount[i];
	}
    for(i=0; i<sortface; i++)
	{
        facesort[ Sortcount[ (Twoarray[i]&0xff00)>>8 ]] = Onearray[ i ];
        Sortcount[ (Twoarray[i]&0xff00)>>8 ]+=1;
	}
}

/*
DWORD stTempz[20000];
void ByteSortFace(DWORD sortface,DWORD *facesort,DWORD *facesortz)
{
	DWORD i;

	memcpy(stTempz,facesortz,sortface*sizeof(DWORD));
	//ù° byte
    memset((void *)dwSortcount,0,260*4);
    for(i=0 ;i<sortface; i++)
	{
        dwSortcount[ (facesortz[i]&255)+1 ]+=1;
	}
	for(i=0; i<256; i++)
	{
        dwSortcount[i+1] += dwSortcount[i];
	}
    for(i=0; i<sortface; i++)
	{
        dwOnearray[ dwSortcount[ facesortz[i]&0xff ]] = facesort[i];
        dwTwoarray[ dwSortcount[ facesortz[i]&0xff ]] = facesortz[i];
        dwSortcount[ facesortz[i]&0xff ]+=1;
	}
	//��° byte...
    memset((void *)dwSortcount,0,260*4);
    for(i=0 ;i<sortface; i++)
	{
        dwSortcount[ ((facesortz[i]&0xff00)>>8) +1] += 1;
	}
	for(i=0; i<256; i++)
	{
        dwSortcount[i+1] += dwSortcount[i];
	}
    for(i=0; i<sortface; i++)
	{
        facesort[ dwSortcount[ (dwTwoarray[i]&0xff00)>>8 ]] = dwOnearray[ i ];
        stTempz[ dwSortcount[ (dwTwoarray[i]&0xff00)>>8 ]] = dwTwoarray[ i ];
        dwSortcount[ (dwTwoarray[i]&0xff00)>>8 ]+=1;
	}
	//��° ����Ʈ
    memset((void *)dwSortcount,0,260*4);
    for(i=0 ;i<sortface; i++)
	{
        dwSortcount[ ((facesortz[i]&0xff0000)>>16) +1] += 1;
	}
	for(i=0; i<256; i++)
	{
        dwSortcount[i+1] += dwSortcount[i];
	}
    for(i=0; i<sortface; i++)
	{
        dwOnearray[ dwSortcount[ (facesortz[i]&0xff0000)>>16 ]] = facesort[i];
        dwTwoarray[ dwSortcount[ (facesortz[i]&0xff0000)>>16 ]] = facesortz[i];
        dwSortcount[ (facesortz[i]&0xff0000)>>16 ]+=1;
	}
	//��° ����Ʈ
    memset((void *)dwSortcount,0,260*4);
    for(i=0 ;i<sortface; i++)
	{
        dwSortcount[ ((facesortz[i]&0xff000000)>>24) +1] += 1;
	}
	for(i=0; i<256; i++)
	{
        dwSortcount[i+1] += dwSortcount[i];
	}
    for(i=0; i<sortface; i++)
	{
        facesort[ dwSortcount[ (dwTwoarray[i]&0xff000000)>>24 ]] = dwOnearray[ i ];
        dwSortcount[ (dwTwoarray[i]&0xff000000)>>24 ]+=1;
	}
}
*/