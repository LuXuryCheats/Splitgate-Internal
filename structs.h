#define rva(addr, size) ((uintptr_t)((UINT_PTR)(addr) + *(PINT)((UINT_PTR)(addr) + ((size) - sizeof(INT))) + (size)))

#include <algorithm>
#include <search.h>
#include "split_sdk/SDK.h"

namespace Scanners
{
	uintptr_t PatternScan(uintptr_t pModuleBaseAddress, const char* sSignature, size_t nSelectResultIndex = 0);
	uintptr_t PatternScan(const char* sSignature, size_t nSelectResultIndex = 0);
}

uintptr_t Scanners::PatternScan(uintptr_t pModuleBaseAddress, const char* sSignature, size_t nSelectResultIndex)
{
	static auto patternToByte = [](const char* pattern)
	{
		auto       bytes = std::vector<int>{};
		const auto start = const_cast<char*>(pattern);
		const auto end = const_cast<char*>(pattern) + strlen(pattern);

		for (auto current = start; current < end; ++current)
		{
			if (*current == '?')
			{
				++current;
				if (*current == '?')
					++current;
				bytes.push_back(-1);
			}
			else {
				bytes.push_back(strtoul(current, &current, 16));
			}
		}
		return bytes;
	};

	const auto dosHeader = (PIMAGE_DOS_HEADER)pModuleBaseAddress;
	const auto ntHeaders = (PIMAGE_NT_HEADERS)((std::uint8_t*)pModuleBaseAddress + dosHeader->e_lfanew);

	const auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
	auto       patternBytes = patternToByte(sSignature);
	const auto scanBytes = reinterpret_cast<std::uint8_t*>(pModuleBaseAddress);

	const auto s = patternBytes.size();
	const auto d = patternBytes.data();

	size_t nFoundResults = 0;

	for (auto i = 0ul; i < sizeOfImage - s; ++i)
	{
		bool found = true;

		for (auto j = 0ul; j < s; ++j)
		{
			if (scanBytes[i + j] != d[j] && d[j] != -1)
			{
				found = false;
				break;
			}
		}

		if (found)
		{
			if (nSelectResultIndex != 0)
			{
				if (nFoundResults < nSelectResultIndex)
				{
					nFoundResults++;
					found = false;
				}
				else
				{
					return reinterpret_cast<uintptr_t>(&scanBytes[i]);
				}
			}
			else
			{
				return reinterpret_cast<uintptr_t>(&scanBytes[i]);
			}
		}
	}

	return NULL;
}

uintptr_t Scanners::PatternScan(const char* sSignature, size_t nSelectResultIndex)
{
	static bool bIsSetted = false;

	static MODULEINFO info = { 0 };

	if (!bIsSetted)
	{
		GetModuleInformation(GetCurrentProcess(), GetModuleHandle(0), &info, sizeof(info));
		bIsSetted = true;
	}

	return PatternScan((uintptr_t)info.lpBaseOfDll, sSignature, nSelectResultIndex);
}


namespace SDK
{
	uintptr_t entity_x;
	CG::UWorld* GWorld;
	CG::ULocalPlayer* LocalPlayer;
	CG::ACharacter* Characters;
	CG::AController* Controller;
	CG::AActor* Actor;
	CG::APlayerController* LocalController;
	CG::UPlayer* Player;
	CG::APortalWarsCharacter* ACharacter;
	CG::USkeletalMeshComponent* USkeletal = Characters->Mesh;
	CG::UKismetMathLibrary* UMath;
	CG::APortalWarsPlayerController* PlayerController;

	CG::FVector ViewPoint = { 0, 0, 0 };
	CG::TArray<CG::AActor*> Actors;
	CG::TArray<CG::UObject*>* ActorArray = 0;

	uintptr_t uworld_sig;
	uintptr_t getobjname;
	uintptr_t getgonematrix;
	uintptr_t is_dead;
	uintptr_t gnames;
	uintptr_t gobjs;
}

