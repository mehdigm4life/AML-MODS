LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE        := SAMPAMLMenu
LOCAL_CFLAGS        := -Wno-error=format-security -fvisibility=hidden -O2 -DNDEBUG
LOCAL_CPPFLAGS      := -std=c++17 -fvisibility=hidden -fno-rtti -fno-exceptions -O2
LOCAL_LDFLAGS       := -Wl,--hash-style=both
LOCAL_LDLIBS        := -llog -landroid

LOCAL_C_INCLUDES    := $(LOCAL_PATH)/includes \
                       $(LOCAL_PATH)/main

LOCAL_SRC_FILES     := main/main.cpp \
                       main/menu.cpp \
                       main/mod.cpp

include $(BUILD_SHARED_LIBRARY)
