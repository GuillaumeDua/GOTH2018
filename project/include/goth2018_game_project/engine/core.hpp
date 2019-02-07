#pragma once

#include <goth2018_game_project/engine/scene.hpp>
#include <goth2018_game_project/engine/fps_manager.hpp>

#include <gcl_cpp/introspection.hpp>
#include <gcl_cpp/tuple_utils.hpp>
#include <gcl_cpp/container/utility.hpp>

#include <vector>
#include <utility>

namespace goth2018::engine
{
	// todo : expose data_context + serializer
	struct core
	{
		core(const core &) = delete;
		core(core &&) = default;

		using scene_collection_type = std::vector<engine::scene>;

		core(sf::RenderWindow & render_window, scene_collection_type && scenes_args)
			: window{ render_window }
			, scenes( std::forward<scene_collection_type>(scenes_args) )
		{	// construct with a scene collection
			assert(scenes.size() > 0);
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
				auto & active_scene = scenes.at(active_scene_index);
				window.setTitle(active_scene.get_name() + " : " + std::to_string(fps_per_second) + " fps");
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
			auto & active_scene = scenes.at(active_scene_index); // what if the event change the active scene ?
			sf::Event event;
			while (window.pollEvent(event))
			{
				active_scene.dispatch_event(event);

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

			auto & active_scene = scenes.at(active_scene_index);
			active_scene.update();
		}
		void draw()
		{
			window.clear();

			auto & active_scene = scenes.at(active_scene_index);
			active_scene.draw(window);

			{	// draw UI navigation menu_bar
				draw_navigation_menubar();
				ImGui::SFML::Render(window);
			}
			window.display();
		}

	private:

		void draw_navigation_menubar()
		{	// switch across scenes
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 10, 10 });
			ImGui::BeginMainMenuBar();

			if (ImGui::BeginMenu("Menu"))
			{
				if (ImGui::MenuItem("Save and quit", "Ctrl+S"))
				{
					/* serialize data_context */
					stop();
				}
				ImGui::EndMenu();
			}
			for (auto scene_index{ 0 }; scene_index < scenes.size(); ++scene_index)
			{
				auto & scene = scenes.at(scene_index);

				ImGui::PushID(scene.get_name().c_str());
				ImGui::BeginGroup();
				if (ImGui::Button(scene.get_name().c_str()))	// ImageButton?
				{	// concurrency
					active_scene_index = scene_index;
				}
				ImGui::EndGroup();
				ImGui::PopID();

			}
			ImGui::EndMainMenuBar();
			ImGui::PopStyleVar();
		}

		sf::RenderWindow & window;
		scene_collection_type scenes;
		std::size_t active_scene_index = 0;
		bool is_running = false;
	};
}