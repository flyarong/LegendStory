#include "SoldierAI.h"
#include "Monster\BattleMode\BattleSoldierFactory.h"
#include "Map\BattleMode\BattleMap.h"
#include "UI\BattleMode\BattleHUDlayer.h"
#include "AudioEngine.h"
#include "ExtensionsAlgorithm.h"
#include "DebugLayer.h"
USING_NS_CC;
using namespace experimental;
NS_BATTLEMODE_BEGAN
enum class SoldierType;
enum class SoldierCamp;
SoldierAI* SoldierAI::static_monsterAi_ptr = nullptr;
namespace
{
	//�ڤ�x�ΰ}�����
	const int const_ourLegionFormationRow=6;
	//�ڤ�x�ΰ}���C��
	const int const_ourLegionFormationColumn=5;
	//�Ĥ�x�ΰ}�����
	const int const_enemyLegionFormationRow = 6;
	//�Ĥ�x�ΰ}���C��
	const int const_enemyLegionFormationColumn = 5;
	//�ڤ�}���_�l�I
	Vec2 FormationStartVec2;
	//�ڤ�}�涡���Z��
	Size const_OurLegionFormationSize = Size(12, 25);
	//�Ĥ�}�涡���Z��
	Size const_EnemyLegionFormationSize = Size(12, 25);
}
SoldierAI::SoldierAI()
{
	visible = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	this->_structAICommand.AttackCommand = true;
	this->_structAICommand.StartBattleCommand = false;
	this->_structAICommand.FindEnemyCommand= true;
	this->_structAICommand.AssignMoveCommand = false;
}
SoldierAI::~SoldierAI()
{
	CC_SAFE_DELETE(static_monsterAi_ptr);
}
void SoldierAI::clearData()
{
	_soldiers.clear();
	_ourSideSoldiers.clear();
	_enemySideSoldiers.clear();
	_ourLegions.clear();
	_enemyLegions.clear();
}
SoldierAI* SoldierAI::getInstance()
{
	if (static_monsterAi_ptr == nullptr)
	{
		static_monsterAi_ptr = new SoldierAI;
	}
	return static_monsterAi_ptr;
}
void SoldierAI::updateBattleSoldier()
{
	if (_soldiers.size() != 0)
	{
		for (auto it = this->_soldiers.begin();it != this->_soldiers.end();)
		{
			if ((*it)->getSoldierHp()<=0 && (*it)->_structSoldierState.isDeath==false)
			{//�p�G��q�p��0
				(*it)->_structSoldierState.isDeath = true;
				(*it)->KillYourSelf();
				BattleSoldierFactory* soldier = static_cast<BattleSoldierFactory*>(*it);
				//�p�G��e���`�����O�x�αN�x
				if (soldier == soldier->_belongLegion->_general)
				{//���N�x���U�h�L����������A
					this->unselectedLegionSoldier(soldier->_belongLegion);
				}
				this->removeFromCampVector((*it)->_soldierCamp, *it);
				this->removeFromSideVector((*it)->_soldierCamp, *it);
				it = this->_soldiers.erase(it);
			}
			else
			{
				(*it)->updateBattleSoldier();
				it++;
			}
		}
	}
}
void SoldierAI::searchTarget()
{
	std::multimap<int, BattleSoldierFactory*> oursoldierTree;
	std::multimap<int, BattleSoldierFactory*> enemysoldierTree;
	for (auto it = _soldiers.begin();it != _soldiers.end();it++)
	{
		if ((*it)->_soldierCamp == SoldierCamp::OurSoldier)
		{
			oursoldierTree.insert(std::pair<int, BattleSoldierFactory*>((*it)->_gridIndex, *it));
		}
		else
		{
			enemysoldierTree.insert(std::pair<int, BattleSoldierFactory*>((*it)->_gridIndex, *it));
		}
	}
	for (auto it = _soldiers.begin();it != _soldiers.end();)
	{
		if (this->_structAICommand.FindEnemyCommand)
		{
			std::vector<int> wantFindGrids;
			wantFindGrids.push_back((*it)->_gridIndex);//��
			wantFindGrids.push_back((*it)->_gridIndex - 1);//��
			wantFindGrids.push_back((*it)->_gridIndex - 2);//��
			wantFindGrids.push_back((*it)->_gridIndex - 3);//��
			wantFindGrids.push_back((*it)->_gridIndex - 4);//��
			wantFindGrids.push_back((*it)->_gridIndex - 5);//��
			wantFindGrids.push_back((*it)->_gridIndex - 1 + const_MaxGridXIndex);//���W
			wantFindGrids.push_back((*it)->_gridIndex - 1 - const_MaxGridXIndex);//���U
			wantFindGrids.push_back((*it)->_gridIndex + 1);//�k
			wantFindGrids.push_back((*it)->_gridIndex + 2);//�k
			wantFindGrids.push_back((*it)->_gridIndex + 3);//�k
			wantFindGrids.push_back((*it)->_gridIndex + 4);//�k
			wantFindGrids.push_back((*it)->_gridIndex + 5);//�k
			wantFindGrids.push_back((*it)->_gridIndex + 1 + const_MaxGridXIndex);//�k�W
			wantFindGrids.push_back((*it)->_gridIndex + 1 - const_MaxGridXIndex);//�k�U
			wantFindGrids.push_back((*it)->_gridIndex + const_MaxGridXIndex);//���W
			wantFindGrids.push_back((*it)->_gridIndex - const_MaxGridXIndex);//���U
			Vec2 pretendCloserDistanceVec2 = Vec2(9999, 9999);
			BattleSoldierFactory* closerEnemy = nullptr;
			int otherSideSoldierCount = 0;
			for (auto grid : wantFindGrids)
			{
				std::multimap<int, BattleSoldierFactory*>::iterator begin;
				std::multimap<int, BattleSoldierFactory*>::iterator end;
				if ((*it)->_soldierCamp == SoldierCamp::OurSoldier)
				{//�p�G��e�h�L�O�ڤ�h�L�A���u�j�M�Ĥ�ĤH
					begin = enemysoldierTree.lower_bound(grid);
					end = enemysoldierTree.upper_bound(grid);
				}
				else
				{//�p�G��e�h�L�O�ڤ�h�L�A���u�j�M�Ĥ�ĤH
					begin = oursoldierTree.lower_bound(grid);
					end = oursoldierTree.upper_bound(grid);
				}
				while (begin != end)
				{
					float distance = (*it)->getPosition().distance(begin->second->getPosition());
					float oldVec2distance = (*it)->getPosition().distance(pretendCloserDistanceVec2);
					//�Ƕi�Ӫ��ĤH��l�Z��  �񰲩w���̤p���٭n��p �N�ѥ��������w�̤p��
					//�C��h�L�̦h�u��Q��ӼĤH��w
					if (distance < oldVec2distance && begin->second->_beingEnemyTagetCount < 2)
					{
						closerEnemy = begin->second;
						pretendCloserDistanceVec2 = begin->second->getPosition();
						otherSideSoldierCount++;
					}
					begin++;
				}
			}
			//����W�䤣��h�L��
			if (otherSideSoldierCount == 0)
				(*it)->_target = nullptr;
			else
			{//��h�L���ؼЮ� �s��ؼЪ����w �ç�ؼЪ�"�Q�ĤH�]���ؼ�"���p��+1
				if (closerEnemy != nullptr)
				{
					(*it)->_target = closerEnemy;
					closerEnemy->_beingEnemyTagetCount++;
				}
			}
		}
		it++;
	}
	for (auto soldier = this->_soldiers.begin();soldier != this->_soldiers.end();soldier++)
	{
		(*soldier)->_beingEnemyTagetCount = 0;
	}
}
void SoldierAI::updateSoldierAI()
{
	if (!this->_soldiers.empty())
	{
		this->searchTarget();
		//_t1=std::thread(&SoldierAI::searchTarget, this);
		//_t1.detach();
	}
}
void SoldierAI::updateSoldierPosition()
{
	if (this->_structAICommand.StartBattleCommand)
	{
		if (!this->_soldiers.empty())
		{
			for (auto it = this->_soldiers.begin();it != this->_soldiers.end();	it++)
			{
				if ((*it)->_soldierCamp == SoldierCamp::OurSoldier)
				{//�ڤ�x��AI
					if ((*it)->_structSoldierState.isAttack == false &&
						(*it)->_target != nullptr &&
						(*it)->getPosition().distance((*it)->_target->getPosition()) < 100.f)
					{//�a��ĤH�ɡA�h�L�|�ۤv�h�����ĤH
						(*it)->MoveToTargetPosition();
					}
					else if (!(*it)->_structSoldierState.isArriveAssignPosition && 
						!(*it)->_structSoldierState.isAttack &&
						_structAICommand.AssignMoveCommand)
					{//�e�����w�a�I
						(*it)->MoveToAssignPosition();
					}
					else
					{//�S�������R�O ��a�ݾ�
						(*it)->stayYourPosition();
					}
				}
				else
				{//�ĤHAI
					//�a��ĤH�@�w���Z���|�ܦ��������A �ҥH�n�P�_�D�������A�� �~�ಾ��
					if ((*it)->_target != nullptr)
					{
						(*it)->MoveToTargetPosition();
					}
					else
					{//�䤣��ĤH�~��e�i
						(*it)->forwardPosition();
					}
				}
				(*it)->checkGrid();
			}
		}
	}
}
void SoldierAI::generateOneLegion(int LegionIndex, int SoldierAmount, SoldierType SoldierType, SoldierCamp SoldierCamp, BattleMap* battleMap , Vec2 generatePos)
{
	Legion* legion = new Legion(LegionIndex, SoldierCamp);
	for (int i = 0;i < SoldierAmount;i++)
	{
		BattleSoldierFactory* Soldier=nullptr;
		switch (SoldierType)
		{
		case BattleMode::SoldierType::Assassin:
			Soldier = Assassin::create();
			break;
		case BattleMode::SoldierType::Musket:
			Soldier = MusketSoldier::create();
			break;
		case BattleMode::SoldierType::ShieldSoldier:
			Soldier = ShieldSoldier::create();
			break;
		default:
			CCASSERT(false, "warning");
		}
		switch (SoldierCamp)
		{
		case BattleMode::SoldierCamp::OurSoldier:
			Soldier->_soldierCamp = BattleMode::SoldierCamp::OurSoldier;
			this->_ourSideSoldiers.push_back(Soldier);
			break;
		case BattleMode::SoldierCamp::EnemySoldier:
			Soldier->_soldierCamp = BattleMode::SoldierCamp::EnemySoldier;
			Soldier->setFaceDirection(true);
			this->_enemySideSoldiers.push_back(Soldier);
			break;
		default:
			CCASSERT(false, "impossible");
		}
		legion->addToLegion(Soldier);//�N�h�L�[�J�x�κ޲z
		Soldier->_belongLegion = legion;//�C�Ӥh�L�ݭn���D�ۤv�����ݭx��
		_soldiers.push_back(Soldier);//�N�h�L�[�J�Գ��W�Ҧ��h�L�޲z
		battleMap->addChild(Soldier, 3);
	}
	//�}��޲z
	switch (SoldierCamp)
	{
	case BattleMode::SoldierCamp::OurSoldier:
		legion->_general = Lyonar::create();//���ͤ@�ӧڤ�N�x
		legion->_general->_soldierCamp = BattleMode::SoldierCamp::OurSoldier;//�N�x���ݰ}��
		legion->_general->_belongLegion = legion;//�N�x�ݭn���D���ݭx��
		_ourLegions.push_back(legion);//���e�x�Υ[��ڤ�}�礤
		_ourSideSoldiers.push_back(legion->_general);//���e�N�x�[�J��ڭ̩Ҧ����޲z
		break;
	case BattleMode::SoldierCamp::EnemySoldier:
		legion->_general = ShieldSoldier::create();//���ͤ@�ӼĤ�N�x
		legion->_general->_soldierCamp = BattleMode::SoldierCamp::EnemySoldier;//�N�x���ݰ}��
		legion->_general->_belongLegion = legion;//�N�x�ݭn���D���ݭx��
		legion->_general->setFaceDirection(true);//�ĤH�����V
		_enemyLegions.push_back(legion);//���e�x�Υ[��Ĥ�}�礤
		_enemySideSoldiers.push_back(legion->_general);
		break;
	default:
		CCASSERT(false, "impossible");
	}
	legion->addToLegion(legion->_general);//��N�x�[�J�x�Τ�
	_soldiers.push_back(legion->_general);//��N�x�[�J��Գ��W�Ҧ��h�L�޲z��
	battleMap->addChild(legion->_general, 3);
	//�x�αư}��
	legion->startFormation(generatePos);
	//���Φa�Ϧ���l�A�åB�i�D�Գ��W���H�A�ۤv����l���ޡC
	splitMapToGrid();
}

