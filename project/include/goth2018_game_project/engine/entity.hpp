#pragma once

#include <goth2018_game_project/graphics/graphics.hpp>
#include <gcl_cpp/pattern/ecs.hpp>

#include <functional>

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
			using function_type = std::function<void()>;
			function_type clicked;

			std::invoke_result_t<decltype(clicked)> operator()()
			{
				std::invoke(clicked);
			}
			std::invoke_result_t<const decltype(clicked)> operator()() const
			{
				std::invoke(clicked);
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
	};

	using manager_type = gcl::pattern::ecs::manager
	<
		components::position,
		components::size,
		components::rendering,
		components::on_click
	>;
	using type = manager_type::entity_type;
}
