#include "menu.h"
#include "mod.h"
#include "mod/amlmod.h"
#include "mod/logger.h"
#include <vector>
#include <string>

/* Drawing primitives -----------------------------------------------------
 * These are weak stubs. In a real build you would point them to the game's
 * RenderWare / ImGui draw calls (resolved via AML->GetSym). When the symbols
 * are missing the menu silently does nothing, but the rest of the mod still
 * works (toggle states + log output).
 */
extern "C" __attribute__((weak)) void DrawRect(float x, float y, float w, float h, unsigned int color);
extern "C" __attribute__((weak)) void DrawText(const char* text, float x, float y, float size, unsigned int color);

static void SafeRect(float x, float y, float w, float h, unsigned int c) {
    if (DrawRect) DrawRect(x, y, w, h, c);
}
static void SafeText(const char* t, float x, float y, float s, unsigned int c) {
    if (DrawText) DrawText(t, x, y, s, c);
}

/* Menu items ------------------------------------------------------------- */

enum MenuItemType { ITEM_HEADER = -1, ITEM_TOGGLE = 0, ITEM_BUTTON = 1 };

struct MenuItem {
    std::string  label;
    int          type;
    bool*        boolValue;
    void       (*callback)();
};

static std::vector<MenuItem> g_Items;
static int   g_Selected = 0;
static float g_MenuX = 40.0f;
static float g_MenuY = 80.0f;
static float g_MenuW = 360.0f;
static float g_RowH  = 26.0f;

/* Callbacks -------------------------------------------------------------- */
static void cb_SpawnInfernus() { SpawnVehicle(411); }
static void cb_SpawnBullet()   { SpawnVehicle(541); }
static void cb_SpawnTurismo()  { SpawnVehicle(451); }
static void cb_SpawnNRG()      { SpawnVehicle(522); }
static void cb_SpawnSultan()   { SpawnVehicle(560); }
static void cb_SpawnHydra()    { SpawnVehicle(520); }
static void cb_SpawnHunter()   { SpawnVehicle(425); }
static void cb_SpawnRhino()    { SpawnVehicle(432); }
static void cb_SpawnSanchez()  { SpawnVehicle(468); }
static void cb_SpawnPCJ()      { SpawnVehicle(461); }

static void cb_Money1M()       { GiveMoney(1000000); }
static void cb_Money10M()      { GiveMoney(10000000); }
static void cb_Money100M()     { GiveMoney(100000000); }

static void cb_TpLS()          { TeleportPlayer( 2495.0f, -1687.0f,  13.5f); }
static void cb_TpSF()          { TeleportPlayer(-1985.0f,   137.0f,  27.6f); }
static void cb_TpLV()          { TeleportPlayer( 2027.0f,  1343.0f,  10.8f); }
static void cb_TpA51()         { TeleportPlayer(  135.0f,  1948.0f,  19.7f); }
static void cb_TpMtChiliad()   { TeleportPlayer(-2334.0f,  -1635.0f, 483.7f); }

static void cb_Heal()          { SetPlayerHealth(200.0f); SetPlayerArmor(100.0f); }
static void cb_NoWanted()      { SetWantedLevel(0); }
static void cb_MaxWanted()     { SetWantedLevel(6); }
static void cb_TimeNoon()      { SetTimeOfDay(12); }
static void cb_TimeMidnight()  { SetTimeOfDay(0); }
static void cb_WeatherSunny()  { SetWeather(0); }
static void cb_WeatherStorm()  { SetWeather(8); }

static void cb_GiveDeagle()    { GiveWeapon(24, 999); }
static void cb_GiveAK47()      { GiveWeapon(30, 9999); }
static void cb_GiveM4()        { GiveWeapon(31, 9999); }
static void cb_GiveSniper()    { GiveWeapon(34, 999); }
static void cb_GiveRPG()       { GiveWeapon(35, 99); }
static void cb_GiveMinigun()   { GiveWeapon(38, 9999); }

