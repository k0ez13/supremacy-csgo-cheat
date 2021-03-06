// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "hooks.hpp"

#include <tchar.h>
#include <iostream>
#include <d3d9.h>
#include <dinput.h>

#include "..\cheats\misc\logs.h"
#include "..\cheats\misc\misc.h"
#include "..\cheats\visuals\other_esp.h"
#include "..\cheats\visuals\radar.h"
#include "../ImGui/imgui_freetype.h"

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment(lib, "freetype.lib")

#include <shlobj.h>
#include <shlwapi.h>
#include <thread>
#include "..\cheats\menu.h"
#include "..\Bytesa.h"
#include "..\bass\sounds.h"
#include "../minhook/minhook.h"
#include "../utils\imports.h"


auto _visible = true;
static auto d3d_init = false;

namespace INIT
{
	HMODULE Dll;
	HWND Window;
	WNDPROC OldWindow;
}



extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
extern IMGUI_IMPL_API LRESULT ImGui_ImplDX9_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);



namespace hooks
{
	void setup_hooks()
	{
		const auto direct3d_reset = reinterpret_cast<void*>(get_virtual(m_device(), 16));
		const auto direct3d_endscene = reinterpret_cast<void*>(get_virtual(m_device(), 42));
		const auto direct3d_present = reinterpret_cast<void*>(get_virtual(m_device(), 17));
		const auto fns = reinterpret_cast<void*>(get_virtual(m_client(), 37));
		const auto wrrite_usseed = reinterpret_cast<void*>(get_virtual(m_client(), 24));
		const auto packet_start = reinterpret_cast<void*>(get_virtual((void*)((uintptr_t)m_clientstate() + 0x8), 5));
		const auto packet_end = reinterpret_cast<void*>(get_virtual((void*)((uintptr_t)m_clientstate() + 0x8), 6));
		const auto paint = reinterpret_cast<void*>(get_virtual(m_panel(), 41));
		const auto createmove = reinterpret_cast<void*>(get_virtual(m_clientmode(), 24));
		//const auto emitsound = reinterpret_cast<void*>(get_virtual(m_enginesound(), 5));
		const auto post_screen = reinterpret_cast<void*>(get_virtual(m_clientmode(), 44));
		const auto ovverrideview = reinterpret_cast<void*>(get_virtual(m_clientmode(), 18));
		const auto drawfog = reinterpret_cast<void*>(get_virtual(m_clientmode(), 17));
		const auto set_key_code = reinterpret_cast<void*>(get_virtual(m_inputinternal(), 91));
		const auto set_mouse_code = reinterpret_cast<void*>(get_virtual(m_inputinternal(), 92));
		const auto is_connected = reinterpret_cast<void*>(get_virtual(m_engine(), 27));
		//const auto getscreen_ratio = reinterpret_cast<void*>(get_virtual(m_engine(), 101));
		const auto is_hltv = reinterpret_cast<void*>(get_virtual(m_engine(), 93));
		const auto scene_end = reinterpret_cast<void*>(get_virtual(m_renderview(), 9));
		const auto gameventmanag_target = reinterpret_cast<void*>(get_virtual(m_eventmanager(), 9));
		const auto begin_frame = reinterpret_cast<void*>(get_virtual(m_materialsystem(), 42));
		const auto get_material = reinterpret_cast<void*>(get_virtual(m_materialsystem(), 84));
		const auto dme = reinterpret_cast<void*>(get_virtual(m_modelrender(), 21));
		const auto lock_kursor = reinterpret_cast<void*>(get_virtual(m_surface(), 67));
		const auto bsp = reinterpret_cast<void*>(get_virtual(m_engine()->GetBSPTreeQuery(), 6));
		const auto run_command = reinterpret_cast<void*>(get_virtual(m_prediction(), 19));
		const auto trace_clip = reinterpret_cast<void*>(get_virtual(m_trace(), 4));
		const auto trace_ray = reinterpret_cast<void*>(get_virtual(m_trace(), 5));
		//      const auto loadfiles = reinterpret_cast<void*>(get_virtual(util::FindSignature(crypt_str("engine.dll"), g_ctx.signatures.at(20).c_str()) + 0x2, 128));

		if (MH_Initialize() != MH_OK) {
			MessageBoxA(NULL, "Failed to initialize Minhook.", MB_OK, MB_ICONERROR);
		}

		if (MH_CreateHook(direct3d_reset, &hooks::Hooked_EndScene_Reset::hook, reinterpret_cast<void**>(&o_reset)) != MH_OK) {
			MessageBoxA(NULL, "Outdated index - Hooked_EndScene_Reset", MB_OK, MB_ICONERROR);
		}

		if (MH_CreateHook(direct3d_endscene, &hooks::Hooked_EndScene::hook, reinterpret_cast<void**>(&o_endscene)) != MH_OK) {
			MessageBoxA(NULL, "Outdated index - Hooked_EndScene", MB_OK, MB_ICONERROR);
		}


		if (MH_CreateHook(direct3d_present, &hooks::hooked_present::hook, reinterpret_cast<void**>(&present)) != MH_OK) {
			MessageBoxA(NULL, "Outdated index - hooked_present", MB_OK, MB_ICONERROR);
		}

		if (MH_CreateHook(fns, &hooks::hooked_fsn::hook, reinterpret_cast<void**>(&o_fns)) != MH_OK) {
			MessageBoxA(NULL, "Outdated index - hooked_fsn", MB_OK, MB_ICONERROR);
		}

		if (MH_CreateHook(wrrite_usseed, &hooks::hooked_writeusercmddeltatobuffer::hook, reinterpret_cast<void**>(&o_writeusser)) != MH_OK) {
			MessageBoxA(NULL, "Outdated index - hooked_writeusercmddeltatobuffer", MB_OK, MB_ICONERROR);
		}

		if (MH_CreateHook(packet_start, &hooks::hooked_packetstart::hook, reinterpret_cast<void**>(&o_packetstart)) != MH_OK) {
			MessageBoxA(NULL, "Outdated index - hooked_packetstart", MB_OK, MB_ICONERROR);
		}

		if (MH_CreateHook(packet_end, &hooks::hooked_packetend::hook, reinterpret_cast<void**>(&o_packetend)) != MH_OK) {
			MessageBoxA(NULL, "Outdated index - hooked_packetend", MB_OK, MB_ICONERROR);
		}

		if (MH_CreateHook(paint, &hooks::hooked_painttraverse::hook, reinterpret_cast<void**>(&o_paint)) != MH_OK) {
			MessageBoxA(NULL, "Outdated index - hooked_painttraverse", MB_OK, MB_ICONERROR);
		}

		if (MH_CreateHook(createmove, &hooks::hooked_createmove::hook, reinterpret_cast<void**>(&o_createmove)) != MH_OK) {
			MessageBoxA(NULL, "Outdated index - hooked_createmove", MB_OK, MB_ICONERROR);
		}

		if (MH_CreateHook(post_screen, &hooks::hooked_postscreeneffects::hook, reinterpret_cast<void**>(&o_posteffect)) != MH_OK) {
			MessageBoxA(NULL, "Outdated index - hooked_postscreeneffects", MB_OK, MB_ICONERROR);
		}
		if (MH_CreateHook(lock_kursor, &hooks::hooked_lockcursor::hook, reinterpret_cast<void**>(&o_cursor)) != MH_OK) {
			MessageBoxA(NULL, "Outdated index - hooked_lockcursor", MB_OK, MB_ICONERROR);
		}

		if (MH_CreateHook(ovverrideview, &hooks::hooked_overrideview::hook, reinterpret_cast<void**>(&o_overrview)) != MH_OK) {
			MessageBoxA(NULL, "Outdated index - hooked_overrideview", MB_OK, MB_ICONERROR);
		}

		if (MH_CreateHook(drawfog, &hooks::hooked_drawfog::hook, reinterpret_cast<void**>(&o_drawfog)) != MH_OK) {
			MessageBoxA(NULL, "Outdated index - hooked_drawfog", MB_OK, MB_ICONERROR);
		}

		if (MH_CreateHook(set_key_code, &hooks::hooked_setkeycodestate::hook, reinterpret_cast<void**>(&o_setkey)) != MH_OK) {
			MessageBoxA(NULL, "Outdated index - hooked_setkeycodestate", MB_OK, MB_ICONERROR);
		}

		if (MH_CreateHook(set_mouse_code, &hooks::hooked_setmousecodestate::hook, reinterpret_cast<void**>(&o_setmouse)) != MH_OK) {
			MessageBoxA(NULL, "Outdated index - hooked_setmousecodestate", MB_OK, MB_ICONERROR);
		}

		if (MH_CreateHook(is_connected, &hooks::hooked_isconnected::hook, reinterpret_cast<void**>(&o_isconnected)) != MH_OK) {
			MessageBoxA(NULL, "Outdated index - hooked_isconnected", MB_OK, MB_ICONERROR);
		}

		/*if (MH_CreateHook(getscreen_ratio, &hooks::hooked_getscreenaspectratio::hook, reinterpret_cast<void**>(&o_getscreen)) != MH_OK) {
			MessageBoxA(NULL, "Outdated index - hooked_getscreenaspectratio", MB_OK, MB_ICONERROR);
		}*/

		if (MH_CreateHook(is_hltv, &hooks::hooked_ishltv::hook, reinterpret_cast<void**>(&o_hltv)) != MH_OK) {
			MessageBoxA(NULL, "Outdated index - hooked_ishltv", MB_OK, MB_ICONERROR);
		}

		if (MH_CreateHook(scene_end, &hooks::hooked_sceneend::hook, reinterpret_cast<void**>(&o_scene)) != MH_OK) {
			MessageBoxA(NULL, "Outdated index - hooked_sceneend", MB_OK, MB_ICONERROR);
		}

		if (MH_CreateHook(begin_frame, &hooks::hooked_beginframe::hook, reinterpret_cast<void**>(&o_beginframe)) != MH_OK) {
			MessageBoxA(NULL, "Outdated index - hooked_beginframe", MB_OK, MB_ICONERROR);
		}

		if (MH_CreateHook(get_material, &hooks::hooked_getmaterial::hook, reinterpret_cast<void**>(&o_getmaterial)) != MH_OK) {
			MessageBoxA(NULL, "Outdated index - hooked_getmaterial", MB_OK, MB_ICONERROR);
		}

		if (MH_CreateHook(dme, &hooks::hooked_dme::hook, reinterpret_cast<void**>(&o_dme)) != MH_OK) {
			MessageBoxA(NULL, "Outdated index - hooked_dme", MB_OK, MB_ICONERROR);
		}

		/*if (MH_CreateHook(emitsound, &hooks::hooked_emitsound::hook, reinterpret_cast<void**>(&o_emitsound)) != MH_OK) {
			MessageBoxA(NULL, "Outdated index - hooked_emitsound", MB_OK, MB_ICONERROR);
		}*/

		if (MH_CreateHook(bsp, &hooks::hooked_listleavesinbox::hook, reinterpret_cast<void**>(&o_bsp)) != MH_OK) {
			MessageBoxA(NULL, "Outdated index - hooked_listleavesinbox", MB_OK, MB_ICONERROR);
		}

		if (MH_CreateHook(run_command, &hooks::hooked_runcommand::hook, reinterpret_cast<void**>(&o_runcommand)) != MH_OK) {
			MessageBoxA(NULL, "Outdated index - hooked_runcommand", MB_OK, MB_ICONERROR);
		}

		if (MH_CreateHook(trace_clip, &hooks::hooked_clip_ray_collideable::hook, reinterpret_cast<void**>(&o_trace_clip)) != MH_OK) {
			MessageBoxA(NULL, "Outdated index - hooked_clip_ray_collideable", MB_OK, MB_ICONERROR);
		}

		if (MH_CreateHook(trace_ray, &hooks::hooked_trace_ray::hook, reinterpret_cast<void**>(&o_trace_ray)) != MH_OK) {
			MessageBoxA(NULL, "Outdated index - hooked_trace_ray", MB_OK, MB_ICONERROR);
		}

		//		if (MH_CreateHook(loadfiles, &hooks::hooked_loosefileallowed::hook, reinterpret_cast<void**>(&o_loadfiles)) != MH_OK) {
		//			MessageBoxA(NULL, "Outdated index - hooked_loosefileallowed", MB_OK, MB_ICONERROR);
		//		}

		MH_CreateHook(gameventmanag_target, &shonax_hooks::FireEventClientSide, reinterpret_cast<void**>(&shonax_hooks::oFireEventClientSidefn));


		static auto getforeignfallbackfontname = (DWORD)(util::FindSignature(crypt_str("vguimatsurface.dll"), g_ctx.signatures.at(9).c_str()));
		hooks::original_getforeignfallbackfontname = (DWORD)DetourFunction((PBYTE)getforeignfallbackfontname, (PBYTE)hooks::hooked_getforeignfallbackfontname); //-V206

		static auto setupbones = (DWORD)(util::FindSignature(crypt_str("client.dll"), g_ctx.signatures.at(10).c_str()));
		hooks::original_setupbones = (DWORD)DetourFunction((PBYTE)setupbones, (PBYTE)hooks::hooked_setupbones); //-V206

		static auto doextrabonesprocessing = (DWORD)(util::FindSignature(crypt_str("client.dll"), g_ctx.signatures.at(11).c_str()));
		hooks::original_doextrabonesprocessing = (DWORD)DetourFunction((PBYTE)doextrabonesprocessing, (PBYTE)hooks::hooked_doextrabonesprocessing); //-V206

		static auto standardblendingrules = (DWORD)(util::FindSignature(crypt_str("client.dll"), g_ctx.signatures.at(12).c_str()));
		hooks::original_standardblendingrules = (DWORD)DetourFunction((PBYTE)standardblendingrules, (PBYTE)hooks::hooked_standardblendingrules); //-V206

		static auto updateclientsideanimation = (DWORD)(util::FindSignature(crypt_str("client.dll"), g_ctx.signatures.at(13).c_str()));
		hooks::original_updateclientsideanimation = (DWORD)DetourFunction((PBYTE)updateclientsideanimation, (PBYTE)hooks::hooked_updateclientsideanimation); //-V206

		static auto physicssimulate = (DWORD)(util::FindSignature(crypt_str("client.dll"), g_ctx.signatures.at(14).c_str()));
		hooks::original_physicssimulate = (DWORD)DetourFunction((PBYTE)physicssimulate, (PBYTE)hooks::hooked_physicssimulate);

		static auto modifyeyeposition = (DWORD)(util::FindSignature(crypt_str("client.dll"), g_ctx.signatures.at(15).c_str()));
		hooks::original_modifyeyeposition = (DWORD)DetourFunction((PBYTE)modifyeyeposition, (PBYTE)hooks::hooked_modifyeyeposition);

		static auto calcviewmodelbob = (DWORD)(util::FindSignature(crypt_str("client.dll"), g_ctx.signatures.at(16).c_str()));
		hooks::original_calcviewmodelbob = (DWORD)DetourFunction((PBYTE)calcviewmodelbob, (PBYTE)hooks::hooked_calcviewmodelbob);

		static auto shouldskipanimframe = (DWORD)(util::FindSignature(crypt_str("client.dll"), g_ctx.signatures.at(17).c_str()));
		DetourFunction((PBYTE)shouldskipanimframe, (PBYTE)hooks::hooked_shouldskipanimframe);

		static auto checkfilecrcswithserver = (DWORD)(util::FindSignature(crypt_str("engine.dll"), g_ctx.signatures.at(18).c_str()));
		DetourFunction((PBYTE)checkfilecrcswithserver, (PBYTE)hooks::hooked_checkfilecrcswithserver);

		static auto processinterpolatedlist = (DWORD)(util::FindSignature(crypt_str("client.dll"), g_ctx.signatures.at(19).c_str()));
		hooks::original_processinterpolatedlist = (DWORD)DetourFunction((byte*)processinterpolatedlist, (byte*)hooks::processinterpolatedlist); //-V206

		

		while (!(INIT::Window = IFH(FindWindow)(crypt_str("Valve001"), nullptr)))
			std::this_thread::sleep_for(std::chrono::milliseconds(100));

		INIT::OldWindow = (WNDPROC)IFH(SetWindowLongPtr)(INIT::Window, GWL_WNDPROC, (LONG_PTR)hooks::Hooked_WndProc);




		hooks::hooked_events.RegisterSelf();



		if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK) {
			MessageBoxA(NULL, "Failed to enable hooks.", MB_OK, MB_ICONERROR);
		}

	}


	int rage_weapon = 0;
	int legit_weapon = 0;
	bool menu_open = false;
	bool input_shouldListen = false;


	ButtonCode_t* input_receivedKeyval;

	IDirect3DStateBlock9* pixel_state = NULL; IDirect3DVertexDeclaration9* vertDec; IDirect3DVertexShader9* vertShader;
	DWORD dwOld_D3DRS_COLORWRITEENABLE;
	DWORD colorwrite, srgbwrite;

	void SaveState(IDirect3DDevice9* device)
	{
		D3DVIEWPORT9 d3d_viewport;
		device->GetViewport(&d3d_viewport);

		device->CreateStateBlock(D3DSBT_ALL, &pixel_state);
		pixel_state->Capture();

		device->SetVertexShader(nullptr);
		device->SetPixelShader(nullptr);
		device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

		device->SetRenderState(D3DRS_LIGHTING, FALSE);
		device->SetRenderState(D3DRS_FOGENABLE, FALSE);
		device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

		device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
		device->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
		device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		device->SetRenderState(D3DRS_STENCILENABLE, FALSE);

		device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
		device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, TRUE);

		device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		device->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, TRUE);
		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		device->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_INVDESTALPHA);
		device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		device->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_ONE);

		device->SetRenderState(D3DRS_SRGBWRITEENABLE, FALSE);
		device->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);
	}
	void RestoreState(IDirect3DDevice9* pDevice) // not restoring everything. Because its not needed.
	{
		pixel_state->Apply();
		pixel_state->Release();
	}

	LRESULT __stdcall hooks::Hooked_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		static auto is_down = true;
		static auto is_clicked = false;

		if (GetAsyncKeyState(VK_INSERT))
		{
			is_clicked = false;
			is_down = true;
		}
		else if (!GetAsyncKeyState(VK_INSERT) && is_down)
		{
			is_clicked = true;
			is_down = false;
		}
		else
		{
			is_clicked = false;
			is_down = false;
		}

		if (is_clicked)
		{
			menu_open = !menu_open;

			if (menu_open && g_ctx.available())
			{
				if (g_ctx.globals.current_weapon != -1)
				{
					if (g_cfg.ragebot.enable)
						rage_weapon = g_ctx.globals.current_weapon;
					else if (g_cfg.legitbot.enabled)
						legit_weapon = g_ctx.globals.current_weapon;
				}
			}
		}



		auto pressed_buttons = false;
		auto pressed_menu_key = uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONUP || uMsg == WM_RBUTTONDOWN || uMsg == WM_RBUTTONUP || uMsg == WM_MOUSEWHEEL;

		if (g_ctx.local()->is_alive() && !pressed_menu_key && !g_ctx.globals.focused_on_input)
			pressed_buttons = true;

		if (!pressed_buttons && d3d_init && menu_open && ImGui_ImplDX9_WndProcHandler(hWnd, uMsg, wParam, lParam) && !input_shouldListen)
			return true;

		if (menu_open && (uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONUP || uMsg == WM_MOUSEMOVE) && !input_shouldListen)
			return false;

		return CallWindowProc(INIT::OldWindow, hWnd, uMsg, wParam, lParam);
	}

	long __stdcall hooks::Hooked_EndScene::hook(IDirect3DDevice9* pDevice)
	{
		return o_endscene(pDevice);
	}

	/*LRESULT __stdcall Hooked_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		static auto is_down = true;
		static auto is_clicked = false;

		if (GetAsyncKeyState(VK_INSERT))
		{
			is_clicked = false;
			is_down = true;
		}
		else if (!GetAsyncKeyState(VK_INSERT) && is_down)
		{
			is_clicked = true;
			is_down = false;
		}
		else
		{
			is_clicked = false;
			is_down = false;
		}

		if (is_clicked)
		{
			menu_open = !menu_open;

			if (menu_open && g_ctx.available())
			{
				if (g_ctx.globals.current_weapon != -1)
				{
					if (g_cfg.ragebot.enable)
						rage_weapon = g_ctx.globals.current_weapon;
					else if (g_cfg.legitbot.enabled)
						legit_weapon = g_ctx.globals.current_weapon;
				}
			}
		}

		

		auto pressed_buttons = false;
		auto pressed_menu_key = uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONUP || uMsg == WM_RBUTTONDOWN || uMsg == WM_RBUTTONUP || uMsg == WM_MOUSEWHEEL;

		if (g_ctx.local()->is_alive() && !pressed_menu_key && !g_ctx.globals.focused_on_input)
			pressed_buttons = true;

		if (!pressed_buttons && d3d_init && menu_open && ImGui_ImplDX9_WndProcHandler(hWnd, uMsg, wParam, lParam) && !input_shouldListen)
			return true;

		if (menu_open && (uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONUP || uMsg == WM_MOUSEMOVE) && !input_shouldListen)
			return false;

		return CallWindowProc(INIT::OldWindow, hWnd, uMsg, wParam, lParam);
	}*/

	/*long __stdcall Hooked_EndScene(IDirect3DDevice9* pDevice)
	{
		static auto original_fn = directx_hook->get_func_address <EndSceneFn> (42);
		return original_fn(pDevice);
	}*/

	void GUI_Init(IDirect3DDevice9* pDevice)
	{
		ImGui::CreateContext();

		ImGui_ImplWin32_Init(INIT::Window);
		ImGui_ImplDX9_Init(pDevice);

		auto& io = ImGui::GetIO();
		auto& style = ImGui::GetStyle();

		style.WindowMinSize = ImVec2(10, 10);

		ImFontConfig m_config;
		m_config.OversampleH = m_config.OversampleV = 3;
		m_config.PixelSnapH = false;

		static const ImWchar ranges[] =
		{
			0x0020, 0x00FF, // Basic Latin + Latin Supplement
			0x2000, 0x206F, // General Punctuation
			0x3000, 0x30FF, // CJK Symbols and Punctuations, Hiragana, Katakana
			0x31F0, 0x31FF, // Katakana Phonetic Extensions
			0xFF00, 0xFFEF, // Half-width characters
			0x4e00, 0x9FAF, // CJK Ideograms
			0x0400, 0x052F, // Cyrillic + Cyrillic Supplement
			0x2DE0, 0x2DFF, // Cyrillic Extended-A
			0xA640, 0xA69F, // Cyrillic Extended-B
			0x3000, 0x3000, // Ideographic Space 
			0,
		};

		char windows_directory[MAX_PATH];
		GetWindowsDirectory(windows_directory, MAX_PATH);

		auto verdana_directory = (std::string)windows_directory + "\\Fonts\\comicbd.ttf";

		// Menu fonts
		c_menu::get().futura_small = io.Fonts->AddFontFromMemoryTTF((void*)futuramediumc, sizeof(futuramediumc), 15.f, &m_config, ranges);
		c_menu::get().futura = io.Fonts->AddFontFromMemoryTTF((void*)futuramediumc, sizeof(futuramediumc), 17.f, &m_config, ranges);
		c_menu::get().futura_large = io.Fonts->AddFontFromMemoryTTF((void*)futuramediumc, sizeof(futuramediumc), 20.f, &m_config, ranges);
		c_menu::get().gotham = io.Fonts->AddFontFromMemoryTTF((void*)futuramediumc, sizeof(futuramediumc), 13.f, &m_config, ranges);

		// Icon fonts
		c_menu::get().ico_menu = io.Fonts->AddFontFromMemoryTTF((void*)icomenu, sizeof(icomenu), 25.f, &m_config, io.Fonts->GetGlyphRangesCyrillic());
		c_menu::get().skeet_icon = io.Fonts->AddFontFromMemoryTTF((void*)skeet, sizeof(skeet), 25.f, &m_config, ranges);

		c_menu::get().ico_bottom = io.Fonts->AddFontFromMemoryTTF((void*)iconbot, sizeof(iconbot), 25.f, &m_config, io.Fonts->GetGlyphRangesCyrillic());


		c_menu::get().logo = io.Fonts->AddFontFromMemoryTTF((void*)logoo, sizeof(logoo), 52.f, &m_config, io.Fonts->GetGlyphRangesCyrillic());

		ImGui_ImplDX9_CreateDeviceObjects();
		d3d_init = true;
	}

	/*long __stdcall hooked_present(IDirect3DDevice9* device, RECT* src_rect, RECT* dest_rect, HWND dest_wnd_override, RGNDATA* dirty_region)
	{
		static auto original = directx_hook->get_func_address <PresentFn> (17);
		g_ctx.local((player_t*)m_entitylist()->GetClientEntity(m_engine()->GetLocalPlayer()), true);



		SaveState(device);




		if (!d3d_init)
			GUI_Init(device);

		c_menu::get().device = device;

		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		c_menu::get().render2(menu_open);
		c_menu::get().keys();
		otheresp::get().spread_crosshair(device);

		if (g_ctx.globals.should_update_radar)
			Radar::get().OnMapLoad(m_engine()->GetLevelNameShort(), device);
		else
			Radar::get().Render();

		ImGui::EndFrame();
		ImGui::Render();
		playback_loop();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

		RestoreState(device);
		return original(device, src_rect, dest_rect, dest_wnd_override, dirty_region);
	}

	long __stdcall Hooked_EndScene_Reset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
	{
		static auto ofunc = directx_hook->get_func_address<EndSceneResetFn>(16);

		if (!d3d_init)
			return ofunc(pDevice, pPresentationParameters);

		ImGui_ImplDX9_InvalidateDeviceObjects();

		auto hr = ofunc(pDevice, pPresentationParameters);

		if (SUCCEEDED(hr))
			ImGui_ImplDX9_CreateDeviceObjects();

		return hr;
	}
	*/

	HRESULT __stdcall hooks::hooked_present::hook(IDirect3DDevice9* device, RECT* src_rect, RECT* dest_rect, HWND dest_wnd_override, RGNDATA* dirty_region)
	{
		g_ctx.local((player_t*)m_entitylist()->GetClientEntity(m_engine()->GetLocalPlayer()), true);



		SaveState(device);




		if (!d3d_init)
			GUI_Init(device);

		c_menu::get().device = device;

		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		c_menu::get().render2(menu_open);
		c_menu::get().keys();
		otheresp::get().spread_crosshair(device);

		if (g_ctx.globals.should_update_radar)
			Radar::get().OnMapLoad(m_engine()->GetLevelNameShort(), device);
		else
			Radar::get().Render();

		ImGui::EndFrame();
		ImGui::Render();
		playback_loop();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

		RestoreState(device);
		return present(device, src_rect, dest_rect, dest_wnd_override, dirty_region);
	}

	HRESULT __stdcall hooks::Hooked_EndScene_Reset::hook(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
	{

		if (!d3d_init)
			return o_reset(pDevice, pPresentationParameters);

		ImGui_ImplDX9_InvalidateDeviceObjects();

		auto hr = o_reset(pDevice, pPresentationParameters);

		if (SUCCEEDED(hr))
			ImGui_ImplDX9_CreateDeviceObjects();

		return hr;
	}

	DWORD original_getforeignfallbackfontname;
	DWORD original_setupbones;
	DWORD original_doextrabonesprocessing;
	DWORD original_standardblendingrules;
	DWORD original_updateclientsideanimation;
	DWORD original_physicssimulate;
	DWORD original_modifyeyeposition;
	DWORD original_calcviewmodelbob;
	DWORD original_processinterpolatedlist;

	/*vmthook* directx_hook;
	vmthook* client_hook;
	vmthook* clientstate_hook;
	vmthook* engine_hook;
	vmthook* clientmode_hook;
	vmthook* inputinternal_hook;
	vmthook* renderview_hook;
	vmthook* panel_hook;
	vmthook* modelcache_hook;
	vmthook* materialsys_hook;
	vmthook* modelrender_hook;
	vmthook* surface_hook;
	vmthook* bspquery_hook;
	vmthook* prediction_hook;
	vmthook* trace_hook;
	vmthook* filesystem_hook;*/

	C_HookedEvents hooked_events;
}

