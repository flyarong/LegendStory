#ifndef __Dialog_SCENE_H__
#define __Dialog_SCENE_H__
#include "cocos2d.h"
#include "cocostudio\CocoStudio.h"
#include "ui\UILoadingBar.h"
#include "cocos-ext.h"
#include "MainCharacter\MainCharacter.h"
#include "Npc\NPCFactory.h"
#include "DefinitionScrollMode.h"

class Dialog : public cocos2d::Layer
{
public:
	Dialog();
	~Dialog();
	static cocos2d::Scene* createScene(cocos2d::RenderTexture* capture, TalkCharacter* person,std::string dialogCode);
	static Dialog* create(TalkCharacter* person,std::string dialogCode);
	virtual void onEnterTransitionDidFinish();
	virtual bool init(TalkCharacter* person,std::string dialogCode);
	//�ھ�index Ū���������@��
	void startDialog(int index);
	std::string _DialogCode;
	bool _isMCTalking;
	bool _isSceneInTalking = false;
private:
	void ceateDramaLabel();
	void waitPlayerOperationScreen();
	void accordingNameCreateNPC(std::string name);
private:
	//*��ͦ�class�ɪ������T
	TalkCharacter* _talkCharacter;
	//*���b���ܪ������T
	TalkCharacter* _talkingNpc;
	cocos2d::Size visible;
	cocos2d::Vec2 origin;
	cocos2d::Node* _sceneNode;
	cocos2d::Node* _optionDialog;
	cocos2d::Node* _skipSprite;
	cocos2d::Label* _dramaLabel;
	cocos2d::Label* _nameLabel;
	cocos2d::Sprite* _nameTag;
	cocos2d::Sprite* _dialogTip;
	bool _isReplacing;
	std::string _dialogStr;
	ScrollMode::MainCharacter* _maincharacter;
	bool _isRunLabelAnimation;
	int _dramaAnimationCount;
	int _dramaIndex;
};
#endif