/* Init ------------------------------------------------------------------- */
void Menu_Initialize() {
    g_Items.clear();

    g_Items.push_back({"--- HACKS / المخترقات ---",  ITEM_HEADER, nullptr, nullptr});
    g_Items.push_back({"Invisible (شفاف)",          ITEM_TOGGLE, &g_ModState.invisible,    nullptr});
    g_Items.push_back({"Godmode (لا تموت)",         ITEM_TOGGLE, &g_ModState.godmode,      nullptr});
    g_Items.push_back({"Infinite Ammo",             ITEM_TOGGLE, &g_ModState.infiniteAmmo, nullptr});
    g_Items.push_back({"No Reload",                 ITEM_TOGGLE, &g_ModState.noReload,     nullptr});
    g_Items.push_back({"Speed Hack",                ITEM_TOGGLE, &g_ModState.speedHack,    nullptr});
    g_Items.push_back({"Jump Hack",                 ITEM_TOGGLE, &g_ModState.jumpHack,     nullptr});
    g_Items.push_back({"No Fall Damage",            ITEM_TOGGLE, &g_ModState.noFall,       nullptr});
    g_Items.push_back({"Wallhack",                  ITEM_TOGGLE, &g_ModState.wallhack,     nullptr});
    g_Items.push_back({"Auto Aim",                  ITEM_TOGGLE, &g_ModState.autoAim,      nullptr});
    g_Items.push_back({"Night Vision",              ITEM_TOGGLE, &g_ModState.nightVision,  nullptr});
    g_Items.push_back({"Freeze Time",               ITEM_TOGGLE, &g_ModState.freezeTime,   nullptr});

    g_Items.push_back({"--- SPAWN VEHICLES ---",    ITEM_HEADER, nullptr, nullptr});
    g_Items.push_back({"Infernus",                  ITEM_BUTTON, nullptr, cb_SpawnInfernus});
    g_Items.push_back({"Bullet",                    ITEM_BUTTON, nullptr, cb_SpawnBullet});
    g_Items.push_back({"Turismo",                   ITEM_BUTTON, nullptr, cb_SpawnTurismo});
    g_Items.push_back({"NRG-500",                   ITEM_BUTTON, nullptr, cb_SpawnNRG});
    g_Items.push_back({"Sultan",                    ITEM_BUTTON, nullptr, cb_SpawnSultan});
    g_Items.push_back({"Hydra",                     ITEM_BUTTON, nullptr, cb_SpawnHydra});
    g_Items.push_back({"Hunter",                    ITEM_BUTTON, nullptr, cb_SpawnHunter});
    g_Items.push_back({"Rhino Tank",                ITEM_BUTTON, nullptr, cb_SpawnRhino});
    g_Items.push_back({"Sanchez",                   ITEM_BUTTON, nullptr, cb_SpawnSanchez});
    g_Items.push_back({"PCJ-600",                   ITEM_BUTTON, nullptr, cb_SpawnPCJ});

    g_Items.push_back({"--- MONEY ---",             ITEM_HEADER, nullptr, nullptr});
    g_Items.push_back({"Give $1,000,000",           ITEM_BUTTON, nullptr, cb_Money1M});
    g_Items.push_back({"Give $10,000,000",          ITEM_BUTTON, nullptr, cb_Money10M});
    g_Items.push_back({"Give $100,000,000",         ITEM_BUTTON, nullptr, cb_Money100M});

    g_Items.push_back({"--- TELEPORT ---",          ITEM_HEADER, nullptr, nullptr});
    g_Items.push_back({"Los Santos",                ITEM_BUTTON, nullptr, cb_TpLS});
    g_Items.push_back({"San Fierro",                ITEM_BUTTON, nullptr, cb_TpSF});
    g_Items.push_back({"Las Venturas",              ITEM_BUTTON, nullptr, cb_TpLV});
    g_Items.push_back({"Area 51",                   ITEM_BUTTON, nullptr, cb_TpA51});
    g_Items.push_back({"Mount Chiliad",             ITEM_BUTTON, nullptr, cb_TpMtChiliad});

    g_Items.push_back({"--- WEAPONS ---",           ITEM_HEADER, nullptr, nullptr});
    g_Items.push_back({"Desert Eagle",              ITEM_BUTTON, nullptr, cb_GiveDeagle});
    g_Items.push_back({"AK-47",                     ITEM_BUTTON, nullptr, cb_GiveAK47});
    g_Items.push_back({"M4",                        ITEM_BUTTON, nullptr, cb_GiveM4});
    g_Items.push_back({"Sniper Rifle",              ITEM_BUTTON, nullptr, cb_GiveSniper});
    g_Items.push_back({"RPG",                       ITEM_BUTTON, nullptr, cb_GiveRPG});
    g_Items.push_back({"Minigun",                   ITEM_BUTTON, nullptr, cb_GiveMinigun});

    g_Items.push_back({"--- WORLD ---",             ITEM_HEADER, nullptr, nullptr});
    g_Items.push_back({"Heal Full Health+Armor",    ITEM_BUTTON, nullptr, cb_Heal});
    g_Items.push_back({"Clear Wanted",              ITEM_BUTTON, nullptr, cb_NoWanted});
    g_Items.push_back({"Max Wanted (6 stars)",      ITEM_BUTTON, nullptr, cb_MaxWanted});
    g_Items.push_back({"Time: Noon",                ITEM_BUTTON, nullptr, cb_TimeNoon});
    g_Items.push_back({"Time: Midnight",            ITEM_BUTTON, nullptr, cb_TimeMidnight});
    g_Items.push_back({"Weather: Sunny",            ITEM_BUTTON, nullptr, cb_WeatherSunny});
    g_Items.push_back({"Weather: Storm",            ITEM_BUTTON, nullptr, cb_WeatherStorm});

    logger->Info("Menu initialized: %zu items.", g_Items.size());
}

