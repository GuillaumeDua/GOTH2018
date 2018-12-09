#pragma once

#include <goth2018_game_project/graphics/graphics.hpp>
#include <gcl_cpp/pattern/ecs.hpp>

#include <functional>

namespace goth2018::engine::entity
{
	struct components
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
	};
	struct contracts
	{
		using drawable = gcl::pattern::ecs::contract
		<
			components::position,
			components::rendering
		>;
	};

	using manager_type = gcl::pattern::ecs::manager
	<
		components::position,
		components::size,
		components::rendering
	>;
	using type = manager_type::entity_type;
}
