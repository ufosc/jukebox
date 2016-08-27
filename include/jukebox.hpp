/**
 * @file
 * @author UFOSC
 * @version 0.0.1
 * @section DESCRIPTION
 * 
 * The jukebox class represents an OpenAL connected object
 */

#ifndef __JUKEBOX__
#define __JUKEBOX__

#include <AL/al.h>
#include <AL/alc.h>
#include <source.hpp>
#include <iostream>
#include <string>
#include <map>
#include <memory>
#include <type_traits>

class jukebox 
{
private:
	ALCdevice* m_dev;
	ALCcontext* m_con;
	std::map<std::string, std::shared_ptr<source>> m_sources;

public:
	/**
	 * Constructor that creates a jukebox object
	 *
	 * @param n_dev The name of a device to use
	 * @param n_con The name of the connection to use
	 */
	jukebox(const ALCchar* n_dev = nullptr, const ALCint* n_con = nullptr);

	/**
	 * Destructor for the jukebox
	 */
	~jukebox();

	/**
	 * Get access to the raw OpenAL device
	 *
	 * @return ALCdevice pointer
	 */
	const ALCdevice* raw();

	/**
	 * Get the name of the audio device being used
	 *
	 * @return String name of device
	 */
	const char* devName();

	/**
	 * Adds a source to be playable with OpenAL
	 *
	 * @param name A string name of the file
	 * @param src  A shared pointer to a source type for the file
	 */
	template <class T>
	void addSource(const std::string name, std::shared_ptr<T> src);

	/**
	 * Plays a source by name
	 *
	 * @param name A string name of the source
	 */
	void play(const std::string name);
};

// Template definitions
template <class T>
void jukebox::addSource(const std::string name, std::shared_ptr<T> src) 
{
	// Make sure that the passed source is a valid source
	static_assert(std::is_base_of<source, T> {}, "ERROR: Not a valid source!");

	// Check that source being added isn't already in map
	if (m_sources.count(name) != 0) {
		std::cerr << "ERROR: Audio source already added: " << name << std::endl;
		throw("Audio: Audio source already added");
	}

	// Add the name / source pair
	m_sources.emplace(name, std::static_pointer_cast<source>(src));
}


#endif // __JUKEBOX__