#pragma once

#include <goth2018_game_project/graphics/graphics.hpp>

#include <functional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <variant>
#include <cassert>

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
	struct ECS_scene
	{
		using menu_drawer_type = std::function<void()>;
		using event_handler_type = std::function<void(const sf::Event&, ECS_manager_type&)>;
		using event_handlers_container_type = std::unordered_multimap<sf::Event::EventType, event_handler_type>;
		using entity_manager_type = ECS_manager_type;
		using ECS_EM_operator_type = ECS_EM_operator<ECS_manager_type>;

		ECS_scene(const ECS_scene &) = delete; // for std::vector initializer_list
		ECS_scene(ECS_scene &&) = default;

		ECS_scene(std::string && scene_name, sf::Sprite && background_sprite, menu_drawer_type && scene_menu_drawer = []() {})
			: menu_drawer{ std::forward<menu_drawer_type>(scene_menu_drawer) }
			, background{ std::forward<sf::Sprite>(background_sprite) }
			, name{ std::forward<std::string>(scene_name) }
		{}

		void update()
		{
			assert(entity_operator.update);
			entity_operator.update(entity_manager);
		}

		void draw(sf::RenderWindow & window)
		{
			window.draw(background);
			menu_drawer();

			assert(entity_operator.draw);
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
