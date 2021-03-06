#pragma once

#include "..\ImGui\imgui.h"
#include "..\ImGui\imgui_internal.h"
#include "..\ImGui\imgui_impl_dx9.h"
#include "..\ImGui\imgui_impl_win32.h"

#include "..\includes.hpp"
#include "..\sdk\interfaces\IBaseClientDll.hpp"
#include "..\sdk\interfaces\IClientMode.hpp"
#include "..\sdk\misc\CUserCmd.hpp"
#include "..\sdk\interfaces\IInputSystem.hpp"
#include "..\sdk\interfaces\IMDLCache.hpp"
#include "..\sdk\interfaces\IGameEventManager.hpp"
#include "..\utils\util.hpp"

#include "vfunc_hook.hpp"

class C_HookedEvents : public IGameEventListener2
{
public:
	void FireGameEvent(IGameEvent * event);
	void RegisterSelf();
	void RemoveSelf();
	int GetEventDebugID(void);
};

namespace INIT
{
	extern HMODULE Dll;
	extern HWND Window;
	extern WNDPROC OldWindow;
}

class c_baseplayeranimationstate;

/*namespace hooks
{
	extern bool menu_open;
	extern bool input_shouldListen;
	extern int rage_weapon;
	extern int legit_weapon;

	extern ButtonCode_t* input_receivedKeyval;

	extern vmthook* directx_hook;
	extern vmthook* client_hook;
	extern vmthook* clientstate_hook;
	extern vmthook* engine_hook;
	extern vmthook* clientmode_hook;
	extern vmthook* inputinternal_hook;
	extern vmthook* renderview_hook;
	extern vmthook* modelcache_hook;
	extern vmthook* panel_hook;
	extern vmthook* materialsys_hook;
	extern vmthook* modelrender_hook;
	extern vmthook* surface_hook;
	extern vmthook* bspquery_hook;
	extern vmthook* prediction_hook;
	extern vmthook* trace_hook;
	extern vmthook* filesystem_hook;

	extern C_HookedEvents hooked_events;

	using GetForeignFallbackFontNameFn = const char*(__thiscall*)(void*);
	using SetupBonesFn = bool(__thiscall*)(void*, matrix3x4_t*, int, int, float);
	using DoExtraBonesProcessingFn = void(__thiscall*)(player_t*, CStudioHdr*, Vector*, Quaternion*, const matrix3x4_t&, uint8_t*, void*);
	using StandardBlendingRulesFn = void(__thiscall*)(player_t*, CStudioHdr*, Vector*, Quaternion*, float, int);
	using UpdateClientSideAnimationFn = void(__fastcall*)(player_t*);
	using PhysicsSimulateFn = void(__fastcall*)(player_t*);
	using ModifyEyePositionFn = void(__thiscall*)(c_baseplayeranimationstate*, Vector&);
	using CalcViewmodelBobFn = void(__thiscall*)(player_t*, Vector&);
	using ProcessInterpolatedListFn = int(*)(void);

	extern DWORD original_getforeignfallbackfontname;
	extern DWORD original_setupbones;
	extern DWORD original_doextrabonesprocessing;
	extern DWORD original_standardblendingrules;
	extern DWORD original_updateclientsideanimation;
	extern DWORD original_physicssimulate;
	extern DWORD original_modifyeyeposition;
	extern DWORD original_calcviewmodelbob;
	extern DWORD original_processinterpolatedlist;

	void __stdcall hooked_fsn(ClientFrameStage_t);
	bool __stdcall hooked_createmove(float, CUserCmd*);
	bool __fastcall hooked_drawfog(void* ecx, void* edx);
	void __stdcall hooked_overrideview(CViewSetup * setup);
	bool __fastcall hooked_isconnected(void* ecx, void* edx);
	float __fastcall hooked_getscreenaspectratio(void* ecx, void* edx, int width, int height);
	bool __fastcall hooked_ishltv(void* ecx, void* edx);
	void __stdcall hooked_dme(IMatRenderContext * ctx, const DrawModelState_t & state, const ModelRenderInfo_t & info, matrix3x4_t * bone_to_world);
	void  __fastcall hooked_postscreeneffects(void * thisptr, void * edx, CViewSetup * setup);
	void __fastcall hooked_setkeycodestate(void* thisptr, void* edx, ButtonCode_t code, bool bDown);
	void __fastcall hooked_setmousecodestate(void* thisptr, void* edx, ButtonCode_t code, MouseCodeState_t state);
	void __fastcall hooked_sceneend(void* ecx, void* edx);
	void __fastcall hooked_findmdl(void* ecx, void* edx, char* FilePath);
	void __fastcall hooked_painttraverse(void* ecx, void* edx, vgui::VPANEL panel, bool force_repaint, bool allow_force);
	void __fastcall hooked_beginframe(void* ecx, void* edx, float ft);
	const char* __fastcall hooked_getforeignfallbackfontname(void* ecx, uint32_t i);
	_declspec(noinline)const char* getforeignfallbackfontname_detour(void* ecx, uint32_t i);
	bool __fastcall hooked_setupbones(void* ecx, void* edx, matrix3x4_t* bone_world_out, int max_bones, int bone_mask, float current_time);
	_declspec(noinline)bool setupbones_detour(void* ecx, matrix3x4_t* bone_world_out, int max_bones, int bone_mask, float current_time);
	void __fastcall hooked_doextrabonesprocessing(player_t* player, void* edx, CStudioHdr* hdr, Vector* pos, Quaternion* q, const matrix3x4_t& matrix, uint8_t* bone_list, void* context);
	_declspec(noinline)void doextrabonesprocessing_detour(player_t* player, CStudioHdr* hdr, Vector* pos, Quaternion* q, const matrix3x4_t& matrix, uint8_t* bone_list, void* context);
	void __fastcall hooked_standardblendingrules(player_t* player, int i, CStudioHdr* hdr, Vector* pos, Quaternion* q, float curtime, int boneMask);
	_declspec(noinline)void standardblendingrules_detour(player_t* player, int i, CStudioHdr* hdr, Vector* pos, Quaternion* q, float curtime, int boneMask);
	void __fastcall hooked_updateclientsideanimation(player_t* player, uint32_t i);
	_declspec(noinline)void updateclientsideanimation_detour(player_t* player);
	void __fastcall hooked_physicssimulate(player_t* player);
	_declspec(noinline)void physicssimulate_detour(player_t* player);
	void __fastcall hooked_modifyeyeposition(c_baseplayeranimationstate* state, void* edx, Vector& position);
	_declspec(noinline)void modifyeyeposition_detour(c_baseplayeranimationstate* state, Vector& position);
	void __fastcall hooked_calcviewmodelbob(player_t* player, void* edx, Vector& position);
	_declspec(noinline)void calcviewmodelbob_detour(player_t* player, Vector& position);
	bool __fastcall hooked_shouldskipanimframe();
	int processinterpolatedlist();
	IMaterial* __fastcall hooked_getmaterial(void* ecx, void* edx, const char* material_name, const char* texture_group_name, bool complain, const char* complain_prefix);
	void __fastcall hooked_packetstart(void* ecx, void* edx, int incoming, int outgoing);
	void __fastcall hooked_packetend(void* ecx, void* edx);
	void __stdcall hooked_lockcursor();
	int __fastcall hooked_listleavesinbox(void* ecx, void* edx, Vector& mins, Vector& maxs, unsigned short* list, int list_max);
	void __fastcall hooked_runcommand(void* ecx, void* edx, player_t* player, CUserCmd* m_pcmd, IMoveHelper* move_helper);
	bool __stdcall hooked_inprediction();
	bool __fastcall hooked_writeusercmddeltatobuffer(void* ecx, void* edx, int slot, bf_write* buf, int from, int to, bool is_new_command);
	void __fastcall hooked_clip_ray_collideable(void* ecx, void* edx, const Ray_t& ray, uint32_t fMask, ICollideable* pCollide, CGameTrace* pTrace);
	void __fastcall hooked_trace_ray(void* ecx, void* edx, const Ray_t& ray, unsigned int fMask, ITraceFilter* pTraceFilter, trace_t* pTrace);
	bool __fastcall hooked_loosefileallowed(void* ecx, void* edx);
	void __fastcall hooked_checkfilecrcswithserver(void* ecx, void* edx);
	LRESULT __stdcall Hooked_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	long __stdcall Hooked_EndScene(IDirect3DDevice9* pDevice);
	long __stdcall Hooked_EndScene_Reset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
	long __stdcall hooked_present(IDirect3DDevice9* device, RECT* src_rect, RECT* dest_rect, HWND dest_wnd_override, RGNDATA* dirty_region);
	void GUI_Init(IDirect3DDevice9* pDevice);

	typedef long(__stdcall *EndSceneFn)(IDirect3DDevice9* device);
	typedef long(__stdcall *EndSceneResetFn)(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
	typedef long(__stdcall* PresentFn)(IDirect3DDevice9*, RECT*, RECT*, HWND, RGNDATA*);

	typedef void(__thiscall* SetKeyCodeState_t) (void*, ButtonCode_t, bool);
	extern SetKeyCodeState_t o_SetKeyCodeState;

	typedef void(__thiscall* SetMouseCodeState_t) (void*, ButtonCode_t, MouseCodeState_t);
	extern SetMouseCodeState_t o_SetMouseCodeState;
}*/

