#include <Windows.h>
#include <vector>
#include <string>
#include <sstream>
#include <conio.h>
#include <Psapi.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_internal.h"

#include <d3d11.h>
#pragma comment (lib, "d3d11.lib")

#include "imgui/MinHook.h"
#pragma comment (lib, "minhook.lib")

#include "settings.h"
#include "structs.h"
#include "menu.h"

HWND hwnd = NULL;
VOID(*FreeInternal)(PVOID) = nullptr;
HWND window = nullptr;
WNDPROC oWndProc;
ID3D11Device* pDevice = nullptr;
ID3D11DeviceContext* pContext = nullptr;
ID3D11RenderTargetView* mainRenderTargetView = nullptr;
bool is_first_intalize = false;

HRESULT(*presenth)(IDXGISwapChain* swapChain, UINT syncInterval, UINT flags) = nullptr;
HRESULT(*resizeh)(IDXGISwapChain* swapChain, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT newFormat, UINT swapChainFlags) = nullptr;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Utilitys
{
	ImGuiWindow& window_scene();
	VOID end_window_scene(ImGuiWindow& scene);

	void Hook_Initalize();
	void Hooks_Release();

	void InitalizeImGuiScene()
	{
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		if (!hwnd)
		{
			hwnd = FindWindowW(L"UnrealWindow", L"PortalWars  ");
			if (!hwnd)
			{
				hwnd = GetForegroundWindow();
			}
		}

		io.Fonts->AddFontFromFileTTF("C:\\Roboto-Regular.ttf", 17);

		ImGui_ImplDX11_Init(hwnd, pDevice, pContext);
		ImGui_ImplDX11_CreateDeviceObjects();
	}

	void ShutdownScene()
	{
		ImGui_ImplDX11_Shutdown();
		mainRenderTargetView->Release();
		pContext->Release();
		pDevice->Release();
		pDevice = nullptr;
	}

	void isSuceeded(const char* format)
	{
		printf(format);
	}

	uintptr_t uworld;
}

namespace Steam {

	uintptr_t GetSteamdModule()
	{
		printf("1\n");
		std::string module = ("GameOverlayRenderer64.dll");
		return (uintptr_t)GetModuleHandleA(module.c_str());
	}

	void InsertHook(__int64 addr, __int64 func, __int64* orig)
	{
		printf("2\n");
		static uintptr_t hook_addr;
		if (!hook_addr)
			hook_addr = Scanners::PatternScan(GetSteamdModule(), ("48 ? ? ? ? 57 48 83 EC 30 33 C0"));

		auto hook = ((__int64(__fastcall*)(__int64 addr, __int64 func, __int64* orig, __int64 smthng))(hook_addr));
		hook((__int64)addr, (__int64)func, orig, (__int64)1);
	}
}

namespace Offsets
{
	uintptr_t PlayerState = 0x240;
	uintptr_t Player_Controller = 0x258;
	uintptr_t AcknowledgedPawn = 0x2a0;
}

namespace Actor
{
	//0x50621d0 uworld
	bool world()
	{

	}

	void Loop()
	{
		
	}
}