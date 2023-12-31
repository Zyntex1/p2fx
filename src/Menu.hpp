#pragma once

#include "Modules/Engine.hpp"
#include "Variable.hpp"

#include <cstdint>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <map>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "../lib/imgui/imgui_internal.h"

namespace Menu {
	inline bool g_shouldDraw = false;

	void Draw();
	void Init();
};  // namespace Menu

namespace Fonts {
	inline ImFont *medium;
	inline ImFont *semibold;

	inline ImFont *logo;
}  // namespace fonts

#define ALPHA (ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoBorder)
#define NO_ALPHA (ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoBorder)

namespace CImGui {
	inline bool Tab(const char *name, bool boolean) {
		ImGuiWindow *window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
			return false;

		ImGuiContext &g = *GImGui;
		const ImGuiStyle &style = g.Style;
		const ImGuiID id = window->GetID(name);
		const ImVec2 label_size = ImGui::CalcTextSize(name, NULL, true);
		ImVec2 pos = window->DC.CursorPos;

		const ImRect rect(pos, ImVec2(pos.x + label_size.x, pos.y + label_size.y));
		ImGui::ItemSize(ImVec4(rect.Min.x, rect.Min.y, rect.Max.x + 10, rect.Max.y), style.FramePadding.y);
		if (!ImGui::ItemAdd(rect, id))
			return false;

		bool hovered, held;
		bool pressed = ImGui::ButtonBehavior(rect, id, &hovered, &held, NULL);

		static std::map<ImGuiID, float> anim;
		auto it_anim = anim.find(id);
		if (it_anim == anim.end()) {
			anim.insert({id, 0.0f});
			it_anim = anim.find(id);
		}

		it_anim->second = ImLerp(it_anim->second, (boolean ? 0.8f : hovered ? 0.6f : 0.4f), 0.07f * (1.0f - ImGui::GetIO().DeltaTime));

		window->DrawList->AddText(rect.Min, ImColor(1.0f, 1.0f, 1.0f, it_anim->second), name);

		return pressed;
	}

	inline void Checkbox(const char *label, const char *var, int onvalue = 1) {
		bool val = Variable(var).GetInt() == onvalue ? true : false;
		ImGui::Checkbox(label, &val);
		Variable(var).SetValue(val ? onvalue : 0);
	}

	inline void CheckboxInv(const char *label, const char *var, int onvalue = 1) {
		bool val = Variable(var).GetInt() == onvalue ? false : true;
		ImGui::Checkbox(label, &val);
		Variable(var).SetValue(val ? 0 : onvalue);
	}

	inline void Slider(const char *label, const char *var, int min, int max, const char *format = "%d") {
		int val = std::clamp(Variable(var).GetInt(), min, max);
		ImGui::SliderInt(label, &val, min, max, format);
		Variable(var).SetValue(val);
	}

	inline void Slider2(const char *label, const char *var, const char *cmd, int min, int max, const char *format = "%d") {
		int val = std::clamp(Variable(var).GetInt(), min, max);
		ImGui::SliderInt(label, &val, min, max, format);
		engine->ExecuteCommand(Utils::ssprintf("%s %d", cmd, val).c_str());
	}

	inline void Sliderf(const char *label, const char *var, float min, float max, const char *format = "%.2f") {
		float val = std::clamp(Variable(var).GetFloat(), min, max);
		ImGui::SliderFloat(label, &val, min, max, format);
		Variable(var).SetValue(val);
	}

	inline void Colorpicker(const char *label, const char *var) {
		auto val = Utils::GetColor(Variable(var).GetString());
		float col[3] = {val->r / 255.0f, val->g / 255.0f, val->b / 255.0f};
		ImGui::SameLine(ImGui::GetWindowWidth() - 16);
		ImGui::ColorEdit4(label, col, NO_ALPHA);
		Variable(var).SetValue(Utils::ssprintf("%.0f %.0f %.0f", col[0] * 255.0f, col[1] * 255.0f, col[2] * 255.0f).c_str());
	}

	inline void Colorpickerf2(const char *label, const char *var1, const char *var2, const char *var3) {
		auto r = Variable(var1).GetFloat();
		auto g = Variable(var2).GetFloat();
		auto b = Variable(var3).GetFloat();
		float col[3] = {r, g, b};
		ImGui::SameLine(ImGui::GetWindowWidth() - 16);
		ImGui::ColorEdit4(label, col, NO_ALPHA);
		Variable(var1).SetValue(col[0]);
		Variable(var2).SetValue(col[1]);
		Variable(var3).SetValue(col[2]);
	}

	inline void Combo(const char *label, const char *var, const char *items) {
		int val = Variable(var).GetInt();
		ImGui::Combo(label, &val, items);
		Variable(var).SetValue(val);
	}

	inline void Combo2(const char *label, const char *var, std::vector<const char *> items) {
		auto val = Variable(var).GetString();
		if (ImGui::BeginCombo(label, val)) {
			for (auto &item : items) {
				const bool isSelected = val == item;

				if (ImGui::Selectable(item, isSelected))
					val = item;

				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		Variable(var).SetValue(val);
	}

	inline void Button(const char *label, const char *cmd, const ImVec2 &size = ImVec2(0, 0)) {
		if (ImGui::Button(label, size)) {
			engine->ExecuteCommand(cmd);
		}
	}
}  // namespace CImGui