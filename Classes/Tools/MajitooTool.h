#ifndef __MajitooTool_H__
#define __MajitooTool_H__

#include "cocos2d.h"

class MajitooTool
{
public:
	enum class SplitType
	{
		wantFacial,		//�y���r��
		wantNameCode,	//�W�l�N��
		MarkFront		//���S��аO���e���r��
	};
public:
	static std::string SplitStr(std::string readSplitStr, SplitType splitType);
};

#endif
