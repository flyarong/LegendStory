#ifndef __DamageMG__H__
#define __DamageMG__H__

#include "cocos2d.h"

class DamageMG : public cocos2d::Layer
{
public:
	enum class Direction
	{
		ToLeft,
		ToRight
	};
public:
	CREATE_FUNC(DamageMG);
	//*��ܶˮ` 1:��ܼƦr 2:�Ʀr���ʤ�V 3:�ǤJ�Ǫ��y��
	void showDamage(int damage,Direction direction, cocos2d::Vec2 monsterPos);
private:
	virtual bool init();
	cocos2d::Size visible;
	cocos2d::Vec2 origin;
	void CallFunc(cocos2d::Label* damageLabel);
};

#endif
