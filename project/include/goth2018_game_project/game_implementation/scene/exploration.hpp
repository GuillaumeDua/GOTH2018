#pragma once

#include <goth2018_game_project/engine/scene.hpp>
#include <goth2018_game_project/configuration.hpp>

#include <iostream>

namespace goth2018::game_implementation::scenes
{
	namespace exploration
	{
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

		static auto generate()
			-> goth2018::engine::scene
		{
			return
			{
				"planet",
				std::string{goth2018::configuration::path::background} +"planet_cartoon_landscape.png",
				[]() { control::draw_exploration_menu(); }
			};
		}
	}
}