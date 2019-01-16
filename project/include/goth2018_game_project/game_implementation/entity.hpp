#pragma once

#include <goth2018_game_project/graphics/graphics.hpp>
#include <gcl_cpp/pattern/ecs.hpp>

#include <functional>

namespace goth2018::engine
{
	struct scene;
}

namespace goth2018::engine::entity
{
	namespace components
	{
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
			using function_type = std::function<void(gcl::pattern::ecs::id_type, goth2018::engine::scene &)>;	// todo : not scene but entity_manager_type
			function_type clicked;

			void operator()(gcl::pattern::ecs::id_type this_entity_id, goth2018::engine::scene & scene)
			{	// /*std::invoke_result_t<decltype(clicked)>*/
				std::invoke(clicked, this_entity_id, scene);
			}
			void operator()(gcl::pattern::ecs::id_type this_entity_id, goth2018::engine::scene & scene) const
			{
				std::invoke(clicked, this_entity_id, scene);
			}
		};
	};
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
	};

	using manager_type = gcl::pattern::ecs::manager
	<
		components::position,
		components::size,
		components::rendering,
		components::on_click
	>;
	using type = manager_type::entity_type;

	namespace contracts
	{
		using everything = manager_type::contract_type;
	}
}
