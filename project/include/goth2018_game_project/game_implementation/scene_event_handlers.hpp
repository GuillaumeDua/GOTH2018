#pragma once

#include <goth2018_game_project/engine/collision.hpp>
#include <goth2018_game_project/engine/scene.hpp>

namespace goth2018::game_implementation::event::handler
{
	using handler_type = goth2018::engine::scene::event_handler_type;

	static auto broadcast_clicked_entity()
	{
		return [](const sf::Event & ev, auto & current_scene)
		{
			using clickable_entities = goth2018::game_implementation::entity::contracts::clickable;
			current_scene.entity_manager.for_each_entities(clickable_entities{}, [&ev, &current_scene](auto & entity, auto & position, auto & size, auto & on_click)
			{
				// todo : if constexpr decltype(size) -> rectlangle or circle shape

				const auto mouse_position = goth2018::engine::collision::position_type{ ev.mouseButton.x, ev.mouseButton.y };
				using collision_algorithm = goth2018::engine::collision::algorithm::rectangle_shape;
				if (collision_algorithm::is_collision(std::forward_as_tuple(position, size), mouse_position))
				{
					on_click(entity, current_scene);
				}
			});
		};
	}
}