namespace hooks
{
	extern bool menu_open;
	extern bool input_shouldListen;
	extern int rage_weapon;
	extern int legit_weapon;
	extern void setup_hooks();
	extern ButtonCode_t* input_receivedKeyval;

	namespace Hooked_EndScene_Reset {
		using fn = HRESULT(__stdcall*)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
		HRESULT __stdcall hook(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pPresentationParameters);
	}

	inline Hooked_EndScene_Reset::fn o_reset = nullptr;

	namespace Hooked_EndScene {
		using fn = long(__stdcall*)(IDirect3DDevice9*);
		long __stdcall hook(IDirect3DDevice9* device);
	}

	inline Hooked_EndScene::fn o_endscene = nullptr;

	namespace hooked_present {
		using fn = HRESULT(__stdcall*)(IDirect3DDevice9*, RECT*, RECT*, HWND, RGNDATA*);
		HRESULT __stdcall hook(IDirect3DDevice9* device, RECT* src_rect, RECT* dest_rect, HWND dest_wnd_override, RGNDATA* dirty_region);
	}

	inline hooked_present::fn present = nullptr;

	namespace hooked_fsn {
		using fn = void(__stdcall*)(ClientFrameStage_t);
		void __stdcall hook(ClientFrameStage_t stage);
	}

