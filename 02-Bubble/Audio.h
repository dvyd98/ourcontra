#ifndef _AUDIO_INCLUDE
#define _AUDIO_INCLUDE

#include <irrKlang.h>

using namespace irrklang;

#define NUM_SOUNDS 1

enum AUDIOS {
	TITLE
};

class Audio
{
public:
	Audio();
	~Audio();

	void init();
	void bindAudio(const char* file, int i);
	void play(int i);

private:
	ISoundEngine* soundEngine;
	ISoundSource *sounds[NUM_SOUNDS];

};

#endif // _AUDIO_INCLUDE