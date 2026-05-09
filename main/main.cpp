/*
 * SAMP AML Menu - Main entry point
 * Loaded by Android Mod Loader (AML) into libGTASA.so process
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <dlfcn.h>
#include <jni.h>
#include <android/log.h>

#include "mod/amlmod.h"
#include "mod/logger.h"
#include "mod/config.h"
#include "mod.h"
#include "menu.h"

MYMOD(com.samp.amlmenu, SAMP-AML-Menu, 1.0, GhostHacker)
NEEDGAME(com.rockstargames.gtasa)

uintptr_t g_GTASA  = 0;
uintptr_t g_libsmp = 0;

ModState g_ModState = {};

/* ----- Game function pointers (resolved at load time) -----
 * NOTE: Symbol names below are illustrative. For real builds, dump exact
 * mangled names from libGTASA.so / libsamp.so using IDA Pro / Ghidra and
 * adjust the symbol strings accordingly. The mod still compiles cleanly
 * even if a symbol fails to resolve at runtime - the corresponding feature
 * just becomes a no-op (logged as a warning).
 */
typedef void  (*tCreateVehicle)(int modelId, float x, float y, float z, float angle);
typedef void  (*tSetPlayerPos) (float x, float y, float z);
typedef void  (*tRequestModel) (int modelId, int flags);
typedef void  (*tLoadAllReq)   (bool priority);
typedef void  (*tGiveWeapon)   (int weapon, int ammo, bool unk);

tCreateVehicle p_CreateVehicle = nullptr;
tSetPlayerPos  p_SetPlayerPos  = nullptr;
tRequestModel  p_RequestModel  = nullptr;
tLoadAllReq    p_LoadAllReq    = nullptr;
tGiveWeapon    p_GiveWeapon    = nullptr;

/* ----- Game frame hook ----- */
typedef void (*tGameLoop)(void);
tGameLoop o_GameLoop = nullptr;

void H_GameLoop() {
    Mod_OnFrame();
    Menu_Render();
    if (o_GameLoop) o_GameLoop();
}

/* ----- Touch input hook -----
 * libGTASA exposes touch events through CPad / OnTouchEvent.
 * We hook to allow the menu to consume touches when open.
 */
typedef bool (*tOnTouch)(float x, float y, int action, int idx);
tOnTouch o_OnTouch = nullptr;

bool H_OnTouch(float x, float y, int action, int idx) {
    if (g_ModState.menuOpen) {
        Menu_HandleTouch(x, y, action);
        return true; // consume
    }
    return o_OnTouch ? o_OnTouch(x, y, action, idx) : false;
}

/* ----- Volume / hardware key hook for opening the menu -----
 * Pressing VOLUME_UP toggles the menu visibility.
 */
typedef int (*tProcessKey)(int keyCode, int action);
tProcessKey o_ProcessKey = nullptr;

int H_ProcessKey(int keyCode, int action) {
    // 24 = KEYCODE_VOLUME_UP, action 0 = down
    if (keyCode == 24 && action == 0) {
        Menu_Toggle();
        return 1;
    }
    return o_ProcessKey ? o_ProcessKey(keyCode, action) : 0;
}

/* ----- Helpers ----- */
static void TryResolve(void** out, uintptr_t lib, const char* sym) {
    if (!AML || !lib) return;
    void* v = AML->GetSym(lib, sym);
    if (v) {
        *out = v;
        logger->Info("Resolved %s = %p", sym, v);
    } else {
        logger->Warn("Symbol not found: %s (feature will be disabled)", sym);
    }
}

/* ----- Mod entry point (called by AML) ----- */
extern "C" void OnModLoad() {
    logger->Info("==============================================");
    logger->Info(" SAMP AML Menu - loading...");
    logger->Info("==============================================");

    if (!AML) {
        logger->Error("AML interface is NULL - is this loaded by AML?");
        return;
    }

    g_GTASA  = (uintptr_t)AML->GetLib(LIB_GTASA);
    g_libsmp = (uintptr_t)AML->GetLib(LIB_SAMP);

    if (!g_GTASA) {
        logger->Error("libGTASA.so not loaded - aborting.");
        return;
    }
    logger->Info("libGTASA.so base = 0x%lx", (unsigned long)g_GTASA);
    if (g_libsmp) logger->Info("libsamp.so  base = 0x%lx", (unsigned long)g_libsmp);
    else          logger->Warn("libsamp.so not found - SAMP features will be limited.");

    // ---- Resolve common engine functions (replace with real symbols) ----
    TryResolve((void**)&p_CreateVehicle, g_GTASA, "_Z14CarCtrlCreateifff");
    TryResolve((void**)&p_SetPlayerPos,  g_GTASA, "_ZN9CPlayerPed6SetPosEfff");
    TryResolve((void**)&p_RequestModel,  g_GTASA, "_ZN9CStreaming12RequestModeli");
    TryResolve((void**)&p_LoadAllReq,    g_GTASA, "_ZN9CStreaming20LoadAllRequestedModelsEb");
    TryResolve((void**)&p_GiveWeapon,    g_GTASA, "_ZN9CPlayerPed10GiveWeaponEiib");

    // ---- Initialize subsystems ----
    Mod_Initialize();
    Menu_Initialize();

    logger->Info("SAMP AML Menu loaded. Press VOLUME_UP to toggle menu.");
}
