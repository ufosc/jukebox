#include <source.hpp>
#include <iostream>

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

	// Set ifstream to throw an exception on I/O failures
	m_file.exceptions(std::ifstream::badbit);

	// Try to open the file & get the file size
	try {
		m_file.open(path, std::ifstream::binary | std::ifstream::ate);
		m_file_size = m_file.tellg();
		m_file.close();

		// Have file open for use
		m_file.open(path, std::ifstream::in);
	} catch (...) {
		std::cerr << "ERROR: Could not open file '" << path << "'"<< std::endl;
		throw("Source: Could not open file");
	}
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
	m_file.close();
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