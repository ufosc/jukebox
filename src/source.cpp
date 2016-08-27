#include <source.hpp>

source::source(const std::string path) 
{
	// Setup a few buffers for streaming
	alGenSources(1, &m_id);
	for (int i = 0; i < STREAM_BUFFERS; ++i) 
	{
		ALuint t_buf;
		alGenBuffers(1, &t_buf);
		m_bufs.push_back(t_buf);
	}

	m_current_buf = 0;

	// Open the file
	file.open(path, std::ifstream::in);
}

source::~source() 
{
	// Clear out the buffers
	for (auto& buf : m_bufs) 
	{
		alDeleteBuffers(1, &buf);
	}

	// Delete the source
	alDeleteSources(1, &m_id);

	// Close the file
	file.close();
}

void source::stream()
{
	// TODO
}

const ALuint source::id() {
	return m_id;
}

void source::play() {
	// Only play the source if it isn't already playing
	if (!this->isPlaying()) {
		this->open(0);

		alSourcePlay(m_id);
	}
}

const bool source::isPlaying() {
	ALint state;
	alGetSourcei(m_id, AL_SOURCE_STATE, &state);

	return (state == AL_PLAYING);
}