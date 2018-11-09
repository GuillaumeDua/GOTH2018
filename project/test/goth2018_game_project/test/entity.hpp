#pragma once

#include <gcl_cpp/test.hpp>

#include <gcl_cpp/container/entity_vector.hpp>
#include <tuple>

namespace test
{
	struct entity
	{
		

		struct container
		{
			static void proceed()
			{
				struct entity_interface {};
				using entity_container_t = gcl::container::entity_vector<entity_interface>;

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

				entity_container_t entities;

				entities.push_back(std::make_unique<A>());
			}
		};

		using dependencies_t = std::tuple<container>;
	};
}