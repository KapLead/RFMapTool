#ifndef __B_SORT_H__
#define __B_SORT_H__

#include <windows.h>

//��� ��Ʈ or byte��Ʈ :100�� �̻��̸� ���󿡼� ���� ���� ���ù��....
//temp_index �� temp_value�� sort_index �� sort_value temp �迭�̴�. 
//��� ���� ������ �ݵ�� ���ƾ��Ѵ�.

// ����� ������������ sort_index�� ���õ� �ε������� �־��ش�. 
// %���������� sort_value���� ���õ��� �ʴ´�. ���ĵ� ������ �Ⱦ��ϱ�....
 

void ByteSortForShort(DWORD sort_num,DWORD *sort_index,short *sort_value
					  ,DWORD *temp_index,short *temp_value);

#endif
