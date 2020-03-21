#pragma once

#include <goth2018_game_project/engine/scene.hpp>
#include <goth2018_game_project/configuration.hpp>
#include <gcl_cpp/pattern/ecs.hpp>

#include <iostream>
#include <memory>

namespace goth2018::game_implementation::scenes::bouncing_balls
{
	// Balls are bounding in a window
	// -> window borders collision
	//
	// There are 2 types of balls : red and blue
	// If a ball collides with another ball of the same color, both are destroyed.
	// -> selective balls collision
	//
	// Add also background elements that does not collide, and/or does not move
	// And/or invisible trigger / hitbox that counts entities colliding
	// And/or subwindow balls bounce in.

	// Ideas :
	//
	// ImGui::PlotLines -> frames rate
	//
	// Particle effect : trace that decay over time then desapear
	//
	// Pause, select a ball, show hp
	// Pause, select area, show balls quantity + list { id, hp /*, others components ...*/ }

	namespace entity
	{
		namespace components
		{
			// todo : velocity component ?

			struct position
			{
				operator const sf::Vector2f() const
				{
					return sf::Vector2f{ x, y };
				}
				float x, y;
			};
			struct direction
			{
				operator const sf::Vector2f() const
				{
					//assert(std::abs(x) + std::abs(y) == 1.f); // when velocity implemented
					return sf::Vector2f{ x, y };
				}

				void invert()
				{
					x *= -1;
					y *= -1;
				}

				float x, y;
			};
			// what if size is {x, y} or {radius} ?
			struct size
			{
				float radius;
			};
			using rendering = sf::CircleShape;
		};

		namespace contracts
		{
			using moveable = gcl::pattern::ecs::contract
			<
				components::position,
				components::direction
			>;

			using drawable = gcl::pattern::ecs::contract
			<
				components::position,
				components::rendering
			>;
			using collisionable = gcl::pattern::ecs::contract
			<
				components::position,
				components::size
			>;
		}

		template <class ECS_EM_type>
		struct operations
		{
			using manager_type = ECS_EM_type;

			struct draw
			{
				void operator()(manager_type & entity_manager, sf::RenderWindow & window) const
				{
					using drawable_entity_contract = entity::contracts::drawable;
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
				void operator()(manager_type & entity_manager)
				{
					// move
					using moveable_entity_contract = entity::contracts::moveable;
					entity_manager.for_each_entities(moveable_entity_contract{}, [](auto & entity, auto & position, auto & direction)
					{
						position.x += direction.x;
						position.y += direction.y;
					});

					// bounce
				}
			};
		};

		using EM_type = gcl::pattern::ecs::manager
		<
			components::position,
			components::direction,
			components::size,
			components::rendering
		>;

		struct generator
		{

			using window_size_type = decltype(sf::Window{}.getSize());

			generator()
			{
				rng.seed(std::random_device()());
			}

			auto generate_ball_components(window_size_type const & window_size)
			{
				auto position = generate_position(window_size);
				auto direction = generate_direction();
				auto size = generate_size();
				auto rendering = generate_rendering(position, size);

				return std::forward_as_tuple
				(
					generate_position(window_size),
					generate_direction(),
					generate_size(),
					generate_rendering(position, size)
				);
			}

		private:
			std::mt19937 rng;

			auto generate_position(window_size_type const & window_size)
				-> entity::components::position
			{
				const auto rnd_x_position = std::uniform_real_distribution<float>{ 0, static_cast<float>(window_size.x) };
				const auto rnd_y_position = std::uniform_real_distribution<float>{ 0, static_cast<float>(window_size.y) };

				return { rnd_x_position(rng), rnd_y_position(rng) };
			}
			auto generate_direction()
				-> entity::components::direction
			{
				static const auto rnd_direction = std::uniform_real_distribution<float>(-1.f, 1.f);
				// also emulate velocity, as abs(x) + abs(y) != 1
				return { rnd_direction(rng), rnd_direction(rng) };
			}
			auto generate_size()
				-> entity::components::size
			{
				static const auto rnd_radius = std::uniform_real_distribution<float>{ 5.f, 50.f };
				return { rnd_radius(rng) };
			}
			auto generate_rendering(entity::components::position const & position_value, entity::components::size const & size_value)
				-> entity::components::rendering
			{
				// rand : blue or red ball type ?
				// ECS : how to integrate states

				/*auto const & size = std::get<entity::components::size&>(components);
				auto const & position = std::get<entity::components::position&>(components);*/

				sf::CircleShape value{ size_value.radius };
				value.setPosition(position_value);

				return value;
			};
		};
	}

	namespace control
	{
		class ECS_EM_menu
		{
			entity::EM_type & ECS_manager;
			entity::generator entity_generator;

		public:

			ECS_EM_menu(entity::EM_type & ecs_em)
				: ECS_manager{ ecs_em }
			{}

			void operator()()
			{
				// ImGui::SetNextWindowPos({ 0, 35 });
				// ImGui::SetNextWindowSize({400, 80});

				ImGui::Begin("Actions", 0, ImGuiWindowFlags_MenuBar);
				{
					static int slider_balls_quantity_value = 1;

					if (ImGui::BeginMenuBar())
					{
						if (ImGui::Button("Clear"))
						{	// remote all balls
							clear_all_balls();
						}
						if (ImGui::Button("Add"))
						{	// add balls
							add_balls(slider_balls_quantity_value);
						}

						ImGui::EndMenuBar();
					}

					ImGui::SliderInt("Balls quantity to add", &slider_balls_quantity_value, 1, 100);

					static bool is_collision_enabled = false;
					ImGui::Checkbox("Collision enabled ?", &is_collision_enabled);
				}
				ImGui::End();
			}

		private:
			void add_one_ball()
			{
				using components_collection_type = decltype(entity_generator.generate_ball_components({ 800, 600 }));

				auto components_values = entity_generator.generate_ball_components({ 800, 600 }); // todo : remove hard-coded values

				auto [entity, components] = ECS_manager.create_entity
				<
					entity::components::position,
					entity::components::direction,
					entity::components::size,
					entity::components::rendering
				>
				(std::move(components_values));
			}
			void add_balls(int quantity)
			{
				assert(quantity > 0);

				for (int i = 0; i < quantity; ++i)
				{
					add_one_ball();
				}
			}
			void clear_all_balls()
			{
				ECS_manager.clear();
			}
		};
	};

	static auto generate(const sf::Window & window)
	{
		using ECS_EM_type = entity::EM_type;
		using scene_type = goth2018::engine::scene_impl<ECS_EM_type>;
		auto scene_background_path = std::string{ goth2018::configuration::path::background } +"background.png";
		auto scene_background = graphics::sprite::create(std::move(scene_background_path));

		auto scene = std::make_unique<scene_type>
		(
			"bouncing balls",
			std::move(scene_background)
		);
		//{
		//	"bouncing balls",
		//	std::move(scene_background)/*,
		//	[]() { control::draw_menu(); }*/
		//});
		//scene.entity_operators = decltype(scene.entity_operators)
		//{
		//	entity::operations<ECS_EM_type>::draw{},
		//	entity::operations<ECS_EM_type>::update{}
		//};

		//scene.menu_drawer = control::ECS_EM_menu{ scene.entity_manager }; // error ! dangling reference to scene.entity_manager

		return scene;
	}
};
