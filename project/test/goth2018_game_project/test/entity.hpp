#pragma once

#include <gcl_cpp/test.hpp>
#include <gcl_cpp/container/entity_vector.hpp>
#include <tuple>
#include <vector>

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

				std::cout << entities.get<available_properties::drawable>().size() << std::endl;
				for (const auto & elem : entities.get())
				{
					std::cout << "\t - " << elem->type().name() << '\n';
				}

				//entities.remove<available_properties::drawable>();
				entities.remove<A>();

				std::cout << entities.get<available_properties::drawable>().size() << std::endl;
				for (const auto & elem : entities.get())
				{
					std::cout << "\t - " << elem->type().name() << '\n';
				}
				/*auto & first_A = std::any_cast<A>(*(entities.get().at(0).get()));
				entities.remove_if()*/
			}
		};

		using dependencies_t = std::tuple<container>;
	};
}