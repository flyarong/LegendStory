#ifndef __AudioManagement_H__
#define __AudioManagement_H__

#include "cocos2d.h"
class AudioManagement
{
public:
	enum class AudioType
	{
		Music,
		Sound
	};
public:
	static AudioManagement* getInstance();
	int play(const char* audioPath, AudioType audioType,bool isloop=false);
	//*�⼽�񤤪����W�A�ھڷs�����q�]�m�ӧ�s�A�s���n���j�p�C
	void updateNewAudioVolume();
	//*�Ȱ��Ҧ����񤤪����W
	void pauseAllAudio();
	//*�_��Ҧ��Ȱ��������W
	void resumeAllAudio();
	//*�������ɡA���Ҧ��n������A�òM������
	void clearAudioManagement();
	//*�ǤJ����ID,�ð����
	void stopAudioFromID(int ID);
private:
	static AudioManagement* static_audioManagement;
	std::map<int, AudioType> _audios;
};

#endif
