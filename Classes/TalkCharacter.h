#ifndef __TalkCharacter_H__
#define __TalkCharacter_H__
#include "MissionMG.h"
#include "Npc\NPCFactory.h"
#include "cocos2d.h"
using namespace cocos2d;
class NPC;
//��Dialog�Ϊ���H���f
class TalkCharacter :public cocos2d::Node{
public:
	enum class FacialType
	{
		NM,	//���q�㲴
		NT,	//���q����
		AG,	//�ͮ𻡸�
		SD,	//�ˤ߻���
		BL,	//�R�廡��
		HP,	//��������
		DO,	//���`��1
		DT,	//���`��2
		DB, //�̲�����
		CF  //�x�b����
	};
public:
	virtual void setFacialType(FacialType facialType) = 0;
	virtual void setFacialType(std::string facialNameStr) = 0;
	virtual void updateArmatureFacial() = 0;
	virtual MissionMG::State getMissionState() = 0;
	virtual std::string getDialogCode() = 0;
	virtual NPC* getNPCInstance()=0;
	virtual void doIdleAnimation() = 0;
public:
	//*example:ZQ
	std::string _nameCode;
	//*example:ZhangQing
	std::string _name;
	//*example:�}����
	std::string _chineseName;
	//*��ܥN��
	std::string _DialogCode;
};
#endif