enum e_bones
{
	root = 0,
	pelvis = 1,
	spine_01 = 2,
	spine_02 = 3,
	spine_03 = 4,
	clavicle_l = 5,
	upperarm_l = 6,
	lowerarm_l = 7,
	hand_l = 8,
	index_01_l = 9,
	index_02_l = 10,
	index_03_l = 11,
	middle_01_l = 12,
	middle_02_l = 13,
	middle_03_l = 14,
	pinky_01_l = 15,
	pinky_02_l = 16,
	pinky_03_l = 17,
	ring_01_l = 18,
	ring_02_l = 19,
	ring_03_l = 20,
	thumb_01_l = 21,
	thumb_02_l = 22,
	thumb_03_l = 23,
	lowerarm_twist_01_l = 24,
	upperarm_twist_01_l = 25,
	clavicle_r = 26,
	upperarm_r = 27,
	lowerarm_r = 28,
	hand_r = 29,
	index_01_r = 30,
	index_02_r = 31,
	index_03_r = 32,
	middle_01_r = 33,
	middle_02_r = 34,
	middle_03_r = 35,
	pinky_01_r = 36,
	pinky_02_r = 37,
	pinky_03_r = 38,
	ring_01_r = 39,
	ring_02_r = 40,
	ring_03_r = 41,
	thumb_01_r = 42,
	thumb_02_r = 43,
	thumb_03_r = 44,
	lowerarm_twist_01_r = 45,
	upperarm_twist_01_r = 46,
	neck_01 = 47,
	head = 48,
	thigh_l = 49,
	calf_l = 50,
	calf_twist_01_l = 51,
	foot_l = 52,
	ball_l = 53,
	thigh_twist_01_l = 54,
	thigh_r = 55,
	calf_r = 56,
	calf_twist_01_r = 57,
	foot_r = 58,
	ball_r = 59,
	thigh_twist_01_r = 60,
	ik_foot_root = 61,
	ik_foot_l = 62,
	ik_foot_r = 63,
	ik_hand_root = 64,
	ik_hand_gun = 65,
	ik_hand_l = 66,
	ik_hand_r = 67,
	vb_knee_target_l = 68,
	vb_knee_target_r = 69,
	vb_rhs_ik_hand_gun = 70,
};


struct Colors
{
	ImColor red = { 255, 0, 0, 255 };
	ImColor green = { 0, 255, 0, 255 };
	ImColor blue = { 0, 136, 255, 255 };
	ImColor aqua_blue = { 0, 255, 255, 255 };
	ImColor cyan = { 0, 210, 210, 255 };
	ImColor royal_purple = { 102, 0, 255, 255 };
	ImColor dark_pink = { 255, 0, 174, 255 };
	ImColor black = { 0, 0, 0, 255 };
	ImColor white = { 255, 255, 255, 255 };
	ImColor purple = { 255, 0, 255, 255 };
	ImColor yellow = { 255, 255, 0, 255 };
	ImColor orange = { 255, 140, 0, 255 };
	ImColor gold = { 234, 255, 0, 255 };
	ImColor royal_blue = { 0, 30, 255, 255 };
	ImColor dark_red = { 150, 5, 5, 255 };
	ImColor dark_green = { 5, 150, 5, 255 };
	ImColor dark_blue = { 100, 100, 255, 255 };
	ImColor navy_blue = { 0, 73, 168, 255 };
	ImColor light_gray = { 200, 200, 200, 255 };
	ImColor dark_gray = { 150, 150, 150, 255 };
};
Colors color;


namespace Render
{
	struct color_keys
	{
		ImVec4 value;
		color_keys() { value.x = value.y = value.z = value.y = 0.0f; }
		color_keys(int r, int g, int b, int a = 255) { float sc = 1.0f / 255.0f; value.x = (float)r * sc; value.y = (float)g * sc; value.z = (float)b * sc; value.w = (float)a * sc; }
		color_keys(float r, float g, float b, float a = 1.0f) { value.x = r; value.y = g; value.z = b; value.w = a; }
		color_keys(const ImVec4& col) { value = col; }
	};
	color_keys* col_keys;

	struct vec_2
	{
		int x, y;
	};

	struct Colors
	{
		ImColor red = { 255, 0, 0, 255 };
		ImColor green = { 0, 255, 0, 255 };
		ImColor blue = { 0, 136, 255, 255 };
		ImColor aqua_blue = { 0, 255, 255, 255 };
		ImColor cyan = { 0, 210, 210, 255 };
		ImColor royal_purple = { 102, 0, 255, 255 };
		ImColor dark_pink = { 255, 0, 174, 255 };
		ImColor black = { 0, 0, 0, 255 };
		ImColor white = { 255, 255, 255, 255 };
		ImColor purple = { 255, 0, 255, 255 };
		ImColor yellow = { 255, 255, 0, 255 };
		ImColor orange = { 255, 140, 0, 255 };
		ImColor gold = { 234, 255, 0, 255 };
		ImColor royal_blue = { 0, 30, 255, 255 };
		ImColor dark_red = { 150, 5, 5, 255 };
		ImColor dark_green = { 5, 150, 5, 255 };
		ImColor dark_blue = { 100, 100, 255, 255 };
		ImColor navy_blue = { 0, 73, 168, 255 };
		ImColor light_gray = { 200, 200, 200, 255 };
		ImColor dark_gray = { 150, 150, 150, 255 };
	};
	Colors color;

