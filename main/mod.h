#ifndef MOD_H
#define MOD_H

#include <stdint.h>

#define LIB_GTASA "libGTASA.so"
#define LIB_SAMP  "libsamp.so"

extern uintptr_t g_GTASA;
extern uintptr_t g_libsmp;

struct ModState {
    bool menuOpen;
    bool invisible;
    bool godmode;
    bool infiniteAmmo;
    bool noReload;
    bool speedHack;
    bool jumpHack;
    bool noFall;
    bool freezeTime;
    bool wallhack;
    bool autoAim;
    bool nightVision;
    bool weaponPickup;
    float speedMultiplier;
    float jumpMultiplier;
};

extern ModState g_ModState;

void Mod_Initialize();
void Mod_OnFrame();

void SpawnVehicle(int modelId);
void GiveMoney(int amount);
void TeleportPlayer(float x, float y, float z);
void SetPlayerHealth(float health);
void SetPlayerArmor(float armor);
void GiveWeapon(int weaponId, int ammo);
void SetWantedLevel(int level);
void SetTimeOfDay(int hour);
void SetWeather(int id);
void RepairVehicle();
void FlipVehicle();
void ExplodeNearby();

#endif
