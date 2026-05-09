#include "mod.h"
#include "mod/amlmod.h"
#include "mod/logger.h"
#include <math.h>
#include <string.h>

extern "C" {
    typedef void (*tCreateVehicle)(int, float, float, float, float);
    typedef void (*tSetPlayerPos) (float, float, float);
    typedef void (*tGiveWeapon)   (int, int, bool);

    extern tCreateVehicle p_CreateVehicle;
    extern tSetPlayerPos  p_SetPlayerPos;
    extern tGiveWeapon    p_GiveWeapon;
}

/* ===== Player structure (very simplified) =====
 * Real CPlayerPed has hundreds of fields; we only access a few via offsets.
 * Offsets shown are placeholders - update for your specific GTA SA APK build.
 */
struct CPlayerPed {
    uint8_t  pad0[0x540];
    float    health;
    float    maxHealth;
    uint8_t  pad1[0x10];
    float    armor;
};

static CPlayerPed* GetLocalPlayer() {
    if (!g_GTASA) return nullptr;
    // FindPlayerPed(int) - placeholder offset (verify with IDA)
    typedef CPlayerPed* (*FindPlayerPed_t)(int);
    static FindPlayerPed_t fn = (FindPlayerPed_t)(g_GTASA + 0x4FC4D8);
    return fn(-1);
}

static void GetPlayerPos(float& x, float& y, float& z) {
    x = 0.0f; y = 0.0f; z = 0.0f;
    CPlayerPed* p = GetLocalPlayer();
    if (!p) return;
    // Position lives in the entity matrix - placeholder offset
    float* pos = (float*)((uintptr_t)p + 0x30);
    x = pos[0]; y = pos[1]; z = pos[2];
}

void Mod_Initialize() {
    memset(&g_ModState, 0, sizeof(g_ModState));
    g_ModState.speedMultiplier = 2.0f;
    g_ModState.jumpMultiplier  = 1.8f;
    logger->Info("Mod state initialized.");
}

/* ===== Per-frame logic =====
 * Anything that needs to be "force-applied" every frame goes here.
 */
void Mod_OnFrame() {
    CPlayerPed* p = GetLocalPlayer();
    if (!p) return;

    if (g_ModState.godmode) {
        p->health    = 200.0f;
        p->maxHealth = 200.0f;
        p->armor     = 100.0f;
    }

    if (g_ModState.invisible) {
        // Visibility byte / alpha flag - placeholder offset
        *(uint8_t*)((uintptr_t)p + 0x3E) = 0;
    }

    if (g_ModState.noFall) {
        // Falling damage flag - placeholder
        *(uint8_t*)((uintptr_t)p + 0x46C) = 0;
    }

    // SpeedHack / JumpHack / InfiniteAmmo / NoReload would be applied
    // either here (per-frame patches) or via permanent code patches at init.
}

/* ===== Feature implementations ===== */

void SpawnVehicle(int modelId) {
    if (!p_CreateVehicle) {
        logger->Warn("CreateVehicle not resolved - cannot spawn %d", modelId);
        return;
    }
    float x, y, z;
    GetPlayerPos(x, y, z);
    p_CreateVehicle(modelId, x + 3.0f, y + 3.0f, z + 1.0f, 0.0f);
    logger->Info("Spawned vehicle id=%d at (%.1f, %.1f, %.1f)", modelId, x, y, z);
}

void GiveMoney(int amount) {
    if (!g_GTASA) return;
    // CWorld::Players[0].m_nMoney - placeholder offset
    int* pMoney = (int*)(g_GTASA + 0x96A8DC);
    *pMoney += amount;
    logger->Info("Money +%d (now %d)", amount, *pMoney);
}

void TeleportPlayer(float x, float y, float z) {
    if (p_SetPlayerPos) {
        p_SetPlayerPos(x, y, z);
        logger->Info("Teleport -> (%.1f, %.1f, %.1f)", x, y, z);
    } else {
        // Fallback: write directly into the position matrix
        CPlayerPed* p = GetLocalPlayer();
        if (!p) return;
        float* pos = (float*)((uintptr_t)p + 0x30);
        pos[0] = x; pos[1] = y; pos[2] = z;
        logger->Info("Teleport (raw) -> (%.1f, %.1f, %.1f)", x, y, z);
    }
}

void SetPlayerHealth(float h) {
    CPlayerPed* p = GetLocalPlayer();
    if (p) { p->health = h; logger->Info("Health = %.1f", h); }
}

void SetPlayerArmor(float a) {
    CPlayerPed* p = GetLocalPlayer();
    if (p) { p->armor = a; logger->Info("Armor = %.1f", a); }
}

void GiveWeapon(int weaponId, int ammo) {
    if (!p_GiveWeapon) { logger->Warn("GiveWeapon not resolved"); return; }
    p_GiveWeapon(weaponId, ammo, true);
    logger->Info("Gave weapon %d with %d ammo", weaponId, ammo);
}

void SetWantedLevel(int level) {
    if (!g_GTASA) return;
    // CWanted::SetWantedLevel - placeholder offset
    int* pWanted = (int*)(g_GTASA + 0x96AAC0);
    *pWanted = level;
    logger->Info("Wanted level = %d", level);
}

void SetTimeOfDay(int hour) {
    if (!g_GTASA) return;
    int* pHour = (int*)(g_GTASA + 0x9712C0); // placeholder
    *pHour = hour;
    logger->Info("Time = %d:00", hour);
}

void SetWeather(int id) {
    if (!g_GTASA) return;
    int* pWeather = (int*)(g_GTASA + 0x9712D0); // placeholder
    *pWeather = id;
    logger->Info("Weather = %d", id);
}

void RepairVehicle()  { logger->Info("RepairVehicle (stub)"); }
void FlipVehicle()    { logger->Info("FlipVehicle (stub)"); }
void ExplodeNearby()  { logger->Info("ExplodeNearby (stub)"); }