	void Text(int posx, int posy, ImColor clr, const char* text)
	{
		ImGui::GetOverlayDrawList()->AddText(ImVec2(posx, posy), ImColor(clr), text);
	}

	void OutlinedText(int posx, int posy, ImColor clr, const char* text)
	{
		ImGui::GetOverlayDrawList()->AddText(ImVec2(posx + 1, posy + 1), ImColor(color.black), text);
		ImGui::GetOverlayDrawList()->AddText(ImVec2(posx - 1, posy - 1), ImColor(color.black), text);
		ImGui::GetOverlayDrawList()->AddText(ImVec2(posx + 1, posy + 1), ImColor(color.black), text);
		ImGui::GetOverlayDrawList()->AddText(ImVec2(posx - 1, posy - 1), ImColor(color.black), text);
		ImGui::GetOverlayDrawList()->AddText(ImVec2(posx, posy), ImColor(clr), text);
	}

	void ShadowText(int posx, int posy, ImColor clr, const char* text)
	{
		ImGui::GetOverlayDrawList()->AddText(ImVec2(posx + 1, posy + 2), ImColor(0, 0, 0, 200), text);
		ImGui::GetOverlayDrawList()->AddText(ImVec2(posx + 1, posy + 2), ImColor(0, 0, 0, 200), text);
		ImGui::GetOverlayDrawList()->AddText(ImVec2(posx, posy), ImColor(clr), text);
	}

	void Rect(int x, int y, int w, int h, ImColor color, int thickness)
	{
		ImGui::GetOverlayDrawList()->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), ImGui::ColorConvertFloat4ToU32(ImVec4(color)), 0, 0, thickness);
	}

	void RectFilledGradient(int x, int y, int w, int h, ImColor color)
	{
		ImGui::GetOverlayDrawList()->AddRectFilledMultiColor(ImVec2(x, y), ImVec2(x + w, y + h), ImGui::ColorConvertFloat4ToU32(ImVec4(color)), ImGui::ColorConvertFloat4ToU32(ImVec4(color)), 0, 0);
	}

	void RectFilled(int x, int y, int w, int h, ImColor color)
	{
		ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), ImGui::ColorConvertFloat4ToU32(ImVec4(color)), 0, 0);
	}
}

static int tabs;

namespace PS
{
	void Text(int posx, int posy, ImColor clr, const char* text)
	{
		ImGui::GetOverlayDrawList()->AddText(ImVec2(posx, posy), ImColor(clr), text);
	}

	void TextOutlined(int posx, int posy, ImColor clr, const char* text)
	{
		ImGui::GetOverlayDrawList()->AddText(ImVec2(posx + 1, posy + 1), ImColor(color.black), text);
		ImGui::GetOverlayDrawList()->AddText(ImVec2(posx - 1, posy - 1), ImColor(color.black), text);
		ImGui::GetOverlayDrawList()->AddText(ImVec2(posx + 1, posy + 1), ImColor(color.black), text);
		ImGui::GetOverlayDrawList()->AddText(ImVec2(posx - 1, posy - 1), ImColor(color.black), text);
		ImGui::GetOverlayDrawList()->AddText(ImVec2(posx, posy), ImColor(clr), text);
	}

	void Rect(int posx, int posy, int width, int height, ImColor clr, int thickness)
	{
		ImGui::GetOverlayDrawList()->AddRect(ImVec2(posx, posy), ImVec2(width, height), ImColor(clr), 0, 0, thickness);
	}

