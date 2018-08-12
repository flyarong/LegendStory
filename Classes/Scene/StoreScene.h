#ifndef __StoreScene_SCENE_H__
#define __StoreScene_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "cocostudio\CocoStudio.h"
#include "UI\CocosGUI.h"
#include "RPGTools\RPGTool.h"
#include "MainCharacter\MainCharacter.h"
class ItemSprite;
class NPC;
class StoreScene : public cocos2d::Layer
{
public:
	virtual void onEnterTransitionDidFinish();
    static cocos2d::Scene* createScene(cocos2d::RenderTexture* capture,NPC* npc);
    virtual bool init(NPC* npc);
	static StoreScene* create(NPC* npc);
private:
	//*���~�ާ@���
	void setDialogInfor(ItemSprite* touchItemSprite,bool isTouchItemBag);
	//*���~�ƶq��ܿ��
	void setAndShowChooseDialog(int ItemID,int ItemAmount);
	//*���s�q�s�ɤ�Ū�����~�w�ƾڡA�åB���s�[�J��scroll view��
	void reCreateScrollView();
	//*�p�G���a��������I���~�ɡAreturn false
	bool setTradeMoney(int itemID,int itemValue,bool isBuy);
private:
	NPC* _npc;
	cocos2d::Size visible;
	cocos2d::Vec2 origin;
	//*scroll view item �@�}�l����l
	cocos2d::Vec2 _ItemScrollViewItem1Pos;
	cocos2d::Vec2 _ItemScrollViewItem2Pos;
	cocos2d::Vec2 _ItemScrollViewItem3Pos;
	cocos2d::Vec2 _StoreScrollViewItem1Pos;
	cocos2d::Vec2 _StoreScrollViewItem2Pos;
	cocos2d::Vec2 _StoreScrollViewItem3Pos;
	cocos2d::Label* _hpValue;
	cocos2d::Label* _attackValue;
	cocos2d::Label* _mpValue;
	cocos2d::Label* _defendValue;
	cocos2d::Label* _savvyValue;
	cocos2d::Label* _popularValue;
	cocos2d::Label* _tradeMoneyLabel;
	cocos2d::Node* _chooseDialog;
	cocos2d::Node* _sceneNode;
	cocos2d::ui::ScrollView* _itemScrollView;
	cocos2d::ui::ScrollView* _storeScrollView;
	std::vector<ItemSprite*> _itemSprites;
	std::vector<ItemSprite*> _storeSprites;
	ItemSprite* _touchItemSprite;
	bool _isTouchItemBag;
	bool _isTouchChooseDialog;
	//���~�e���@�}�l��Y��
	float ItemScrollViewContainerInitPosY;
	//�ө��e���@�}�l��Y��
	float StoreScrollViewContainerInitPosY;
	//*Ĳ�N�I�]��쪺�ĴX��C
	int _touchIndex;
	int _currentMoney;
	int _tradeMoney;
	int _prevSliderPercent;
	//*��e���~�̤j�ƶq �Ψӵ�Choose Dialog���P�_
	int _maxItemAmount;
};

#endif
