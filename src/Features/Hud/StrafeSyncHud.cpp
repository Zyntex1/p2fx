#include "StrafeSyncHud.hpp"

#include "Features/Stats/Sync.hpp"
#include "Modules/Engine.hpp"
#include "Modules/Scheme.hpp"
#include "Modules/Surface.hpp"
#include "Variable.hpp"

Variable p2fx_hud_strafesync_offset_x("p2fx_hud_strafesync_offset_x", "0", 0, "X offset of strafesync HUD.\n");
Variable p2fx_hud_strafesync_offset_y("p2fx_hud_strafesync_offset_y", "1000", 0, "Y offset of strafesync HUD.\n");
Variable p2fx_hud_strafesync_split_offset_y("p2fx_hud_strafesync_split_offset_y", "1050", 0, "Y offset of strafesync HUD.\n");
Variable p2fx_hud_strafesync_color("p2fx_hud_strafesync_color", "0 150 250 255", 0, "RGBA font color of strafesync HUD.\n", 0);
Variable p2fx_hud_strafesync_font_index("p2fx_hud_strafesync_font_index", "1", 0, "Font index of strafesync HUD.\n");

StrafeSyncHud strafeSyncHud;

StrafeSyncHud::StrafeSyncHud()
	: Hud(HudType_InGame | HudType_Paused | HudType_Menu, true) {
}
bool StrafeSyncHud::ShouldDraw() {
	return p2fx_strafesync.GetBool() && Hud::ShouldDraw();
}
void StrafeSyncHud::Paint(int slot) {
	int width, height;
	engine->GetScreenSize(nullptr, width, height);

	auto xOffset = width / 2 + p2fx_hud_strafesync_offset_x.GetInt();
	auto yOffset = p2fx_hud_strafesync_offset_y.GetInt();

	auto font = scheme->GetFontByID(p2fx_hud_strafesync_font_index.GetInt());
	auto fontColor = Utils::GetColor(p2fx_hud_strafesync_color.GetString(),false).value_or(Color(255,255,255,255));

	surface->DrawTxt(font, xOffset, yOffset, fontColor, "%.2f", synchro->GetStrafeSync(slot));

	for (size_t i = 0; i < synchro->splits.size(); ++i) {
		char txt[16];
		std::sprintf(txt, "%d: %.2f  ", i, synchro->splits[i]);

		auto length = surface->GetFontLength(font, txt);
		surface->DrawRectAndCenterTxt(Color(255, 0, 0), i * length + 10, p2fx_hud_strafesync_split_offset_y.GetInt(), i * length + length, p2fx_hud_strafesync_split_offset_y.GetInt(), font, fontColor, txt);
	}
}
bool StrafeSyncHud::GetCurrentSize(int &xSize, int &ySize) {
	return false;
}