	inline hooked_fsn::fn o_fns = nullptr;

	namespace hooked_writeusercmddeltatobuffer {
		using fn = bool(__thiscall*)(void*, int, bf_write*, int, int, bool);
		bool __fastcall hook(void* ecx, void* edx, int slot, bf_write* buf, int from, int to, bool is_new_command);
	}

	inline hooked_writeusercmddeltatobuffer::fn o_writeusser = nullptr;

	namespace hooked_packetstart {
		using fn = void(__thiscall*)(void*, int, int);
		void __fastcall hook(void* ecx, void* edx, int incoming, int outgoing);
	}

	inline hooked_packetstart::fn o_packetstart = nullptr;

	namespace hooked_packetend {
		using fn = void(__thiscall*)(void*);
		void __fastcall hook(void* ecx, void* edx);
	}

	inline hooked_packetend::fn o_packetend = nullptr;

	namespace hooked_painttraverse {
		using fn = void(__thiscall*)(void*, vgui::VPANEL, bool, bool);
		void __fastcall hook(void* ecx, void* edx, vgui::VPANEL panel, bool force_repaint, bool allow_force);
	}

	inline hooked_painttraverse::fn o_paint = nullptr;

	namespace hooked_createmove {
		using fn = bool(__stdcall*)(float, CUserCmd*);
		bool __stdcall hook(float smt, CUserCmd* m_pcmd);
	}

