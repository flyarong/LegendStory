#ifndef __SoldierAI__H__
#define __SoldierAI__H__
#include "DefinitionBattleMode.h"
#include "cocos2d.h"
NS_BATTLEMODE_BEGAN
class Legion;
class BattleSoldierFactory;
class BattleMap;
enum class SoldierType
{
	ShieldSoldier,
	Assassin,
	Musket,
	Lyonar
};
enum class SoldierCamp//�}��
{
	OurSoldier,
	EnemySoldier
};
class SoldierAI
{
public:
	struct AICommand
	{
		bool AttackCommand;
		bool StartBattleCommand;
		bool FindEnemyCommand;
		bool AssignMoveCommand;
	};
public:
	SoldierAI();
	~SoldierAI();
	static SoldierAI* getInstance();
	void clearData();
	void generateOneLegion(int LegionIndex, int SoldierAmount, SoldierType SoldierType, SoldierCamp SoldierCamp, BattleMap* battlemMap,cocos2d::Vec2 generatePos);
	void updateSoldierAI();
	void updateBattleSoldier();
	void updateSoldierPosition();
	//*according class Legion _isSelected is true
	//*update class Legion _targetFlagPosition;
	void updateSelectedLegionFlagTargetPosition(cocos2d::Vec2 FlagPos);
	void updateMusketSoldierReadyToShootState();
	void updateMusketSoldierShoot();
	int countHowManyLegionGetSelected();
	int getOurSoldierSelectedTotal();
	void searchTarget();
public:
	static SoldierAI* static_monsterAi_ptr;
	//*Ai thread
	std::thread _t1;
	//*�Գ��W�Ҧ����
	std::vector<BattleSoldierFactory*> _soldiers;
	//*�ڭ̰}��Ҧ����
	std::vector<BattleSoldierFactory*> _ourSideSoldiers;
	//*�Ĥ�}��Ҧ����
	std::vector<BattleSoldierFactory*> _enemySideSoldiers;
	//*�ڭ̩Ҧ��x��
	std::vector<Legion*> _ourLegions;
	//*�Ĥ�Ҧ��x��
	std::vector<Legion*> _enemyLegions;
	const int const_MaxGridXIndex = 20;
	const int const_MaxGridYIndex = 12;
	const int const_GridLengthX = 100;
	const int const_GridLengthY = 50;
	AICommand _structAICommand;
private:
	//*��h�L�ݭn�q�Գ��W�����ɡA�q�U�۩��ݪ��x��vector����
	void removeFromCampVector(SoldierCamp soldierCamp, BattleSoldierFactory* RemoveSoldierPtr);
	//*��h�L�ݭn�q�Գ��W�����ɡA�q�U�۩��ݪ��}��vector����
	void removeFromSideVector(SoldierCamp soldierCamp, BattleSoldierFactory* RemoveSoldierPtr);
	void unselectedLegionSoldier(Legion* selectedLegion);
	//���Φa�Ϧ���l�A�åB�i�D�Գ��W���H�A�ۤv����l���ޡC
	void splitMapToGrid();
private:
	cocos2d::Size visible;
	cocos2d::Vec2 origin;
	std::mutex _mu;
};

class Legion
{
	friend SoldierAI;
public:
	Legion(int legionIndex, SoldierCamp soldierCamp);
	//*let under soldier _standPointSprite getting visible
	void showSoldierStandSprite();
	//*let under soldier _standPointSprite getting invisible
	void hideSoldierStandSprite();
	void addToLegion(BattleSoldierFactory* battleSoldier);
private:
	//*when create Legion,formation all soldiers.
	void startFormation(cocos2d::Vec2 generatePos);
	//*general touch listener
	void addGeneralTouchListener(BattleSoldierFactory* generalPtr);
public:
	int _LegionIndex;
	//*SoldierAI::SoldierCamp enum to determine whether legion belongs
	BattleMode::SoldierCamp _SoldierCamp;
	bool _isSelected = false;
	cocos2d::Vec2 _targetFlagPosition;
	std::vector<BattleSoldierFactory*> _legionSoldiers;
	BattleSoldierFactory* _general;
};
NS_BATTLEMODE_END

#endif 

