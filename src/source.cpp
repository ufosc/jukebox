#include <source.hpp>
#include <iostream>

source::source(const std::string path)
{
	// Setup the source
	alGenSources(1, &m_id);

	// Setup the thread
	m_run_thr = false;

	// Set ifstream to throw an exception on I/O failures
	m_file.exceptions(std::ifstream::badbit);

	// Try to open the file
	try 
	{
		// Have file open for use
		m_file.open(path, std::ifstream::in);
	} catch (...) {
		std::cerr << "ERROR: Could not open file '" << path << "'"<< std::endl;
		throw("Source: Could not open file");
	}
}

source::~source()
{
	// Stop and Delete the source
	if (this->isPlaying()) stop();
	alDeleteSources(1, &m_id);

	// Close the file
	m_file.close();
}

void source::stream(std::function<void (ALuint)> ready)
{
	// Clear out any errors before this call
	ALenum err = alGetError();
	if (err != 0)
	{
		std::cerr << "ERROR: OpenAL reports an error with code: " << err << std::endl;
		throw("OpenAL: Error");
	}

	// Run until killed or end of file is reached
	long bytes;
	int processed;

	// Setup buffers to stream to
	ALuint buffers[STREAM_BUFS];
	ALuint temp[STREAM_BUFS];
	alGenBuffers(STREAM_BUFS, buffers);

	// Queue the first few buffers
	for (int i = 0; i < STREAM_BUFS; ++i)
	{
		bytes = read(buffers[i]);
		alSourceQueueBuffers(m_id, 1, &buffers[i]);
	}

	// Start Playing and streaming
	ready(m_id);

	do {
		alGetSourcei(m_id, AL_BUFFERS_PROCESSED, &processed);
		if (!processed) continue;
		
		alSourceUnqueueBuffers(m_id, processed, temp);

		// Read & enqueue
		for (int i = 0; i < processed; ++i)
		{
			bytes = read(temp[i]);
			alSourceQueueBuffers(m_id, 1, &temp[i]);
		}
	} while (bytes != 0 && m_run_thr);

	// Clean up buffers
	alDeleteBuffers(STREAM_BUFS, buffers);
}

const ALuint source::id()
{
	return m_id;
}

void source::play()
{
	// Only play the source if it isn't already playing
	if (!this->isPlaying())
	{
		m_run_thr = true;
		m_thr = std::thread([=]() 
		{
			this->stream([](ALuint id) 
			{
				alSourcePlay(id);
			});
		});
	}
}

void source::stop()
{
	// Only stop if playing
	if (this->isPlaying() || m_run_thr)
	{
		m_run_thr = false;
		if (m_thr.joinable()) 
			m_thr.join();

		alSourceStop(m_id);
	}
}

const bool source::isPlaying()
{
	ALint state;
	alGetSourcei(m_id, AL_SOURCE_STATE, &state);

	return (state == AL_PLAYING);
}