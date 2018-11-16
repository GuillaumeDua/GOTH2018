#pragma once

#include <goth2018_game_project/graphics/graphics.hpp>

#include <functional>
#include <string>
#include <unordered_map>

namespace goth2018::engine
{
	struct scene
	{
		using menu_drawer_type = std::function<void()>;
		using event_handler_type = std::function<void(const sf::Event&, scene &)>;
		using event_handlers_container_type = std::unordered_map<sf::Event::EventType, event_handler_type>;

		scene(std::string && scene_name, sf::Sprite & background_sprite, menu_drawer_type && scene_menu_drawer = []() {})
			: menu_drawer{ scene_menu_drawer }
			, background{ background_sprite }
			, name{ std::forward<std::string>(scene_name) }
		{}
		scene(std::string && scene_name, const std::string & background_path, menu_drawer_type && scene_menu_drawer = []() {})
			: menu_drawer{ scene_menu_drawer }
			, background{ goth2018::graphics::textures::to_sprite(goth2018::graphics::textures::get(background_path)) }
			, name{ std::forward<std::string>(scene_name) }
		{}

		void draw(sf::RenderWindow & window)
		{
			window.draw(background);
			menu_drawer();
		}
		void update()
		{

		}
		void dispatch_event(sf::Event & event)
		{
			auto match = event_handlers.find(event.type);
			if (match == std::cend(event_handlers))
				return;
			match->second(event, *this);
		}

		const std::string name;
		const sf::Sprite background;
		event_handlers_container_type event_handlers;

	private:
		const menu_drawer_type menu_drawer;
		// std::vector<entities>
	};
}