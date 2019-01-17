#pragma once

#include <goth2018_game_project/engine/scene.hpp>
#include <goth2018_game_project/engine/fps_manager.hpp>

#include <gcl_cpp/introspection.hpp>
#include <gcl_cpp/tuple_utils.hpp>

#include <vector>
#include <utility>

namespace goth2018::engine
{
	// todo : expose data_context + serializer
	template <class ECS_manager_type>
	struct core
	{
		using scene_type = scene<ECS_manager_type>;
		using scene_collection = std::vector<scene_type>;

		core(const core &) = delete;
		core(core &&) = default;

		core(sf::RenderWindow & render_window, scene_collection && scenes_values)
			: window{ render_window }
			, scenes{ std::forward<std::decay_t<decltype(scenes_values)>>(scenes_values) }
		{	// construct with a scene collection
			if (scenes.size() == 0)
				throw std::runtime_error{ "goth2018::graphics::core::ctor : no scenes" };
			active_scene_ptr = &(*std::begin(scenes));
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
				window.setTitle(active_scene_ptr->name + " : " + std::to_string(fps_per_second) + " fps");
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
		void draw()
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
				{	// concurrency
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
		typename scene_collection::value_type * active_scene_ptr;
		bool is_running = false;
	};
}