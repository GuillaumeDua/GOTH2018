#pragma once

#include <goth2018_game_project/engine/scene.hpp>
#include <goth2018_game_project/configuration.hpp>
#include <goth2018_game_project/engine/entity.hpp>
#include <goth2018_game_project/graphics/graphics.hpp>

#include <iostream>
#include <algorithm>
#include <random>

namespace goth2018::game_implementation::scenes
{
	namespace space_map
	{
		static auto generate()
		{
			goth2018::engine::scene space_map_scene{ "space", std::string{ goth2018::configuration::path::background } +"background.png" };
			std::decay_t<decltype(space_map_scene.event_handlers)> event_handlers
			{
				{
					sf::Event::EventType::MouseButtonPressed,
					[](const sf::Event & ev, decltype(space_map_scene) &)
					{
						std::cout
							<< ev.mouseButton.button << " => "
							<< ev.mouseButton.x << " x " << ev.mouseButton.y
							<< std::endl;
					}
				}
			};
			space_map_scene.event_handlers = std::move(event_handlers);

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

			for (auto counter = 0; counter < 5; ++counter)
			{
				std::mt19937 rng;
				rng.seed(std::random_device()());
				std::uniform_int_distribution<std::mt19937::result_type> distribution(0, 14);

				space_map_scene.entities.create_entity
				<
					goth2018::engine::entity::components::position,
					goth2018::engine::entity::components::size,
					goth2018::engine::entity::components::rendering
				>
				(
					goth2018::engine::entity::components::position{ 50.f + counter * 100.f, 50.f },
					goth2018::engine::entity::components::size{ 384.f, 384.f },
					planet_sprites.at(distribution(rng))
				);
				space_map_scene.entities.create_entity
				<
					goth2018::engine::entity::components::position,
					goth2018::engine::entity::components::size,
					goth2018::engine::entity::components::rendering
				>
				(
					goth2018::engine::entity::components::position{ 50.f + counter * 100.f, 150.f },
					goth2018::engine::entity::components::size{ 384.f, 384.f },
					planet_sprites.at(distribution(rng))
				);
				space_map_scene.entities.create_entity
				<
					goth2018::engine::entity::components::position,
					goth2018::engine::entity::components::size,
					goth2018::engine::entity::components::rendering
				>
				(
					goth2018::engine::entity::components::position{ 50.f + counter * 100.f, 250.f },
					goth2018::engine::entity::components::size{ 384.f, 384.f },
					planet_sprites.at(distribution(rng))
				);
			}

			return space_map_scene;
		}
	}
}