	inline hooked_createmove::fn o_createmove = nullptr;

	namespace hooked_postscreeneffects {
		using fn = void(__thiscall*)(void*, CViewSetup*);
		void __fastcall hook(void* thisptr, void* edx, CViewSetup* setup);
	}

	inline hooked_postscreeneffects::fn o_posteffect = nullptr;

	namespace hooked_overrideview {
		using fn = void(__stdcall*)(CViewSetup*);
		void __stdcall hook(CViewSetup* viewsetup);
	}

	inline hooked_overrideview::fn o_overrview = nullptr;

	namespace hooked_drawfog {
		using fn = bool(__thiscall*)(void*);
		bool __fastcall hook(void* ecx, void* edx);
	}

	inline hooked_drawfog::fn o_drawfog = nullptr;

	namespace hooked_setkeycodestate {
		using fn = void(__thiscall*)(void*, ButtonCode_t, bool);
		void __fastcall hook(void* thisptr, void* edx, ButtonCode_t code, bool bDown);
	}

	inline hooked_setkeycodestate::fn o_setkey = nullptr;

	namespace hooked_setmousecodestate {
		using fn = void(__thiscall*)(void*, ButtonCode_t, MouseCodeState_t);
		void __fastcall hook(void* thisptr, void* edx, ButtonCode_t code, MouseCodeState_t state);
	}

	inline hooked_setmousecodestate::fn o_setmouse = nullptr;

	namespace hooked_isconnected {
		using fn = bool(__fastcall*)(void*);
		bool __fastcall hook(void* ecx, void* edx);
	}

	inline hooked_isconnected::fn o_isconnected = nullptr;

	/*namespace hooked_getscreenaspectratio {
		using fn = float(__fastcall*)(void*, int, int);
		float __fastcall hook(void* ecx, void* edx, int width, int height);
	}

	inline hooked_getscreenaspectratio::fn o_getscreen = nullptr;*/

	namespace hooked_ishltv {
		using fn = bool(__fastcall*)(void*);
		bool __fastcall hook(void* ecx, void* edx);
	}

	inline hooked_ishltv::fn o_hltv = nullptr;

	namespace hooked_sceneend {
		using fn = void(__thiscall*)(void*);
		void __fastcall hook(void* ecx, void* edx);
	}

	inline hooked_sceneend::fn o_scene = nullptr;

	namespace hooked_beginframe {
		using fn = void(__thiscall*)(void*, float);
		void __fastcall hook(void* ecx, void* edx, float ft);
	}

	inline hooked_beginframe::fn o_beginframe = nullptr;

	namespace hooked_getmaterial {
		using fn = IMaterial * (__thiscall*)(void*, const char*, const char*, bool, const char*);
		IMaterial* __fastcall hook(void* ecx, void* edx, const char* material_name, const char* texture_group_name, bool complain, const char* complain_prefix);
	}

	inline hooked_getmaterial::fn o_getmaterial = nullptr;

	namespace hooked_dme {
		using fn = void(__thiscall*)(IVModelRender*, IMatRenderContext*, const DrawModelState_t&, const ModelRenderInfo_t&, matrix3x4_t*);
		void __stdcall hook(IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& info, matrix3x4_t* bone_to_world);
	}

	inline hooked_dme::fn o_dme = nullptr;

	/*namespace hooked_emitsound {
		using fn = void(__thiscall*)(void*, IRecipientFilter&, int, int, const char*, unsigned int, const char*, float, int, float, int, int, const Vector*, const Vector*, void*, bool, float, int, int);
		void __stdcall hook(IRecipientFilter& filter, int iEntIndex, int iChannel, const char* pSoundEntry, unsigned int nSoundEntryHash, const char* pSample, float flVolume, int nSeed, float flAttenuation, int iFlags, int iPitch, const Vector* pOrigin, const Vector* pDirection, void* pUtlVecOrigins, bool bUpdatePositions, float soundtime, int speakerentity, int unk);
	}

	inline hooked_emitsound::fn o_emitsound = nullptr;*/

