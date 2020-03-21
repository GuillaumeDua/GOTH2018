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

	struct scene
	{	// scene requires to be a polymorphic, heap-allocable type

		template <typename T, typename = std::enable_if_t<not std::is_same_v<T, scene>>>
		scene(T && arg)
			: value{ std::make_unique<model<T>>(std::forward<T>(arg)) }
		{}

		scene(scene &) = delete;
		scene(scene &&) = default;

		const std::string & get_name()
		{
			return value->get_name();
		}
		void update()
		{
			value->update();
		}
		void draw(sf::RenderWindow & window)
		{
			value->draw(window);
		}
		void dispatch_event(sf::Event & event)
		{
			value->dispatch_event(event);
		}

	private:
		struct concept_type
		{
			virtual ~concept_type() = default;
			virtual const std::string & get_name() = 0;
			virtual void update() = 0;
			virtual void draw(sf::RenderWindow & window) = 0;
			virtual void dispatch_event(sf::Event & event) = 0;
		};
		template <typename T>
		struct model final : concept_type
		{
			static_assert(not std::is_same_v<std::decay_t<T>, scene>);

			model(T && arg)
				: data(std::forward<T>(arg))
			{}

			const std::string & get_name() override
			{
				return data.get_name();
			}
			void update() override
			{
				data.update();
			}
			void draw(sf::RenderWindow & window) override
			{
				data.draw(window);
			}
			void dispatch_event(sf::Event & event) override
			{
				data.dispatch_event(event);
			}

			T data;
		};

		using value_type = std::unique_ptr<concept_type>;
		value_type value;
	};

	template <class ECS_manager_type>
	struct scene_impl
	{
		using menu_drawer_type = std::function<void()>;
		using event_handler_type = std::function<void(const sf::Event&, ECS_manager_type&)>;
		using event_handlers_container_type = std::unordered_multimap<sf::Event::EventType, event_handler_type>;
		using entity_manager_type = ECS_manager_type;
		using ECS_EM_operators_type = ECS_EM_operator<ECS_manager_type>;

		scene_impl(const scene_impl &) = delete; // for std::vector initializer_list
		scene_impl(scene_impl &&) = default;

		scene_impl(std::string && scene_name, sf::Sprite && background_sprite, menu_drawer_type && scene_menu_drawer = []() {})
			: menu_drawer{ std::forward<menu_drawer_type>(scene_menu_drawer) }
			, background{ std::forward<sf::Sprite>(background_sprite) }
			, name{ std::forward<std::string>(scene_name) }
		{}

		void update()
		{
			assert(entity_operators.update);
			entity_operators.update(entity_manager);
		}
		void draw(sf::RenderWindow & window)
		{
			window.draw(background);
			menu_drawer();

			assert(entity_operators.draw);
			entity_operators.draw(entity_manager, window);
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

		event_handlers_container_type event_handlers;
		entity_manager_type entity_manager{ 1 }; // default capacity to 1 for early dev. emphasis storage reallocation. // todo : extend default capacity (later dev stages)
		ECS_EM_operators_type entity_operators;

		const auto & get_name()
		{
			return name;
		}

		menu_drawer_type menu_drawer;
	private:
		const std::string name;
		const sf::Sprite background;
	};

	// scene_creator
	//
	// create().attach_menu().attach_ECS_operators() ...
	// return *this -> scene_impl &
}
