#pragma once

#include <goth2018_game_project/graphics/graphics.hpp>

#include <chrono>
#include <memory>
#include <thread>
#include <Windows.h>

namespace interactive_test
{
	void spritesheet()
	{
		HWND consoleWindow = GetConsoleWindow();
		SetWindowPos(
			consoleWindow, 0,
			0, 0,
			600, 900,
			SWP_NOZORDER);

		const sf::Sprite background_sprite{ goth2018::graphics::textures::get(goth2018::graphics::rendering_system::sprites_path + "background.png") };

		std::string spritesheet_path;
		do
		{
			std::cout << "Spritesheet path :\n>> ";
			std::getline(std::cin, spritesheet_path);
		} while (not std::cin);

		[](std::string & path)
		{	// clean path
			path.erase
			(
				std::remove_if(std::begin(path), std::end(path), [](std::decay_t<decltype(path)>::value_type value) { return value == '"'; }),
				std::end(path)
			);
		}(spritesheet_path);
		std::cout << "\nUsing path : [" << spritesheet_path << "]\n";

		std::size_t construction_policy_choice;
		do
		{
			std::cout
				<< "Spritesheet construction policy :\n"
				<< "-> choose 1 to use sprites quantity\n"
				<< "-> choose 2 to use sprites dimension\n"
				<< ">> "
				;
			std::cin >> construction_policy_choice;
			std::cout << "choice : [" << construction_policy_choice << "]\n";
		} while (not std::cin ||
			(construction_policy_choice not_eq 1 and
				construction_policy_choice not_eq 2));

		std::unique_ptr<goth2018::graphics::spritesheet> test_spritesheet;
		[&]()
		{	// create spritesheet
			using arg_type = std::pair<std::size_t, std::size_t>;
			arg_type arg;

			if (construction_policy_choice == 1)
			{
				std::cout << "sprites per row :\n>> ";
				do { std::cin >> arg.first; } while (not std::cin);
				std::cout << "sprites per column :\n>> ";
				do { std::cin >> arg.second; } while (not std::cin);

				test_spritesheet = std::make_unique<goth2018::graphics::spritesheet>
					(
						goth2018::graphics::spritesheet
						(
							goth2018::graphics::spritesheet::construction_policy::using_sprite_quantity{},
							spritesheet_path, { arg.first, arg.second }
				));
			}
			else
			{
				std::cout << "sprite's width  :\n>> ";
				do { std::cin >> arg.first; } while (not std::cin);
				std::cout << "sprite's height :\n>> ";
				do { std::cin >> arg.second; } while (not std::cin);

				test_spritesheet = std::make_unique<goth2018::graphics::spritesheet>
					(
						goth2018::graphics::spritesheet
						(
							goth2018::graphics::spritesheet::construction_policy::using_sprite_dimension{},
							spritesheet_path, { arg.first, arg.second }
				));
			}
		}();

		goth2018::graphics::animation test_animation{ { sf::Sprite{ test_spritesheet->texture } } };
		auto generate_animation = [&test_animation, &test_spritesheet]()
		{
			goth2018::graphics::spritesheet::range_type sprite_range;
			do
			{
				std::cout
					<< "Select animation range, from 0 to " << test_spritesheet->texture_it.sprite_count - 1 << '\n'
					<< "from :\n>> ";
				std::cin >> sprite_range.first;
			} while (not std::cin);
			do
			{
				std::cout << "to   :\n>> ";
				std::cin >> sprite_range.second;
			} while (not std::cin || sprite_range.second > (test_spritesheet->texture_it.sprite_count - 1));
			test_animation = goth2018::graphics::animation{ test_spritesheet->generate_sprites(std::move(sprite_range)) };
		};

		std::cout
			<< "----------------------------------------\n"
			<< "- Press <space> to create an animation\n"
			<< "- Press <enter> to change FPS rate\n"
			<< "- Close window to quit\n"
			<< "----------------------------------------\n"
			;

		std::chrono::milliseconds time_between_frames{ 200 };
		sf::RenderWindow window{ sf::VideoMode(1200, 900), "spritesheet test" };
		window.setPosition({ 650, 0 });
		while (window.isOpen())
		{
			sf::Event event;
			while (window.pollEvent(event))
			{
				switch (event.type)
				{
				case sf::Event::Closed:
					window.close();
					break;
				case sf::Event::KeyPressed:
				{
					SetFocus(consoleWindow);
					if (event.key.code == sf::Keyboard::Space)
						generate_animation();
					else if (event.key.code == sf::Keyboard::Add)
					{
						time_between_frames = std::chrono::milliseconds
						{
							time_between_frames.count() <= 5
							? 1
							: time_between_frames.count() - 5
						};
					}
					else if (event.key.code == sf::Keyboard::Subtract)
					{
						time_between_frames += std::chrono::milliseconds{ 5 };
					}
					else if (event.key.code == sf::Keyboard::Enter)
					{
						std::size_t fps;
						do
						{
							std::cout << "FPS requiered (from 1 to 60) :\n>> ";
							std::cin >> fps;
						} while (not std::cin || fps < 1 || fps > 60);
						time_between_frames = std::chrono::milliseconds{ std::chrono::milliseconds{ std::chrono::seconds{ 1 } }.count() / fps };
						std::cout
							<< "Setting FPS to " << fps
							<< ", which represents " << time_between_frames.count()
							<< " ms between frames\n";
					}
					//if (not window.hasFocus())
					window.requestFocus();
				}
				default:
					break;
				}
			}

			window.clear();
			window.draw(background_sprite);
			window.draw(test_animation.get_sprite());
			window.display();
			std::this_thread::sleep_for(time_between_frames);
		}
	}
}