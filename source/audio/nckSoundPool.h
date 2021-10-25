
/**
 * nctoolkit, realtime aplication framework, under zlib software license.
 * https://github.com/nczeroshift/nctoolkit
 */

#ifndef NCK_SOUND_POOL_H
#define NCK_SOUND_POOL_H

#include "nckThread.h"
#include "nckAudioDevice.h"
#include <list>

_AUDIO_BEGIN

class Sound {
public:
    ~Sound();
    int Write(uint8_t * outputBuffer, uint32_t outputBufferSize, uint32_t playHead, float volume);
    static Sound * Load(const std::string & fileName);
private:
    Sound();
    uint32_t m_DataLength;
    uint8_t * m_AudioData;
};

class SoundPool {
public:
    SoundPool();
    ~SoundPool();

    void Play(Sound * snd, float volume = 1.0f);
    void Write(uint8_t * outputBuffer, uint32_t outputBufferSize);
    void SetVolume(float volume);
    float GetVolume();

private:
    struct SoundContext {
        SoundContext(Sound * s, float v) { sound = s; volume = v; head = 0;}
        Sound * sound;
        uint32_t head;
        float volume;
    };

    float m_Volume;
    Core::Mutex * mutex;
    std::list<SoundContext> queuedSounds;
};

_AUDIO_END

#endif