	void RectFilled(int posx, int posy, int width, int height, ImColor clr)
	{
		ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(posx, posy), ImVec2(width, height), ImColor(clr));
	}

	void Line(int x1, int y1, int x2, int y2, ImColor clr, int thickness)
	{
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), ImColor(clr), thickness);
	}

	void Circle(int x, int y, int radius, ImColor clr, int segments)
	{
		ImGui::GetOverlayDrawList()->AddCircle(ImVec2(x, y), radius, ImColor(clr), segments);
	}

	void CreateMenu(const char* title)
	{
		ImGui::Begin(title);
	}

	ImVec2 p;
	void Checkbox(const char* v, bool* option, float x, float y, const char* title)
	{
		ImGui::SetCursorPos({ x, y });
		ImGui::Checkbox(v, option);

		p = ImGui::GetWindowPos();

		ImGui::SetCursorPos({ x + 2, y - 20 });
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(0, 0, 0)));
		ImGui::Text(title);
		ImGui::PopStyleColor();

		RectFilled(p.x + x + 3, p.y + y + 3, 16, 16, ImColor(200, 200, 200));

		if (*option)
		{
			RectFilled(p.x + x + 7, p.y + y + 7, 8, 8, ImColor(0, 0, 0, 255));
		}
	}

	void Slider(const char* v, float* option, float x, float y, float min, float max, const char* title)
	{
		ImGui::SetCursorPos({ x, y });
		ImGui::SliderFloat(v, option, min, max);

		ImGui::SetCursorPos({ x + 2, y - 20 });
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(245, 245, 245)));
		ImGui::Text(title);
		ImGui::PopStyleColor();
	}

	void Tab(const char* v, float size_x, float size_y, static int tab_name, int tab_next)
	{
		if (ImGui::Button(v, ImVec2{ size_x, size_y })) tab_name = tab_next;
	}

	void CL_ToggleButton(const char* v, bool* option, float x, float y, float x2)
	{
		ImGui::SetCursorPos({ x + x2, y });

		ImVec2 pos = ImGui::GetWindowPos();
		float height = 20;
		float width = 40;

		if (ImGui::Button(v, ImVec2{ width, height }))
			*option = !*option;

		Render::Rect(pos.x + x + x2, pos.y + y, 40, 20, ImColor(50, 50, 50), 2);
		Render::RectFilled(pos.x + x + x2, pos.y + y, 40, 20, ImColor(100, 100, 100, 255));
		Render::ShadowText(pos.x + x - 5, pos.y + y, ImColor(255, 187, 0, 200), v);

		if (*option)
		{
			Render::RectFilled(pos.x + x + x2, pos.y + y, 20, 20, ImColor(0, 200, 0, 255));
		}
		else
		{
			Render::RectFilled(pos.x + x + 20 + x2, pos.y + y, 20, 20, ImColor(200, 0, 0, 255));
		}
	}

	void CL_Tab(const char* v, int tab_next, float x, float y)
	{
		ImGui::SetCursorPos({ x, y });
		if (ImGui::Button(v, ImVec2(120, 35))) tabs = tab_next;

		ImVec2 pos = ImGui::GetWindowPos();
		Render::RectFilled(pos.x + x, pos.y + y, 120, 35, ImColor(1, 133, 182, 255));
		Render::Rect(pos.x + x, pos.y + y, 120, 35, ImColor(0, 67, 91, 255), 2);
		Render::ShadowText(pos.x + x + 33, pos.y + y + 6, ImColor(255, 187, 0, 200), v);
	}

	void CL_Slider(const char* v, float option, float x, float y)
	{

	}

	///////////////////////////////////////

	void Checkbox(const char* v, bool* option, float x, float y)
	{
		ImGui::SetCursorPos({ x, y });
		ImGui::Checkbox(v, option);

		p = ImGui::GetWindowPos();

		Render::RectFilled(p.x + x + 3, p.y + y + 3, 16, 16, ImColor(200, 200, 200));
		Render::Rect(p.x + x + 3, p.y + y + 3, 16, 16, ImColor(0, 0, 0, 255), 1);
		Render::Text(p.x + x + 28, p.y + y + 2, ImColor(200, 200, 200, 255), v);

		if (*option)
		{
			Render::RectFilled(p.x + x + 7, p.y + y + 7, 8, 8, ImColor(70, 70, 70, 255));
			Render::Rect(p.x + x + 7, p.y + y + 7, 8, 8, ImColor(0, 0, 0, 255), 2);
		}
	}

	void AC_Tab(const char* v, int tab_next, float x, float y)
	{
		ImGui::SetCursorPos({ x, y });
		if (ImGui::Button(v, ImVec2(140, 30))) tabs = tab_next;

		ImVec2 pos = ImGui::GetWindowPos();
		Render::RectFilled(pos.x + x, pos.y + y, 140, 30, ImColor(234, 234, 234, 255));
		Render::Rect(pos.x + x, pos.y + y, 140, 30, ImColor(175, 175, 175, 255), 1);
		Render::Text(pos.x + x + 48, pos.y + y + 6, ImColor(0, 0, 0, 255), v);
	}

	void Tab(const char* v, int size_x, int size_y, float x, float y, int tab_number)
	{
		ImGui::SetCursorPos({ x, y });
		if (ImGui::Button(v, ImVec2(size_x, size_y))) tabs = tab_number;

		ImVec2 p = ImGui::GetWindowPos();

		ImColor tab_color;
		if (ImGui::IsItemHovered())
			Render::RectFilled(p.x + x, p.y + y, size_x, size_y, ImColor(40, 40, 40, 255));
		else
			Render::RectFilled(p.x + x, p.y + y, size_x, size_y, ImColor(30, 30, 30, 255));

		Render::Rect(p.x + x, p.y + y, size_x, size_y, ImColor(0, 0, 0, 255), 1);
		Render::Text(p.x + x, p.y + y, ImColor(200, 200, 200, 200), v);
	}

	void New_Menu_Frame(const char* title, float size_x, float size_y)
	{
		ImGuiStyle* style = &ImGui::GetStyle();
		style->WindowPadding = ImVec2(15, 15);
		style->WindowRounding = 0.0f;
		style->FramePadding = ImVec2(2, 2);
		style->FrameRounding = 0.0f;
		style->ItemSpacing = ImVec2(8, 8);
		style->ItemInnerSpacing = ImVec2(8, 6);
		style->IndentSpacing = 25.0f;
		style->ScrollbarSize = 15.0f;
		style->ScrollbarRounding = 0.0f;
		style->GrabMinSize = 10.0f;
		style->GrabRounding = 0.0f;
		style->ChildRounding = 0.f;

		style->WindowTitleAlign.x = 0.50f;
		style->FrameRounding = 2.0f;

		ImGui::SetNextWindowSize({ size_x, size_y });
		ImGui::Begin(title, 0, ImGuiWindowFlags_::ImGuiWindowFlags_NoResize | ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);

		ImVec2 p = ImGui::GetWindowPos();

		//Render::RectFilled(p.x, p.y, size_x, size_y, ImColor(60, 60, 60, 255));
		Render::RectFilled(p.x, p.y, size_x, 30, ImColor(45, 45, 45, 255));
		Render::Rect(p.x, p.y, size_x, size_y, ImColor(0, 0, 0, 255), 1);
		Render::Text(p.x + size_x - 185 + (size_x / size_y - 100), p.y + 5, ImColor(200, 200, 200, 200), title);
	}

	const char* ctx;
}

