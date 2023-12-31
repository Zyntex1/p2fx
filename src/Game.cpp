#include "Game.hpp"

#include "Command.hpp"
#include "Modules/Engine.hpp"
#include "Utils.hpp"

#include <cstring>
#include <string>

#include "Games/Portal2.hpp"

#define HAS_GAME_FLAG(flag, name)  \
	if (version & (flag)) {           \
		games += std::string(name "\n"); \
		version &= ~(flag);              \
	}
#define HAS_GAME_FLAGS(flags, name) \
	if (version == (flags)) {          \
		games += std::string(name "\n");  \
		version &= ~(flags);              \
	}

#define TARGET_MOD MODULE("server")

std::vector<std::string> Game::mapNames;
std::map<std::string, int> Game::chapters;
std::vector<AchievementData> Game::achievements;

const char *Game::Version() {
	return "Unknown";
}
Game *Game::CreateNew() {
	auto GetModDir = [](const char *targetMod) {
		auto modDir = std::string();
		auto target = Memory::ModuleInfo();
		if (Memory::TryGetModule(targetMod, &target)) {
			modDir = std::string(target.path);
			modDir = modDir.substr(0, modDir.length() - std::strlen(targetMod));
#ifndef _WIN32
			if (modDir.length() >= 9 && modDir.substr(modDir.length() - 9) == "/linux32/") {
				modDir = modDir.substr(0, modDir.length() - 8);
			}
			if (modDir.length() >= 2 && modDir.substr(modDir.length() - 2) == "//") {
				// I don't really know why this happens but it sometimes does
				modDir = modDir.substr(0, modDir.length() - 1);
			}
#endif
			modDir = modDir.substr(0, modDir.length() - 5);
			if (modDir[modDir.size() - 1] == '/') modDir = modDir.substr(0, modDir.length() - 1);
			modDir = modDir.substr(modDir.find_last_of("\\/") + 1);
		}
		return modDir;
	};

	auto modDir = GetModDir(TARGET_MOD);

	if (Utils::ICompare(modDir, Portal2::ModDir())) {
		return new Portal2();
	}

	return nullptr;
}
std::string Game::VersionToString(int version) {
	auto games = std::string("");
	while (version > 0) {
		HAS_GAME_FLAG(SourceGame_Portal2, "Portal 2")
	}
	return games;
}

bool Game::IsSpeedrunMod() {
	static bool checked = false;
	static bool srm = false;

	if (!checked) {
		auto serverPlugin = (uintptr_t)(engine->s_ServerPlugin->ThisPtr());
		auto count = *(int *)(serverPlugin + 16); // CServerPlugin::m_Size
		if (count > 0) {
			auto plugins = *(uintptr_t *)(serverPlugin + 4); // CServerPlugin::m_Plugins
			for (int i = 0; i < count; ++i) {
				auto ptr = *(CPlugin **)(plugins + i * sizeof (uintptr_t));
				if (!strcmp(ptr->m_szName, "Speedrun Mod was a mistake.")) {
					srm = true;
					break;
				}
			}
		}

		checked = true;
	}

	return srm;
}

