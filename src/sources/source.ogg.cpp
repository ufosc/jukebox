#include <sources/source.ogg.hpp>
#include <vorbis/vorbisfile.h>
#include <iostream>
#include <stdexcept>

// Adapter definition for read
size_t ogg::ifs_read(void *ptr, size_t size, size_t nmemb, void *datasource) {
	// Cast data source to IFS stream
	auto& ifs = *reinterpret_cast<std::ifstream*>(datasource);
	auto start = ifs.tellg();

	auto& ifs_end = ifs.read(reinterpret_cast<char*>(ptr), size * nmemb);
	auto end = ifs_end.tellg();

	return end - start;
}

// Adapter definition for close
int ogg::ifs_close(void *datasource) {
	auto& ifs = *reinterpret_cast<std::ifstream*>(datasource);
	ifs.close();
}

// TODO: Clean this up for eventual streaming
void ogg::open(const int offset) {
	// Initialize needed stuff
	ALenum format;
	int endian = 0; // 0 for Little-Endian, 1 for Big-Endian
	int bitStream;
	long bytes;
	char array[BUFFER_SIZE];
	float freq;

	vorbis_info *oInfo;
	OggVorbis_File oggFile;

	try {
		// Test that the file is actually an ogg file (this breaks everything :( )
		// if (ov_test_callbacks(oggRawFile, &oggFile, NULL, 0, OV_CALLBACKS_DEFAULT)) {
		// 	std::cerr << "ERROR: Specified sound file is not OGG format: " << oggPath << std::endl; 
		// 	throw("Audio: Specified sound file wrong format");
		// }

		// Try opening the file
		ov_callbacks ov_adapt {};
		ov_adapt.read_func  = ifs_read;
		ov_adapt.close_func = ifs_close;
		if (ov_open_callbacks(&file, &oggFile, nullptr, 0, ov_adapt) != 0)
		{
			std::cerr << "ERROR: Error opening file for decoding." << std::endl;
			throw("Audio: Could not open sound file");
		}

		// Get file info
		oInfo = ov_info(&oggFile, -1);
		if (oInfo->channels == 1) // Check the number of channels... always use 16-bit samples
			format = AL_FORMAT_MONO16;
		else
			format = AL_FORMAT_STEREO16;
		freq = oInfo->rate;

		// Read in the data
		m_data.clear();
		do {
			bytes = ov_read(&oggFile, array, BUFFER_SIZE, endian, 2, 1, &bitStream);
			if (bytes < 0)
            {
	            ov_clear(&oggFile);
	            std::cerr << "ERROR: Error decoding" << std::endl;
	            throw("Audio: Error decoding ogg file");
            }

			// Append to end of buffer
			m_data.insert(m_data.end(), array, array + bytes);
		} while (bytes > 0);

		// Buffer the data & attach
		alBufferData(m_bufs[m_current_buf], format, &m_data[0], static_cast<ALsizei>(m_data.size()), freq);
		alSourcei(m_id, AL_BUFFER, m_bufs[m_current_buf]);

		// Clear the file
		ov_clear(&oggFile);

	} catch (const std::exception& e) {
		std::cerr << "ERROR: Could not load sound file | " << e.what() << std::endl;
		throw("AAAAAA");
	}
}