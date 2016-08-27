#include <jukebox.hpp>
#include <stdexcept>

jukebox::jukebox(const ALCchar* n_dev, const ALCint* n_con) 
{
	// The following lines open an audio device for use and set it as the
	// current default device. Refer to the documentation of OpenAL for
	// more info
	m_dev = alcOpenDevice(n_dev);
	m_con = alcCreateContext(m_dev, n_con);
	alcMakeContextCurrent(m_con);
}

jukebox::~jukebox() 
{
	// Frees up the audio device and connection upon destruction
	alcDestroyContext(m_con);
	alcCloseDevice(m_dev);
}

const ALCdevice* jukebox::raw() 
{
	return m_dev;
}

const char* jukebox::devName() 
{
	return alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);
}

void jukebox::play(const std::string name) {
	// Check that source has been added
	if (m_sources.count(name) == 0) {
		std::cerr << "ERROR: Audio source not added: " << name << std::endl;
		throw("Audio: Audio source not added");
	}

	// Actually play the source
	m_sources[name]->play();
}