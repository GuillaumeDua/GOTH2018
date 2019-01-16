#pragma once

#include <gcl_cpp/test.hpp>
#include <gcl_cpp/container/entity_vector.hpp>
#include <tuple>
#include <vector>

#include <goth2018_game_project/game_implementation/entity.hpp>

#include <gcl_cpp/introspection.hpp>
#include <type_traits>

namespace goth2018::engine::entity
{
	namespace property
	{
		struct drawable {};
		struct updatable {};
	};
}

// namespace gcl::introspection::generated :
GCL_Introspection_Generate__has_nested(drawable);
GCL_Introspection_Generate__has_nested(updatable);

GCL_Introspection_Generate__has_member_function(notify_collision);
GCL_Introspection_Generate__has_nested(hitbox);

namespace goth2018::engine::entity::contract
{
	template <typename T>
	constexpr bool has_collision()
	{
		using type = std::decay_t<T>;
		return
			gcl::introspection::generated::has_nested_type::hitbox<type> &&
			gcl::introspection::generated::has_member_function::notify_collision<type>
			;
	}
}

namespace goth2018::test
{
	struct entity
	{
		struct container
		{
			static void proceed()
			{
				using entity_container_t = gcl::container::entity_vector;

				struct available_properties
				{	// todo : property -> contract -> accessor
					struct drawable {};
					struct has_collision {};
					struct user_controlable {};
					struct ui_immutable {};
				};

				struct A { using properties_t = std::tuple<available_properties::drawable, available_properties::has_collision>; };
				struct B { using properties_t = std::tuple<available_properties::drawable>; };
				struct C { using properties_t = std::tuple<available_properties::has_collision>; };

				entity_container_t entities{ A{}, B{}, C{}, A{}, B{}, C{}, A{} };

				entities.remove<A>();
				GCL_TEST__EXPECT_VALUE(entities.get<A>().size(), 0);
				GCL_TEST__EXPECT_VALUE(entities.get().size(), 4);
				entities.remove<available_properties::drawable>();
				GCL_TEST__EXPECT_VALUE(entities.get<B>().size(), 0);
				GCL_TEST__EXPECT_VALUE(entities.get().size(), 2);
			}
		};

		using dependencies_t = std::tuple<container>;
	};
}