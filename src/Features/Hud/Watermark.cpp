#include "Version.hpp"

#	include "Hud.hpp"
#	include "Modules/Engine.hpp"
#	include "Modules/Surface.hpp"
#	include "Modules/Scheme.hpp"

#	define WATERMARK_MSG_HEADER "P2FX"
#	define WATERMARK_MSG_HELPTEXT "P2FX is loaded. Do not use in speedruns."

class WatermarkHud : public Hud {
public:
	WatermarkHud()
		: Hud(HudType_InGame | HudType_Paused | HudType_Menu | HudType_LoadingScreen, false) {
	}

	bool ShouldDraw() override {
		return !engine->demoplayer->IsPlaying();
	}

	bool GetCurrentSize(int &w, int &h) override {
		return false;
	}

	void Paint(int slot) override {
		int screenWidth, screenHeight;
		engine->GetScreenSize(nullptr, screenWidth, screenHeight);

		Surface::HFont headerFont = scheme->GetFontByID(33);
		Surface::HFont subTextFont = scheme->GetFontByID(32);

		int fontSize = surface->GetFontHeight(headerFont);

		int xPos = screenWidth - std::max(surface->GetFontLength(headerFont, "%s", WATERMARK_MSG_HEADER), surface->GetFontLength(subTextFont, "%s", WATERMARK_MSG_HELPTEXT)) - fontSize * 2;

		surface->DrawTxt(headerFont, xPos, screenHeight - fontSize * 3, Color{255, 255, 255, 100}, "%s", WATERMARK_MSG_HEADER);
		surface->DrawTxt(subTextFont, xPos, screenHeight - fontSize * 2, Color{255, 255, 255, 100}, "%s", WATERMARK_MSG_HELPTEXT);
	}
};

WatermarkHud watermark;