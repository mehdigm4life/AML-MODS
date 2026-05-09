#ifndef AMLMOD_H
#define AMLMOD_H

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

/*
 * Minimal AML (Android Mod Loader) header - compatible with TheOfficialFloW / RusJJ AML.
 * For full feature set, replace this file with the official one from:
 *   https://github.com/TheOfficialFloW/AndroidModLoader
 *   https://github.com/RusJJ/SAAML-Template
 */

#define MYMOD(pkg, name, ver, author) \
    extern "C" __attribute__((visibility("default"))) const char* __MOD_PACKAGE = #pkg; \
    extern "C" __attribute__((visibility("default"))) const char* __MOD_NAME    = #name; \
    extern "C" __attribute__((visibility("default"))) const char* __MOD_VERSION = #ver;  \
    extern "C" __attribute__((visibility("default"))) const char* __MOD_AUTHOR  = #author;

#define NEEDGAME(pkg) \
    extern "C" __attribute__((visibility("default"))) const char* __MOD_GAME = #pkg;

#define HOOK(fn, addr)     AML->Hook((void*)(addr),    (void*)Hooked##fn, (void**)&fn)
#define HOOKPLT(fn, addr)  AML->HookPLT((void*)(addr), (void*)Hooked##fn, (void**)&fn)
#define SET_TO(var, addr)  var = (decltype(var))(addr)

class IAML {
public:
    virtual void* GetLib(const char* name) = 0;
    virtual void* GetSym(uintptr_t lib, const char* sym) = 0;
    virtual void  Hook(void* addr, void* fn, void** orig) = 0;
    virtual void  HookPLT(void* addr, void* fn, void** orig) = 0;
    virtual void  Unprot(uintptr_t addr, size_t size) = 0;
    virtual void  Write(uintptr_t addr, void* data, size_t size) = 0;
    virtual void  PlaceNOP(uintptr_t addr, size_t count) = 0;
    virtual void  PlaceRET(uintptr_t addr) = 0;
    virtual void  PlaceB(uintptr_t addr, uintptr_t target) = 0;
    virtual void  PlaceBL(uintptr_t addr, uintptr_t target) = 0;
};

extern "C" __attribute__((weak)) IAML* AML;

extern "C" __attribute__((visibility("default"))) void OnModLoad();

#endif