void SoldierAI::removeFromCampVector(SoldierCamp soldierCamp,BattleSoldierFactory* RemoveSoldierPtr)
{
	std::vector<Legion*>* Legions;
	switch (soldierCamp)
	{
	case SoldierCamp::OurSoldier:
		Legions = &_ourLegions;
		break;
	case SoldierCamp::EnemySoldier:
		Legions = &_enemyLegions;
		break;
	}
	for (auto Legion = (*Legions).begin();Legion != (*Legions).end();)
	{
		if ((*Legion)->_legionSoldiers.size() != 0)
		{
			//�x���̪��x�Ϊ��C�Ӥh�L
			for (auto LegionSoldier =(*Legion)->_legionSoldiers.begin();LegionSoldier != (*Legion)->_legionSoldiers.end();LegionSoldier++)
			{
				if (*LegionSoldier == RemoveSoldierPtr)
				{
					(*Legion)->_legionSoldiers.erase(LegionSoldier);
					break;
				}
			}
			Legion++;
		}
		//�h�L��0 �Q���ΡC�åB��Q���Τh�L�A���X�}��x��
		else
		{
			Legion=(*Legions).erase(Legion);
		}
	}
}
void SoldierAI::removeFromSideVector(SoldierCamp soldierCamp, BattleSoldierFactory * RemoveSoldierPtr)
{
	std::vector<BattleSoldierFactory*>* SideSoldiers;
	switch (soldierCamp)
	{
	case SoldierCamp::OurSoldier:
		SideSoldiers = &_ourSideSoldiers;
		break;
	case SoldierCamp::EnemySoldier:
		SideSoldiers = &_enemySideSoldiers;
		break;
	}
	for (auto sideSoldier = (*SideSoldiers).begin();sideSoldier != (*SideSoldiers).end();sideSoldier++)
	{
		if (*sideSoldier == RemoveSoldierPtr)
		{
			SideSoldiers->erase(sideSoldier);
			break;
		}
	}
}

