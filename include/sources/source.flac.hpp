/**
 * @file
 * @author UFOSC
 * @version 0.0.1
 * @section DESCRIPTION
 * 
 * A handler for the FLAC file format
 */

#ifndef __JUKEBOX_SOURCE_FLAC__
#define __JUKEBOX_SOURCE_FLAC__

#include <source.hpp>

class flac : public source 
{
protected:
	void open(const int offset);
	
public:
	using source::source;
	~flac() = default;
};

#endif // __JUKEBOX_SOURCE_FLAC