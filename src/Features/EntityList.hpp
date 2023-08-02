#pragma once
#include "Command.hpp"
#include "Feature.hpp"
#include "Utils.hpp"
#include <deque>

class EntityList : public Feature {
public:
	EntityList();
	CEntInfo *GetEntityInfoByIndex(int index);
	CEntInfo *GetEntityInfoByName(const char *name);
	CEntInfo *GetEntityInfoByClassName(const char *name);
	int GetEntityInfoIndexByHandle(void *entity);
	IHandleEntity *LookupEntity(const CBaseHandle &handle);
	CEntInfo *QuerySelector(const char *selector);
};

struct EntitySlotSerial {
	bool done = false;
	int slot = -1;
	int serial = -1;
};

extern EntityList *entityList;

extern Command p2fx_list_ents;
extern Command p2fx_find_ent;
extern Command p2fx_find_ents;
extern std::deque<EntitySlotSerial> g_ent_slot_serial;
