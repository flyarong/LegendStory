#ifndef __ItemScene_SCENE_H__
#define __ItemScene_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "cocostudio\CocoStudio.h"
#include "UI\CocosGUI.h"
#include "RPGTools\RPGTool.h"
#include "MainCharacter\MainCharacter.h"
class ItemSprite;
class ItemScene : public cocos2d::Layer
{
public:
	virtual void onEnterTransitionDidFinish();
    static cocos2d::Scene* createScene(cocos2d::RenderTexture* capture);
	ItemSprite* getEquipmentItemSpriteaccordingItemID(int ItemID);
    virtual bool init();
	virtual void onExit();
    CREATE_FUNC(ItemScene);
private:
	void setDialogInfor(int ItemID,int ItemRank);
	//*�ǤJ���~ID �ھڨ䳡�� ��s�D���ʵe�A��˫᪺�ˤl�C
	void UnEquipCharacterCloth(int ItemID);
	//*���s�q�s�ɤ�Ū�����~�w�ƾڡA�åB���s�[�J��scroll view��
	void reCreateScrollView();
	//*��s�����O��T
	void setNewCharacterInfor(cocos2d::Ref* sender);
private:
	cocos2d::Size visible;
	cocos2d::Vec2 origin;
	//*scroll view item �@�}�l����l
	cocos2d::Vec2 _scrollViewItem1Pos;
	cocos2d::Vec2 _scrollViewItem2Pos;
	cocos2d::Vec2 _scrollViewItem3Pos;
	cocos2d::Label* _hpValue;
	cocos2d::Label* _attackValue;
	cocos2d::Label* _mpValue;
	cocos2d::Label* _defendValue;
	cocos2d::Label* _speedValue;
	cocos2d::Label* _popularValue;
	cocos2d::Node* _sceneNode;
	bool _isTouchEquipment;
	cocos2d::ui::ScrollView* _scrollView;
	std::vector<ItemSprite*> _itemSprites;
	ScrollMode::MainCharacter* _mainCharacter;
	ItemSprite* _touchItemSprite;
	//�@�}�l��Y��
	float ContainerInitPosY;
	//*Ĳ�N�I�]��쪺�ĴX��C
	//*�p�GĲ�N�O�˳Ƥ������A�h�O���RPGTool::CharacterPart�C
	int _touchIndex;
	
};

#endif