void __fastcall hooks::hooked_setkeycodestate::hook(void* thisptr, void* edx, ButtonCode_t code, bool bDown)
{
	if (input_shouldListen && bDown)
	{
		input_shouldListen = false;

		if (input_receivedKeyval)
			*input_receivedKeyval = code;
	}

	return o_setkey(thisptr, code, bDown);
}

void __fastcall hooks::hooked_setmousecodestate::hook(void* thisptr, void* edx, ButtonCode_t code, MouseCodeState_t state)
{

	if (input_shouldListen && state == BUTTON_PRESSED)
	{
		input_shouldListen = false;

		if (input_receivedKeyval)
			*input_receivedKeyval = code;
	}

	return o_setmouse(thisptr, code, state);
}
/*void __fastcall hooks::hooked_setkeycodestate(void* thisptr, void* edx, ButtonCode_t code, bool bDown)
{
	static auto original_fn = inputinternal_hook->get_func_address <SetKeyCodeState_t> (91);

	if (input_shouldListen && bDown)
	{
		input_shouldListen = false;

		if (input_receivedKeyval)
			*input_receivedKeyval = code;
	}

	return original_fn(thisptr, code, bDown);
}

void __fastcall hooks::hooked_setmousecodestate(void* thisptr, void* edx, ButtonCode_t code, MouseCodeState_t state)
{
	static auto original_fn = inputinternal_hook->get_func_address <SetMouseCodeState_t> (92);

	if (input_shouldListen && state == BUTTON_PRESSED)
	{
		input_shouldListen = false;

		if (input_receivedKeyval)
			*input_receivedKeyval = code;
	}

	return original_fn(thisptr, code, state);
}*/