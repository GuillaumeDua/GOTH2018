#pragma once

#include <goth2018_game_project/engine/scene.hpp>
#include <goth2018_game_project/engine/fps_manager.hpp>

#include <gcl_cpp/introspection.hpp>
#include <gcl_cpp/tuple_utils.hpp>
#include <gcl_cpp/container/selector.hpp>
#include <gcl_cpp/container/utility.hpp>

#include <vector>
#include <utility>

namespace goth2018::engine
{
	// todo : expose data_context + serializer
	//template <class ECS_manager_type>
	template <class ... scene_type>
	struct core
	{
		/*using scene_type = ECS_scene<ECS_manager_type>;
		using scene_collection = std::vector<scene_type>;*/

		core(const core &) = delete;
		core(core &&) = default;

		using scene_collection_type = decltype(gcl::container::make_variant_array(std::declval<scene_type>()...));

		core(sf::RenderWindow & render_window, scene_type && ... scenes_values)
			: window{ render_window }
			, scene_selector{ std::move(gcl::container::make_variant_array(std::forward<scene_type>(scenes_values)...))}
			/*, scenes{ std::forward<std::decay_t<decltype(scenes_values)>>(scenes_values) }*/
		{	// construct with a scene collection
			static_assert(sizeof...(scenes_values) > 0, "goth2018::graphics::core::ctor : no scenes");
		}

		void run()
		{
			fps_manager frame_manager{ 60 };
			frame_manager.on_frame_drop = [](auto requiered_fps, auto last_second_fps)
			{
				std::cerr
					<< "[goth2018]::[warning] : frame drop detected : "
					<< last_second_fps << " / " << requiered_fps << '\n';
			};
			frame_manager.per_second = [this](auto fps_per_second)
			{
				std::visit([this, &fps_per_second](auto & scene)
				{
					window.setTitle(scene.name + " : " + std::to_string(fps_per_second) + " fps");
				}, scene_selector.get_selected());
			};
			frame_manager.per_frame = [this](const auto & elapsed_time)
			{
				input();
				update();
				draw();
			};

			is_running = true;
			while (is_running)
			{
				frame_manager.update(); // tick
			}
		}
		void stop()
		{
			is_running = false;
			if (window.isOpen())
				window.close();
		}

		void input()
		{
			sf::Event event;
			while (window.pollEvent(event))
			{
				std::visit([&event](auto & scene)
				{
					scene.dispatch_event(event);
				}, scene_selector.get_selected());

				ImGui::SFML::ProcessEvent(event);
				if (event.type == sf::Event::Closed)
				{
					stop();
				}
			}
		}
		void update()
		{
			static sf::Clock deltaClock;
			ImGui::SFML::Update(window, deltaClock.restart());

			std::visit([](auto & scene)
			{
				scene.update();
			}, scene_selector.get_selected());
		}
		void draw()
		{
			window.clear();

			std::visit([&window = this->window](auto & scene)
			{
				scene.draw(window);
			}, scene_selector.get_selected());

			{	// draw UI navigation menu_bar
				//draw_navigation_menubar(scenes); // todo !
				ImGui::SFML::Render(window);
			}
			window.display();
		}

	private:
		void draw_navigation_menubar(typename scene_collection_type::value_type & scenes)
		{	// switch across scenes
			//ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 10, 10 });
			//ImGui::BeginMainMenuBar();

			//if (ImGui::BeginMenu("Menu"))
			//{
			//	if (ImGui::MenuItem("Save and quit", "Ctrl+S"))
			//	{
			//		/* serialize data_context */
			//		stop();
			//	}
			//	ImGui::EndMenu();
			//}
			//for (auto & scene : scenes)
			//{
			//	ImGui::PushID(scene.name.c_str());
			//	ImGui::BeginGroup();
			//	if (ImGui::Button(scene.name.c_str()))	// ImageButton?
			//	{	// concurrency
			//		active_scene_ptr = &scene;
			//	}
			//	ImGui::EndGroup();
			//	ImGui::PopID();

			//}
			//ImGui::EndMainMenuBar();
			//ImGui::PopStyleVar();
		}

		sf::RenderWindow & window;
		gcl::container::selector<scene_collection_type> scene_selector;
		bool is_running = false;
	};
}