#pragma once

#include <goth2018_game_project/engine/scene.hpp>
#include <vector>
#include <utility>

namespace goth2018::engine
{
	// todo : expose data_context + serializer
	struct core
	{
		using scene_collection = std::vector<scene>;

		core(sf::RenderWindow & render_window, scene_collection && scenes_collection)
			: window{ render_window }
			, scenes{ std::forward<scene_collection>(scenes_collection) }
		{
			if (scenes.size() == 0)
				throw std::runtime_error{ "goth2018::graphics::core::ctor : no scenes" };
		}

		void run()
		{
			using clock_t = std::chrono::steady_clock;
			using duration_t = std::chrono::duration<long long, std::nano>;

			static const std::size_t fps = 60;
			static const duration_t frame_delay = duration_t{ 1s } / fps;
			static auto time = clock_t::now();
			duration_t elapsed_time;

			is_running = true;
			while (is_running)
			{
				elapsed_time = clock_t::now() - time;
				if (elapsed_time < frame_delay) // handle fps excess
					std::this_thread::sleep_for(frame_delay - elapsed_time);
				//else if (elapsed_time > frame_delay)
				//{	// todo
				//	std::cout << "fps drop : " << (frame_delay - elapsed_time).count() << " us\n";
				//}
				time = clock_t::now();

				input();
				update();
				render_once();
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
				auto & active_scene = *active_scene_ptr;
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

			auto & active_scene = *active_scene_ptr;
			active_scene.update();
		}
		void render_once()
		{
			window.clear();
			{	// draw scene
				auto & active_scene = *active_scene_ptr;
				active_scene.draw(window);
			}
			{	// draw UI navigation menu_bar
				draw_navigation_menubar(scenes);
				ImGui::SFML::Render(window);
			}
			window.display();
		}

		void draw_navigation_menubar(scene_collection & scenes)
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
			for (auto & scene : scenes)
			{
				ImGui::PushID(scene.name.c_str());
				ImGui::BeginGroup();
				if (ImGui::Button(scene.name.c_str()))	// ImageButton?
				{
					active_scene_ptr = &scene;
				}
				ImGui::EndGroup();
				ImGui::PopID();

			}
			ImGui::EndMainMenuBar();
			ImGui::PopStyleVar();
		}

	private:
		sf::RenderWindow & window;
		scene_collection scenes;
		scene_collection::value_type * active_scene_ptr = &(*std::begin(scenes));
		bool is_running = false;
	};
}