namespace detail
{
	extern "C" void* _spoofer_stub();

	template <typename Ret, typename... Args>
	static inline auto shellcode_stub_helper(
		const void* shell,
		Args... args
	) -> Ret
	{
		auto fn = (Ret(*)(Args...))(shell);
		return fn(args...);
	}

	template <std::size_t Argc, typename>
	struct argument_remapper
	{
		template<
			typename Ret,
			typename First,
			typename Second,
			typename Third,
			typename Fourth,
			typename... Pack
		>
			static auto do_call(const void* shell, void* shell_param, First first, Second second,
				Third third, Fourth fourth, Pack... pack) -> Ret
		{
			return shellcode_stub_helper< Ret, First, Second, Third, Fourth, void*, void*, Pack... >(shell, first, second, third, fourth, shell_param, nullptr, pack...);
		}
	};

	template <std::size_t Argc>
	struct argument_remapper<Argc, std::enable_if_t<Argc <= 4>>
	{
		template<
			typename Ret,
			typename First = void*,
			typename Second = void*,
			typename Third = void*,
			typename Fourth = void*
		>
			static auto do_call(
				const void* shell,
				void* shell_param,
				First first = First{},
				Second second = Second{},
				Third third = Third{},
				Fourth fourth = Fourth{}
			) -> Ret
		{
			return shellcode_stub_helper<
				Ret,
				First,
				Second,
				Third,
				Fourth,
				void*,
				void*
			>(
				shell,
				first,
				second,
				third,
				fourth,
				shell_param,
				nullptr
				);
		}
	};
}

namespace Spoof {

	namespace nig
	{
		uintptr_t GetImageBase() {
			return (uintptr_t)GetModuleHandleA(0);
		}

