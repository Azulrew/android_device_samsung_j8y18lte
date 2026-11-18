/*
   Copyright (c) 2016, The CyanogenMod Project. All rights reserved.
   SPDX-License-Identifier: Apache-2.0
 */

#include <stdlib.h>
#include <string.h>
#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/properties.h>
#include <android-base/strings.h>

#include "vendor_init.h"

using android::base::GetProperty;

static void property_override(char const prop[], char const value[], bool add = true)
{
    auto pi = (prop_info *) __system_property_find(prop);

    if (pi != nullptr) {
        __system_property_update(pi, value, strlen(value));
    } else if (add) {
        __system_property_add(prop, strlen(prop), value, strlen(value));
    }
}

static void set_ro_build_prop(const std::string &prop, const std::string &value)
{
    const char *partitions[] = { "", "odm.", "product.", "system.", "system_ext.", "vendor." };
    for (const char *p : partitions) {
        std::string prop_name = std::string("ro.") + p + prop;
        property_override(prop_name.c_str(), value.c_str());
    }
}

static void set_ro_product_prop(const std::string &prop, const std::string &value)
{
    const char *partitions[] = { "", "odm.", "product.", "system.", "system_ext.", "vendor." };
    for (const char *p : partitions) {
        std::string prop_name = std::string("ro.product.") + p + prop;
        property_override(prop_name.c_str(), value.c_str());
    }
}

static void set_dalvik_heap()
{
    property_override("dalvik.vm.heapstartsize", "14m");
    property_override("dalvik.vm.heapgrowthlimit", "192m");
    property_override("dalvik.vm.heapsize", "384m");
    property_override("dalvik.vm.heaptargetutilization", "0.75");
    property_override("dalvik.vm.heapminfree", "6m");
    property_override("dalvik.vm.heapmaxfree", "8m");
}

void vendor_load_properties()
{
    std::string bootloader = GetProperty("ro.bootloader", "");

    if (bootloader.find("J810G") != std::string::npos) {
        /* SM-J810G */
        set_ro_build_prop("build.fingerprint", "samsung/j8y18ltedd/j8y18lte:10/QP1A.190711.020/J810GDDU6CUI1:user/release-keys");
        property_override("ro.build.description", "j8y18ltedd-user 10 QP1A.190711.020 J810GDDU6CUI1 release-keys");
        set_ro_product_prop("model", "SM-J810G");
        set_ro_product_prop("device", "j8y18lte");
    } else if (bootloader.find("J810F") != std::string::npos) {
        /* SM-J810F */
        set_ro_build_prop("build.fingerprint", "samsung/j8y18ltejv/j8y18lte:10/QP1A.190711.020/J810FJXU6CVA1:user/release-keys");
        property_override("ro.build.description", "j8y18ltejv-user 10 QP1A.190711.020 J810FJXU6CVA1 release-keys");
        set_ro_product_prop("model", "SM-J810F");
        set_ro_product_prop("device", "j8y18lte");
    } else if (bootloader.find("J810Y") != std::string::npos) {
        /* SM-J810Y */
        set_ro_build_prop("build.fingerprint", "samsung/j8y18ltedx/j8y18lte:10/QP1A.190711.020/J810YDXU6CVA1:user/release-keys");
        property_override("ro.build.description", "j8y18ltedx-user 10 QP1A.190711.020 J810YDXU6CVA1 release-keys");
        set_ro_product_prop("model", "SM-J810Y");
        set_ro_product_prop("device", "j8y18lte");
    } else {
        /* Default / SM-J810M (Brazil / Latin America) */
        set_ro_build_prop("build.fingerprint", "samsung/j8y18lteub/j8y18lte:10/QP1A.190711.020/J810MUBU3CUK1:user/release-keys");
        property_override("ro.build.description", "j8y18lteub-user 10 QP1A.190711.020 J810MUBU3CUK1 release-keys");
        set_ro_product_prop("model", "SM-J810M");
        set_ro_product_prop("device", "j8y18lte");
    }

    set_dalvik_heap();
}
