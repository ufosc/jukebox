#include <sources/source.ogg.hpp>
#include <iostream>
#include <stdexcept>

// Constructor
ogg::ogg(const std::string path): source(path) {
	try {
		// Try opening the ogg file
		ov_callbacks ov_adapt {};
		ov_adapt.read_func  = ifs_read;
		ov_adapt.close_func = ifs_close;

		int res = ov_open_callbacks(&m_file, &m_ogg_file, nullptr, 0, ov_adapt);
		if (res != 0)
		{
			std::string err = "ERROR: ";
			switch (res) {
				case OV_EREAD:
					err += "Read error";
					break;
				case OV_ENOTVORBIS:
					err += "Specified file is not a vorbis file";
					break;
				case OV_EVERSION:
					err += "Vorbis version mismatch";
					break;
				case OV_EBADHEADER:
					err += "Bad vorbis header";
					break;
				case OV_EFAULT:
					err += "Internal logic fault";
					break;
				default:
					err += "Unknown error";
					break;
			}

			std::cerr << err << std::endl;
			throw("Audio: Could not open sound file");
		}

		// Get file info
		vorbis_info* oInfo = ov_info(&m_ogg_file, -1);
		if (oInfo->channels == 1) // Check the number of channels... always use 16-bit samples
			m_format = AL_FORMAT_MONO16;
		else
			m_format = AL_FORMAT_STEREO16;
		m_freq = oInfo->rate;
	} catch (...) {
		std::cerr << "ERROR: Unable to open source file: " << path << std::endl;
		throw("Audio: Could not open sound file");
	}
}

// Deconstructor
ogg::~ogg() {
	// Stop self
	stop();

	// Clear the file
	ov_clear(&m_ogg_file);
}

// Adapter definition for read
size_t ogg::ifs_read(void *ptr, size_t size, size_t nmemb, void *datasource) {
	// Cast data source to IFS stream
	auto& ifs = *reinterpret_cast<std::ifstream*>(datasource);
	ifs.read(reinterpret_cast<char*>(ptr), size * nmemb);

	return ifs.gcount();
}

// Adapter definition for close
int ogg::ifs_close(void *datasource) {
	auto& ifs = *reinterpret_cast<std::ifstream*>(datasource);
	ifs.close();

	return 0;
}

long ogg::read(ALuint buffer) {
	// Initialize needed stuff
	int endian = 0; // 0 for Little-Endian, 1 for Big-Endian
	int bitStream;
	long bytes;
	char array[BUFFER_SIZE];

	try {
		// Read in the data
		bytes = ov_read(&m_ogg_file, array, BUFFER_SIZE, endian, 2, 1, &bitStream);
		if (bytes < 0)
		{
			ov_clear(&m_ogg_file);
			std::cerr << "ERROR: Error decoding" << std::endl;
			throw("Audio: Error decoding ogg file");
		}

		// Buffer the data & attach
		alBufferData(buffer, m_format, array, bytes, m_freq);

		// Return how many bytes were read
		return bytes;

	} catch (const std::exception& e) {
		std::cerr << "ERROR: Could not load sound file | " << e.what() << std::endl;
		throw("OGG: Could not load sound file");
	}
}