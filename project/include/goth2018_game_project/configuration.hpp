#pragma once

#include <string_view>
#include <string>

namespace goth2018::configuration
{
	struct path
	{
		path() = delete;
#ifndef RESSOURCES_DIRECTORY
		static_assert(false, "Preprocessor macro not defined : ressources directory (RESSOURCES_DIRECTORY)");
#endif
		static inline const constexpr std::string_view ressources = RESSOURCES_DIRECTORY;
		static inline const constexpr std::string_view sprites = RESSOURCES_DIRECTORY "\\sprites";
		static inline const constexpr std::string_view audio = RESSOURCES_DIRECTORY "\\audio";
		static inline const constexpr std::string_view sounds = RESSOURCES_DIRECTORY "\\audio\\sounds";
		static inline const constexpr std::string_view musics = RESSOURCES_DIRECTORY "\\audio\\musics";
	};
}