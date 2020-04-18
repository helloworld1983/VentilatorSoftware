/* Copyright 2020, Edwin Chiu

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef VERSION_H
#define VERSION_H
#include <types.h>
typedef union {
  struct {
    uint32_t major : 8;
    uint32_t minor : 8;
    uint32_t patch : 16;
  };
  uint32_t as_uint32;
} Version_t;

const Version_t version_fw = {0, 1, 0};
const Version_t version_hw = {0, 1, 0};

// Guaranteed to be 8 bytes excluding the null-terminator.
inline const char *version_getAbout() { return "01.01.01"; }
inline const Version_t version_getFwVersion() { return version_fw; }
inline const Version_t version_getHwVersion() { return version_hw; }

#endif // VERSION_H
