#ifndef __ItemSprite_H__
#define __ItemSprite_H__

#include "cocos2d.h"

class ItemSprite : public cocos2d::Sprite
{
public:
	struct ItemData
	{
		int itemId;
		int itemValue;
	};
public:
	//*�ǤJ�O�I�]�ĴX��MRPGTool��ItemID enum class id
	static ItemSprite* create(const int Itemindex,const bool isSquareBG);
	//*�ǤJ���~ID�M���~���šA�Ȼs�ƪ��~�Ϥ�
	static ItemSprite* create(const int ItemID,const int ItemValue,const bool isSquareBG);
	//*���ө�ScrollView�Ϊ��A�ǤJ�ĴX�檫�~�M���~ID�M���~���šA�Ȼs�ƪ��~�Ϥ�
	static ItemSprite* create(const int ItemIndex,const int ItemID, const int ItemValue, const bool isSquareBG);
    virtual bool init(const int Itemindex,const bool isSquareBG);
	virtual bool init(const int ItemID, const int ItemRank,const bool isSquareBG);
	virtual bool init(const int ItemIndex,const int ItemID, const int ItemRank, const bool isSquareBG);
	void setTouchRect();
	//*�󴫪��~�Ϥ� �ǤJ(int)RPGTool::ItemID ItemRank
	void setNewItemSpritePicture(const int ItemID, const int ItemValue);
	//*���ê��~�Ϥ��Mlabel�A�åB��ItemID�MItemValue�]��-1�A�]��-1�N���వ�@�t�C�ƥ�P�_
	void inVisibleItemPicAndLabel();
	cocos2d::Color3B getLabel3BColorAccordingItemRank(const int itemRank);
	cocos2d::Color4B getLabel4BColorAccordingItemRank(const int itemRank);
public:
	ItemData _structItemData;
	//*�I�]�ĴX���
	int _Itemindex;
	cocos2d::Rect _touchRect;
	cocos2d::Sprite* _itemBG;
	cocos2d::Sprite* _blackBG;
private:
	cocos2d::Sprite* _ItemSpritePicture;
	cocos2d::Label* _LabelValue;
private:
	void commonInit(const bool isSquareBG);
};

#endif
