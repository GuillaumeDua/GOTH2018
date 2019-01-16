#pragma once

#include <goth2018_game_project/graphics/graphics.hpp>
#include <goth2018_game_project/engine/entity.hpp>
#include <gcl_cpp/pattern/ecs.hpp>

#include <functional>
#include <string>
#include <unordered_map>

namespace goth2018::engine
{
	struct scene
	{
		using menu_drawer_type = std::function<void()>;
		using event_handler_type = std::function<void(const sf::Event&, scene&)>;
		using event_handlers_container_type = std::unordered_multimap<sf::Event::EventType, event_handler_type>;
		using entity_manager_type = goth2018::game_implementation::entity::manager_type;

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

		void draw(sf::RenderWindow & window)
		{
			window.draw(background);
			menu_drawer();

			using drawable_entity_contract = goth2018::game_implementation::entity::contracts::drawable;
			entity_manager.for_each_entities(drawable_entity_contract{}, [&window](auto & entity, auto & position, auto & rendering)
			{
				// todo : debug_warning : calc AABB window and position
				rendering.setPosition(position);
				window.draw(rendering);
			});
		}
		void update()
		{
			if (on_update)
				on_update();
			update_entities();
		}
		void dispatch_event(sf::Event & event)
		{
			auto match = event_handlers.equal_range(event.type);
			for (auto event_handler_it = match.first; event_handler_it != match.second; ++event_handler_it)
			{
				event_handler_it->second(event, *this);
			}
			entity_manager.reorder(); // event handlers could add / remove entities
		}

		const std::string name;
		const sf::Sprite background;
		event_handlers_container_type event_handlers;

		entity_manager_type entity_manager{ 1 }; // default capacity to 1 for early dev. emphasis storage reallocation. // todo : extend default capacity
		using entity_update_type = std::function<void
		(
			entity_manager_type::entity_type &,
			goth2018::game_implementation::entity::contracts::AI::parameters
		)>;
		entity_update_type on_entity_update;

		using on_update_type = std::function<void()>;
		on_update_type on_update;

	private:
		void update_entities()
		{	// todo : check interface
			if (on_entity_update == nullptr)
				return;

			using AI_entity_contract = goth2018::game_implementation::entity::contracts::AI;
			entity_manager.for_each_entities(AI_entity_contract{}, [this](auto & entity, auto & position)
			{
				on_entity_update(entity, std::forward_as_tuple(position));
			});
			entity_manager.reorder(); // on_entity_update could add / remove entities
		}

		const menu_drawer_type menu_drawer;
	};
}
