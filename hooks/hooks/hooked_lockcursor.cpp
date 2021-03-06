// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "..\hooks.hpp"

void __stdcall hooks::hooked_lockcursor::hook()
{
	if (!menu_open)
		return o_cursor(m_surface());

	m_surface()->UnlockCursor();
}

/*using LockCursor_t = void(__thiscall*)(void*);

void __stdcall hooks::hooked_lockcursor() 
{
	static auto original_fn = surface_hook->get_func_address <LockCursor_t> (67);

	if (!menu_open)
		return original_fn(m_surface());

	m_surface()->UnlockCursor();
}*/

