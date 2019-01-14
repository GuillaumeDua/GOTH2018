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
	struct star_factory
	{
		std::vector<sf::Sprite> sprites = graphics::spritesheet
		{
			graphics::spritesheet::construction_policy::using_sprite_quantity{},
			configuration::path::planet_sprites.data(),
			graphics::spritesheet::range_type{4,4}
		}.generate_sprites({ 0, 15 }); // only 15 sprites right now

		star_factory()
		{
			for (auto & sprite : sprites)
			{
				sprite.setScale(0.05f, 0.05f);
				sprite.setOrigin(50.f, 50.f);
				sprite.setRotation(90.f);
			}

			rng.seed(std::random_device()());
		}

		void generate_one_entity(goth2018::engine::scene::entity_manager_type & entity_manager)
		{
			auto[entity, components] = entity_manager.create_entity
			<
				goth2018::engine::entity::components::position,
				goth2018::engine::entity::components::size,
				goth2018::engine::entity::components::rendering
			>
			(
				goth2018::engine::entity::components::position{ (float)x_distribution(rng), (float)y_distribution(rng) },
				goth2018::engine::entity::components::size{ 20.f, 20.f }, // scale is 0.2f
				sprites.at(sprite_distribution(rng))
			);
		}

	private:
		std::mt19937 rng;
		std::uniform_int_distribution<std::mt19937::result_type> x_distribution{ 0, 400 }; // 0 -> 800
		std::uniform_int_distribution<std::mt19937::result_type> y_distribution{ 0, 300 }; // 0 -> 600
		std::uniform_int_distribution<std::mt19937::result_type> sprite_distribution{ 0, 14 };
	};

	struct stars_view
	{
		static auto generate()
		{
			goth2018::engine::scene stars_view_scene{ "stars_view", std::string{ goth2018::configuration::path::background } +"background.png" };
			{	// events
				std::decay_t<decltype(stars_view_scene.event_handlers)> event_handlers
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
				stars_view_scene.event_handlers.merge(event_handlers);
			}

			stars_view_scene.on_update = []()
			{
				
			};

			return stars_view_scene;
		}

		static auto entities_generator(goth2018::engine::scene::entity_manager_type & entity_manager)
		{
			star_factory factory;

			auto start = std::chrono::system_clock::now();

			for (std::size_t i = 0; i < 10000; ++i)
				factory.generate_one_entity(entity_manager);

			auto end = std::chrono::system_clock::now();
			std::chrono::duration<double> elapsed_seconds = end - start;
			std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
		}
	};
}