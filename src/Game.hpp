#pragma once
#include <string>
#include <vector>
#include <map>

enum SourceGameVersion {
	SourceGame_Unknown = 0,

	SourceGame_Portal2 = (1 << 0)
};

struct AchievementData {
	const char *keyName;
	const char *displayName;
	bool coop;
	bool claimed = false;
};

class Game {
protected:
	SourceGameVersion version = SourceGame_Unknown;

public:
	virtual ~Game() = default;
	virtual void LoadOffsets() = 0;
	virtual const char *Version();
	virtual const float Tickrate() = 0;
	inline bool Is(int game) { return this->version & game; }
	inline SourceGameVersion GetVersion() { return this->version; }

	static Game *CreateNew();

	static std::string VersionToString(int version);
	static std::vector<std::string> mapNames;
	static std::map<std::string, int> chapters;
	static std::vector<AchievementData> achievements;

	static bool IsSpeedrunMod();
};
