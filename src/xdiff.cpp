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

#include "xdiff.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <string>

auto ReadFileData(const std::string& path) -> std::vector<uint8_t>{
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file) {
        std::cerr << "无法打开文件: " << path << std::endl;
        return {};
    }
    
    size_t fileSize = file.tellg();
    file.seekg(0);
    
    std::vector<uint8_t> data(fileSize);
    if (!file.read(reinterpret_cast<char*>(data.data()), fileSize)) {
        std::cerr << "读取文件失败: " << path << std::endl;
        return {};
    }
    return data;
}

auto GeneratePatch(const std::vector<uint8_t>& old_data, const std::vector<uint8_t>& new_data) -> std::vector<DiffRecord>{
    std::vector<DiffRecord> records;
    size_t i = 0;
    size_t old_size = old_data.size();
    size_t new_size = new_data.size();

    while (i < old_size || i < new_size) {
        while (i < old_size && i < new_size && old_data[i] == new_data[i]) {
            ++i;
        }
        if (i >= old_size && i >= new_size) break;

        DiffRecord record;
        record.offset = static_cast<uint32_t>(i);

        // 计算旧数据范围
        size_t old_end = i;
        while (old_end < old_size && (old_end >= new_size || old_data[old_end] != new_data[old_end])) {
            ++old_end;
        }
        record.old_length = static_cast<uint32_t>(old_end - i);

        // 计算新数据范围
        size_t new_end = i;
        while (new_end < new_size && (new_end >= old_size || new_data[new_end] != old_data[new_end])) {
            ++new_end;
        }
        record.new_length = static_cast<uint32_t>(new_end - i);
        
        if (record.new_length > 0) {
            record.new_data.resize(record.new_length);
            std::copy(new_data.begin() + i, new_data.begin() + new_end, record.new_data.begin());
        }

        records.push_back(record);
        i = std::max(old_end, new_end);
    }

    sort(records.begin(), records.end(), [](const DiffRecord& a, const DiffRecord& b) {
        return a.offset > b.offset;
    });
    return records;
}

bool WritePatchFile(const std::string& path, const std::vector<DiffRecord>& records) {
    std::ofstream out(path, std::ios::binary);
    if (!out) return false;

    for (const auto& r : records) {
        out.write(reinterpret_cast<const char*>(&r.offset), sizeof(r.offset));
        out.write(reinterpret_cast<const char*>(&r.old_length), sizeof(r.old_length));
        out.write(reinterpret_cast<const char*>(&r.new_length), sizeof(r.new_length));
        if (r.new_length > 0) {
            out.write(reinterpret_cast<const char*>(r.new_data.data()), r.new_length);
        }
    }
    return true;
}

auto ReadPatchFile(const std::string& path) -> std::vector<DiffRecord>{
    std::ifstream in(path, std::ios::binary);
    std::vector<DiffRecord> records;

    while (in) {
        DiffRecord r;
        in.read(reinterpret_cast<char*>(&r.offset), sizeof(r.offset));
        in.read(reinterpret_cast<char*>(&r.old_length), sizeof(r.old_length));
        in.read(reinterpret_cast<char*>(&r.new_length), sizeof(r.new_length));
        if (!in) break;

        if (r.new_length > 0) {
            r.new_data.resize(r.new_length);
            if (!in.read(reinterpret_cast<char*>(r.new_data.data()), r.new_length)) {
                std::cerr << "补丁文件损坏" << std::endl;
                return {};
            }
        }
        records.push_back(r);
    }
    return records;
}

bool ApplyPatch(const std::vector<uint8_t>& old_data, const std::string& new_path, const std::vector<DiffRecord>& records) {
    std::vector<uint8_t> new_data = old_data;

    for (const auto& r : records) {
        size_t offset = std::min(static_cast<size_t>(r.offset), new_data.size());
        size_t del_len = std::min(static_cast<size_t>(r.old_length), new_data.size() - offset);

        // 执行删除
        if (del_len > 0) {
            new_data.erase(new_data.begin() + offset, new_data.begin() + offset + del_len);
        }

        // 执行插入
        if (r.new_length > 0) {
            new_data.insert(new_data.begin() + offset, r.new_data.begin(), r.new_data.end());
        }
    }

    std::ofstream out(new_path, std::ios::binary);
    if (!out) return false;
    out.write(reinterpret_cast<const char*>(new_data.data()), new_data.size());
    return true;
}