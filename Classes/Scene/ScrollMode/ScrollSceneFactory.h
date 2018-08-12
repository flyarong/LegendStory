#ifndef __GameScene_SCENE_H__
#define __GameScene_SCENE_H__
#include "cocos2d.h"
#include "cocos-ext.h"
#include "DefinitionScrollMode.h"
#include "UI\ScrollMode\Dialog.h"
#include "UI\DamageMG.h"
#include "Scene\SplashSceneFactory.h"
#include "Monster\ScrollMode\ScrollMonsterFactory.h"
NS_SCROLLMODE_BEGAN
class ScrollMap;
class HUDlayer;
class MainCharacter;
class TransferPoint;
class MapNameTip;
class ScrollMonsterFactory;
#define NoprevCharacterPos Vec2(-1000,-1000)
class ScrollSceneFactory
{
public:
	enum class SceneName
	{
		LuoYangCityScene,
		ForestFieldScene,
		ChiuanJenMountScene,
		ChiuanJenScene,
		CaveScene,
		TestScene
	};
	static cocos2d::Scene* createScene(SceneName sceneName);
	//*�x�s�D���W�@�i�a�Ϫ��ǰe�I��l�A�Ψӭp���U�@�i�a�ϮɡA���Ӧb���@�Ӷǰe�I�X�{�C
	static cocos2d::Vec2 static_prevCharacterPos;
};

class ScrollScene : public cocos2d::Layer
{
public:
	enum
	{
		WEATHER_ZORDER,
		GAMEMAP_ZORDER,
		DROP_ITEM_ZORDER,
		MONSTER_ZORDER,
		MAINCHARACTER_ZORDER,
		NPC_ZORDER,
		NPC2_ZORDER,
		Control_BUTTON_ZORDER,
		ShowMonsterOnScreenLabel,
		ShowDamagedNum_ZORDER,
		DIALOG_ZORDER
	};
public:
	cocos2d::PhysicsWorld* _physicsWorld;
	ScrollScene();
	virtual ~ScrollScene();
	static ScrollScene* getScrollScene();
	void ShakeCamera(float duration,float degree);
	//*�l���غc�l���ACREATE_FUNC�|�I�sinit()���
	//*���n�h�����L�A���D�A���էA�b�F�ƻ�C
	bool init();
	virtual void ReCheckForceDialog(bool isInitCall)=0;
public:
	//*���Y����
	int cameraHeight = -60;
	DamageMG* _damageMG;
	ScrollMap* _gameMap;
	MainCharacter* _maincharacter;
	//*�D���O�_�i�H���U,��ScrollMap�P�_
	bool _canCharacterJumpDown;
	//*�P�_���S����L�a��j��M������,�p�G�����ܦ����N���n�A�M��
	bool _ClearAudioManagement;
protected:
	cocos2d::Size visible;
	cocos2d::Vec2 origin;
	cocos2d::Vec2 _characterBornPos;
	//*�����W���ǰe�I
	std::vector<TransferPoint*> _transferPoints;
	HUDlayer* _hudlayer;
	//*�����W�Ǫ�����
	std::vector<ScrollMonsterFactory::MonsterName>_monterTypes;
	MissionMG::State _missionState;
protected:
	void UpdateCamera();
	//*�Ѯ�Ҳ�
	void addWeatherModule(int weatherMinY,int weatherMaxY);
	void addTransferPoint(int x, int y, SplashSceneFactory::preLoadScene nextScene);
	//*�P�o�i�a�ϳ̪񪺬O���@�Ӷǰe�I
	cocos2d::Vec2 checkCharacterLastMapPos();
	//*NPC Zorder rand,will behind the character or not.
	inline int NPCzorderRand() { return rand() % 2 ? NPC_ZORDER : NPC2_ZORDER; };
	//*�h�֬��u�Xdialog
	void forceDialog(MissionMG::MainName WhichMissionState, std::string dialogCode, float delay);
	//*�ǤJx y isBigger�p�G��false �N�|�ݥD��pos�O���O�p��ǤJ��Pos �p�G����F��N�|�u�Xdialog
	void forceDialogWithCharacterPos(MissionMG::MainName WhichMissionState, std::string dialogCode, cocos2d::Vec2 pos,bool isBigger);
private:
	//*�n�P�_���a�ާ@�ѤW���U��,�Ĥ@��Ĳ�N��pos
	cocos2d::Vec2 _firstTouchPos;
	//*�D���O�_�i�H���U,�ۤv�����P�_
	bool _canJumpDown;
	static ScrollScene* static_ScrollScene;
	MapNameTip* _mapNameTip;
	bool _isAddMonsterModule;
	//*�O�_��F�a�����
	bool _isOnMaplimit;
	int _weatherMinY;
	int _weatherMaxY;
	int _forceDialogSchduleCount;
private:
	//*�W�[�@����
	//*isFirstTime��true���X�ͦ�l���H��
	//*isFirstTime��false�ɶ��X�ͦ�l�O�b�a�Ϫ��k��
	void addOneCloud(bool isFirstTime);
	//*������ͦ��ɡA�ھڥD���y�Ъ�l�����Y��l
	void initCamera();
	//*�ˬd�D�����S���b�W�@�i�a�Ϫ���l�A�p�G������l�A�ھڦ�l�h����W�@�i�a�Ϫ��ǰe�I
	void updateHUDLayerWithTransferPoint();
	//*�ھڥD���X���I,�ӧP�_�D���ӭ�����Ӥ�V
	void fixMainCharacterFaceDirection();
	cocos2d::Vec2 CheckMissionStateAndSetCharacterBornPos();
};
class LuoYangScene :public ScrollScene
{
public:
	static cocos2d::Scene* createScene();
	CREATE_FUNC(LuoYangScene);
	LuoYangScene();
	void ReCheckForceDialog(bool isInitCall);
};
class ChiuanJenMountScene :public ScrollScene
{
public:
	static cocos2d::Scene* createScene();
	CREATE_FUNC(ChiuanJenMountScene);
	ChiuanJenMountScene();
	void ReCheckForceDialog(bool isInitCall);
};
class ChiuanJenScene :public ScrollScene
{
public:
	static cocos2d::Scene* createScene();
	CREATE_FUNC(ChiuanJenScene);
	ChiuanJenScene();
	void ReCheckForceDialog(bool isInitCall);
};
class ForestFieldScene :public ScrollScene
{
public:
	static cocos2d::Scene* createScene();
	CREATE_FUNC(ForestFieldScene);
	ForestFieldScene();
	void ReCheckForceDialog(bool isInitCall);
};
class CaveScene :public ScrollScene
{
public:
	static cocos2d::Scene* createScene();
	CREATE_FUNC(CaveScene);
	void ReCheckForceDialog(bool isInitCall);
	CaveScene();
};
class TestScene :public ScrollScene
{
public:
	static cocos2d::Scene* createScene();
	CREATE_FUNC(TestScene);
	void ReCheckForceDialog(bool isInitCall);
	TestScene();
};
NS_SCROLLMODE_END
#endif 
