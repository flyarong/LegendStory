#ifndef __MainCharacter__H__
#define __MainCharacter__H__
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "DefinitionScrollMode.h"
#include "dragonBones\cocos2dx\CCDragonBonesHeaders.h"
#include "Npc\NPCFactory.h"
NS_SCROLLMODE_BEGAN
class MainCharacter;
//�D���ʵeclass
class EXCharacter
{
	friend class MainCharacter;
public:
	EXCharacter();
	~EXCharacter();
	dragonBones::CCArmatureDisplay* init(MainCharacter* mainCharacter);
	void play(const std::string& animationName, int playTimes,bool isAttackAnimation);
	inline dragonBones::CCArmatureDisplay* getArmatureDisplay() { return _armatureDisplay; };
	inline bool isAnimationPlaying() { return _armatureDisplay->getAnimation()->isPlaying(); };
	inline void setScale(float scale) { _armatureDisplay->setScale(scale); };
	inline void setScaleX(float scaleX) { _armatureDisplay->setScaleX(scaleX); };
	inline void setScaleY(float scaleY) { _armatureDisplay->setScaleY(scaleY); };
	inline float getAnimationTotalTime(const std::string& animationName) { return _armatureDisplay->getAnimation()->getState(animationName)->getTotalTime(); };
	inline float getLastAnimationTotalTime() { return _armatureDisplay->getAnimation()->getLastAnimationState()->getTotalTime(); };
	inline float getLastAnimationCurrentTime() { return _armatureDisplay->getAnimation()->getLastAnimationState()->getCurrentTime(); };
	//*�M�Ũ��W�Ҧ��˳�
	void resetAllEquipMent();
	void setAllEquipMentAccordingToSave();
	void setCloth(int ItemID);
	void setArmatureFacial(std::string newFacial);
private:
	//*�ѩ�C������ʵe�A�|�л\���ð��f���]�w�A�ҥH����ưw��Z���h�P�_�C
	void updateWeapon();
	void updateHair();
	void updateHead();
private:
	dragonBones::DragonBonesData* _dragonBonesData;
	dragonBones::Armature* _armature;
	dragonBones::CCArmatureDisplay* _armatureDisplay;
	dragonBones::CCFactory _factory;
	int _WeaponIndex;
	int _HairType;
	int _HeadType;
	int _ClothType;
	int _PantType;
private:
	MainCharacter* _mainCharacter;
};
//�D���ݩ�class
class MainCharacter : public cocos2d::Sprite
{
public:
	friend class EXCharacter;
	friend class HUDlayer;
	friend class Monster;
public:
	struct Status
	{
		bool _isIdle;
		bool _isRun;
		bool _isAttack;
		bool _isHurt;
		bool _isJump;
		bool _isDying;
		bool _faceDirection;//�����Vfalse����k��Atrue���索��
		bool _isGodMode;//�L�Ī��A
	};
	struct Property
	{//������ݩ�
		int _Blood;    //���
		int _Force;	   //�Z�O
		int _Magic;	   //���O
		int _Defend;   //���m
		int _Dodge;	   //���k
		int _Popular;  //�n��
	};
	struct Infor
	{
		int _level;
		float _moveSpeed;
		int _exp;
		float _hp;
		float _mp;
		int _maxexp;
		float _maxhp;
		float _maxmp;
		float _damage;
		float _defend;
		float _dodge;
	};
	enum
	{
		RUNNING_TAG,
		IDLE_TAG,
		JUMP_TAG,
		SWINGSWORD_TAG,
		KICK_TAG,
		SHIED_TAG,
		JUMPATTACK_TAG
	};
public:
	MainCharacter();
	virtual ~MainCharacter();
	static MainCharacter* getMainCharacter();
	static MainCharacter* create(float rebornPosX, float rebornPosY,bool isShowCase);
	void resetAllStatus();
	//�q�s�ɤ�Ū���A�í��s��X�̷s�����ݩʼƭ�
	void refreshBaseProperty();
	//*����ۦ��W�٦^��Ĳ�o�ˮ`���ɾ�
	//*�åB�ھکۦ��W�ٵ����D�������Z��
	//*�åB�ھکۦ��W�ٵ����ĤH����ˮ`�᪺���z�ʤO
	float getAttackTiming(const std::string& animationName);
	void addExp(float exp);
	//�ǤJ�����Ӫ���V
	void HurtAnimation(bool AttackDirection);
	//��ﭱ�諸��V
	void reviseDirection(bool faceDirection);
	void addHPMPToFull();
	void doDyingAnimation();
public:
	//*hudlayer�Q�γo�Ӱ����q�A����hud�b��ۥD�����ʮɡA���b���T����l�C
	float _startPosY;
	cocos2d::PhysicsBody* _CharacterPhysicsBody;
	Status status;
	//*�ʸˤF�@��EXCharacter �s���ʵe���A�M������D���ʵe
	EXCharacter* _exCharacter;
	Property struct_property;
	Infor struct_infor;
	bool _isReplaceScene;
private:
	bool initMainCharacter(float rebornPosX, float rebornPosY, bool isShowCase);
	//*�Ψӭp�ɥD�������ʵe�A�õo�e�ˮ`�������Ǫ��C
	void AttackMethod();
	void AccordingAttackPlaySound(const std::string& animationName);
	void RunAnimation(Ref* ref);
	void AttackAnimation(Ref* ref);
	void JumpAnimation(Ref* ref);
	void FallDownAnimation(Ref* ref);
	void checkIdleAnimation();
	void checkToNPCDistance();
	void StopRunningAnimation(Ref* ref);
	void updateCharacterOnTheSkyState();
	inline int getMaxJumpCount() { return _MaxJumpCount; }
	//*�^��true�ɥN���e�]�O�������ө�ޯ�,�Ϥ��h�_�C
	bool checkAndSetMP(const std::string & animationName);
	void updatePhysicsFriction();
private:
	static MainCharacter* static_mainCharacter;
	cocos2d::Size _characterAttackRange;//�D�������Z��
	cocos2d::Size visible;
	cocos2d::Vec2 origin;
	//*�Ψӵ����q�s������ɪ����X��
	bool lockNormalClick;
	//*�p�G�O�i�ܥΡA�N���|�����z�����C���O�b�Ш����ɡAShowCase�N�i�H�]��true�C
	bool _isShowCase;
	//*��e���D����
	int _currentJumpCount;
	//*�̤j���D����
	int _MaxJumpCount;
	//*��e���q�������ơA���q�����@�T��
	int _normalAttackCount;
	//*����ScrollMonster::HurtStatus enum����
	//*getAttackTiming()��Ƥ��i���ȤΧ�s�C
	int _SendToMonsterHurtAnimation = 0;
	//*0�N��u�������ĤH���|����ˮ` 
	//*1�N��u���I���ĤH�|����ˮ`
	//*2�N��⭱�ĤH���|����ˮ`
	int _AttackRangeType = 0;
	//*��ޯ�ɪ��ˮ`�[��
	int _SkillDamage = 0;
	//*0 �N���q����
	//*1 �N��ޯ�1
	//*2 �N��ޯ�2
	//*3 �N��ޯ�3
	int _AttackType;
};


NS_SCROLLMODE_END
#endif
