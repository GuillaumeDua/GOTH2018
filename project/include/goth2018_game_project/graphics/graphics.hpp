#pragma once

#include <goth2018_game_project/sfml_wrappers.hpp>
#include <SFML/Graphics.hpp>

namespace goth2018::graphics
{
	namespace textures
	{
		static const sf::Texture & get(std::string const & path)
		{	// singleton texture bank, loaded from file
			struct factory_t
			{
				const auto & get(std::string const & path)
				{
					try
					{
						return storage.at(path);
					}
					catch (const std::out_of_range &)
					{
						if (!storage[path].loadFromFile(path))
							throw std::runtime_error{ "textures::factory : cannot load" + path };
						return storage.at(path);
					}
				}
			private:
				using storage_t = std::unordered_map<std::string, sf::Texture>;
				storage_t storage;
			} static factory;
			return factory.get(path);
		}
	}

	struct spritesheet
	{	// hold a texture and convert to sprite
		spritesheet() = delete;
		spritesheet(const spritesheet &) = default;
		spritesheet(spritesheet &&) = default;

		struct construction_policy
		{
			construction_policy() = delete;

			struct using_sprite_dimension {};
			struct using_sprite_quantity {};
		};

		using dimension_type = std::pair<std::size_t, std::size_t>;
		using range_type = std::pair<std::size_t, std::size_t>;

		spritesheet(construction_policy::using_sprite_dimension,
			std::string const & path, dimension_type && dim)
			: texture{ textures::get(path) }
			, sprite_dimension{ std::forward<dimension_type>(dim) }
		{}
		spritesheet(construction_policy::using_sprite_quantity,
			std::string const & path, range_type && sprite_quantity)
			: texture{ textures::get(path) }
			, sprite_dimension{ texture.getSize().x / sprite_quantity.first,  texture.getSize().y / sprite_quantity.second }
		{}

		struct texture_iterator
		{	// calculate position to sprite over a spritesheet-texture
			// sprite.setTextureRect(sf::IntRect(10, 10, 32, 32));

			constexpr texture_iterator(sfml_wrappers::cx_sf_vector2i texture_size, sfml_wrappers::cx_sf_vector2i sprite_size)
				: sprite{ texture_size.first % sprite_size.first == 0 ? sprite_size : throw std::logic_error("spritesheet::texture_iterator : width") }
				, texture{ texture_size.second % sprite_size.second == 0 ? texture_size : throw std::logic_error("spritesheet::texture_iterator : height") }
				, sprite_quantity{ texture_size.first / sprite_size.first, texture_size.second / sprite_size.second }
			{}

			constexpr auto calc_position(std::size_t i) const
				-> sfml_wrappers::cx_sf_vector2i
			{
				const decltype(i) index = i % sprite_count;
				return
				{
					(index * sprite.first) % (sprite_quantity.first * sprite.first),
					sprite.second * (index / sprite_quantity.first)
				};
			}

			const sfml_wrappers::cx_sf_vector2i sprite, texture, sprite_quantity;
			const std::size_t sprite_count = sprite_quantity.first * sprite_quantity.second;
		};

		auto generate_sprite(std::size_t index = 0)
			-> sf::Sprite
		{
			auto coordinates = texture_it.calc_position(index);
			return { texture, sf::IntRect{ { coordinates.first, coordinates.second },{ sprite_dimension.first, sprite_dimension.second } } };
		}
		auto generate_sprites(range_type && range)
		{
			if (not (range.first < range.second))
				throw std::invalid_argument("graphics::spritesheet::generate_sprites(range_type) : bad range");
			std::vector<sf::Sprite> sprites(range.second - range.first);
			std::generate(std::begin(sprites), std::end(sprites), [counter = range.first, this]() mutable->sf::Sprite
			{
				return generate_sprite(counter++);
			});
			return sprites;
		}
		auto generate_sprites()
		{
			std::vector<sf::Sprite> sprites(texture_it.sprite_count);
			std::generate(std::begin(sprites), std::end(sprites), [counter = 0, this]() mutable->sf::Sprite
			{
				return generate_sprite(counter++);
			});
			return sprites;
		}

		const sf::Texture & texture;
		const sfml_wrappers::cx_sf_vector2i sprite_dimension;
		const texture_iterator texture_it{ { texture.getSize().x, texture.getSize().y }, sprite_dimension };
	};

	struct scene
	{
		const sf::Sprite background;
		// std::vector<entities>
		// events
	};

	struct animation
	{
		using sprites_type = std::vector<sf::Sprite>;
		animation(sprites_type && sprites_value)
			: sprites{ std::forward<sprites_type>(sprites_value) }
		{
			if (sprites.size() == 0)
				throw std::runtime_error{ "graphics::animation : empty sprites vector" };
		}

		template <typename func_type>
		void visit_sprites(func_type visitor)
		{
			std::for_each(std::begin(sprites), std::end(sprites), visitor);
		}
		template <typename func_type>
		void visit_sprites(func_type visitor) const
		{
			std::for_each(std::cbegin(sprites), std::cend(sprites), visitor);
		}
		auto & get_sprite()
		{
			auto & sprite = *sprite_it;
			if (++sprite_it == std::end(sprites))
				sprite_it = std::begin(sprites);
			return *sprite_it;
		}

	private:
		sprites_type sprites;
		sprites_type::iterator sprite_it = std::begin(sprites);
	};

	struct rendering_system
	{
		static inline const std::string sprites_path = "C:/Users/Guss/Documents/Visual Studio 2017/Projects/quick_test_SFML/ressources/sprites/";

		void render_once(const scene & scene)
		{

		}

		void render_once(sf::RenderWindow & window)
		{
			window.clear();
			window.draw(background_sprite);
			window.display();
		}

	private:
		const sf::Sprite background_sprite{ graphics::textures::get(sprites_path + "background.png") };
	};
}