	namespace hooked_lockcursor {
		using fn = void(__thiscall*)(void*);
		void __stdcall hook();
	}

	inline hooked_lockcursor::fn o_cursor = nullptr;

	namespace hooked_listleavesinbox {
		using fn = int(__thiscall*)(void*, const Vector&, const Vector&, unsigned short*, int);
		int __fastcall hook(void* ecx, void* edx, Vector& mins, Vector& maxs, unsigned short* list, int list_max);
	}

	inline hooked_listleavesinbox::fn o_bsp = nullptr;

	namespace hooked_runcommand {
		using fn = void(__thiscall*)(void*, player_t*, CUserCmd*, IMoveHelper*);
		void __fastcall hook(void* ecx, void* edx, player_t* player, CUserCmd* m_pcmd, IMoveHelper* move_helper);
	}

	inline hooked_runcommand::fn o_runcommand = nullptr;

	namespace hooked_clip_ray_collideable {
		using fn = void(__thiscall*)(void*, const Ray_t&, uint32_t, ICollideable*, CGameTrace*);
		void __fastcall hook(void* ecx, void* edx, const Ray_t& ray, uint32_t fMask, ICollideable* pCollide, CGameTrace* pTrace);
	}

	inline hooked_clip_ray_collideable::fn o_trace_clip = nullptr;

	namespace hooked_trace_ray {
		using fn = void(__thiscall*)(void*, const Ray_t&, unsigned int, ITraceFilter*, trace_t*);
		void __fastcall hook(void* ecx, void* edx, const Ray_t& ray, unsigned int fMask, ITraceFilter* pTraceFilter, trace_t* pTrace);
	}

	inline hooked_trace_ray::fn o_trace_ray = nullptr;

	namespace hooked_loosefileallowed {
		using fn = bool(__thiscall*)();
		bool __fastcall hook(void* ecx, void* edx);
	}

	inline hooked_loosefileallowed::fn o_loadfiles = nullptr;

	struct recv_proxy
	{
		static recvProxyData* hook;
		using fn = void(__thiscall*)(const recvProxyData* data, void* entity, void* output);
		static void hooked(const recvProxyData* data, void* entity, void* output);
	};

	extern C_HookedEvents hooked_events;

	using GetForeignFallbackFontNameFn = const char* (__thiscall*)(void*);
	using SetupBonesFn = bool(__thiscall*)(void*, matrix3x4_t*, int, int, float);
	using DoExtraBonesProcessingFn = void(__thiscall*)(player_t*, CStudioHdr*, Vector*, Quaternion*, const matrix3x4_t&, uint8_t*, void*);
	using StandardBlendingRulesFn = void(__thiscall*)(player_t*, CStudioHdr*, Vector*, Quaternion*, float, int);
	using UpdateClientSideAnimationFn = void(__fastcall*)(player_t*);
	using PhysicsSimulateFn = void(__fastcall*)(player_t*);
	using ModifyEyePositionFn = void(__thiscall*)(c_baseplayeranimationstate*, Vector&);
	using CalcViewmodelBobFn = void(__thiscall*)(player_t*, Vector&);
	using ProcessInterpolatedListFn = int(*)(void);

	extern DWORD original_getforeignfallbackfontname;
	extern DWORD original_setupbones;
	extern DWORD original_doextrabonesprocessing;
	extern DWORD original_standardblendingrules;
	extern DWORD original_updateclientsideanimation;
	extern DWORD original_physicssimulate;
	extern DWORD original_modifyeyeposition;
	extern DWORD original_calcviewmodelbob;
	extern DWORD original_processinterpolatedlist;

