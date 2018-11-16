#pragma once

#include <string_view>
#include <string>
#include <unordered_map>
#include <filesystem>

namespace goth2018::configuration
{
	struct path
	{
		path() = delete;
#ifndef RESSOURCES_DIRECTORY
		static_assert(false, "Preprocessor macro not defined : ressources directory (RESSOURCES_DIRECTORY)");
#endif
		static inline const constexpr std::string_view ressources = RESSOURCES_DIRECTORY;
		static inline const constexpr std::string_view images = RESSOURCES_DIRECTORY "\\images\\";
		static inline const constexpr std::string_view sprites = RESSOURCES_DIRECTORY "\\images\\sprites\\";
		static inline const constexpr std::string_view background = RESSOURCES_DIRECTORY "\\images\\background\\";
		static inline const constexpr std::string_view audio = RESSOURCES_DIRECTORY "\\audio\\";
		static inline const constexpr std::string_view sounds = RESSOURCES_DIRECTORY "\\audio\\sounds\\";
		static inline const constexpr std::string_view musics = RESSOURCES_DIRECTORY "\\audio\\musics\\";
	};

	/*{
		{ 42, 42 }
		{ 1, 2 }

		enum class QWE : uint8_t
		{
			a,b,c,d,e,f,g,

			first = a,
			last = g
		}
	}*/

	struct spritesheet
	{	// todo : template enum type instead of uint8_t ?
		using animation_range = std::pair<std::size_t, std::size_t>; // from (index), to (index)
		using animation_descriptors_type = std::unordered_map<uint8_t, animation_range>;

		spritesheet(std::filesystem::path && path)
		{
			load_from_file(std::forward<decltype(path)>(path));
		}

		const auto & get_animations_descriptors() const
		{
			return animations_descriptors;
		}

	private:
		void load_from_file(std::filesystem::path && path)
		{

		}

		animation_descriptors_type animations_descriptors;
	};
}