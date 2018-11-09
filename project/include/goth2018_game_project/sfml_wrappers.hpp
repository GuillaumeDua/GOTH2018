#pragma once

#include <SFML/Graphics.hpp>
#include <utility>
#include <tuple>

namespace goth2018::sfml_wrappers
{
	struct cx_sf_vector2i : std::pair<std::uint16_t, std::uint16_t>
	{	// cx sf::Vector2i wrapper for unsigned values
		//std::uint16_t first, second;

		using value_type = std::uint16_t;
		using base_type = std::pair<value_type, value_type>;

		template <class first_type, class second_type>
		constexpr cx_sf_vector2i(first_type && v1, second_type && v2)
			: base_type{ static_cast<value_type>(v1), static_cast<value_type>(v2) }
		{}
		template <class first_type, class second_type>
		constexpr cx_sf_vector2i(std::pair<first_type, second_type> && value)
			: base_type{ static_cast<value_type>(value.first), static_cast<value_type>(value.second) }
		{}
		constexpr cx_sf_vector2i(sf::Vector2i && v)
			: base_type{ v.x, v.y }
		{}

		template
		<
			typename first_type,
			typename second_type,
			template <typename, typename> class container_type
		>
		bool operator==(const typename container_type<first_type, second_type> & value) const
		{
			return first == std::get<0>(value) && second == std::get<1>(value);
		}

		operator sf::Vector2i() const
		{
			return { static_cast<int>(first), static_cast<int>(second) };
		}
	};
}