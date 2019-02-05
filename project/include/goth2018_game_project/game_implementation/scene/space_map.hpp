#pragma once

#include <goth2018_game_project/game_implementation/entity.hpp>
#include <goth2018_game_project/game_implementation/scene_event_handlers.hpp>

#include <goth2018_game_project/engine/scene.hpp>
#include <goth2018_game_project/graphics/graphics.hpp>

#include <iostream>
#include <algorithm>
#include <random>

namespace goth2018::game_implementation::scenes
{
	struct space_map
	{
		static auto generate()
		{
			using ECS_manager_type = typename goth2018::game_implementation::entity::manager_type;
			using scene_type = goth2018::engine::scene<ECS_manager_type>;

			auto scene_background_path = std::string{ goth2018::configuration::path::background } +"background.png";
			auto scene_background = graphics::sprite::create(std::move(scene_background_path));
			scene_type scene{ "space", std::move(scene_background) };
			{	// events

				using event_handler_type = goth2018::game_implementation::event::handler<goth2018::game_implementation::entity::manager_type>;

				std::decay_t<decltype(scene.event_handlers)> event_handlers
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
					},
					{
						sf::Event::EventType::MouseButtonPressed,
						event_handler_type::broadcast_clicked_entity{}
					}
				};
				scene.event_handlers.merge(event_handlers);
			}
			scene.entity_operator = decltype(scene.entity_operator)
			{
				game_implementation::entity::operations::draw{},
				game_implementation::entity::operations::update{}
			};

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
				sprite.setOrigin(50.f, 50.f);
				sprite.setRotation(90.f);
			}

			std::mt19937 rng;
			rng.seed(std::random_device()());
			std::uniform_int_distribution<std::mt19937::result_type> distribution(0, 14);

			for (auto column_counter = 0; column_counter < 5; ++column_counter)
			{	// 5 columns
				for (auto row_counter = 0; row_counter < 3; ++row_counter)
				{	// 3 rows
					auto[entity, components] = scene.entity_manager.create_entity
					<
						goth2018::game_implementation::entity::components::position,
						goth2018::game_implementation::entity::components::size,
						goth2018::game_implementation::entity::components::rendering
					>
					(
						goth2018::game_implementation::entity::components::position{ 50.f + column_counter * 150.f, 50.f + (100.f * row_counter) },
						goth2018::game_implementation::entity::components::size{ 384.f / 5, 384.f / 5 }, // scale is 0.2f
						planet_sprites.at(distribution(rng))
					);

					scene.entity_manager.entity_add_component<goth2018::game_implementation::entity::components::on_click>(entity, [](auto this_entity_id, auto & ecs_manager)
					{
						auto & entity = ecs_manager.entity_at(this_entity_id);

						std::cout << "clicked element : {" << entity.id << ", " << entity.persistent_id << "}\n";

						auto & rendering_component = ecs_manager.entity_get_component<goth2018::game_implementation::entity::components::rendering>(this_entity_id);
						rendering_component.setColor(sf::Color::Red);
					});
				}
			}

			return scene;
		}
	};
}