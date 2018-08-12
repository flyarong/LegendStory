#ifndef __SkillMG__H__
#define __SkillMG__H__

#include "cocos2d.h"

class SkillMG
{
public:
	enum class Name
	{
		Sword,
		Fist
	};
	enum class SkillPart
	{
		Skill1,
		Skill2,
		Skill3,
		Normal
	};
	enum class SkillType
	{
		Skill,
		Normal
	};
public:
	static SkillMG* getInstance();
	//*�^��HUDlayer�����s�Ϥ����|
	std::string getCurrentSkillPath(SkillMG::SkillPart skillPart);
	//*�ǤJ�O�ޯ��٬O�Ϥ�,�åB�O�ĴX���q��
	//*�^�ǹ������D���ʵe�W��
	std::string getCurrentSkillAnimationName(SkillMG::SkillType skillType,std::string stageStr);
	virtual ~SkillMG();
private:
	static SkillMG* static_skillMG;
};

#endif
