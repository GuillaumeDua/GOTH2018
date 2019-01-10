#pragma once

#include <goth2018_game_project/engine/scene.hpp>
#include <goth2018_game_project/configuration.hpp>

#include <iostream>

namespace goth2018::game_implementation::scenes
{
	struct exploration
	{
		struct control
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
		};

		static auto generate()
			-> goth2018::engine::scene
		{
			
			auto scene = goth2018::engine::scene
			{
				"planet",
				std::string{goth2018::configuration::path::background} +"planet_cartoon_landscape.png",
				[]() { control::draw_exploration_menu(); }
			};

			// -- test --

			auto planet_sprites = graphics::spritesheet
			{
				graphics::spritesheet::construction_policy::using_sprite_quantity{},
				configuration::path::planet_sprites.data(),
				graphics::spritesheet::range_type{4,4}
			}.generate_sprites({ 0, 15 }); // only 15 sprites right now
			assert(planet_sprites.size() == 15);
			for (auto & sprite : planet_sprites)
			{
				sprite.setScale(0.2f, 0.2f);
			}

			std::mt19937 rng;
			rng.seed(std::random_device()());
			std::uniform_int_distribution<std::mt19937::result_type> sprite_distribution(0, 14);
			std::uniform_int_distribution<std::mt19937::result_type> x_distribution(0, 700);
			std::uniform_int_distribution<std::mt19937::result_type> y_distribution(0, 500);

			scene.entities.create_entity
			<
				goth2018::engine::entity::components::position,
				goth2018::engine::entity::components::size,
				goth2018::engine::entity::components::rendering
			>
			(
				goth2018::engine::entity::components::position{ (float)x_distribution(rng), (float)y_distribution(rng) },
				goth2018::engine::entity::components::size{ 384.f / 5, 384.f / 5 }, // scale is 0.2f
				planet_sprites.at(sprite_distribution(rng))
			);

			// -- test --

			return scene;
		}
	};
}