	const char* __fastcall hooked_getforeignfallbackfontname(void* ecx, uint32_t i);
	_declspec(noinline)const char* getforeignfallbackfontname_detour(void* ecx, uint32_t i);
	bool __fastcall hooked_setupbones(void* ecx, void* edx, matrix3x4_t* bone_world_out, int max_bones, int bone_mask, float current_time);
	_declspec(noinline)bool setupbones_detour(void* ecx, matrix3x4_t* bone_world_out, int max_bones, int bone_mask, float current_time);
	void __fastcall hooked_doextrabonesprocessing(player_t* player, void* edx, CStudioHdr* hdr, Vector* pos, Quaternion* q, const matrix3x4_t& matrix, uint8_t* bone_list, void* context);
	_declspec(noinline)void doextrabonesprocessing_detour(player_t* player, CStudioHdr* hdr, Vector* pos, Quaternion* q, const matrix3x4_t& matrix, uint8_t* bone_list, void* context);
	void __fastcall hooked_standardblendingrules(player_t* player, int i, CStudioHdr* hdr, Vector* pos, Quaternion* q, float curtime, int boneMask);
	_declspec(noinline)void standardblendingrules_detour(player_t* player, int i, CStudioHdr* hdr, Vector* pos, Quaternion* q, float curtime, int boneMask);
	void __fastcall hooked_updateclientsideanimation(player_t* player, uint32_t i);
	_declspec(noinline)void updateclientsideanimation_detour(player_t* player);
	void __fastcall hooked_physicssimulate(player_t* player);
	_declspec(noinline)void physicssimulate_detour(player_t* player);
	void __fastcall hooked_modifyeyeposition(c_baseplayeranimationstate* state, void* edx, Vector& position);
	_declspec(noinline)void modifyeyeposition_detour(c_baseplayeranimationstate* state, Vector& position);
	void __fastcall hooked_calcviewmodelbob(player_t* player, void* edx, Vector& position);
	_declspec(noinline)void calcviewmodelbob_detour(player_t* player, Vector& position);
	bool __fastcall hooked_shouldskipanimframe();
	int processinterpolatedlist();
	void __fastcall hooked_checkfilecrcswithserver(void* ecx, void* edx);
	LRESULT __stdcall Hooked_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void GUI_Init(IDirect3DDevice9* pDevice);

	//	typedef long(__stdcall *EndSceneFn)(IDirect3DDevice9* device);
	//	typedef long(__stdcall *EndSceneResetFn)(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
	//	typedef long(__stdcall* PresentFn)(IDirect3DDevice9*, RECT*, RECT*, HWND, RGNDATA*);

	typedef void(__thiscall* SetKeyCodeState_t) (void*, ButtonCode_t, bool);
	extern SetKeyCodeState_t o_SetKeyCodeState;

	typedef void(__thiscall* SetMouseCodeState_t) (void*, ButtonCode_t, MouseCodeState_t);
	extern SetMouseCodeState_t o_SetMouseCodeState;
}


extern LRESULT ImGui_ImplDX9_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);





namespace shonax_hooks
{

	//using GCRetrieveMessage = unsigned long(__thiscall*)(void*, uint32_t* punMsgType, void* pubDest, uint32_t cubDest, uint32_t* pcubMsgSize);
	//using GCSendMessage = unsigned long(__thiscall*)(void*, uint32_t unMsgType, const void* pubData, uint32_t cubData);

	//inline GCRetrieveMessage oGCRetrieveMessage;
	//inline GCSendMessage oGCSendMessage;

	//extern unsigned long __fastcall GCRetrieveMessageHook(void* ecx, void*, uint32_t* punMsgType, void* pubDest, uint32_t cubDest, uint32_t* pcubMsgSize);
	//extern unsigned long __fastcall GCSendMessageHook(void* ecx, void*, uint32_t unMsgType, const void* pubData, uint32_t cubData);



	//inline recvProxy sequence_hook;
	//extern void RecvProxy(recvProxyData& pData, void* entity, void* output);
	//inline recvProxy viewmodel_hook;
	//extern void Viewmodel(recvProxyData& pData, void* pStruct, void* pOut);



	typedef bool(__thiscall* FireEventClientSidefn)(void*, IGameEvent*);
	inline FireEventClientSidefn oFireEventClientSidefn;


	extern bool __fastcall FireEventClientSide(void* ecx, void* edx, IGameEvent* pEvent);
}