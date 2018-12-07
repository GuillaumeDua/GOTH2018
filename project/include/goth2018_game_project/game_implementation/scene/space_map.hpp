#pragma once

#include <goth2018_game_project/engine/scene.hpp>
#include <goth2018_game_project/configuration.hpp>
#include <goth2018_game_project/engine/entity.hpp>

#include <iostream>

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

			for (auto counter = 0; counter < 5; ++counter)
			{
				static const auto generate_planet = [](sf::Vector2f && position)
				{
					sf::CircleShape circle;
					circle.setRadius(10);
					circle.setOutlineColor(sf::Color::Red);
					circle.setOutlineThickness(5);
					circle.setPosition(position);
					return circle;
				};
				auto add_entity = [&](auto && rendering_type)
				{
					space_map_scene.entities.create_entity
					<
						goth2018::engine::entity::components::position,
						goth2018::engine::entity::components::size,
						goth2018::engine::entity::components::rendering
					>
					(
							goth2018::engine::entity::components::position{  },
							goth2018::engine::entity::components::size{ 0,0 },
							std::forward<std::decay_t<decltype(rendering_type)>>(rendering_type)
					);
				};

				add_entity(generate_planet({ 50 + counter * 100.f, 50.f }));
				add_entity(generate_planet({ 50 + counter * 100.f, 100.f }));
				add_entity(generate_planet({ 50 + counter * 100.f, 150.f }));
			}

			return space_map_scene;
		}
	}
}