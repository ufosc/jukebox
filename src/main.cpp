#include <iostream>
#include <jukebox.hpp>
#include <source.hpp>
#include <sources/source.flac.hpp>
#include <sources/source.ogg.hpp>

int main() {
	auto juke = std::make_shared<jukebox>();
	auto file1 = std::make_shared<ogg>("sounds/m420.ogg");
	// auto file2 = std::make_shared<flac>("girl");
	
	std::cout << file1->id() << std::endl;
	// std::cout << file2->id() << std::endl;

	std::cout << juke->devName() << std::endl;
	juke->addSource("file1", file1);
	// juke->addSource("file2", file2);

	juke->play("file1");

	std::cout << "Hit enter to stop playback" << std::endl;
	
	std::string garbage;
	std::getline(std::cin, garbage);

	return 0;
}