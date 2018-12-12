#pragma once

#include <goth2018_game_project/graphics/graphics.hpp>
#include <goth2018_game_project/engine/entity.hpp>
#include <goth2018_game_project/engine/collision.hpp>
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

			using drawable_entity_contract = goth2018::engine::entity::contracts::drawable;
			entities.for_each_entities(drawable_entity_contract{}, [&window](auto & entity, auto & position, auto & rendering)
			{
				// todo : debug_warning : calc AABB window and position
				rendering.setPosition(position);
				window.draw(rendering);
			});
		}
		void update()
		{

		}
		void dispatch_event(sf::Event & event)
		{
			auto match = event_handlers.equal_range(event.type);
			for (auto event_handler_it = match.first; event_handler_it != match.second; ++event_handler_it)
			{
				event_handler_it->second(event, *this);
			}
		}

		const std::string name;
		const sf::Sprite background;
		event_handlers_container_type event_handlers
		{
			{
				sf::Event::MouseButtonPressed,
				[](const sf::Event & ev, scene & current_scene)
				{
					using clickable_entities = goth2018::engine::entity::contracts::clickable;
					current_scene.entities.for_each_entities(clickable_entities{}, [&ev](auto & entity, auto & position, auto & size, auto & on_click)
					{
						// todo : if constexpr decltype(size) -> rectlangle or circle shape

						const auto mouse_position = collision::position_type{ ev.mouseButton.x, ev.mouseButton.y };
						using collision_algorithm = goth2018::engine::collision::algorithm::rectangle_shape;
						if (collision_algorithm::is_collision(std::forward_as_tuple(position, size), mouse_position))
						{
							on_click();
						}
					});
				}
			}
		};
		goth2018::engine::entity::manager_type entities{ 10 };

	private:
		const menu_drawer_type menu_drawer;
	};
}