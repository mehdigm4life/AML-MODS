#ifndef MOD_CONFIG_H
#define MOD_CONFIG_H

#include <stdio.h>
#include <stdint.h>
#include <string>

class CConfig {
public:
    bool LoadBool(const char* /*key*/, bool def)  { return def; }
    int  LoadInt (const char* /*key*/, int  def)  { return def; }
    void Save() {}
};

static CConfig g_Config;
#define config (&g_Config)

#endif
