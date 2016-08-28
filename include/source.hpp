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

class source 
{
protected:
	ALuint m_id;
	std::vector<ALuint> m_bufs;
	std::vector<char> m_data;
	unsigned int m_current_buf;
	
	std::ifstream m_file;
	size_t m_file_size;

	/**
	 * Opens a file for streaming
	 *
	 * @param offset An integer multiple of the block size offset to read from
	 */
	virtual void open(const int offset) = 0;

	/**
	 * Threaded streaming of file
	 *
	 * This will open a new thread which will watch the OpenAL buffer and load the file
	 * in chunks as needed
	 */
	void stream();

public:
	// 48 KB buffers
	static constexpr size_t BUFFER_SIZE = 48000;
	static constexpr size_t STREAM_BUFFERS = 3;

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
	 * Check if track is playing
	 *
	 * @return Boolean True if song is playing
	 */
	const bool isPlaying();
};


#endif // __JUKEBOX_SOURCE__