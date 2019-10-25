#ifndef _AUDIO_INCLUDE
#define _AUDIO_INCLUDE

#include <irrKlang.h>

using namespace irrklang;

#define NUM_SOUNDS 14

enum AUDIOS {
	TITLE_MUSIC, STAGE1_MUSIC, STAGE2_MUSIC, GAMEOVER_MUSIC, BASEBOSS_MUSIC,
	BOSS_DEATH_1_AUDIO, BOSS_DEATH_2_AUDIO, BRIDGE_EXPLOSION_SOUND, DEATH_SOUND,
	ENEMY_DEATH_SOUND, LASER_TOUCH_SOUND, NORMAL_GUN_SOUND, SPREAD_GUN_SOUND, TURRET_HIT_SOUND
};

class Audio
{
public:
	Audio();
	~Audio();

	void init();
	void bindAudio(const char* file, int i);
	void play(int i);
	void stopAllSounds();

private:
	ISoundEngine* soundEngine;
	ISoundSource *sounds[NUM_SOUNDS];

};

#endif // _AUDIO_INCLUDE