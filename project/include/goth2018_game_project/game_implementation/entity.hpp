#pragma once

#include <goth2018_game_project/graphics/graphics.hpp>
#include <gcl_cpp/pattern/ecs.hpp>

#include <functional>

namespace goth2018::game_implementation::entity
{
	struct manager_type;	// forward-declaration so components know ECS entity manager type

	namespace components
	{
		// todo : velocity component
		// todo : movable  contract

		struct position
		{
			operator sf::Vector2f() const
			{
				return sf::Vector2f{ x, y };
			}
			float x, y;
		};
		struct size
		{
			operator sf::Vector2f() const
			{
				return sf::Vector2f{ width, height };
			}
			float width, height;
		};
		using rendering = sf::Sprite;

		struct on_click
		{
			using function_type = std::function<void(gcl::pattern::ecs::id_type, manager_type&)>;
			function_type clicked;

			void operator()(gcl::pattern::ecs::id_type this_entity_id, manager_type & em) const
			{
				std::invoke(clicked, this_entity_id, em);
			}
		};
	};

	struct manager_type : gcl::pattern::ecs::manager
	<	// forward-declaration prevent type-alias (no using directive here)
		components::position,
		components::size,
		components::rendering,
		components::on_click
	>
	{};

	using type = manager_type::entity_type;

	namespace contracts
	{
		using drawable = gcl::pattern::ecs::contract
		<
			components::position,
			components::rendering
		>;
		using clickable = gcl::pattern::ecs::contract
		<
			components::position,
			components::size,
			components::on_click
		>;
		using AI = gcl::pattern::ecs::contract
		<
			components::position
		>;
	}

	namespace operations
	{
		struct draw
		{
			void operator()(manager_type & entity_manager, sf::RenderWindow & window) const
			{
				using drawable_entity_contract = goth2018::game_implementation::entity::contracts::drawable;
				entity_manager.for_each_entities(drawable_entity_contract{}, [&window](auto & entity, auto & position, auto & rendering)
				{
					// todo : debug_warning : calc AABB window and position, warning when OOB
					rendering.setPosition(position);
					window.draw(rendering);
				});
			}
		};
		struct update
		{
			void operator()(manager_type & entity_manager) const
			{	// todo : check interface
				using AI_entity_contract = goth2018::game_implementation::entity::contracts::AI;
				entity_manager.for_each_entities(AI_entity_contract{}, [](auto & entity, auto & position)
				{
					// test AI
					position.x += 1.f;
					position.y += 1.f;

					// todo : different AIs with a std::variant<AIs...>
				});
				entity_manager.reorder(); // on_entity_update could add / remove entities
			}
		};
	}
}
