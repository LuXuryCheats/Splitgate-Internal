#include "utils.h"

float X;
float Y;

LRESULT WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

HRESULT hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (!pDevice)
	{
		ID3D11Texture2D* renderTarget = 0;
		ID3D11Texture2D* backBuffer = 0;
		D3D11_TEXTURE2D_DESC backBufferDesc = { 0 };
		pSwapChain->GetDevice(__uuidof(pDevice), (PVOID*)&pDevice);
		pDevice->GetImmediateContext(&pContext);

		pSwapChain->GetBuffer(0, __uuidof(renderTarget), (PVOID*)&renderTarget);
		pDevice->CreateRenderTargetView(renderTarget, nullptr, &mainRenderTargetView);
		renderTarget->Release();
		pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (PVOID*)&backBuffer);
		backBuffer->GetDesc(&backBufferDesc);
		is_first_intalize = true;

		backBuffer->Release();

		X = (float)backBufferDesc.Width;
		Y = (float)backBufferDesc.Height;

		Utilitys::InitalizeImGuiScene();
	}

	pContext->OMSetRenderTargets(1, &mainRenderTargetView, nullptr);
	auto& present_scene = Utilitys::window_scene();

	if (Settings::Watermark)
	{
		PS::Text(50, 50, ImColor(255, 255, 255, 200), "FloydWare - Worst Paste");
	}

	if (Settings::AimbotFOV)
	{
		ImGui::GetOverlayDrawList()->AddCircle(ImVec2(X / 2, Y / 2), Settings::AimbotRadius, ImColor(255, 255, 255, 255), 45, 1);
	}

	Actor::Loop();
	Sus::Menu();

	Utilitys::end_window_scene(present_scene);
	return presenth(pSwapChain, SyncInterval, Flags);
}

HRESULT hkResize(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags)
{
	Utilitys::ShutdownScene();
	return resizeh(pSwapChain, BufferCount, Width, Height, NewFormat, SwapChainFlags);
}

ImGuiWindow& Utilitys::window_scene() {
	ImGui_ImplDX11_NewFrame();
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));
	ImGui::Begin("##litlesus", nullptr, ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoTitleBar);
	auto& io = ImGui::GetIO();
	ImGui::SetWindowPos(ImVec2(0, 0), ImGuiCond_Always);
	ImGui::SetWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y), ImGuiCond_Always);
	return *ImGui::GetCurrentWindow();
}

VOID Utilitys::end_window_scene(ImGuiWindow& window) {
	window.DrawList->PushClipRectFullScreen();
	ImGui::End();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar(2);
	ImGui::Render();
}

void Utilitys::Hook_Initalize()
{
	HWND split_hwnd = FindWindow(L"UnrealWindow", L"PortalWars  ");

	auto address = Scanners::PatternScan(Steam::GetSteamdModule(), ("48 89 6C 24 18 48 89 74 24 20 41 56 48 83 EC 20 41 8B E8"));
	Steam::InsertHook(address, (uintptr_t)hkPresent, (long long*)&presenth);

	oWndProc = (WNDPROC)SetWindowLongPtr(split_hwnd, GWLP_WNDPROC, (LONG_PTR)WndProc);	
}

void Utilitys::Hooks_Release()
{
	Hook_Initalize();
}