void SoldierAI::unselectedLegionSoldier(Legion* selectedLegion)
{
	selectedLegion->_isSelected = false;
	for (auto LegionSoldier : selectedLegion->_legionSoldiers)
	{
		static_cast<BattleSoldierFactory*>(LegionSoldier)->_standPointSprite->setVisible(false);
	}
}

void SoldierAI::splitMapToGrid()
{
	std::vector<BattleSoldierFactory*> _TotalSoldier = _soldiers;
	int GridIndex = 0;
	//�ĴX��X��l
	int gridXIndex = 0;
	//�ĴX��Y��l
	int gridYIndex = 0;
	//200*600�j�p�a�� �Q����100*100���p��l �@��120��l
	for (int i=0;i<const_MaxGridXIndex*const_MaxGridYIndex;i++)
	{
		if (GridIndex % const_MaxGridXIndex == 0 && GridIndex != 0)
		{//���C��
			gridXIndex = 0;
			gridYIndex++;
		}
		for (auto soldier = _TotalSoldier.begin();soldier != _TotalSoldier.end();)
		{
			if (((*soldier)->getPosition().x >= gridXIndex * const_GridLengthX &&
				(*soldier)->getPosition().x < (gridXIndex + 1) * const_GridLengthX)&&
				((*soldier)->getPosition().y >= gridYIndex * const_GridLengthY &&
				(*soldier)->getPosition().y < (gridYIndex + 1) * const_GridLengthY))
			{
				(*soldier)->_gridIndex = GridIndex;
				(*soldier)->_gridXIndex = gridXIndex;
				(*soldier)->_gridYIndex = gridYIndex;
				soldier = _TotalSoldier.erase(soldier);
			}
			else
			{
				soldier++;
			}
		}
		gridXIndex++;
		GridIndex++;
	}
}


