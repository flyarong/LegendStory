#ifndef __MissionMG_H__
#define __MissionMG_H__

#include "cocos2d.h"

class MissionMG
{
public:
	enum class MainName
	{
		FirstInChiuanJen,
		FirstMeetZhangZhen,
		FirstMeetZhangQing,
		FirstMeetZhangHen,
		FirstMeetZhangYin,
		FirstMeetZhangFong,
		FirstMeetZhangWo,
		MasterFirstOrder,
		ZhangQingTip,
		FirstAttack,
		REFirstAttack,
		ZhangZhenZombieStart,
		GoToCheckCave,
		KillZombie,
		LeaveCave,
		NotTheEnd
	};
	enum class State
	{
		None,
		GoForestField,
		GoChiuanJen,
		GoChiuanJenMount,
		GoCave,
		GoLuoYangCity,
		TalkZhangQing,
		TalkZhangYin,
		TalkZhangZhen,
		TalkZhangHen,
		TalkZhangFong,
		TalkZhangWo,
		KillSmallThief,
		KillZombie
	};
public:
	static MissionMG* getInstance();
	MissionMG();
	//* ���ȧ����ؼ�
	State getCurrentMissionCompleteState();
	//* ���ȥؼмƶq�W�[����
	State getCurrentMissionAmountAddState();
	//* ���ȥؼмƶq
	int getCurrentMissionTargetAmount();
	//* ���ȼ��D
	std::string getCurrentMissionTitle();
	//* ���Ȥ��e
	std::string getCurrentMissionContent();
	//* ���ȴ���
	std::string getCurrentMissionTip();
	//* ���ȧ�����ܥN��
	std::string getCurrentMissionDialogCode();
	//* �������Ȯ�,�ˬd�W�@�ӥ��ȬO�_����o���������y�C
	void checkCompleteMissionReward();
	void NextMission();
	void PrevMission();
	//* �ǤJ�����A�O���ȥؼЪ����A��,�^��true,�_�h�^��false
	bool isCompleteMission(State newState);
	//* �����e���ȶi�׽s��
	inline int getCurrentMissionTag() { return _currentMissionTag; };
private:
	//*�N�r��State �ন Enum State
	State switchStringStateToState(std::string stringState);
	//*�N�s�ɤ�������IDŪ�X�ӫ�A�ഫ����Json�ݪ����ȥN��
	static std::string switchIDToName(int MissionID);
	static MissionMG* static_missionMG;
	int _currentMissionTag;
private:
};

#endif
