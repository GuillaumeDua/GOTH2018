#pragma once

#include <chrono>
#include <functional>
#include <iostream>
#include <iomanip>
#include <thread>

namespace goth2018::engine
{
	using namespace std::chrono_literals;

	struct fps_manager
	{
		using clock_type = std::chrono::steady_clock;
		using duration_t = std::chrono::duration<long long, std::nano>;

		fps_manager(std::size_t fps)
		{
			requiere_fps(fps);
		}

		void update()
		{
			static auto next_second = clock_type::now() + 1s;
			static auto last_frame_time = clock_type::now();
			static std::size_t fps_counter = 0;

			elapsed_time = clock_type::now() - last_frame_time;
			if (elapsed_time >= requiered_frame_delay)
			{
				last_frame_time = clock_type::now();
				++fps_counter;
				per_frame(elapsed_time);
			}
			if (clock_type::now() >= next_second)
			{
				next_second = clock_type::now() + 1s;
				if (fps_counter != requiered_fps)
					on_frame_drop(requiered_fps, fps_counter);
				per_second(fps_counter);
				fps_counter = 0;
			}

		}
		void requiere_fps(std::size_t fps)
		{
			if (fps == 0)
				throw std::invalid_argument("fps_manager::requiere_fps : 0 fps");
			requiered_fps = fps;
			requiered_frame_delay = duration_t{ 1s } / fps;
		}

		using per_frame_type = std::function<void(const duration_t &)>;
		per_frame_type per_frame = [](const duration_t &) {};
		using per_seconds_type = std::function<void(std::size_t)>;
		per_seconds_type per_second = [](std::size_t) {};
		using on_frame_drop_type = std::function<void(std::size_t, std::size_t)>;
		on_frame_drop_type on_frame_drop = [](std::size_t, std::size_t) {};

	private:
		duration_t elapsed_time;
		duration_t requiered_frame_delay;
		std::size_t requiered_fps;
	};
}
