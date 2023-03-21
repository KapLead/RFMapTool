#ifndef __R3PROFILER_H__
#define __R3PROFILER_H__

/*�������� ����

	BeginR3Profiler();
	StartR3ProfilerName("ĳ���� �����ӹ���");
	...
	StartR3ProfilerName("��� �����ӹ���");
	...
	StartR3ProfilerName("ĳ���ͷ���");
	...
	StartR3ProfilerName("��添��");
	...
	EndR3Profiler();

	���ۺ��� �������� 100���� ����. 
*/

typedef struct{
	char mName[32];
	UINT64 m_qwStartTicks;
	float m_TotalTime;
	float m_LoopTime;
}_PROFILE_ELMENT;

#define _TOTAL_ELEMENT	64

class CR3Profiler{
protected :
	_PROFILE_ELMENT m_ProfileTime[_TOTAL_ELEMENT];	//
	DWORD m_NowID,m_TotalIDNum;

	BOOL m_bTimerInitialized;
	UINT64 m_qwTicks;
	UINT64 m_qwStartTicks;
	UINT64 m_qwTicksPerSec;
	UINT64 m_qwTicksPerFrame;
	float m_fTicksPerSec,m_fSecsPerFrame;
	BOOL m_bIsBegin;
	float m_TotalTime;


	UINT64 GetTicks();
	float IntervalTime(UINT64 old_qw,UINT64 qw);

public:
	CR3Profiler();
	~CR3Profiler();

	void BeginR3Profiler();
	void CleanR3Profiler();		//��ŻŸ���� �����. ÷���� �ٽð���ϱ����ؼ�..
	void EndR3Profiler();
	void StartR3ProfilerName(char *name);

	DWORD GetR3ProfilerTotalNum();
	char *GetR3ProfilerIDName(DWORD id);
	float GetR3ProfilerIDPercent(DWORD id);
	float GetR3ProfilerIDTotalTime(DWORD id);
	float GetR3ProfilerIDLoopTime(DWORD id);
};

#endif