#include "StoreScene.h"
#include "tools\JsonParser.h"
#include "tools\AudioManagement.h"
#include "ui\ScrollMode\HUDlayer.h"
#include "ui\MyButton.h"
#include "RPGTools\ItemSprite.h"
#include "Scene\ScrollMode\ScrollSceneFactory.h"
#include "TipsLabel.h"
using namespace cocos2d::ui;
using namespace cocos2d::extension;
using namespace cocos2d::experimental;
using namespace MyTools;
USING_NS_CC;
namespace
{
	const float const_SellScale = 0.2f;
}
void StoreScene::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
	if (ScrollMode::HUDlayer::getHUDlayer() != nullptr)
	{
		ScrollMode::HUDlayer::getHUDlayer()->_isReplaceingScene = false;
	}
}
StoreScene * StoreScene::create(NPC * npc)
{
	StoreScene* pRet = new(std::nothrow) StoreScene();
	if (pRet && pRet->init(npc))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}
Scene* StoreScene::createScene(cocos2d::RenderTexture* capture, NPC* npc)
{
    auto scene = Scene::create();
    auto layer = StoreScene::create(npc);
    scene->addChild(layer);
	if (capture != nullptr)
	{
		auto visible = Director::getInstance()->getVisibleSize();
		auto origin = Director::getInstance()->getVisibleOrigin();
		auto pause_background = Sprite::createWithTexture(capture->getSprite()->getTexture());
		pause_background->setFlipY(true);
		pause_background->setColor(Color3B::GRAY);
		pause_background->setPosition(Vec2(visible.width / 2 + origin.x, visible.height / 2 + origin.y));
		layer->addChild(pause_background, 0);
	}
    return scene;
}
bool StoreScene::init(NPC* npc)
{
    if ( !Layer::init() )
    {
        return false;
    }
    visible = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
	this->_npc = npc;
	_isTouchItemBag = false;
	_isTouchChooseDialog = false;
	_sceneNode = CSLoader::createNode("StoreScene/StoreScene.csb");
	this->addChild(_sceneNode, 1);
	_sceneNode->getChildByName("DialogSprite")->setVisible(false);
	this->_sceneNode->getChildByName("DialogSprite")->setPosition(_sceneNode->getChildByName("DialogInVisiblePos")->getPosition());
	_sceneNode->getChildByName("DialogSprite")->setZOrder(100);
													
	this->_itemScrollView = (cocos2d::ui::ScrollView*)_sceneNode->getChildByName("ItemScrollView");
	this->_itemScrollView->setSwallowTouches(false);
	this->_storeScrollView = (cocos2d::ui::ScrollView*)_sceneNode->getChildByName("StoreScrollView");
	this->_storeScrollView->setSwallowTouches(false);
	//"����ө�"���D
	Label* titleLabel = Label::createWithTTF(JsonParser::getJsonString("StoreScene", "Title", JsonParser::JsonType::UserInterFace),
		JsonParser::fontType(), JsonParser::fontSize(17));
	titleLabel->setColor(Color3B::BLACK);
	titleLabel->setAnchorPoint(Vec2(0.5f, 0.5f));
	titleLabel->enableGlow(Color4B::WHITE);
	titleLabel->setPosition(_sceneNode->getChildByName("TitlePos")->getPosition());
	_sceneNode->addChild(titleLabel,10);
	//���s��z���~���s
	auto ReSortItemBtn = ShakeButton::create(JsonParser::getJsonString("StoreScene", "SortItem", JsonParser::JsonType::UserInterFace),
		JsonParser::fontType(), JsonParser::fontSize(16), Color4B::BLACK, "ui/Button.png",
		[this](ShakeButton* shakeButton)
	{
		std::vector<std::pair<int, int>> itemDatas;
		JsonParser::getItemAllDataFromSave(itemDatas);
		/*std::sort(itemDatas.begin(), itemDatas.end(), [](std::pair<int, int> &left, std::pair<int, int> &right) {
			return left.first > right.first;
		});*/
		std::sort(itemDatas.begin(), itemDatas.end());
		std::reverse(itemDatas.begin(), itemDatas.end());
		JsonParser::setItemAllDataFromSave(itemDatas);
		this->reCreateScrollView();
	});
	ReSortItemBtn->setPosition(this->_sceneNode->getChildByName("RefreshItemPos")->getPosition());
	this->addChild(ReSortItemBtn, 100);
	//"�ӤH"���D
	Label* StoreLabel = Label::createWithTTF(JsonParser::getJsonString("StoreScene", "Store", JsonParser::JsonType::UserInterFace),
		JsonParser::fontType(), JsonParser::fontSize(15));
	StoreLabel->setColor(Color3B::WHITE);
	StoreLabel->setAnchorPoint(Vec2(0.5f, 0.5f));
	StoreLabel->enableGlow(Color4B::BLACK);
	StoreLabel->setPosition(_sceneNode->getChildByName("StoreTitleLabelPos")->getPosition());
	_sceneNode->addChild(StoreLabel);
	//"�I�]"���D
	Label* bagLabel = Label::createWithTTF(JsonParser::getJsonString("StoreScene", "YourBag", JsonParser::JsonType::UserInterFace),
		JsonParser::fontType(), JsonParser::fontSize(15));
	bagLabel->setColor(Color3B::WHITE);
	bagLabel->setAnchorPoint(Vec2(0.5f, 0.5f));
	bagLabel->enableGlow(Color4B::BLACK);
	bagLabel->setPosition(_sceneNode->getChildByName("ItemTitleLabelPos")->getPosition());
	_sceneNode->addChild(bagLabel);
	//"������B"���D
	Label* GotMoneyTitleLabel = Label::createWithTTF(JsonParser::getJsonString("StoreScene", "GotMoney", JsonParser::JsonType::UserInterFace),
		JsonParser::fontType(), JsonParser::fontSize(16));
	GotMoneyTitleLabel->setColor(Color3B::BLACK);
	GotMoneyTitleLabel->setAnchorPoint(Vec2(0.5f, 0.5f));
	GotMoneyTitleLabel->enableGlow(Color4B::WHITE);
	GotMoneyTitleLabel->setPosition(_sceneNode->getChildByName("GotMoneyTitlePos")->getPosition());
	_sceneNode->addChild(GotMoneyTitleLabel, 10);
	//"�������B"���D
	Label* CurrentMoneyTitleLabel = Label::createWithTTF(JsonParser::getJsonString("StoreScene", "CurrentMoney", JsonParser::JsonType::UserInterFace),
		JsonParser::fontType(), JsonParser::fontSize(16));
	CurrentMoneyTitleLabel->setColor(Color3B::BLACK);
	CurrentMoneyTitleLabel->setAnchorPoint(Vec2(0.5f, 0.5f));
	CurrentMoneyTitleLabel->enableGlow(Color4B::WHITE);
	CurrentMoneyTitleLabel->setPosition(_sceneNode->getChildByName("CurrentMoneyTitlePos")->getPosition());
	_sceneNode->addChild(CurrentMoneyTitleLabel, 10);
	//���a��e�֦����B
	this->_currentMoney = atoi(JsonParser::getJsonString("Infor", "Money", JsonParser::JsonType::Save).c_str());
	auto currentMoneyLabel = Label::createWithTTF(JsonParser::getJsonString("Infor", "Money", JsonParser::JsonType::Save).c_str(),
		"fonts/arial.ttf", JsonParser::fontSize(24));
	currentMoneyLabel->setColor(Color3B::WHITE);
	currentMoneyLabel->setAnchorPoint(Vec2(1, 0.5));
	currentMoneyLabel->setPosition(_sceneNode->getChildByName("CurrentMoneyPos")->getPosition());
	_sceneNode->addChild(currentMoneyLabel, 10);
	//������B
	this->_tradeMoney = 0;
	_tradeMoneyLabel=Label::createWithTTF("0","fonts/arial.ttf", JsonParser::fontSize(24));
	_tradeMoneyLabel->setColor(Color3B::BLACK);
	_tradeMoneyLabel->setAnchorPoint(Vec2(1,0.5));
	_tradeMoneyLabel->setPosition(_sceneNode->getChildByName("tradeMoneyPos")->getPosition());
	_sceneNode->addChild(_tradeMoneyLabel, 10);

	//��ܪ��~�ƶqDialog
	_chooseDialog = _sceneNode->getChildByName("ChooseDialogBG");
	_chooseDialog->setZOrder(100);
	_chooseDialog->setPosition(_sceneNode->getChildByName("DialogInVisiblePos")->getPosition());
	auto chooseTitleLabel = Label::createWithTTF(JsonParser::getJsonString("StoreScene","ChooseTitleLabel",
		JsonParser::JsonType::UserInterFace),JsonParser::fontType(),JsonParser::fontSize(24));
	_chooseDialog->addChild(chooseTitleLabel);
	chooseTitleLabel->setPosition(_chooseDialog->getChildByName("ChooseTitlePos")->getPosition());

	auto chooseAmountLabel = Label::createWithTTF(JsonParser::getJsonString("StoreScene", "ChooseAmount",
		JsonParser::JsonType::UserInterFace), JsonParser::fontType(), JsonParser::fontSize(20));
	chooseAmountLabel->setAnchorPoint(Vec2(0, 0.5));
	chooseAmountLabel->setColor(Color3B::BLACK);
	_chooseDialog->addChild(chooseAmountLabel);
	chooseAmountLabel->setPosition(_chooseDialog->getChildByName("ChooseAmountLabelPos")->getPosition());

	auto chooseMaxAmountLabel = Label::createWithTTF(JsonParser::getJsonString("StoreScene", "MaxAmount",
		JsonParser::JsonType::UserInterFace), JsonParser::fontType(), JsonParser::fontSize(20));
	chooseMaxAmountLabel->setAnchorPoint(Vec2(0, 0.5));
	chooseMaxAmountLabel->setColor(Color3B::BLACK);
	_chooseDialog->addChild(chooseMaxAmountLabel);
	chooseMaxAmountLabel->setPosition(_chooseDialog->getChildByName("MaxAmountLabelPos")->getPosition());

	auto AddBtn= ShakeButton::create("",JsonParser::fontType(), 14, Color4B::BLACK, "ui/Add.png",
		[this](ShakeButton* shakeBtn)
	{
		//CCLOG("Add");
		AudioManagement::getInstance()->play("sound/ui_select.mp3", AudioManagement::AudioType::Sound, false);
		Label* itemAmountLabel = (Label*)_chooseDialog->getChildByName("itemAmountLabel");
		Label* itemMaxAmountLabel = (Label*)_chooseDialog->getChildByName("itemMaxAmountLabel");
		int currentAmount = atoi(itemAmountLabel->getString().c_str());
		int maxAmount = atoi(itemMaxAmountLabel->getString().c_str());
		if (currentAmount == maxAmount)
		{
			return;
		}
		else
		{
			char newItemAmountStr[10];
			sprintf(newItemAmountStr, "%d", ++currentAmount);
			itemAmountLabel->setString(newItemAmountStr);
		}
	});
	AddBtn->setPosition(_chooseDialog->getChildByName("AddPos")->getPosition());
	_chooseDialog->addChild(AddBtn);

	auto SubBtn = ShakeButton::create("", JsonParser::fontType(), 14, Color4B::BLACK, "ui/Sub.png",
		[this](ShakeButton* shakeBtn)
	{
		//CCLOG("Sub");
		AudioManagement::getInstance()->play("sound/ui_select.mp3", AudioManagement::AudioType::Sound, false);
		Label* itemAmountLabel=(Label*)_chooseDialog->getChildByName("itemAmountLabel");
		Label* itemMaxAmountLabel = (Label*)_chooseDialog->getChildByName("itemMaxAmountLabel");
		int currentAmount = atoi(itemAmountLabel->getString().c_str());
		int maxAmount = atoi(itemMaxAmountLabel->getString().c_str());
		if (currentAmount == 1)
		{//�i��ּ̤ܳƶq�N�O1
			return;
		}
		else
		{
			char newItemAmountStr[10];
			sprintf(newItemAmountStr, "%d" , --currentAmount);
			itemAmountLabel->setString(newItemAmountStr);
		}
	});
	SubBtn->setPosition(_chooseDialog->getChildByName("SubPos")->getPosition());
	_chooseDialog->addChild(SubBtn);

	auto MaxBtn = ShakeButton::create("", JsonParser::fontType(), 14, Color4B::BLACK, "ui/Max.png",
		[this](ShakeButton* shakeBtn)
	{
		//CCLOG("Max");
		AudioManagement::getInstance()->play("sound/ui_select2.mp3", AudioManagement::AudioType::Sound, false);
		Label* itemAmountLabel = (Label*)_chooseDialog->getChildByName("itemAmountLabel");
		Label* itemMaxAmountLabel = (Label*)_chooseDialog->getChildByName("itemMaxAmountLabel");
		if (itemAmountLabel->getString() == itemMaxAmountLabel->getString())
			return;
		else
		{
			itemAmountLabel->setString(itemMaxAmountLabel->getString());
		}
	});
	MaxBtn->setPosition(_chooseDialog->getChildByName("MaxPos")->getPosition());
	_chooseDialog->addChild(MaxBtn);

	auto MinBtn = ShakeButton::create("", JsonParser::fontType(), 14, Color4B::BLACK, "ui/Min.png",
		[this](ShakeButton* shakeBtn)
	{
		//CCLOG("Min");
		AudioManagement::getInstance()->play("sound/ui_select2.mp3", AudioManagement::AudioType::Sound, false);
		Label* itemAmountLabel = (Label*)_chooseDialog->getChildByName("itemAmountLabel");
		Label* itemMaxAmountLabel = (Label*)_chooseDialog->getChildByName("itemMaxAmountLabel");
		if (itemAmountLabel->getString() == "1")
			return;
		else
		{
			itemAmountLabel->setString("1");
		}
	});
	MinBtn->setPosition(_chooseDialog->getChildByName("MinPos")->getPosition());
	_chooseDialog->addChild(MinBtn);


	auto chooseSubmitBtn = ShakeButton::create(JsonParser::getJsonString("StoreScene", "ChooseSubmit", JsonParser::JsonType::UserInterFace),
		JsonParser::fontType(), 14, Color4B::BLACK, "ui/Button.png",
		[this](ShakeButton* shakeBtn)
	{//ChooseSubmit
		//CCLOG("ChooseSubmit");
		AudioManagement::getInstance()->play("sound/ui_select2.mp3", AudioManagement::AudioType::Sound, false);
		Label* itemAmountLabel = (Label*)_chooseDialog->getChildByName("itemAmountLabel");
		int chooseItemAmount = atoi(itemAmountLabel->getString().c_str());
		ItemSprite* ChooseItemSprite= (ItemSprite*)this->_chooseDialog->getChildByName("itemSprite");
		int ChooseItemID = ChooseItemSprite->_structItemData.itemId;
		int ChooseItemValue = ChooseItemSprite->_structItemData.itemValue;
		if (this->_isTouchItemBag)
		{//�p�G�O�I���ۤv�I�]�O��X
			if (this->_storeSprites.size() <= RPGTool::Const_MaxItemCount)
			{//�p�G�ө��٦��Ŧ�N��檫�~
				bool isfindSameItem = false;
				for (auto storeItem = _npc->_sellItems.begin();storeItem != _npc->_sellItems.end();storeItem++)
				{
					if (std::get<0>(*storeItem) == ChooseItemID)
					{
						if (std::get<1>(*storeItem) == RPGTool::Const_MaxItemStack)
						{
							continue;
						}
						else
						{
							if (std::get<1>(*storeItem) + chooseItemAmount <= RPGTool::Const_MaxItemStack)
							{//��s�[�J�����~�A�[�J�w�������~�|�[�_��
								std::get<1>(*storeItem) += chooseItemAmount;
							}
							else
							{
								//�h�l�X�Ӫ��ƶq�A��s��l
								int newItemAmount = std::get<1>(*storeItem) + chooseItemAmount - RPGTool::Const_MaxItemStack;
								_npc->_sellItems.push_back(std::make_tuple(std::get<0>(*storeItem) == ChooseItemID, newItemAmount,100));
							}
						}
						isfindSameItem = true;
						break;
					}
				}
				if (!isfindSameItem)
				{//�p�G�䤣��ۦP���~�A�N�s�W�s��l�񪫫~
					_npc->_sellItems.push_back(std::make_tuple(ChooseItemID, chooseItemAmount,100));
				}
				JsonParser::removeItemFromSave(_touchIndex);
				this->setTradeMoney(ChooseItemID, chooseItemAmount, false);
				this->reCreateScrollView();
			}
			else
			{//�ө��S���Ŧ�檫�~
				TipsLabel::showTip(JsonParser::getJsonString("StoreScene", "StoreFullBoughtFail", JsonParser::JsonType::UserInterFace), this,30, false);
			}
		}
		else
		{//�I���ө��]�X�����O�R�J
			if (JsonParser::getItemCountFromSave() <= RPGTool::Const_MaxItemCount)
			{//�R�U���~�e�ˬd�I�]�O�_�B��
				if (!this->setTradeMoney(ChooseItemID, chooseItemAmount,true))
				{//�S���F�ʶR����
					TipsLabel::showTip(JsonParser::getJsonString("StoreScene", "BuyItemFail", JsonParser::JsonType::UserInterFace), this,30, false);
					return;
				}
				for (auto storeItem = _npc->_sellItems.begin();storeItem != _npc->_sellItems.end();storeItem++)
				{
					if (std::get<0>(*storeItem) == ChooseItemID)
					{
						if (std::get<1>(*storeItem) == chooseItemAmount)
						{//�ӫ~�������~���Q�R�U
							_npc->_sellItems.erase(storeItem);
						}
						else
						{
							std::get<1>(*storeItem) -= chooseItemAmount;
						}
						JsonParser::AddItemToSave(ChooseItemID,chooseItemAmount);
						break;
					}
				}
			}
			else
			{//�I�]�w�B���L�k�ʶR
				TipsLabel::showTip(JsonParser::getJsonString("StoreScene", "ItemBagFullBoughtFail", JsonParser::JsonType::UserInterFace), this,30, false);
			}
		}
		this->reCreateScrollView();
		_chooseDialog->setPosition(_sceneNode->getChildByName("DialogInVisiblePos")->getPosition());
		_isTouchChooseDialog = false;
	});
	chooseSubmitBtn->setScale(1.7f);
	chooseSubmitBtn->setPosition(_chooseDialog->getChildByName("ChooseSubmit")->getPosition());
	_chooseDialog->addChild(chooseSubmitBtn);

	auto chooseCancelBtn = ShakeButton::create(JsonParser::getJsonString("StoreScene", "ChooseCancel", JsonParser::JsonType::UserInterFace),
		JsonParser::fontType(), 14, Color4B::BLACK, "ui/Button.png",
		[this](ShakeButton* shakeBtn)
	{//ChooseCancel
		AudioManagement::getInstance()->play("sound/ui_select2.mp3", AudioManagement::AudioType::Sound, false);
		_chooseDialog->setPosition(_sceneNode->getChildByName("DialogInVisiblePos")->getPosition());
		_isTouchChooseDialog = false;
	});
	chooseCancelBtn->setScale(1.7f);
	chooseCancelBtn->setPosition(_chooseDialog->getChildByName("ChooseCancel")->getPosition());
	_chooseDialog->addChild(chooseCancelBtn);

	
	//��^��
	auto returnSprite= _sceneNode->getChildByName("ReturnSprite");
	auto returnSpriteListener = EventListenerTouchOneByOne::create();
	returnSpriteListener->onTouchBegan = [this](Touch* touch, Event* event)
	{
		if (event->getCurrentTarget()->getBoundingBox().containsPoint(touch->getLocation()))
		{
			int newMoney = this->_currentMoney + _tradeMoney;
			char newMoneyStr[10];
			sprintf(newMoneyStr, "%d", newMoney);
			JsonParser::setSave("Infor", "Money", newMoneyStr);
			Director::getInstance()->popScene([](Scene* scene)
			{
				return TransitionFade::create(0.3f, scene, Color3B::BLACK);
			});
		}
		return true;
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(returnSpriteListener, returnSprite);

	Vec2 itemSpritePos;
	//ItemScrollView Item
	_ItemScrollViewItem1Pos = _sceneNode->getChildByName("ItemScrollView")->getChildByName("example1")->getPosition();
	_ItemScrollViewItem2Pos = _sceneNode->getChildByName("ItemScrollView")->getChildByName("example2")->getPosition();
	_ItemScrollViewItem3Pos = _sceneNode->getChildByName("ItemScrollView")->getChildByName("example3")->getPosition();
	itemSpritePos.y = _sceneNode->getChildByName("ItemScrollView")->getChildByName("example1")->getPosition().y;
	for (int i = 0;i < RPGTool::Const_MaxItemCount;i++)
	{
		if (i % 3 == 0 && i>=3)
		{//�C�T��l����
			itemSpritePos.y = itemSpritePos.y-50;
		}
		if (i % 3 == 0)
		{
			itemSpritePos.x= _ItemScrollViewItem1Pos.x;
		}
		else if (i % 3 == 1)
		{
			itemSpritePos.x = _ItemScrollViewItem2Pos.x;
		}
		else
			itemSpritePos.x = _ItemScrollViewItem3Pos.x;
		ItemSprite* itemSprite = ItemSprite::create(i,true);
		_itemSprites.push_back(itemSprite);
		itemSprite->setPosition(itemSpritePos);
		itemSprite->setTouchRect();
		_itemScrollView->addChild(itemSprite);
	}
	ItemScrollViewContainerInitPosY = _itemScrollView->getInnerContainerPosition().y;
	//touch
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = [&](Touch* touch,Event* event)
	{
		if (event->getCurrentTarget()->getBoundingBox().containsPoint(touch->getLocation()))
		{//�p�GĲ�N�I�OScrollView
			for (auto it : _itemSprites)
			{//itemSpritePos.Y �@�}�l����l�O987.85 ���p�ڤ@�}�l�I�����I�O�Ĥ@�Ӫ����~ �èD�X�I���ù��W��Y�b ��169.30
			 //987.85-169.30=818.55f �o�ˤl �ڴN���X �I�]�����~�������q
				auto touchNodeSpace = _itemScrollView->convertToNodeSpace(touch->getLocation());
				touchNodeSpace.y = touchNodeSpace.y + 1318.55f - fabs(ItemScrollViewContainerInitPosY - _itemScrollView->getInnerContainerPosition().y);
				if (it->_touchRect.containsPoint(touchNodeSpace) && it->_structItemData.itemId != -1)
				{
					ItemSprite* TouchItemSprite=it;	
					this->setDialogInfor(TouchItemSprite,true);
					
					break;
				}
			}
		}
		return true;
	};
	touchListener->onTouchMoved = [&](Touch* touch, Event* event)
	{
		if (event->getCurrentTarget()->getBoundingBox().containsPoint(touch->getLocation()))
		{//�p�GĲ�N�I�OScrollView
			for (auto it : _itemSprites)
			{
				auto touchNodeSpace = _itemScrollView->convertToNodeSpace(touch->getLocation());
				touchNodeSpace.y = touchNodeSpace.y + 1318.55f - fabs(ItemScrollViewContainerInitPosY - _itemScrollView->getInnerContainerPosition().y);
				if (it->_touchRect.containsPoint(touchNodeSpace) && it->_structItemData.itemId !=-1)
				{
					if (_touchIndex != it->_Itemindex)
					{//Ĳ�N��s���I�}�l�p��
						ItemSprite* TouchItemSprite = it;
						this->setDialogInfor(TouchItemSprite,true);
					}
					break;
				}
			}
		}
	};
	touchListener->onTouchEnded = [&](Touch* touch, Event* event)
	{
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, _itemScrollView);

	//StoreScrollView Item
	_StoreScrollViewItem1Pos = _sceneNode->getChildByName("StoreScrollView")->getChildByName("example1")->getPosition();
	_StoreScrollViewItem2Pos = _sceneNode->getChildByName("StoreScrollView")->getChildByName("example2")->getPosition();
	_StoreScrollViewItem3Pos = _sceneNode->getChildByName("StoreScrollView")->getChildByName("example3")->getPosition();
	itemSpritePos.y = _sceneNode->getChildByName("StoreScrollView")->getChildByName("example1")->getPosition().y;


	for (int i = 0;i < RPGTool::Const_MaxItemCount;i++)
	{
		if (i % 3 == 0 && i >= 3)
		{//�C�T��l����
			itemSpritePos.y = itemSpritePos.y - 50;
		}
		if (i % 3 == 0)
		{
			itemSpritePos.x = _StoreScrollViewItem1Pos.x;
		}
		else if (i % 3 == 1)
		{
			itemSpritePos.x = _StoreScrollViewItem2Pos.x;
		}
		else
			itemSpritePos.x = _StoreScrollViewItem3Pos.x;
		ItemSprite* itemSprite;
		if (i < _npc->_sellItems.size())
		{//�w�g�T�{�W�[�����~
			itemSprite = ItemSprite::create(i, std::get<0>(_npc->_sellItems[i]), std::get<1>(_npc->_sellItems[i]), true);
		}
		else
		{//�Ů�
			itemSprite = ItemSprite::create(-1,true);
		}
		itemSprite->setPosition(itemSpritePos);
		itemSprite->setTouchRect();
		_storeSprites.push_back(itemSprite);
		_storeScrollView->addChild(itemSprite);
	}
	StoreScrollViewContainerInitPosY = _storeScrollView->getInnerContainerPosition().y;

	//touch storeTouchListener
	auto storetouchListener = EventListenerTouchOneByOne::create();
	storetouchListener->onTouchBegan = [&](Touch* touch, Event* event)
	{
		if (event->getCurrentTarget()->getBoundingBox().containsPoint(touch->getLocation()))
		{//�p�GĲ�N�I�OStoreScrollView
			for (auto it : _storeSprites)
			{//itemSpritePos.Y �@�}�l����l�O987.85 ���p�ڤ@�}�l�I�����I�O�Ĥ@�Ӫ����~ �èD�X�I���ù��W��Y�b ��169.30
			 //987.85-169.30=818.55f �o�ˤl �ڴN���X �I�]�����~�������q
				auto touchNodeSpace = _storeScrollView->convertToNodeSpace(touch->getLocation());
				touchNodeSpace.y = touchNodeSpace.y + 1318.55f - fabs(StoreScrollViewContainerInitPosY - _storeScrollView->getInnerContainerPosition().y);
				if (it->_touchRect.containsPoint(touchNodeSpace) && it->_structItemData.itemId != -1)
				{
					ItemSprite* TouchItemSprite = it;
					this->setDialogInfor(TouchItemSprite,false);
					break;
				}
			}
		}
		return true;
	};
	storetouchListener->onTouchMoved = [&](Touch* touch, Event* event)
	{
		if (event->getCurrentTarget()->getBoundingBox().containsPoint(touch->getLocation()))
		{//�p�GĲ�N�I�OScrollView
			for (auto it : _storeSprites)
			{
				auto touchNodeSpace = _storeScrollView->convertToNodeSpace(touch->getLocation());
				touchNodeSpace.y = touchNodeSpace.y + 1318.55f - fabs(StoreScrollViewContainerInitPosY - _storeScrollView->getInnerContainerPosition().y);
				if (it->_touchRect.containsPoint(touchNodeSpace) && it->_structItemData.itemId != -1)
				{
					if (_touchIndex != it->_Itemindex)
					{//Ĳ�N��s���I�}�l�p��
						ItemSprite* TouchItemSprite = it;
						this->setDialogInfor(TouchItemSprite,false);
					}
					break;
				}
			}
		}
	};
	storetouchListener->onTouchEnded = [&](Touch* touch, Event* event)
	{
		
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(storetouchListener, _storeScrollView);


	//��ܮ� ����
	auto dialogCancel=this->_sceneNode->getChildByName("DialogSprite")->getChildByName("DialogCancel");
	auto dialogCancelListener = EventListenerTouchOneByOne::create();
	dialogCancelListener->onTouchBegan = [](Touch* touch, Event* event)
	{
		return true;
	};
	dialogCancelListener->onTouchEnded = [this](Touch* touch, Event* event)
	{
		auto NodeSpaceTouch= this->_sceneNode->getChildByName("DialogSprite")->convertToNodeSpace(touch->getLocation());
		if (event->getCurrentTarget()->getBoundingBox().containsPoint(NodeSpaceTouch))
		{
			this->_sceneNode->getChildByName("DialogSprite")->setVisible(false);
			this->_sceneNode->getChildByName("DialogSprite")->setPosition(_sceneNode->getChildByName("DialogInVisiblePos")->getPosition());
		}
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(dialogCancelListener, dialogCancel);

	//��ܮ� �ާ@���s
	auto submitBtn = ShakeButton::create(JsonParser::getJsonString("ItemScene", "Equipment", JsonParser::JsonType::UserInterFace),
		JsonParser::fontType(),14,Color4B::BLACK, "ui/Button.png",
		[this](ShakeButton* shakeBtn)
	{
		if (_isTouchItemBag)
		{//�p�GĲ�N���O�I�]�������~
			ItemSprite* itemSprite = (ItemSprite*)this->_sceneNode->getChildByName("DialogSprite")->getChildByName("DialogItemSprite");
			if (this->_storeSprites.size() <= RPGTool::Const_MaxItemCount)
			{//�p�G�ө��٦��Ŧ�N��檫�~
				if (RPGTool::isItemIDCanStack(itemSprite->_structItemData.itemId))
				{//�p�G���~�i�H���|�A�h�I�s��ܼƶq���
					this->setAndShowChooseDialog(itemSprite->_structItemData.itemId, itemSprite->_structItemData.itemValue);
					//���~��T������
					this->_sceneNode->getChildByName("DialogSprite")->setVisible(false);
					this->_sceneNode->getChildByName("DialogSprite")->setPosition(_sceneNode->getChildByName("DialogInVisiblePos")->getPosition());
					return;
				}
				else
				{
					JsonParser::removeItemFromSave(_touchIndex);
					this->_npc->_sellItems.push_back(std::make_tuple(itemSprite->_structItemData.itemId,
						itemSprite->_structItemData.itemValue,100));
				}
				this->reCreateScrollView();
				this->setTradeMoney(itemSprite->_structItemData.itemId, itemSprite->_structItemData.itemValue,false);
			}
			else
			{//�ө��S���Ŧ�檫�~
				TipsLabel::showTip(JsonParser::getJsonString("StoreScene", "StoreFullBoughtFail", JsonParser::JsonType::UserInterFace), this,30, false);
			}
		}
		else
		{//�p�GĲ�N���O�ө��������~
			if (JsonParser::getItemCountFromSave() <= RPGTool::Const_MaxItemCount)
			{//�R�U���~�e�ˬd�I�]�O�_�B��
				AudioManagement::getInstance()->play("sound/ui_select2.mp3", AudioManagement::AudioType::Sound, false);
				ItemSprite* itemSprite = (ItemSprite*)this->_sceneNode->getChildByName("DialogSprite")->getChildByName("DialogItemSprite");
				int itemSpriteID = itemSprite->_structItemData.itemId;
				int itemSpriteValue = itemSprite->_structItemData.itemValue;
				if (RPGTool::isItemIDCanStack(itemSprite->_structItemData.itemId))
				{//�����~����|�A���X���|���
					this->setAndShowChooseDialog(itemSprite->_structItemData.itemId, itemSprite->_structItemData.itemValue);
					//���~��T������
					this->_sceneNode->getChildByName("DialogSprite")->setVisible(false);
					this->_sceneNode->getChildByName("DialogSprite")->setPosition(_sceneNode->getChildByName("DialogInVisiblePos")->getPosition());
				}
				else
				{//�����~������|�A���O�Z���B�˳Ƶ���
					if (!this->setTradeMoney(itemSpriteID, itemSpriteValue, true))
					{//�S���F�ʶR����
						TipsLabel::showTip(JsonParser::getJsonString("StoreScene", "BuyItemFail", JsonParser::JsonType::UserInterFace), this,30, false);
						return;
					}
					JsonParser::AddItemToSave(itemSpriteID, itemSpriteValue);
					this->_npc->_sellItems.erase(_npc->_sellItems.begin()+_touchIndex);
				}
				this->reCreateScrollView();
			}
			else
			{//�I�]�w�B���L�k�ʶR
				TipsLabel::showTip(JsonParser::getJsonString("StoreScene", "ItemBagFullBoughtFail", JsonParser::JsonType::UserInterFace), this,30, false);
			}
		}
		//���~��T������
		this->_sceneNode->getChildByName("DialogSprite")->setVisible(false);
		this->_sceneNode->getChildByName("DialogSprite")->setPosition(_sceneNode->getChildByName("DialogInVisiblePos")->getPosition());
	});
	submitBtn->setScale(1.1f);
	submitBtn->setName("DialogSubmitBtn");
	this->_sceneNode->getChildByName("DialogSprite")->addChild(submitBtn);
	submitBtn->setPosition(this->_sceneNode->getChildByName("DialogSprite")->getChildByName("OperatorBtn")->getPosition());
	//dialog�W�����~�������D
	std::string DialogItemPriceTitleStr = JsonParser::getJsonString("StoreScene", "Price",
		JsonParser::JsonType::UserInterFace);
	auto DialogItemPriceTitle = Label::create(DialogItemPriceTitleStr, JsonParser::fontType(), JsonParser::fontSize(20));
	DialogItemPriceTitle->setAnchorPoint(Vec2(0.5, 0.5));
	DialogItemPriceTitle->setColor(Color3B::WHITE);
	DialogItemPriceTitle->enableOutline(Color4B::BLACK, 1);
	DialogItemPriceTitle->setPosition(this->_sceneNode->getChildByName("DialogSprite")->getChildByName("ItemPriceTitlePos")->getPosition());
	this->_sceneNode->getChildByName("DialogSprite")->addChild(DialogItemPriceTitle);

	return true;
}
void StoreScene::setDialogInfor(ItemSprite* touchItemSprite, bool isTouchItemBag)
{
	if (_isTouchChooseDialog)//�w�g�q�X��ܿ��A���~��T�A�N���|��ܥX��
		return;
	int ItemID = touchItemSprite->_structItemData.itemId;
	int ItemRank = touchItemSprite->_structItemData.itemValue;
	this->_touchIndex = touchItemSprite->_Itemindex;
	this->_touchItemSprite = touchItemSprite;
	this->_isTouchItemBag = isTouchItemBag;
	AudioManagement::getInstance()->play("sound/ui_select.mp3", AudioManagement::AudioType::Sound, false);
	Node* dialogSprite = this->_sceneNode->getChildByName("DialogSprite");
	if (_isTouchItemBag)
	{//Ĳ�N�˳Ƥ����˳� dialog�b�k��A�_�h�b����
		dialogSprite->setPosition(_sceneNode->getChildByName("DialogLeftPos")->getPosition());
		ShakeButton* DialogSubmitBtn = (ShakeButton*)dialogSprite->getChildByName("DialogSubmitBtn");
		//dialog "�R�U"�ܦ�"��X"
		DialogSubmitBtn->setBtnString(JsonParser::getJsonString("StoreScene", "Sell", JsonParser::JsonType::UserInterFace));
	}
	else
	{
		dialogSprite->setPosition(_sceneNode->getChildByName("DialogRightPos")->getPosition());
		ShakeButton* DialogSubmitBtn = (ShakeButton*)dialogSprite->getChildByName("DialogSubmitBtn");
		//dialog "��X"�ܦ�"�R�U"
		DialogSubmitBtn->setBtnString(JsonParser::getJsonString("StoreScene", "Buy", JsonParser::JsonType::UserInterFace));
	}
	dialogSprite->setVisible(true);
	if (dialogSprite->getChildByName("DialogItemSprite")!=nullptr)
	{
		dialogSprite->getChildByName("DialogItemSprite")->removeFromParent();
	}
	if (dialogSprite->getChildByName("DialogTitleLabel") != nullptr)
	{
		dialogSprite->getChildByName("DialogTitleLabel")->removeFromParent();
	}
	if (dialogSprite->getChildByName("DialogInforLabel") != nullptr)
	{
		dialogSprite->getChildByName("DialogInforLabel")->removeFromParent();
	}
	if (dialogSprite->getChildByName("DialogItemPrice") != nullptr)
	{
		dialogSprite->getChildByName("DialogItemPrice")->removeFromParent();
	}
	//dialog�p�ϥ�
	ItemSprite* dialogItemSprite = ItemSprite::create(ItemID, ItemRank, true);
	dialogItemSprite->setPosition(this->_sceneNode->getChildByName("DialogSprite")->getChildByName("DialogItemSpritePos")->getPosition());
	dialogItemSprite->setName("DialogItemSprite");
	dialogSprite->addChild(dialogItemSprite);
	//dialog���~�W��
	std::string DialogTitleStr =JsonParser::getJsonString(RPGTool::accordingItemIDToData(dialogItemSprite->_structItemData.itemId).ItemName.c_str(),
												"Title",JsonParser::JsonType::ItemDOC);
	auto DialogTitleLabel = Label::create(DialogTitleStr, JsonParser::fontType(), JsonParser::fontSize(20));
	DialogTitleLabel->setAnchorPoint(Vec2(0, 1));
	DialogTitleLabel->setColor(Color3B::WHITE);
	DialogTitleLabel->enableOutline(Color4B::BLACK, 1);
	DialogTitleLabel->setPosition(dialogSprite->getChildByName("DialogTitlePos")->getPosition());
	DialogTitleLabel->setName("DialogTitleLabel");
	dialogSprite->addChild(DialogTitleLabel);
	//dialog���~��T
	std::string DialogInforStr = JsonParser::getJsonString(RPGTool::accordingItemIDToData(dialogItemSprite->_structItemData.itemId).ItemName.c_str(),
		"Infor", JsonParser::JsonType::ItemDOC);
	auto DialogInforLabel = Label::create(DialogInforStr, JsonParser::fontType(), JsonParser::fontSize(14));
	DialogInforLabel->setMaxLineWidth(130.f);
	DialogInforLabel->setAnchorPoint(Vec2(0, 1));
	//DialogInforLabel->setColor(Color3B(244,199,110));
	DialogInforLabel->setColor(Color3B::WHITE);
	DialogInforLabel->setPosition(dialogSprite->getChildByName("DialogInforPos")->getPosition());
	DialogInforLabel->setName("DialogInforLabel");
	dialogSprite->addChild(DialogInforLabel);
	//dialog �W�����~����
	char DialogItemPriceStr[10];
	int itemUnitPrice = RPGTool::accordingItemIDToData(dialogItemSprite->_structItemData.itemId).Price;
	if (!RPGTool::isItemIDCanStack(dialogItemSprite->_structItemData.itemId) &&
		dialogItemSprite->_structItemData.itemValue!=0)
	{
		itemUnitPrice = sqrt(itemUnitPrice*(dialogItemSprite->_structItemData.itemValue + 1)) * 50;
	}
	if (isTouchItemBag && !RPGTool::isItemIDCanStack(dialogItemSprite->_structItemData.itemId))
	{//��������n���W�Ӽ�
		itemUnitPrice *= const_SellScale;
	}
	sprintf(DialogItemPriceStr, "%d", itemUnitPrice);
	auto DialogItemPrice = Label::create(DialogItemPriceStr, JsonParser::fontType(), JsonParser::fontSize(20));
	DialogItemPrice->setAnchorPoint(Vec2(0, 0.5));
	DialogItemPrice->setColor(Color3B::WHITE);
	DialogItemPrice->enableOutline(Color4B::ORANGE, 1);
	DialogItemPrice->setName("DialogItemPrice");
	DialogItemPrice->setPosition(dialogSprite->getChildByName("ItemPricePos")->getPosition());
	dialogSprite->addChild(DialogItemPrice);
}
void StoreScene::setAndShowChooseDialog(int ItemID, int ItemAmount)
{
	_isTouchChooseDialog = true;
	if (_chooseDialog->getChildByName("itemSprite") != nullptr)
	{
		_chooseDialog->getChildByName("itemSprite")->removeFromParent();
	}
	if (_chooseDialog->getChildByName("itemNameLabel") != nullptr)
	{
		_chooseDialog->getChildByName("itemNameLabel")->removeFromParent();
	}
	if (_chooseDialog->getChildByName("itemAmountLabel") != nullptr)
	{
		_chooseDialog->getChildByName("itemAmountLabel")->removeFromParent();
	}
	if (_chooseDialog->getChildByName("itemMaxAmountLabel") != nullptr)
	{
		_chooseDialog->getChildByName("itemMaxAmountLabel")->removeFromParent();
	}
	this->_maxItemAmount = ItemAmount;
	char ItemIDStr[10];
	char ItemAmountStr[10];
	sprintf(ItemIDStr, "%d", ItemID);
	sprintf(ItemAmountStr, "%d", ItemAmount);
	this->_chooseDialog->setPosition(_sceneNode->getChildByName("ChooseDialogVisiblePos")->getPosition());
	//ItemSprite
	auto itemSprite = ItemSprite::create(ItemID, ItemAmount,true);
	itemSprite->setPosition(this->_chooseDialog->getChildByName("ItemSpritePos")->getPosition());
	itemSprite->setName("itemSprite");
	this->_chooseDialog->addChild(itemSprite);
	//ItemName
	auto itemNameLabel = Label::createWithTTF(JsonParser::getJsonString(RPGTool::accordingItemIDToData(ItemID).ItemName.c_str(),"Title",JsonParser::JsonType::ItemDOC)
		, JsonParser::fontType(),JsonParser::fontSize(28));
	itemNameLabel->setColor(Color3B::WHITE);
	itemNameLabel->enableOutline(Color4B::BLACK, 1);
	itemNameLabel->setPosition(this->_chooseDialog->getChildByName("ItemNamePos")->getPosition());
	itemNameLabel->setName("itemNameLabel");
	this->_chooseDialog->addChild(itemNameLabel);
	//choose item amount
	auto itemAmountLabel = Label::createWithTTF(ItemAmountStr, JsonParser::fontType(),
		JsonParser::fontSize(24));
	itemAmountLabel->setColor(Color3B::BLACK);
	itemAmountLabel->setPosition(this->_chooseDialog->getChildByName("ChooseAmountPos")->getPosition());
	itemAmountLabel->setName("itemAmountLabel");
	this->_chooseDialog->addChild(itemAmountLabel);
	//choose max item amount
	auto itemMaxAmountLabel = Label::createWithTTF(ItemAmountStr, JsonParser::fontType(),
		JsonParser::fontSize(24));
	itemMaxAmountLabel->setColor(Color3B::BLACK);
	itemMaxAmountLabel->setPosition(this->_chooseDialog->getChildByName("MaxAmountPos")->getPosition());
	itemMaxAmountLabel->setName("itemMaxAmountLabel");
	this->_chooseDialog->addChild(itemMaxAmountLabel);
}
void StoreScene::reCreateScrollView()
{
	this->_itemSprites.clear();
	this->_storeSprites.clear();
	_itemScrollView->removeAllChildren();
	_storeScrollView->removeAllChildren();
	Vec2 itemSpritePos;
	itemSpritePos.y = _ItemScrollViewItem1Pos.y;
	for (int i = 0;i < RPGTool::Const_MaxItemCount;i++)
	{
		if (i % 3 == 0 && i >= 3)
		{//�C�T��l����
			itemSpritePos.y = itemSpritePos.y - 50;
		}
		if (i % 3 == 0)
		{
			itemSpritePos.x = _ItemScrollViewItem1Pos.x;
		}
		else if (i % 3 == 1)
		{
			itemSpritePos.x = _ItemScrollViewItem2Pos.x;
		}
		else
			itemSpritePos.x = _ItemScrollViewItem3Pos.x;
		ItemSprite* itemSprite = ItemSprite::create(i, true);
		_itemSprites.push_back(itemSprite);
		itemSprite->setPosition(itemSpritePos);
		itemSprite->setTouchRect();
		_itemScrollView->addChild(itemSprite);
	}
	itemSpritePos.y = _StoreScrollViewItem1Pos.y;
	for (int i = 0;i < RPGTool::Const_MaxItemCount;i++)
	{
		if (i % 3 == 0 && i >= 3)
		{//�C�T��l����
			itemSpritePos.y = itemSpritePos.y - 50;
		}
		if (i % 3 == 0)
		{
			itemSpritePos.x = _StoreScrollViewItem1Pos.x;
		}
		else if (i % 3 == 1)
		{
			itemSpritePos.x = _StoreScrollViewItem2Pos.x;
		}
		else
			itemSpritePos.x = _StoreScrollViewItem3Pos.x;
		ItemSprite* itemSprite;
		if (i < _npc->_sellItems.size())
		{//�w�g�T�{�W�[�����~
			itemSprite = ItemSprite::create(i,std::get<0>(_npc->_sellItems[i]),std::get<1>(_npc->_sellItems[i]), true);
		}
		else
		{//�Ů�
			itemSprite = ItemSprite::create(-1, true);
		}
		itemSprite->setPosition(itemSpritePos);
		itemSprite->setTouchRect();
		_storeSprites.push_back(itemSprite);
		_storeScrollView->addChild(itemSprite);
	}
}
bool StoreScene::setTradeMoney(int itemID, int itemValue,bool isBuy)
{//�p�G���a��������I���~�ɡAreturn false
	int payMoney = 0;
	if (RPGTool::isItemIDCanStack(itemID))
	{
		payMoney = RPGTool::accordingItemIDToData(itemID).Price*itemValue;
	}
	else
	{//���~���Ťj��0�ɡA����}�}�Ѹ��p��
		if (itemValue == 0)
			payMoney = RPGTool::accordingItemIDToData(itemID).Price;
		else
			payMoney = sqrt(RPGTool::accordingItemIDToData(itemID).Price*(itemValue+1))*50;
	}
	if (isBuy)
	{//�p�G�O�ʶRpayMoney�N�ܬ��t�ơA�c��h�O����
		payMoney *= -1;
		if (this->_currentMoney + payMoney + this->_tradeMoney < 0)
		{//�L�k��I���~
			return false;
		}
	}
	else
	{//��X���B  ���W�@�ӫY��
		if (!RPGTool::isItemIDCanStack(itemID))
		{
			payMoney *= const_SellScale;
		}
	}
	this->_tradeMoney += payMoney;
	char tradeMoneyStr[10];
	sprintf(tradeMoneyStr, "%d", this->_tradeMoney);
	this->_tradeMoneyLabel->setString(tradeMoneyStr);
	return true;
}
