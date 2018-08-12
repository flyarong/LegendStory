#ifndef __HUDlayer_SCENE_H__
#define __HUDlayer_SCENE_H__
#include "cocos2d.h"
#include "DefinitionScrollMode.h"
#include "cocos-ext.h"
#include "cocostudio\CocoStudio.h"
#include "ui\UILoadingBar.h"
#include "Npc\NPCFactory.h"
#include "Scene\ScrollMode\ScrollSceneObject.h"
NS_SCROLLMODE_BEGAN
class MainCharacter;
class HUDlayer : public cocos2d::Layer
{
public:
	enum class BtnType
	{
		NormalBtn,
		SkillBtn,
		PotionBtn
	};
	enum SkillType
	{
		Normal,
		Taji
	};
	//*���q�������s���A
	struct NormalSkillState
	{
		bool Transfer = false;
		bool Attack = false;
		bool Talk = false;
	};
	enum
	{
		LeftMove,
		RightMove,
		NormalAttack,
		JUMP,
		SKILL1,
		SKILL2,
		SKILL3,
		HPPotion,
		MPPotion
	};
public:
	HUDlayer();
	virtual ~HUDlayer();
	static HUDlayer* getHUDlayer();
	static HUDlayer* create(MainCharacter* mainCharacter);
	virtual bool init(MainCharacter* mainCharacter);
	void setHPBarPercent(float hp);
	void setMPBarPercent(float mp);
	void setEXPBarPercent(float exp);
	float getHPBarPercent();
	float getMPBarPercent();
	float getEXPBarPercent();
	//*�������ܧޯ�ϼ�
	void SwitchTalkBtn();
	//*�������q�����ޯ�ϼ�
	void SwitchAttackBtn();
	//*�����ǰe�ޯ�ϼ�
	void SwitchTransferBtn();
	//*��sHUD layer ���Scene pos �h����
	void updateLayerWithScenePos(float x, float y);
	//���s��s�����ƾ� ���Obar�W����r �Ĥ��ϼе���
	void refreshHUDlayer();
	void refreshMPBar();
	//���s��s�ޯ�ϼ�
	void refreshSkillSprite();
	void setItemTip(bool wantShow);//�ǤJtrue�άOfalse�M�w�O�_�n��ܴ���
	void setMissionTip(bool wantShow);//�ǤJtrue�άOfalse�M�w�O�_�n��ܴ���
public:
	NPC* _closerNPC;
	TransferPoint* _transferPoint;
	//*�p�G�O�b�����������A
	bool _isReplaceingScene;
	NormalSkillState _normalSkillState;
private:
	const float const_NormalBtn = 0.54f;
	const float const_SkillBtn = 0.41f;
	const float const_PotionBtn = 1.0f;
	static HUDlayer* static_hudlayer;
	MainCharacter* _mainCharacter;
	cocos2d::Node* _sceneCSB;
	cocos2d::Label* _HPDetailLabel;
	cocos2d::Label* _MPDetailLabel;
	cocos2d::Label* _EXPDetailLabel;
	cocos2d::Label* _levelLabel;
	cocos2d::Sprite* _leftMoveClick;
	cocos2d::Sprite* _leftMoveUnClick;
	cocos2d::Sprite* _rightMoveClick;
	cocos2d::Sprite* _rightMoveUnClick;
	cocos2d::Sprite* _normalBtn;
	cocos2d::Sprite* _jumpBtn;
	cocos2d::Sprite* _skill1Btn;
	cocos2d::Sprite* _skill2Btn;
	cocos2d::Sprite* _skill3Btn;
	cocos2d::Sprite* _HPPotionBtn;
	cocos2d::Sprite* _MPPotionBtn;
	cocos2d::Sprite* _missionTipBall;
	cocos2d::Sprite* _itemTipBall;
	//*SkillBG
	cocos2d::Sprite* _normalBG;
	cocos2d::Sprite* _jumpBG;
	cocos2d::Sprite* _skill1BG;
	cocos2d::Sprite* _skill2BG;
	cocos2d::Sprite* _skill3BG;
	//*���U���ʫ��s�ɬ�true �_�h��false
	bool _isTouchMove;
	std::vector<cocos2d::Sprite*> _moveBtns;
	std::vector<cocos2d::Sprite*> _skillBtns;
private:
	//���s��s�Ĥ����s��
	void refreshPotionBG();
	//���s��sbar�W�����Ʀr
	void refreshBarNumberLabel();
	//���s��s����label
	void refreshLevelLabel();
	void NormalAttackCallBack();
	void Skill1CallBack();
	void Skill2CallBack();
	void Skill3CallBack();
	void hpPotionCallBack();
	void mpPotionCallBack();
	void JumpCallBack();
	void MoveListenerSendNotifiToCharacter(cocos2d::Sprite* moveSprite);
	void ShakeBtn(cocos2d::Node* wantToShakeNode, BtnType btnType);
	virtual void onExit();
private:
	cocos2d::Size visible;
	cocos2d::Vec2 origin;
	//*0�ǰe
	//*1����
	//*2���q����
	int _intNormalState;

};
NS_SCROLLMODE_END
#endif
