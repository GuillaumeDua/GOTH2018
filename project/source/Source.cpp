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
#include <goth2018_game_project/configuration.hpp>
#include <goth2018_game_project/engine/core.hpp>

// todo contracts  :
//		  property -> exposure
//      + concept -> contract-accessor ?

// #include <goth2018_game_project/test/interactive_test.hpp>

namespace control
{
	static void draw_exploration_menu()
	{
		ImGui::SetNextWindowPos({ 0, 35 });
		ImGui::Begin("exploration", 0, ImGuiWindowFlags_MenuBar);
		if (ImGui::BeginMenuBar())
		{
			ImGui::Button("explore");
			ImGui::Button("laboratory");
			ImGui::Button("population");
			ImGui::EndMenuBar();
		}
		float f = 0.42f;
		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
		ImGui::End();
	}
}

int main()
{
	try
	{
		std::cout << "[+] Loading ressources from : [" << goth2018::configuration::path::ressources << ']' << std::endl;

		goth2018::graphics::window window{ {800, 600}, "GOTH2018 : game window" };
		goth2018::engine::core system
		{
			window,
			{
				{ "space", std::string{goth2018::configuration::path::background} +"background.png"},
				{ "planet", std::string{goth2018::configuration::path::background} +"planet_cartoon_landscape.png", []() {control::draw_exploration_menu(); }},
			}
		};
		system.run();


		//gcl::test::components
		//<
		//	goth2018::test::graphics::texture_iterator
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