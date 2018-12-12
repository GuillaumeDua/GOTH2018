#pragma once

#include <gcl_cpp/introspection.hpp>

namespace goth2018::engine::collision
{
	using position_type = struct
	{
		int x, y;
	};

	namespace algorithm
	{
		struct rectangle_shape
		{
			template <typename size_type, typename position_type_1, typename position_type_2>
			static bool is_collision(const std::tuple<position_type_1&, size_type&> & hitbox, const position_type_2 & point)
			{
				const auto & size = std::get<size_type&>(hitbox);
				const auto & position = std::get<position_type_1&>(hitbox);
				return
				(
					position.x < point.x + size.width &&
					position.x + size.width > point.x &&
					position.y < point.y &&
					position.y + size.height > point.y
				);
			}
			template <typename size_type, typename position_type>
			static bool is_collision(const std::tuple<size_type&, position_type&> & hitbox_1, const std::tuple<size_type&, position_type&> & hitbox_2)
			{
				const auto & hitbox_1_size = std::get<size_type&>(hitbox_1);
				const auto & hitbox_1_position = std::get<position_type&>(hitbox_1);
				const auto & hitbox_2_size = std::get<size_type&>(hitbox_2);
				const auto & hitbox_2_position = std::get<position_type&>(hitbox_2);

				return
				(
					hitbox_1_position.x < hitbox_2_position.x + hitbox_1_size.width &&
					hitbox_1_position.x + hitbox_1_size.width > hitbox_2_position.x &&
					hitbox_1_position.y < hitbox_2_position.y + hitbox_2_size.height &&
					hitbox_1_position.y + hitbox_1_size.height > hitbox_2_position.y
				);
			}
		};
	}

	/*
	GCL_Introspection_Generate__has_member_function(notify_collision);
	GCL_Introspection_Generate__has_nested(size);
	GCL_Introspection_Generate__has_nested(position);

	namespace goth2018::engine::entity::contract
	{
		template <typename T>
		constexpr static bool has_shape()
		{
			using type = std::decay_t<T>;
			return
				gcl::introspection::generated::has_nested_type::position<type> &&
				gcl::introspection::generated::has_nested_type::size<type>
				;
		}

		template <typename T>
		constexpr static bool has_collision()
		{
			using type = std::decay_t<T>;
			return
				has_shape<type> &&
				gcl::introspection::generated::has_member_function::notify_collision<type>
				;
		}
	}
	namespace algorithm
	{
		using point_type = struct
		{
			std::size_t x, y;
		};

		struct rectangle_shape
		{
			template <typename T, typename U>
			static bool is_collision(const T & hitbox_1, const point_type & point)
			{
				return
				(
					hitbox_1.position.x < point.x + hitbox_1.size.width &&
					hitbox_1.position.x + hitbox_1.size.width > point.x &&
					hitbox_1.position.y < point.y &&
					hitbox_1.position.y + hitbox_1.size.height > point.y
				);
			}
			template <typename T, typename U>
			static bool is_collision(const T & hitbox_1, const U & hitbox_2)
			{
				return
				(
					hitbox_1.position.x < hitbox_2.position.x + hitbox_1.size.width &&
					hitbox_1.position.x + hitbox_1.size.width > hitbox_2.position.x &&
					hitbox_1.position.y < hitbox_2.position.y + hitbox_2.size.height &&
					hitbox_1.position.y + hitbox_1.size.height > hitbox_2.position.y
				);
			}
		};
		struct circle_shape
		{
			template <typename T, typename U>
			static bool is_collision(const T & hitbox_1, const point_type & point)
			{
				const auto distance = std::sqrt
				(
					((hitbox_1.position.x - point.x) * (hitbox_1.position.x – point.x))
					+ ((hitbox_1.position.y - point.y) * (hitbox_1.position.y – point.y))
				);
				return (distance < hitbox_1.size.radius);
			}
			template <typename T, typename U>
			static bool is_collision(const T & hitbox_1, const U & hitbox_2)
			{
				const auto distance = std::sqrt
				(
					((hitbox_1.position.x - hitbox_2.position.x) * (hitbox_1.position.x – hitbox_2.position.x))
					+ ((hitbox_1.position.y - hitbox_2.position.y) * (hitbox_1.position.y – hitbox_2.position.y))
				);
				return (distance < hitbox_1.size.radius + hitbox_2.size.radius);
			}
		};
	}*/
}