//                             _______   ______   .___________. __    __      ___     ___    __    ___                             
//                            /  _____| /  __  \  |           ||  |  |  |    |__ \   / _ \  /_ |  / _ \                            
//                           |  |  __  |  |  |  | `---|  |----`|  |__|  |       ) | | | | |  | | | (_) |                           
//                           |  | |_ | |  |  |  |     |  |     |   __   |      / /  | | | |  | |  > _ <                            
//                           |  |__| | |  `--'  |     |  |     |  |  |  |     / /_  | |_| |  | | | (_) |                           
//                            \______|  \______/      |__|     |__|  |__|    |____|  \___/   |_|  \___/                            
//                                                                                                                                 
//   _______  __    __       _______.     _______.               __  ___      ___      .__   __. .__   __.      ___       __  ___   ______   
//  /  _____||  |  |  |     /       |    /       |     ___      |  |/  /     /   \     |  \ |  | |  \ |  |     /   \     |  |/  /  /  __  \  
// |  |  __  |  |  |  |    |   (----`   |   (----`    ( _ )     |  '  /     /  ^  \    |   \|  | |   \|  |    /  ^  \    |  '  /  |  |  |  | 
// |  | |_ | |  |  |  |     \   \        \   \        / _ \/\   |    <     /  /_\  \   |  . `  | |  . `  |   /  /_\  \   |    <   |  |  |  | 
// |  |__| | |  `--'  | .----)   |   .----)   |      | (_>  <   |  .  \   /  _____  \  |  |\   | |  |\   |  /  _____  \  |  .  \  |  `--'  | 
//  \______|  \______/  |_______/    |_______/        \___/\/   |__|\__\ /__/     \__\ |__| \__| |__| \__| /__/     \__\ |__|\__\  \______/  
//                                                                                                                                                                                                                                               
// 

#include <goth2018_game_project/test/graphics.hpp>
#include <goth2018_game_project/test/entity.hpp>

#include <goth2018_game_project/configuration.hpp>
#include <goth2018_game_project/engine/core.hpp>

#include <goth2018_game_project/game_implementation/scene/space_map.hpp>
#include <goth2018_game_project/game_implementation/scene/exploration.hpp>
#include <goth2018_game_project/game_implementation/scene/stars_view.hpp>

#include <goth2018_game_project/game_implementation/entity.hpp>

#include <gcl_cpp/container/utility.hpp>

auto main() -> int
{
	try
	{
		std::cout << "[+] Loading ressources from : [" << goth2018::configuration::path::ressources << ']' << std::endl;

		goth2018::graphics::window window{ {800, 600}, "GOTH2018 : game window" };

		using ECS_manager_type = goth2018::game_implementation::entity::manager_type;

		/*using scene_type = goth2018::engine::ECS_scene<ECS_manager_type>;
		auto scenes = gcl::container::make_vector<scene_type>
		(
			goth2018::game_implementation::scenes::space_map::generate(),
			goth2018::game_implementation::scenes::exploration::generate(),
			goth2018::game_implementation::scenes::stars_view::generate()
		);*/

		//using core_type = goth2018::engine::core<ECS_manager_type>;
		goth2018::engine::core system
		{
			window,
			//goth2018::game_implementation::scenes::space_map::generate(),
			//goth2018::game_implementation::scenes::exploration::generate(),
			goth2018::game_implementation::scenes::stars_view::generate()
		};

		// todo : behaviors


		system.run();
	}
	catch (const std::exception & ex)
	{
		std::cerr << "exception : " << ex.what() << std::endl;
	}

	system("pause");
	return 0;
}