
/**
 * nctoolkit, realtime aplication framework, under zlib software license.
 * https://github.com/nczeroshift/nctoolkit
 */

#include "nckSoundPool.h"
#include "nckWavStream.h"
#include "nckUtils.h"
#include "nckException.h"

_AUDIO_BEGIN

Sound * Sound::Load(const std::string & fileName) {
    if(!Core::FileReader::Exists(fileName))
        THROW_EXCEPTION("Sound file \""+fileName+"\" doesn't exist");

    size_t length = Core::FileReader::Size(fileName);
    if(length == 0)
        THROW_EXCEPTION("Sound file \"" + fileName + "\" is empty");
    
    Sound * snd = new Sound();
    snd->m_AudioData = new uint8_t[length];

    uint32_t readSize = 0;
    WavStream * ws = WavStream::Load(fileName);
    while ((readSize = ws->Read(4096, &snd->m_AudioData[snd->m_DataLength])) > 0) 
        snd->m_DataLength += readSize;
    
    SafeDelete(ws);

    return snd;
}

Sound::Sound() {
    m_DataLength = 0;
    m_AudioData = NULL;
}


Sound::~Sound() {
    SafeArrayDelete(m_AudioData);
}

int Sound::Write(uint8_t * outputBuffer, uint32_t outputBufferSize, uint32_t playHead, float volume) {
    // For now, everything is S16 Stereo formated.
    int16_t * output_buffer = (int16_t*)outputBuffer;
    int16_t * src_buffer = (int16_t*)m_AudioData;

    if (playHead >> 1 >= m_DataLength >> 1)
        return 0;

    const uint32_t limit = MIN(outputBufferSize >> 1, m_DataLength >> 1 - playHead >> 1);

    uint32_t tt = playHead >> 1;

    for (int i = 0; i < limit; i += 2) {
        int16_t left = src_buffer[tt + i];
        int16_t right = src_buffer[tt + i + 1];

        output_buffer[i] += volume * left;
        output_buffer[i + 1] += volume * right;
    }

    return playHead + limit << 1;
}


SoundPool::SoundPool() {
    mutex = Core::CreateMutex();
    m_Volume = 1.0;
}

SoundPool::~SoundPool() {
    SafeDelete(mutex);
}

void SoundPool::Play(Sound * snd, float volume) {
    mutex->Lock();
    queuedSounds.push_back(SoundContext(snd, volume));
    mutex->Unlock();
}

void SoundPool::Write(uint8_t * outputBuffer, uint32_t outputBufferSize) {
    mutex->Lock();
    std::list<SoundContext>::iterator i = queuedSounds.begin();
    while (i != queuedSounds.end()) {
        uint32_t tmpHead = i->head;
        tmpHead = i->sound->Write(outputBuffer, outputBufferSize, tmpHead, i->volume * m_Volume);
        if (tmpHead == 0) {
            i = queuedSounds.erase(i);
            continue;
        }

        i->head = tmpHead;
        i++;
    }

    mutex->Unlock();
}

_AUDIO_END