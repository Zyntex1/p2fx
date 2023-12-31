﻿#include "DemoHud.hpp"

#include "Event.hpp"

#include "Modules/Scheme.hpp"
#include "Modules/Surface.hpp"
#include "Modules/Engine.hpp"

#include "Features/DemoViewer.hpp"
#include "Features/Camera.hpp"
#include "Features/Renderer.hpp"

#include "Variable.hpp"

#include "Input.hpp"

#define DRAW_MULTI_COLOR_TEXT(font, x, y, clr1, clr2, txt1, txt2) \
	surface->DrawTxt(font, x, y, clr1, txt1);                        \
	surface->DrawTxt(font, x + surface->GetFontLength(font, txt1), y, clr2, txt2);

#define DRAW_CENTERED_TEXT(font, x, y, clr, ...) \
	surface->DrawTxt(font, x - surface->GetFontLength(font, __VA_ARGS__) / 2, y, clr, __VA_ARGS__);

DemoHud demoHud;

DemoHud::DemoHud()
	: Hud(HudType_InGame, false) {
}

bool DemoHud::ShouldDraw() {
	return g_shouldDraw && engine->demoplayer->IsPlaying() && Hud::ShouldDraw();
}

void DemoHud::Paint(int slot) {
	int playbackTicks = demoViewer->g_demoPlaybackTicks;

	if (playbackTicks == -1)
		return;

	int scrW, scrH;

	engine->GetScreenSize(nullptr, scrW, scrH);

	const int w = 600;
	const int h = 120;

	int x = scrW / 2 - w / 2;
	int y = scrH - h;

	auto font = scheme->GetFontByID(62);
	auto iconFont = scheme->GetFontByID(168);

	const Color white = {255, 255, 255};
	const Color accent = {45, 140, 235};

	DRAW_MULTI_COLOR_TEXT(font, x, y - 30, accent, white, "F4 ", "Toggle Controls");

	if (camera->controlType == Drive) {
		auto cmtx = "Insert Camera Marker";
		for (const auto &state : camera->states) {
			float dist = (camera->currentState.origin - state.second.origin).Length();
			if (dist < 30.0f) {
				cmtx = "Remove Camera Marker";
				break;
			}
		}

		int cmtxLength = surface->GetFontLength(font, "F %s", cmtx);
		int cmtxPos = w / 2 - cmtxLength / 2;
		DRAW_MULTI_COLOR_TEXT(font, x + cmtxPos, y - 30, accent, white, "F ", cmtx);
	}

	int tghtxLength = surface->GetFontLength(font, "F2 Toggle Hud");
	int tghtxPos = w - tghtxLength;
	DRAW_MULTI_COLOR_TEXT(font, x + tghtxPos, y - 30, accent, white, "F2 ", "Toggle Hud");

	surface->DrawRect({0, 0, 0, 200}, x, y, x + w, y + h);

	y += 22;

	surface->DrawRect({70, 70, 70}, x + 16, y, x + w - 16, y + 64);
	surface->DrawRect({28, 28, 28}, x + 17, y + 1, x + w - 17, y + 63);

	x += 16;

	int pbtLength = surface->GetFontLength(font, "%d", playbackTicks);

	surface->DrawRect({70, 70, 70}, x + 8, y + 8, x + w - pbtLength - 48, y + 26);
	surface->DrawRect({20, 20, 20}, x + 9, y + 9, x + w - pbtLength - 49, y + 25);
	surface->DrawRect({0, 0, 0}, x + 9, y + 16, x + w - pbtLength - 49, y + 25);

	int timelineSize = x + w - pbtLength - 49 - (x + 9) - 1;

	int tick = engine->demoplayer->GetTick();

	int demoStart = demoViewer->g_demoStart;
	float demoStartFrac = (float)demoStart / (float)playbackTicks;
	float demoStartPos = demoStartFrac * timelineSize;

	surface->DrawRect({100, 0, 0}, x + 9, y + 9, x + demoStartPos, y + 25);

	surface->DrawTxt(font, x + w - pbtLength - 40, y + 3, white, "%d", playbackTicks);

	float playbackFrac = (float)tick / (float)playbackTicks;
	int playbackMarker = playbackFrac * timelineSize;

	surface->DrawColoredLine(x + 9 + playbackMarker - 1, y + 6, x + 9 + playbackMarker + 1, y + 6, white);
	surface->DrawColoredLine(x + 9 + playbackMarker - 2, y + 5, x + 9 + playbackMarker + 2, y + 5, white);
	surface->DrawColoredLine(x + 9 + playbackMarker - 3, y + 4, x + 9 + playbackMarker + 3, y + 4, white);
	surface->DrawColoredLine(x + 9 + playbackMarker - 4, y + 3, x + 9 + playbackMarker + 4, y + 3, white);
	surface->DrawColoredLine(x + 9 + playbackMarker - 5, y + 2, x + 9 + playbackMarker + 5, y + 2, white);
	surface->DrawColoredLine(x + 9 + playbackMarker - 6, y + 1, x + 9 + playbackMarker + 6, y + 1, white);
	surface->DrawColoredLine(x + 9 + playbackMarker - 7, y, x + 9 + playbackMarker + 7, y, white);

	DRAW_CENTERED_TEXT(font, x + 9 + playbackMarker, y - 24, white, "%d", tick);

	int totalSegTicks = 0;
	for (size_t i = 0; i < demoViewer->g_demoSpeedrunTime.nSplits; ++i) {
		auto split = demoViewer->g_demoSpeedrunTime.splits[i];

		for (size_t j = 0; j < split.nSegments; ++j) {
			totalSegTicks += split.segments[j].ticks;
			
			int speedrunLength = demoViewer->g_demoSpeedrunLength;
			int segTick = totalSegTicks;
			float segFrac = (float)segTick / (float)speedrunLength;
			int segMarker = segFrac * timelineSize;

			surface->DrawRect({255, 255, 255}, x + 9 + segMarker, y + 9, x + 10 + segMarker, y + 25);
		}
	}

	for (auto const &state : camera->states) {
		int keyframeTick = state.first;
		float keyframeFrac = (float)keyframeTick / (float)playbackTicks;

		// dont draw keyframes that are out of the window
		if (keyframeFrac > 1.0f)
			continue;
			
		int keyframeMarker = keyframeFrac * timelineSize;

		surface->DrawRect({255, 255, 0}, x + 9 + keyframeMarker, y + 9, x + 10 + keyframeMarker, y + 25);
	}

	y += 30;

	surface->DrawRect({20, 20, 20}, x + 8, y, x + 168, y + 30);
	surface->DrawRect({36, 36, 36}, x + 10, y + 2, x + 166, y + 28);

	auto controlType = camera->controlType;
	const char *controlStr = "";
	switch (controlType) {
	case Default: controlStr = "DEFAULT"; break;
	case Drive: controlStr = "DRIVE"; break;
	case Cinematic: controlStr = "CINEMATIC"; break;
	case Follow: controlStr = "FOLLOW"; break;
	}
	DRAW_CENTERED_TEXT(font, x + 88, y + 2, white, "%s", controlStr);

	surface->DrawRect({20, 20, 20}, x + 172, y, x + 453, y + 30);
	surface->DrawRect({36, 36, 36}, x + 174, y + 2, x + 451, y + 28);

	surface->DrawTxt(iconFont, x + 202, y - 19, Input::keys[VK_LEFT].IsHeld() ? accent : white, "Y");

	float host_timescale = Variable("host_timescale").GetFloat();
	if (host_timescale > 1.0f) {
		surface->DrawTxt(iconFont, x + 244, y - 19, Input::keys[VK_DOWN].IsHeld() ? accent : white, "W");
		surface->DrawTxt(font, x + 347, y + 2, Input::keys[VK_UP].IsHeld() ? accent : white, "%.1fx", host_timescale);
	} else {
		surface->DrawTxt(font, x + 243, y + 2, Input::keys[VK_DOWN].IsHeld() ? accent : white, "%.1fx", host_timescale);
		surface->DrawTxt(iconFont, x + 355, y - 19, Input::keys[VK_UP].IsHeld() ? accent : white, "S");
	}

	if (engine->demoplayer->IsPaused())
		surface->DrawTxt(iconFont, x + 307, y - 19, Input::keys[VK_SPACE].IsHeld() ? accent : white, "R");
	else
		surface->DrawTxt(iconFont, x + 306, y - 19, Input::keys[VK_SPACE].IsHeld() ? accent : white, "P");

	surface->DrawTxt(iconFont, x + 410, y - 19, Input::keys[VK_RIGHT].IsHeld() ? accent : white, "X");

	surface->DrawRect({20, 20, 20}, x + 457, y, x + 560, y + 30);
	surface->DrawRect({36, 36, 36}, x + 459, y + 2, x + 558, y + 28);
	DRAW_CENTERED_TEXT(font, x + 508, y + 2, Renderer::g_render.isRendering ? accent : white, "RECORD");

	y += 40;

	DRAW_CENTERED_TEXT(font, x + 88, y, accent, "F3");
	DRAW_CENTERED_TEXT(font, x + 207, y - 2, accent, "<");
	DRAW_CENTERED_TEXT(font, x + 260, y - 2, accent, "v");
	DRAW_CENTERED_TEXT(font, x + 311, y, accent, "SPACE");
	DRAW_CENTERED_TEXT(font, x + 364, y + 3, accent, "^");
	DRAW_CENTERED_TEXT(font, x + 416, y - 2, accent, ">");
	DRAW_CENTERED_TEXT(font, x + 508, y, accent, "R");
}

bool DemoHud::GetCurrentSize(int& xSize, int& ySize) {
	return false;
}