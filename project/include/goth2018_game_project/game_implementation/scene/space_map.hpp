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
			{	// events
				std::decay_t<decltype(space_map_scene.event_handlers)> event_handlers
				{
					{
						sf::Event::EventType::MouseButtonPressed,
						[](const auto & ev, auto&)
						{
							std::cout
								<< ev.mouseButton.button << " => "
								<< ev.mouseButton.x << " x " << ev.mouseButton.y
								<< std::endl;
						}
					}
				};
				space_map_scene.event_handlers.merge(event_handlers);
			}

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
			std::uniform_int_distribution<std::mt19937::result_type> distribution(0, 14);

			for (auto column_counter = 0; column_counter < 5; ++column_counter)
			{	// 5 columns
				for (auto row_counter = 0; row_counter < 3; ++row_counter)
				{	// 3 rows
					auto [entity, components] = space_map_scene.entities.create_entity
					<
						goth2018::engine::entity::components::position,
						goth2018::engine::entity::components::size,
						goth2018::engine::entity::components::rendering
					>
					(
						goth2018::engine::entity::components::position{ 50.f + column_counter * 150.f, 50.f + (100.f * row_counter) },
						goth2018::engine::entity::components::size{ 384.f / 5, 384.f / 5 }, // scale is 0.2f
						planet_sprites.at(distribution(rng))
					);

					space_map_scene.entities.entity_add_component<goth2018::engine::entity::components::on_click>(entity, []()
					{
						std::cout << "clicked\n";
					});
				}
			}

			using position_contract = gcl::pattern::ecs::contract<goth2018::engine::entity::components::position>;
			space_map_scene.entities.for_each_entities(position_contract{}, [](const auto & entity, const auto & pos)
			{
				std::cout << entity << " -> " << static_cast<sf::Vector2f>(pos).x << ", " << static_cast<sf::Vector2f>(pos).y << std::endl;
			});

			return space_map_scene;
		}
	}
}