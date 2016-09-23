#include <iostream>
#include <jukebox.hpp>
#include <source.hpp>
#include <sources/source.flac.hpp>
#include <sources/source.ogg.hpp>

int main(int argc, char* argv[]) {
	// Check that args are correct
	if (argc != 2)
	{
		std::cout << "usage: " << argv[0] << " /path/to/audio/file" << std::endl;
		return -1;
	}

	auto juke = std::make_shared<jukebox>();
	auto file1 = std::make_shared<ogg>(argv[1]);
	
	std::cout << file1->id() << std::endl;

	std::cout << juke->devName() << std::endl;
	juke->addSource("file1", file1);

	juke->play("file1");

	std::cout << "Hit enter to stop playback" << std::endl;
	
	std::string garbage;
	std::getline(std::cin, garbage);

	return 0;
}