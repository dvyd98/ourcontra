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
	bindAudio("music/Title_Screen.mp3", TITLE);
}

void Audio::bindAudio(const char * file, int i) {
	sounds[i] = soundEngine->addSoundSourceFromFile(file);
}

void Audio::play(int i) {
	soundEngine->play2D(sounds[i]);
}