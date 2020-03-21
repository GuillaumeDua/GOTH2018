#pragma once

#include <goth2018_game_project/engine/collision.hpp>
#include <goth2018_game_project/engine/scene.hpp>

namespace goth2018::game_implementation::event
{
	template <class ECS_manager_type>
	struct handler
	{
		using handler_type = typename goth2018::engine::scene_impl<ECS_manager_type>::event_handler_type;

		struct broadcast_clicked_entity
		{
			template <typename ECS_EM_type>
			void operator()(const sf::Event & ev, ECS_EM_type & entity_manager) const
			{
				using clickable_entities = typename goth2018::game_implementation::entity::contracts::clickable;
				entity_manager.for_each_entities(clickable_entities{}, [&ev, &entity_manager](auto & entity, auto & position, auto & size, auto & on_click)
				{
					// todo : if constexpr decltype(size) -> rectlangle or circle shape

					const auto mouse_position = goth2018::engine::collision::position_type{ ev.mouseButton.x, ev.mouseButton.y };
					using collision_algorithm = goth2018::engine::collision::algorithm::rectangle_shape;
					if (collision_algorithm::is_collision(std::forward_as_tuple(position, size), mouse_position))
					{
						on_click(entity, entity_manager);
					}
				});
			}
		};
		static_assert(std::is_trivial_v<broadcast_clicked_entity>);
	};
}