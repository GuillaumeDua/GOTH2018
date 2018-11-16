#pragma once

#include <goth2018_game_project/engine/scene.hpp>
#include <goth2018_game_project/configuration.hpp>

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

			return space_map_scene;
		}
	}
}