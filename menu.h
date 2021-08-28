#define g_pLevels 0x30
#define g_pAActors 0x0
#define g_pTeamIndex 0x338

namespace Sus
{
	void Menu()
	{
		if (GetAsyncKeyState(VK_INSERT) & 1)
		{
			Settings::Menu = !Settings::Menu;
		}

		ImGui::StyleColorsLight();
		if (Settings::Menu)
		{
			ImGuiStyle* style = &ImGui::GetStyle();
			ImVec4* colors = style->Colors;

			colors[ImGuiCol_WindowBg] = ImColor(60, 60, 60, 255);
			colors[ImGuiCol_FrameBg] = ImColor(60, 60, 60, 0);

			PS::New_Menu_Frame("FloydWare.cc", 500, 500);
			PS::Checkbox("Enable Features", &Settings::Features_Enabled, 10, 50);
			if (Settings::Features_Enabled)
			{
				PS::Checkbox("Memory Aimbot", &Settings::Aimbot, 10, 80);
				PS::Checkbox("Aimbot FOV", &Settings::AimbotFOV, 10, 110);
				PS::Checkbox("Allow Smoothing", &Settings::Allow_Smoothing, 10, 140);
				PS::Checkbox("Crosshair", &Settings::Crosshair, 10, 170);

				PS::Checkbox("Box ESP", &Settings::BoxESP, 200, 50);
				PS::Checkbox("Corner ESP", &Settings::CornerESP, 200, 80);
				PS::Checkbox("Name ESP", &Settings::NameESP, 200, 110);
				PS::Checkbox("Distance ESP", &Settings::DistanceESP, 200, 140);
				PS::Checkbox("Show Skeleton", &Settings::Skeletons, 200, 170);

				PS::Checkbox("Rapid Fire", &Settings::RapidFire, 380, 50);
				PS::Checkbox("No Spread", &Settings::NoSpread, 380, 80);
				PS::Checkbox("FOV Slider", &Settings::FOV_Slider, 380, 110);

				PS::Slider("Smoothing Amount", &Settings::Smoothing, 10, 300, 0, 15, "Smoothness");
				PS::Slider("Aimbot Radius", &Settings::AimbotRadius, 10, 340, 0, 1100, "Aimbot FOV Radius");
				PS::Slider("Fov Slider Sus", &Settings::FOV_Amount, 10, 380, 0, 160, "FOV Slider Amount");
			}

			ImGui::End();
		}
	}
}