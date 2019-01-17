#pragma once

#include <goth2018_game_project/graphics/graphics.hpp>

#include <functional>
#include <string>
#include <unordered_map>

namespace goth2018::engine
{
	template <class ECS_manager_type>
	struct ECS_EM_operator
	{	// entity manager operator
		// define ways to interact with an ECS EM

		using draw_operation_type = std::function<void(ECS_manager_type & entity_manager, sf::RenderWindow & window)>;
		draw_operation_type draw;

		using update_operation_type = std::function<void(ECS_manager_type & entity_manager)>;
		update_operation_type update;
	};

	template <class ECS_manager_type>
	struct scene
	{
		using menu_drawer_type = std::function<void()>;
		using event_handler_type = std::function<void(const sf::Event&, ECS_manager_type&)>;
		using event_handlers_container_type = std::unordered_multimap<sf::Event::EventType, event_handler_type>;
		using entity_manager_type = ECS_manager_type;
		using ECS_EM_operator_type = ECS_EM_operator<ECS_manager_type>;

		scene(const scene &) = delete; // for std::vector initializer_list
		scene(scene &&) = default;

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

		void update()
		{
			if (entity_operator.update)
				entity_operator.update(entity_manager);
		}

		void draw(sf::RenderWindow & window)
		{
			window.draw(background);
			menu_drawer();

			if (entity_operator.draw)
				entity_operator.draw(entity_manager, window);
		}

		void dispatch_event(sf::Event & event)
		{
			auto match = event_handlers.equal_range(event.type);
			for (auto event_handler_it = match.first; event_handler_it != match.second; ++event_handler_it)
			{
				event_handler_it->second(event, entity_manager);
			}
			entity_manager.reorder(); // event handlers could add / remove entities
		}

		const std::string name;
		const sf::Sprite background;
		event_handlers_container_type event_handlers;
		entity_manager_type entity_manager{ 1 }; // default capacity to 1 for early dev. emphasis storage reallocation. // todo : extend default capacity (later dev stages)
		ECS_EM_operator_type entity_operator;

	private:
		const menu_drawer_type menu_drawer;
	};
}
