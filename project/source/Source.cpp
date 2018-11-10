#include <SFML/Graphics.hpp>

#include <string_view>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <cassert>
#include <functional>
#include <algorithm>

#include <goth2018_game_project/test/graphics.hpp>
#include <goth2018_game_project/configuration.hpp>

// todo : property -> exposure
//      + concept -> contract-accessor ?

#include <goth2018_game_project/test/interactive_test.hpp>

int main()
{
	try
	{
		std::cout << "[+] Loading ressources from : [" << goth2018::configuration::path::ressources << ']' << std::endl;

		goth2018::test::interactive::spritesheet();

		//gcl::test::components
		//<
		//	test::texture_iterator
		//	//test::entity
		//>::test();
		
	}
	catch (const std::exception & ex)
	{
		std::cerr << "exception : " << ex.what() << std::endl;
	}

	system("pause");
	return 0;
}