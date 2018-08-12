#ifndef __MyJsonParser__H__
#define __MyJsonParser__H__
#include "cocos2d.h"
#include "RPGTools\ItemSprite.h"
namespace MyTools
{
	class JsonParser
	{
	public:
		enum class JsonType
		{
			Save,
			UserInterFace,
			TableCode,
			NPC,
			ItemDOC,
			Mission
		};
		enum class Equipment
		{
			GetID,
			GetValue
		};
	public:
		static std::string getJsonString(const char* first, const char* second, JsonType jsonType);
		//*�M�����D���ϥΪ����f
		static std::string getJsonStringWithOutSave(const char* first, const char* second, JsonType jsonType);
		static std::string getNPCJsonFile(const char* NPCName, const char* Opa, int index = 0);
		static std::string getDialogString(std::string DialogCode, int index);
		//*����s�ɩҦ��r��
		static std::string getSaveJsonStr();
		static void setSaveJsonStr(std::string newSaveJsonStr);
		static void setSave(const char* first, const char* second, const char* newValue);
		//*ItemSprite class �ϥγo��API�i�H��o���~��T
		static void getItemDataFromSave(int index, ItemSprite::ItemData& itemData);
		//*���s��z���s �ϥγo��API�i�H��o�Ҧ����~��T
		//*�åB��Ҧ����~��T���ǤJ��vector
		static void getItemAllDataFromSave(std::vector<std::pair<int,int>>& itemDatas);
		//*RPGTool::CharacterPart
		static int getEquipmentFromSave(int characterPart,Equipment equipmentOperator);
		//*�����e�I�]���~�ƶq
		static int getItemCountFromSave();
		//*�����e�I�]���~�٦��h�֪Ŧ�
		static int getFreeItemSpaceCountFromSave();
		//*�ǤJ���~ID�A��Ʒ|�۰��ഫ�����������ⳡ��
		//*�ק�˳Ƥ������~
		static void setEquipmentFromSave(int ItemID,int ItemRank);
		//*�ǤJ�n�ק諸���~�b�I�]�����ĴX��A�s��ID�A�s������
		//*�ק�I�]�������~
		static void setItemBagFromSave(int ItemIndex,int ItemID, int ItemRank);
		//*�ǤJ�w�g�����~�Ҧ���ƪ�vector �N���s�Ҧ����~��T
		static void setItemAllDataFromSave(std::vector<std::pair<int, int>>& itemDatas);
		//*RPGTool::ItemID   �ĤG�ӰѼƮھڲĤ@�ӰѼƦ����P�ĪG
		//*�p�GItemID < _STACK_LINE �N��ItemValue=ItemRank
		//*�p�GItemID > _STACK_LINE �N��ItemValue=ItemAmount
		//*�W�[���~���\�^�ǧ⪫�~�[��ĴX�檫�~�A�W�[���Ѧ^��-1
		static int AddItemToSave(int ItemID, int ItemValue);
		//*�������~
		static void removeItemFromSave(int index);
		//*�ھڦs�ɸ̪��y���A�ӵ������P�r��
		static char* fontType();
		//*�ھڦs�ɸ̪��y���A�ӵ������P�r�Τj�p
		static float fontSize(float Size);
		static void buildNewSave();
		static bool haveSaveFile();
	private:
		static char* accordingEnumGivingPath(JsonType jsonType);
		static std::string getFromSaveLanguage();
		static std::string ReadDramaJsonFile(const char* fileName, const char* DialogCode,int index);
		static std::string ReadJsonFile(const char* first, const char* second, const char* fileName);
		//*�w��Ū��TableCodeŪ���ɳ]�p�����f�A��second�����䤣��� �|�^��second�r��C
		static std::string ReadTableCodeJsonFile(const char* first, const char* second, const char* fileName);
		static void SetJsonFile(const char* first, const char* second, const char* fileName, const char* newValue);

	};
}
#endif