#pragma once

#include <functional>
#include <string>
#include <goth2018_game_project/graphics/graphics.hpp>

namespace goth2018::engine
{
	struct scene
	{
		using menu_drawer_type = std::function<void()>;

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
		void input()
		{

		}

		const std::string name;
		const sf::Sprite background;

		// events
	private:
		const menu_drawer_type menu_drawer;
		// std::vector<entities>
	};
}