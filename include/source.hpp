/**
 * @file
 * @author UFOSC
 * @version 0.0.1
 * @section DESCRIPTION
 * 
 * A handler for different sources (parent class)
 */

#ifndef __JUKEBOX_SOURCE__
#define __JUKEBOX_SOURCE__

#include <AL/al.h>
#include <AL/alc.h>
#include <vector>
#include <string>
#include <fstream>
#include <thread>

class source 
{
protected:
	ALuint m_id;
	
	// File Info
	std::ifstream m_file;
	ALenum m_format;
	float m_freq;

	// Thread Info
	std::thread m_thr;
	bool m_run_thr;

	/**
	 * Opens a file for streaming
	 *
	 * @param offset An integer multiple of the block size offset to read from
	 * @param current_buf An unsigned integer of which buffer to write to
	 *
	 * @return long How many bytes were read
	 */
	virtual long read(ALuint buffer) = 0;

	/**
	 * Threaded streaming of file
	 *
	 * This will open a new thread which will watch the OpenAL buffer and load the file
	 * in chunks as needed
	 *
	 * @param ready A lambda to be called when the stream is ready to be played
	 */
	void stream(std::function<void (ALuint)> ready);

public:
	// 4 KB buffers
	static constexpr size_t BUFFER_SIZE = 4 * 1024;
	static constexpr size_t STREAM_BUFS = 3;

	/**
	 * Constructor for the source object
	 *
	 * @param path The string path to the file to load
	 */
	source(const std::string path);

	/**
	 * Destructor for the source object
	 */
	~source();

	/**
	 * Returns the OpenAL ID of the playable file
	 *
	 * @return ALuint OpenAL ID
	 */
	const ALuint id();

	/**
	 * Plays the source
	 */
	void play();

	/**
	 * Stops the source
	 */
	void stop();

	/**
	 * Check if track is playing
	 *
	 * @return Boolean True if song is playing
	 */
	const bool isPlaying();
};


#endif // __JUKEBOX_SOURCE__