		template <typename Ret, typename... Args>
		static inline auto SpoofCall(Ret(*fn)(Args...), Args... args) -> Ret
		{
			static const void* jmprbx = nullptr;
			if (!jmprbx) {
				const auto ntdll = reinterpret_cast<const unsigned char*>(::GetModuleHandleW(NULL));
				const auto dos = reinterpret_cast<const IMAGE_DOS_HEADER*>(ntdll);
				const auto nt = reinterpret_cast<const IMAGE_NT_HEADERS*>(ntdll + dos->e_lfanew);
				const auto sections = IMAGE_FIRST_SECTION(nt);
				const auto num_sections = nt->FileHeader.NumberOfSections;

				constexpr char section_name[5]{ '.', 't', 'e', 'x', 't' };
				const auto     section = std::find_if(sections, sections + num_sections, [&](const auto& s) {
					return std::equal(s.Name, s.Name + 5, section_name);
					});

				constexpr unsigned char instr_bytes[2]{ 0xFF, 0x26 };
				const auto              va = ntdll + section->VirtualAddress;
				jmprbx = std::search(va, va + section->Misc.VirtualSize, instr_bytes, instr_bytes + 2);
			}

			struct shell_params
			{
				const void* trampoline;
				void* function;
				void* rdx;
			};

			shell_params p
			{
				jmprbx,
				reinterpret_cast<void*>(fn)
			};

			using mapper = detail::argument_remapper<sizeof...(Args), void>;
			return mapper::template do_call<Ret, Args...>((const void*)&detail::_spoofer_stub, &p, args...);
		}
	}
}

namespace values
{

	BOOL valid_pointer(DWORD64 address)
	{
		if (!IsBadWritePtr((LPVOID)address, (UINT_PTR)8)) return TRUE;
		else return FALSE;
	}

	template<typename T>
	T read(DWORD_PTR address, const T& def = T())
	{
		if (valid_pointer(address))
			return *(T*)address;
	}

	template<typename T>
	T write(DWORD_PTR address, DWORD_PTR value, const T& def = T())
	{
		if (valid_pointer(address))
			return *(T*)address = value;
		else
			return T();
	}
}

namespace Functions
{
	void FreeMemory(uintptr_t Ptr, int Length = 8)
	{
		Spoof::nig::SpoofCall(VirtualFree, (LPVOID)Ptr, (SIZE_T)0, (DWORD)MEM_RELEASE);
	}

	template <typename T>
	std::string GetObjectName(T Object, bool GetOuter = false)
	{
		static uintptr_t GetObjectName;

		if (!GetObjectName)
			GetObjectName = Scanners::PatternScan(("40 53 48 83 EC 20 48 8B D9 48 85 D2 75 45 33 C0 48 89 01 48 89 41 08 8D 50 05 E8 ? ? ? ? 8B 53 08 8D 42 05 89 43 08 3B 43 0C 7E 08 48 8B CB E8 ? ? ? ? 48 8B 0B 48 8D 15 ? ? ? ? 41 B8 ? ? ? ? E8 ? ? ? ? 48 8B C3 48 83 C4 20 5B C3 48 8B 42 18"));

		auto UFUNGetObjectNameByIndex = reinterpret_cast<CG::FString * (__fastcall*)(DWORD*, CG::FString*)>(GetObjectName);

		DWORD ObjectIndex = *(DWORD*)((PBYTE)Object + 0x18);
		if (!ObjectIndex || ObjectIndex >= MAXDWORD) return ("");

		if (!GetOuter)
		{
			CG::FString A;
			std::string B = "";
			Spoof::nig::SpoofCall(UFUNGetObjectNameByIndex, &ObjectIndex, &A);
			B = A.ToString();
			return B;
		}

		std::string Ret;

		CG::UObject* Object_ = (CG::UObject*)Object;
		for (int i = 0; Object_; Object_ = Object_->Outer, i++)
		{
			CG::FString Result;

			Spoof::nig::SpoofCall(UFUNGetObjectNameByIndex, &ObjectIndex, &Result);

			std::string tmp = Result.ToString();

			if (tmp.c_str() == 0) return ("");

			FreeMemory((__int64)Result.c_str(), tmp.size() + 8);

			char return_string[1024] = {};
			for (size_t i = 0; i < tmp.size(); i++)
			{
				return_string[i] += tmp[i];
			}

			Ret = return_string + std::string(i > 0 ? "." : "") + Ret;
		}

		return Ret;
	}

	class Vector3
	{
	public:
		Vector3() : x(0.f), y(0.f), z(0.f)
		{

		}

		Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z)
		{

		}
		~Vector3()
		{

		}

		float x;
		float y;
		float z;

		float Dot(Vector3 v)
		{
			return x * v.x + y * v.y + z * v.z;
		}

		float Distance(Vector3 v)
		{
			auto A = powf((float)v.x - x, (float)2.0);
			auto B = powf((float)v.y - y, (float)2.0);
			auto C = powf((float)v.z - z, (float)2.0);
			return float(sqrtf(A + B + C));
		}

		Vector3 ClampAngles() {

			if (x < -180.0f)
				x += 360.0f;

			if (x > 180.0f)
				x -= 360.0f;

			if (y < -74.0f)
				y = -74.0f;

			if (y > 74.0f)
				y = 74.0f;

			return Vector3(x, y, 0);
		}