//@Legion
Legion::Legion(int legionIndex, SoldierCamp soldierCamp):
_LegionIndex(legionIndex),
_SoldierCamp(soldierCamp)
{
	
}
void Legion::showSoldierStandSprite()
{
	for (auto& it : this->_legionSoldiers)
	{
		it->getChildByTag(BattleSoldierFactory::STAND_POINT_TAG)->setVisible(true);
	}
}
void Legion::hideSoldierStandSprite()
{
	for (auto& it : _legionSoldiers)
	{
		it->getChildByTag(BattleSoldierFactory::STAND_POINT_TAG)->setVisible(false);
	}
}
void Legion::addToLegion(BattleSoldierFactory* battleSoldier)
{
	_legionSoldiers.push_back(battleSoldier);
}
void Legion::startFormation(Vec2 generatePos)
{
	int currentRow=0;
	int currentColumn=1;
	Vec2 currentVec2=generatePos;
	FormationStartVec2 = currentVec2;
	for (auto it: _legionSoldiers)
	{
		if (it == this->_general)//�x�αN�x
		{
			switch (this->_SoldierCamp)
			{
			case SoldierCamp::OurSoldier:
				it->setPosition(Vec2(currentVec2.x-const_OurLegionFormationSize.width,
					FormationStartVec2.y - (const_OurLegionFormationSize.height*const_ourLegionFormationRow)/2));
				//�ڤ�x�μW�[Ĳ���ƥ�
				this->addGeneralTouchListener(it);
				break;
			case SoldierCamp::EnemySoldier:
				it->setPosition(Vec2(currentVec2.x+const_EnemyLegionFormationSize.width*4,
					FormationStartVec2.y - (const_EnemyLegionFormationSize.height*const_enemyLegionFormationRow) / 2.5));
				break;
			}			
		}
		else//�h�L
		{
			it->setPosition(currentVec2);
			switch (this->_SoldierCamp)
			{
			case SoldierCamp::OurSoldier:
				if (currentRow < const_ourLegionFormationRow-1)
				{
					currentVec2.x = currentVec2.x + const_OurLegionFormationSize.width;
					currentVec2.y = currentVec2.y - const_OurLegionFormationSize.height;
					currentRow++;
				}
				else
				{
					currentVec2.x = FormationStartVec2.x - currentColumn*const_OurLegionFormationSize.width * 2;
					currentVec2.y = FormationStartVec2.y;
					currentRow = 0;
					currentColumn++;
				}
				break;
			case SoldierCamp::EnemySoldier:
				
				if (currentRow < const_enemyLegionFormationRow - 1)
				{
					currentVec2.x = currentVec2.x + const_EnemyLegionFormationSize.width;
					currentVec2.y = currentVec2.y - const_EnemyLegionFormationSize.height;
					currentRow++;
				}
				else
				{
					currentVec2.x = FormationStartVec2.x + currentColumn*const_EnemyLegionFormationSize.width * 2;
					currentVec2.y = FormationStartVec2.y;
					currentRow = 0;
					currentColumn++;
				}
				break;
			}
		}
		//���w���w��l�A�N�O��e��l�C�Ψӫ᭱���w��l����k�C�D�n���ت��O���L���D�}�����ˤl
		it->_assignPosition = it->getPosition();
	}
}
void Legion::addGeneralTouchListener(BattleSoldierFactory* general)
{
	//@touch
	//Ĳ���x�αN�x�ƥ�
	auto generalTouchEvent = EventListenerTouchOneByOne::create();
	generalTouchEvent->onTouchBegan = [this](Touch* touch, Event* event)
	{
		//�ѩ��j�Y�p �|���T�I�ܧ�A�w�]�I����l�O�̷�Vec2(0,0)�T�I�h�P�_
		//�g�LĲ���I�[�W�T�I*�a�Ϥj�p�A�N�O�ܧ�T�I��s�������q
		Vec2 battleMapAnchorPoint = BattleMap::getBattleMap()->getAnchorPoint();
		Size battleMapSize = BattleMap::getBattleMap()->getBoundingBox().size;
		battleMapSize.width = battleMapSize.width*battleMapAnchorPoint.x;
		battleMapSize.height = battleMapSize.height*battleMapAnchorPoint.y;
		//Ĳ���I�[�W�a�ϰ����q=�I���b�a�ϤW��Vec2
		Vec2 touchOnBattleMap = touch->getLocation() + (-BattleMap::getBattleMap()->getPosition());
		//�Y��᪺�a�Ϥj�p
		touchOnBattleMap.x = touchOnBattleMap.x + battleMapSize.width;
		touchOnBattleMap.y = touchOnBattleMap.y + battleMapSize.height;
		//�I���N�x�P�_
		if (ExtensionsAlgorithm::ContainsPoint(
			touchOnBattleMap, _general->getPosition()*BattleMap::getBattleMap()->getScale(), _general->getBoundingBox().size.width, _general->getBoundingBox().size.width))
		{
			//�I���N�x�ɡA����Ĳ������
			
			//*�q�N�x���o�����ݭx�ΡA�óq���x�ΰ���selevtLegionSoldier
			Legion* selectedLegion = ((BattleSoldierFactory*)event->getCurrentTarget())->_belongLegion;
			if (!selectedLegion->_isSelected)
			{
				AudioEngine::play2d("sound/selected.mp3", false);
				selectedLegion->showSoldierStandSprite();
			}
			else
			{
				AudioEngine::play2d("sound/waitingCommand.mp3", false);
				selectedLegion->hideSoldierStandSprite();
			}
			selectedLegion->_isSelected = !selectedLegion->_isSelected;
		}
		return false;
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(generalTouchEvent, this->_general);
	//Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(generalTouchEvent, 3);
}
int SoldierAI::getOurSoldierSelectedTotal()
{
	int selectedSum = 0;
	for (auto& it : _ourLegions)
	{
		if (it->_isSelected)
		{
			selectedSum++;
		}
	}
	return selectedSum;
}
int SoldierAI::countHowManyLegionGetSelected()
{
	int count = 0;
	for (auto& it : _ourLegions)
	{
		if (it->_isSelected)
		{
			count++;
		}
	}
	return count;
}
void SoldierAI::updateSelectedLegionFlagTargetPosition(Vec2 FlagPos)
{
	int HowManyOurLegionGetSelected = this->countHowManyLegionGetSelected();
	int index = 0;
	Vec2 lastGeneralAssignPos;
	for (auto& it : _ourLegions)
	{
		if (it->_isSelected)
		{
			//�U�����O�������ܪ��A
			it->_isSelected = false;
			it->hideSoldierStandSprite();
			it->_targetFlagPosition = FlagPos;
			Vec2 GeneralToFlagPosVec2= it->_targetFlagPosition - it->_general->_assignPosition;
			if (HowManyOurLegionGetSelected > 1 && index == 0)
			{//���ܭx�μƶW�L1�ӥH�W�� ���e�N�x���w��l�s�_�ӡA�d���U�ӭx�Ϋ��w�ɪ��۹��l�A�H�����ӭx�������}��
				lastGeneralAssignPos = it->_general->_assignPosition;
			}
			for (auto& legionSoldier : it->_legionSoldiers)
			{
				//�p�G��ܪ��x�� �j��@���H�W Y�b����
				if (HowManyOurLegionGetSelected > 1)
				{
					legionSoldier->_assignPosition.x += GeneralToFlagPosVec2.x- (lastGeneralAssignPos.x-it->_general->_assignPosition.x);
					legionSoldier->_assignPosition.y += 0;
				}
				else//�u����ܤ@���x�� ���`���w�a�I
					legionSoldier->_assignPosition+=GeneralToFlagPosVec2;
				legionSoldier->_structSoldierState.isArriveAssignPosition = false;
			}
			index++;
		}
	}
}
void SoldierAI::updateMusketSoldierReadyToShootState()
{
	for (auto& it : _ourLegions)
	{
		if (it->_isSelected)
		{
			for (auto& legionSoldier : it->_legionSoldiers)
			{
				if (legionSoldier->_soliderType == SoldierType::Musket )
				{//�૬��musketSoldier�l��  �~��ϥ�musketSoldier�W�����\��
					MusketSoldier* musketSoldier = static_cast<MusketSoldier*>(legionSoldier);
					//�D���� �D�������A �åB���W���u�� �~���|�j
					if (musketSoldier->_structExtensionsSoldierState.HaveAmmo && 
						!musketSoldier->_structSoldierState.isAttack &&
						!musketSoldier->_structSoldierState.isRun)
					{
						musketSoldier->resetAllSoldierState();
						musketSoldier->_structExtensionsSoldierState.isReadyToShoot = true;
						musketSoldier->ReadyToShootMethod();
					}
				}
			}
		}
	}
}
void SoldierAI::updateMusketSoldierShoot()
{
	for (auto& it : _ourLegions)
	{
		if (it->_isSelected)
		{
			for (auto& legionSoldier : it->_legionSoldiers)
			{
				if (legionSoldier->_soliderType == SoldierType::Musket && 
					!legionSoldier->_structSoldierState.isRun)
				{//�૬��musketSoldier�l��  �~��ϥ�musketSoldier�W�����\��
					MusketSoldier* musketSoldier = static_cast<MusketSoldier*>(legionSoldier);
					//�ݭn�A�|�j�ʧ@�U �~��}�j
					if (musketSoldier->_structExtensionsSoldierState.isReadyToShoot)
					{
						musketSoldier->resetAllSoldierState();
						musketSoldier->_structExtensionsSoldierState.isShoot = true;
						musketSoldier->ShootMethod();
					}
				}
			}
		}
	}
}
NS_BATTLEMODE_END