#ifndef __BattleSoldierFactory_SCENE_H__
#define __BattleSoldierFactory_SCENE_H__
#include "cocos2d.h"
#include "Monster\BattleMode\SoldierAI.h"
#include "DefinitionBattleMode.h"
NS_BATTLEMODE_BEGAN
class Legion;
class SoldierAI;
class BattleSoldierFactory : public cocos2d::Sprite
{
public:
	struct AnimationInterval
	{
		float attackInterval;
		float IdleInterval;
		float RunInterval;
		float DeathInterval;
	};
	struct SoldierState
	{
		bool isRun=false;
		bool isIdle = false;
		bool isAttack = false;
		bool isDeath = false;
		//*false face right  true face left
		bool faceDirection = false;
		bool isArriveAssignPosition = true;
	};
	enum
	{
		ATTACK_TAG,
		IDLE_TAG,
		DEATH_TAG,
		RUN_TAG,
		STAND_POINT_TAG
	};
public:
	BattleSoldierFactory();
	void updateBattleSoldier();
	//*false left true right
	void setFaceDirection(bool direction);
	virtual void resetAllSoldierState();
	void MoveToAssignPosition();
	void MoveToTargetPosition();
	void forwardPosition();
	void stayYourPosition();
	void setHpByDamage(float damage);
	void AttackMethod();
	void RunMethod();
	//*start this function �Asolider will running die animation and remove on battle field.
	void KillYourSelf();
	inline float getSoldierHp() { return _hp; };
	//*�ˬd��e���ʪ���l
	void checkGrid();
public:
	//*this soldier all state
	SoldierState _structSoldierState;
	//*this soldier all animation interval
	AnimationInterval _structAnimationInterval;
	//*enemy target
	BattleSoldierFactory* _target;
	Legion* _belongLegion;
	//*under soldier sprite�Acan determine whether it is selected
	cocos2d::Sprite* _standPointSprite;
	//*���w�a�I�y��
	cocos2d::Vec2 _assignPosition;
	//*only use for action
	cocos2d::Sprite* _actionNode;
	SoldierCamp _soldierCamp;
	SoldierType _soliderType;
	//*�����ĤH�ؼЮ� �p�ƥ[1
	int _beingEnemyTagetCount;
	//*�a�Ϯ�l����
	int _gridIndex;
	//�ĴX��X��l
	int _gridXIndex;
	//�ĴX��Y��l
	int _gridYIndex;
	float _attackRange;
protected:
	float _speedX;
	float _speedY;
	float _hp;
	//*�h��
	float _morale;
	float _attackDamage;
protected:
	virtual cocos2d::Animate* AttackAnimation() = 0;
	virtual cocos2d::Animate* RunAnimation() = 0;
	virtual cocos2d::Animate* IdleAnimation() = 0;
	virtual cocos2d::Animate* DeathAnimation() = 0;
	virtual float getUnitOnAttackAnimation() = 0;
	void ActionNodeInit(char* fileName, const char* plistFile);
private:
	//*if sodlier do nothing�Awill running idle animation
	virtual void CheckIdleState();
private:
	//*�ۦ�e�������ĤH�ɡA�|��������
	float _targetPositionOffest;
};


class ShieldSoldier :public BattleSoldierFactory
{
public:
	CREATE_FUNC(ShieldSoldier);
private:
	virtual bool init();
	virtual cocos2d::Animate* AttackAnimation()override;
	virtual cocos2d::Animate* RunAnimation()override;
	virtual cocos2d::Animate* IdleAnimation()override;
	virtual cocos2d::Animate* DeathAnimation()override;
	virtual float getUnitOnAttackAnimation()override;
};

class MusketSoldier :public BattleSoldierFactory
{
public:
	struct extensionsAnimationInterval
	{
		float ReadyToShootInterval;
		float ShootInterval;
		float ReloadInterval;
	};
	struct extensionsSoldierState
	{
		bool isReadyToShoot;
		bool isShoot;
		bool isReload;
		bool HaveAmmo;
	};
public:
	CREATE_FUNC(MusketSoldier);
	cocos2d::Animate* soldierReadyToShoot();
	cocos2d::Animate* soldierShoot();
	cocos2d::Animate* soldierReload();
	virtual void resetAllSoldierState();
	void ReadyToShootMethod();
	void ShootMethod();
	extensionsAnimationInterval _structExtensionsAnimationInterval;
	extensionsSoldierState _structExtensionsSoldierState;
	int _shootDamage;
private:
	virtual bool init();
	virtual void CheckIdleState();
	virtual cocos2d::Animate* AttackAnimation()override;
	virtual cocos2d::Animate* RunAnimation()override;
	virtual cocos2d::Animate* IdleAnimation()override;
	virtual cocos2d::Animate* DeathAnimation()override;
	virtual float getUnitOnAttackAnimation()override;
};

class Lyonar :public BattleSoldierFactory
{
public:
	CREATE_FUNC(Lyonar);

private:
	virtual bool init();
	virtual cocos2d::Animate* AttackAnimation()override;
	virtual cocos2d::Animate* RunAnimation()override;
	virtual cocos2d::Animate* IdleAnimation()override;
	virtual cocos2d::Animate* DeathAnimation()override;
	virtual float getUnitOnAttackAnimation()override;
};

class Assassin :public BattleSoldierFactory
{
public:
	CREATE_FUNC(Assassin);

private:
	virtual bool init();
	virtual cocos2d::Animate* AttackAnimation()override;
	virtual cocos2d::Animate* RunAnimation()override;
	virtual cocos2d::Animate* IdleAnimation()override;
	virtual cocos2d::Animate* DeathAnimation()override;
	virtual float getUnitOnAttackAnimation()override;
};


NS_BATTLEMODE_END
#endif
