#ifndef __RPGTool_H__
#define __RPGTool_H__

#include "cocos2d.h"

class RPGTool
{
public:
	enum class ItemID
	{
		ItemID_Start,
		Hair_Tail,
		Hair_Explore,
		Hair_Blade,
		Hair_Band,
		Hair_Hat,
		Head_Normal,
		Head_Goat,
		Cloth_Farmer,
		Cloth_ChiuanJen,
		Cloth_Naked,
		Cloth_BlueKnight,
		Cloth_Blody,
		Pant_ChiuanJen,
		Pant_Farmer,
		Pant_Naked,
		Pant_BlueKnight,
		Pant_Blody,
		Weapon_Fist,
		Weapon_Fist_Blue,
		Weapon_Sword,
		Weapon_NoobSword,
		Weapon_Shadow,
		Weapon_Blody,


		_STACK_LineBegin,
		_Drink_LineBegin,

		_HP_PotionLineBegin,
		SmallRedPotion,
		MiddleRedPotion,
		LargeRedPotion,
		_HP_PotionLineEnd,

		_MP_PotionLineBegin,
		SmallBluePotion,
		MiddleBluePotion,
		LargeBluePotion,
		_MP_PotionLineEnd,

		_Drink_LineEnd,
		_STACK_LineEnd,
		
		ItemID_End
	};
	enum class CharacterPart
	{
		Hair,    //�Y��+��q
		NeckLace,//����+���O  
		Belt,    //�y�a+���O
		Cloth,   //�W��+���m
		Pant,    //�U�� + ���m
		Weapon,  //�Z��+�����O
		Head,     //�D�i��ﶵ��
		HPBar,
		MPBar,
		None     //�����O
	};
	enum class BaseProperty
	{
		Blood,    //���
		Force,    //�Z�O
		Magic,    //���O
		Defend,   //���m
		Dodge,    //���k
		Popular,  //�n��
	};
	struct BaseData
	{
		CharacterPart characterPart;
		std::string SpritePath;//�Ϥ����|
		std::string ItemName;
		int Price=0;//���ө�������
		int Blood=0;
		int Force=0;
		int Magic=0;
		int Defend=0;
		int Dodge=0;
		int Popular=0;
		int RecoverHP = 0;//�^�_Hp�q
		int RecoverMP = 0;//�^�_Mp�q
	};
	//*�ǤJRPGTool::ItemID �p�G�����~����|�A�^��true �_�hfalse
	static bool isItemIDCanStack(const int ItemID);
	//*�ǤJRPGTool::ItemID �p�G�����~��ܤU�A�^��true �_�hfalse
	static bool isItemIDCanDrink(const int ItemID);
	static BaseData accordingItemIDToData(const int itemId);
	//*�ǤJ�Q�n���D���ݩʡA��b���W���˳ơA�@�[�h��
	static int getEquipmentBaseProperty(BaseProperty baseProperty);
	//*�^��0~4�������H����
	static inline int getRandMaxItemValue() { return rand() % 11; };
	//*�I�]�̰����|��
	static const int Const_MaxItemStack;
	static const int Const_MaxItemCount;
};

#endif
