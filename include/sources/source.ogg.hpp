/**
 * @file
 * @author UFOSC
 * @version 0.0.1
 * @section DESCRIPTION
 * 
 * A handler for the OGG file format
 */

#ifndef __JUKEBOX_SOURCE_OGG__
#define __JUKEBOX_SOURCE_OGG__

#include <source.hpp>
#include <vorbis/vorbisfile.h>

class ogg : public source 
{
private:
	/**
	 * IFS Adapter
	 *
	 * Adapter methods for using c++ input file streams with ogg vorbis. Only
	 * need read and close since streaming will not seek within the file.
	 *
	 */
	static size_t ifs_read(void *ptr, size_t size, size_t nmemb, void *datasource);
	static int    ifs_close(void *datasource);

	// OGG file handler
	OggVorbis_File m_ogg_file;

protected:
	long read(ALuint buffer);

public:
	ogg(const std::string path);
	~ogg();
};

#endif // __JUKEBOX_SOURCE_OGG__