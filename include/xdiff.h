/*
   Author: 熊义 (Aurson)
   Date: 2025-01-30
   Copyright 2025 jassimxiong@gmail.com

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

#ifndef __XDIFF_H__
#define __XDIFF_H__

#include <vector>
#include <cstdint>
#include <string>

struct DiffRecord {
    uint32_t offset;
    uint32_t old_length;
    uint32_t new_length;
    std::vector<uint8_t> new_data;
};
auto ReadFileData(const std::string& path) -> std::vector<uint8_t>;
auto GeneratePatch(const std::vector<uint8_t>& old_data, const std::vector<uint8_t>& new_data) -> std::vector<DiffRecord>;
bool WritePatchFile(const std::string& path, const std::vector<DiffRecord>& records);
auto ReadPatchFile(const std::string& path) -> std::vector<DiffRecord>;
bool ApplyPatch(const std::vector<uint8_t>& old_data, const std::string& new_path, const std::vector<DiffRecord>& records);

#endif // __XDIFF_H__