void Menu_Toggle() {
    g_ModState.menuOpen = !g_ModState.menuOpen;
    logger->Info("Menu %s", g_ModState.menuOpen ? "OPENED" : "CLOSED");
}

/* Render ----------------------------------------------------------------- */
void Menu_Render() {
    if (!g_ModState.menuOpen) return;

    float h = g_RowH * (float)g_Items.size() + 60.0f;
    SafeRect(g_MenuX,        g_MenuY,        g_MenuW, h,        0xCC101010);
    SafeRect(g_MenuX,        g_MenuY,        g_MenuW, 36.0f,    0xFF1E90FF);

    SafeText("SAMP AML MENU",          g_MenuX + 12.0f, g_MenuY + 8.0f, 1.1f, 0xFFFFFFFF);
    SafeText("VOL_UP = open/close",    g_MenuX + 12.0f, g_MenuY + 24.0f, 0.7f, 0xFFCCCCCC);

    float y = g_MenuY + 44.0f;
    for (size_t i = 0; i < g_Items.size(); ++i) {
        const MenuItem& it = g_Items[i];
        unsigned int col = 0xFFFFFFFF;
        std::string text = it.label;

        if (it.type == ITEM_HEADER) {
            col = 0xFFFFA500;
        } else if (it.type == ITEM_TOGGLE && it.boolValue) {
            bool on = *(it.boolValue);
            text += on ? "   [ON]" : "   [OFF]";
            col   = on ? 0xFF00FF66 : 0xFFCCCCCC;
        } else if (it.type == ITEM_BUTTON) {
            col = 0xFFFFFFFF;
        }
        if ((int)i == g_Selected) col = 0xFFFFFF00;

        SafeText(text.c_str(), g_MenuX + 12.0f, y, 0.85f, col);
        y += g_RowH;
    }
}

/* Touch handling --------------------------------------------------------- */
void Menu_HandleTouch(float x, float y, int action) {
    if (!g_ModState.menuOpen) return;
    if (action != 0) return; // 0 = ACTION_DOWN

    if (x < g_MenuX || x > g_MenuX + g_MenuW) return;
    if (y < g_MenuY + 44.0f) return;

    int idx = (int)((y - (g_MenuY + 44.0f)) / g_RowH);
    if (idx < 0 || idx >= (int)g_Items.size()) return;

    MenuItem& it = g_Items[idx];
    g_Selected = idx;

    if (it.type == ITEM_TOGGLE && it.boolValue) {
        *(it.boolValue) = !*(it.boolValue);
        logger->Info("Toggled '%s' -> %s", it.label.c_str(), *(it.boolValue) ? "ON" : "OFF");
    } else if (it.type == ITEM_BUTTON && it.callback) {
        logger->Info("Pressed '%s'", it.label.c_str());
        it.callback();
    }
}