		double Length() {
			return sqrtf(x * x + y * y + z * z);
		}

		Vector3 operator+(Vector3 v)
		{
			return Vector3(x + v.x, y + v.y, z + v.z);
		}

		Vector3 operator-(Vector3 v)
		{
			return Vector3(x - v.x, y - v.y, z - v.z);
		}

		Vector3 operator*(float flNum) { return Vector3(x * flNum, y * flNum, z * flNum); }
	};

	struct FMatrix
	{
		float M[4][4];
	};
	static FMatrix* myMatrix = new FMatrix();


	/*Vector3 bone_matrix(int BoneIndex)
	{
		static auto addr = (uintptr_t)0;
		if (!addr) {
			addr = Scanners::PatternScan("E8 ? ? ? ? 48 8B 47 30 F3 0F 10 45");
			addr = (uintptr_t)rva(addr, 5);
		}

		auto GetBoneMatrix = ((CG::FMatrix * (__fastcall*)(sus * Mesh, FMatrix*, int BoneID))(addr));

		FMatrix Matrix = { 0 };

		Spoof::nig::SpoofCall(GetBoneMatrix, this, &Matrix, BoneIndex);
		return Vector3(Matrix.M[3][0], Matrix.M[3][1], Matrix.M[3][2]);
	}*/

	static Vector3 GetBoneLocation(CG::APortalWarsCharacter* CurrentActor, int id, Vector3* out)
	{
		static uintptr_t addr = 0;
		if (!addr) {
			addr = Scanners::PatternScan(("E8 ? ? ? ? 48 8B 47 30 F3 0F 10 45"));
			addr = (addr + *(DWORD*)(addr + 0x1) + 0x5 - 0x100000000);
			if (!addr) {
				Spoof::nig::SpoofCall(MessageBoxA, (HWND)0, (LPCSTR)("Something Failed!!"), (LPCSTR)0, (UINT)0);
				exit(0);
			}
		}

		uintptr_t mesh = *(uintptr_t*)(CurrentActor, SDK::Characters->Mesh);
		if (!mesh) return { 0,0,0 };

		auto fGetBoneMatrix = ((FMatrix * (__fastcall*)(uintptr_t, FMatrix*, int))(addr));
		Spoof::nig::SpoofCall(fGetBoneMatrix, mesh, myMatrix, id);

		out->x = myMatrix->M[3][0];
		out->y = myMatrix->M[3][1];
		out->z = myMatrix->M[3][2];

		return Vector3(0, 0, 0);
	}

	class World
	{
	public:
		CG::FVector2D w2s(Vector3 World)
		{
			static auto addr = (uintptr_t)0;
			if (!addr) {
				addr = Scanners::PatternScan("E8 ? ? ? ? 41 88 06 48 83 C4 30");
				addr = (uintptr_t)(rva(addr, 5));
			}
			CG::FVector2D B(0, 0);
			auto _ProjectWorldToScreen = ((bool(__fastcall*)(CG::AController * Controller, Vector3 WorldPosition, CG::FVector2D * pScreenPosition, bool bPlayerViewportRelative))(addr));
			if (_ProjectWorldToScreen, this, World, &B, false)
				return B;

			return { 0,0 };
		}
	};
	World* W;

	inline float calc_distance(CG::FVector camera_location, CG::FVector pawn)
	{
		float x = camera_location.X - pawn.X;
		float y = camera_location.Y - pawn.Y;
		float z = camera_location.Z - pawn.Z;
		float distance = Spoof::nig::SpoofCall(sqrtf, x * x + y * y + z * z) / 100.0f;
		return distance;
	}

	CG::FVector2D inline Subtract(CG::FVector2D point1, CG::FVector point2)
	{
		CG::FVector2D vector{ 0, 0 };
		vector.X = point1.X - point2.X;
		vector.Y = point1.Y - point2.Y;
		return vector;
	}

	CG::FVector inline VectorSubtract(CG::FVector Vector1, CG::FVector Vector2)
	{
		CG::FVector Vec{ 0, 0, 0 };
		Vec.X = Vector1.X - Vector2.X;
		Vec.Y = Vector1.Y - Vector2.Y;
		Vec.Z = Vector1.Z - Vector2.Z;
		return Vec;
	}

	CG::FVector inline VectorAdd(CG::FVector Vector1, CG::FVector Vector2)
	{
		CG::FVector Vec{ 0, 0, 0 };
		Vec.X = Vector1.X + Vector2.X;
		Vec.Y = Vector1.Y + Vector2.Y;
		Vec.Z = Vector1.Z + Vector2.Z;
		return Vec;
	}

	CG::FRotator inline RotatorAdd(CG::FRotator Vector1, CG::FRotator Vector2)
	{
		CG::FRotator Vec;
		Vec.Pitch = Vector1.Pitch + Vector2.Pitch;
		Vec.Yaw = Vector1.Yaw + Vector2.Yaw;
		Vec.Roll = Vector1.Roll + Vector2.Roll;
		return Vec;
	}

	CG::FRotator inline LimitRotation(CG::FRotator rStartRotation, CG::FRotator rEndRotation)
	{
		float nMaxRotation = 0.16;

		CG::FRotator rDeltaRotation;
		rDeltaRotation = SDK::UMath->STATIC_NormalizedDeltaRotator(rEndRotation, rStartRotation);

		if (rDeltaRotation.Yaw > nMaxRotation) { rDeltaRotation.Yaw = nMaxRotation; }
		else if (rDeltaRotation.Yaw < -nMaxRotation) { rDeltaRotation.Yaw = -nMaxRotation; }

		if (rDeltaRotation.Pitch > nMaxRotation) { rDeltaRotation.Pitch = nMaxRotation; }
		else if (rDeltaRotation.Pitch < -nMaxRotation) { rDeltaRotation.Pitch = -nMaxRotation; }

		return RotatorAdd(rStartRotation, rDeltaRotation);
	}

	void memory_aim(CG::APlayerCameraManager* Local_Camera, CG::APortalWarsCharacter* Target)
	{
		CG::FVector cam_loc = Local_Camera->GetCameraLocation();
		CG::FRotator cam_rot = Local_Camera->GetCameraRotation();

		CG::FVector aim_loc;
		CG::FVector my_loc = CG::FVector();

		CG::FRotator aim_rot, cur_rot;
		CG::FVector velocity = CG::FVector();
		CG::FVector difference = CG::FVector();

		float distance;
		difference = VectorSubtract(aim_loc, my_loc);
		distance = SDK::UMath->STATIC_VSize(difference);

		float time_scale = 0.0193f;
		short game_ping = SDK::LocalController->Pawn->PlayerState->Ping * 4;
		float scale_value = (float)game_ping + (float)(distance * time_scale);

		if (scale_value < 1.0f) scale_value = 1.0;
		float g_velocity;
		velocity = SDK::LocalController->GetVelocity();
		g_velocity = SDK::UMath->STATIC_VSize(velocity);

		if (g_velocity > 90.0f)
		{
			velocity.X *= scale_value / g_velocity; velocity.Y *= scale_value / g_velocity; velocity.Z *= scale_value / g_velocity;
			aim_loc = VectorAdd(aim_loc, velocity);
		}

		aim_rot = SDK::UMath->STATIC_FindLookAtRotation(cam_loc, aim_loc);

		aim_rot.Pitch = (aim_rot.Pitch - cam_rot.Pitch) / SDK::LocalController->InputPitchScale;
		aim_rot.Yaw = (aim_rot.Yaw - cam_rot.Yaw) / SDK::LocalController->InputYawScale;

		SDK::LocalController->AddPitchInput(aim_rot.Pitch);
		SDK::LocalController->AddYawInput(aim_rot.Yaw);
	}

	struct Vector2
	{
		int X, Y;
	};
}

void sigs_initalize()
{
	SDK::uworld_sig = Scanners::PatternScan("48 8B 1D ? ? ? ? 48 85 DB 74 3B");
	SDK::uworld_sig = rva(SDK::uworld_sig, 7);

	SDK::gnames = Scanners::PatternScan("48 8D 05 ? ? ? ? EB 13 48 8D");
	SDK::gnames = rva(SDK::gnames, 7);

	SDK::gobjs = Scanners::PatternScan("48 8B 05 ? ? ? ? 48 8B 0C C8 48 8D 1C D1 EB 03 49 8B DD");

	auto SussyLittleBaseModule = GetModuleHandle(0);

	AllocConsole();
	freopen("conin$", "r", stdin);
	freopen("conout$", "w", stdout);
	freopen("conout$", "w", stderr);

	printf("\n gnames: 0x%x", SDK::gnames - (uintptr_t)SussyLittleBaseModule);
	printf("\n uworld: 0x%x", SDK::uworld_sig - (uintptr_t)SussyLittleBaseModule);
	printf("\n gobjects: 0x%x", SDK::gobjs - (uintptr_t)SussyLittleBaseModule);

	CG::InitSdk();
}
