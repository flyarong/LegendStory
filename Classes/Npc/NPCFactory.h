#ifndef __NPCFactory_H__
#define __NPCFactory_H__
#include "cocos2d.h"
#include "dragonBones\cocos2dx\CCDragonBonesHeaders.h"
#include "MissionMG.h"
#include "TalkCharacter.h"
class NPC;
class NPCFactory
{
public:
	enum class NPCName
	{
		ZhangQing,
		ZhangYin,
		ZhangZhen,
		ZhangHen,
		ZhangFong,
		ZhangWo
   };
	static NPC* create(NPCName npcName,bool isShowCase,cocos2d::Vec2 bornPos=cocos2d::Vec2::ZERO);
private:
	
};

class NPC : public TalkCharacter
{
public:
	struct Status
	{
		bool _isIdle = false;
		bool _isIdle2 = false;
		bool _isWalk = false;
		bool _isRun = false;
		bool _isDying = false;
		bool _isDeath = false;
		bool _isTalk = false;
		bool _faceDirection;//*�@�}�l�ҭ��諸��V
	};
public:
	//*�l�� �غc��ƪ�l�Ƨ��A�~�|�I�sinit��ơA
	//*���n����init��ơA���D�A���D�A�b�F�ƻ�C
	bool init(bool isShowCase);
	NPC();
	~NPC();
	//*NPC_AI update this method
	virtual void update();
	void resetAllStatus();
	void setFaceDirection(bool newFaceDirection);
	void changeChatLabelAndFacialState();
	//*���a�i��d�� ��snpc�����V
	void updateFaceDirectionWithCharacter();
	void setFacialType(FacialType facialType);
	void setFacialType(std::string facialNameStr);
	std::string inline getFacialType() { return this->_facial; };
	//*�ھ�_facial�����ܶq �ӧ�s���ʵe
	void updateArmatureFacial();
	virtual void doIdleAnimation();
	void doIdle2Animation();
	void doWalkAnimation();
	void doRunAnimation();
	void doDyingAnimation();
	inline MissionMG::State getMissionState() { return _missionState; };
	inline NPC* getNPCInstance() { return this; };
	//*���NPC�n������ܥN��
	std::string getDialogCode();
public:
	//*��ܮضZ��npc�h��x  and  y
	cocos2d::Vec2 _dialogXY;
	cocos2d::Vec2 _prevPos;
	cocos2d::Sprite* _chatSprite;
	cocos2d::Label* _chatLabel;
	cocos2d::PhysicsBody* _physicsBody;
	dragonBones::DragonBonesData* _dragonBonesData;
	dragonBones::Armature* _armature;
	dragonBones::CCArmatureDisplay* _armatureDisplay;
	dragonBones::CCFactory _factory;
	Status status;
	//���~ID,���~value,�X�{���v
	std::vector<std::tuple<int, int, int>> _sellItems;
	//*show case��true,�N���|�u�X��ܮ�
	bool _isShowCase;
	//*NPC��_�ۥѨ���
	bool _isNpcCanMove;
	//*���ʳt�v
	float _moveSpeed;
	//*NPC��ݨ��D�����i���Z��
	float _visibleRange;
protected:
	//*�����Үɶ����j
	float _thinkInterval;
	float _thinkInterval2;
	//*�����ܼ@�������ȶi��
	int _unlockDialogCodeStage;
	//*��ܥN��
	std::string _DialogCode;
	//*��ĸ�
	cocos2d::Sprite* _exclam;
	MissionMG::State _missionState;
private:
	//*�Ψ��x�sNPC�W�@�y���ܪ����e�O���@��
	int _prevRandNum = -1;
	std::string _facial;
private:
	//*�ھڤl���غc�l�ɩҲK�[���c�檫�~�A�è̷Ө���v�h�P�_���Ӫ��~�O�_�|�X�{�b�����椤�C
	void judgmentSellItem();
	//*�T�w������AI
	void ThinkAI(float dt);
	void ThinkAI2(float dt);
};

////@WeaponTrader
//class WeaponTrader:public NPC
//{
//public:
//	static WeaponTrader* create(bool isShowCase);
//private:
//	WeaponTrader();
//};


//�}����
class ZhangQing :public NPC
{
public:
	static ZhangQing* create(bool isShowCase);
private:
	ZhangQing();
};

//Ĭ���N
class ZhangYin :public NPC
{
public:
	static ZhangYin* create(bool isShowCase);
private:
	ZhangYin();
};

//���u�D��
class ZhangZhen :public NPC
{
public:
	static ZhangZhen* create(bool isShowCase);
private:
	ZhangZhen();
};

//�x����
class ZhangHen :public NPC
{
public:
	static ZhangHen* create(bool isShowCase);
private:
	ZhangHen();
};

//������
class ZhangFong :public NPC
{
public:
	static ZhangFong* create(bool isShowCase);
private:
	ZhangFong();
};

//������
class ZhangWo :public NPC
{
public:
	static ZhangWo* create(bool isShowCase);
private:
	ZhangWo();
};
#endif
