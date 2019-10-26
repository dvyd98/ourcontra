#include "Audio.h"
#include "Windows.h"


Audio::Audio()
{
	soundEngine = createIrrKlangDevice();
	init();
}


Audio::~Audio()
{
	for (int i = 0; i < NUM_SOUNDS; ++i) {
		sounds[i]->drop();
		sounds[i] = 0;
	}
	soundEngine->drop();
}
 
void Audio::init()
{
	bindAudio("music/Title_Screen.mp3", TITLE_MUSIC);
	bindAudio("music/Stage1.mp3", STAGE1_MUSIC);
	bindAudio("music/Stage2.mp3", STAGE2_MUSIC);
	bindAudio("music/GameOver.mp3", GAMEOVER_MUSIC);
	bindAudio("music/BaseBoss.mp3", BASEBOSS_MUSIC);
	bindAudio("music/Boss death 1.wav", BOSS_DEATH_1_AUDIO);
	bindAudio("music/Boss death 2.wav", BOSS_DEATH_2_AUDIO);
	bindAudio("music/Bridge explosion effect.wav", BRIDGE_EXPLOSION_SOUND);
	bindAudio("music/Death.wav", DEATH_SOUND);
	bindAudio("music/Enemy death.wav", ENEMY_DEATH_SOUND);
	bindAudio("music/Laser touch.wav", LASER_TOUCH_SOUND);
	bindAudio("music/Normal gun.wav", NORMAL_GUN_SOUND);
	bindAudio("music/Spread gun.wav", SPREAD_GUN_SOUND);
	bindAudio("music/Turret got hit.wav", TURRET_HIT_SOUND);
}

void Audio::bindAudio(const char * file, int i) {
	sounds[i] = soundEngine->addSoundSourceFromFile(file);
}

void Audio::play(int i, bool loop) {
	soundEngine->play2D(sounds[i], loop);
}

void Audio::stopAllSounds() {
	soundEngine->stopAllSounds();
}
