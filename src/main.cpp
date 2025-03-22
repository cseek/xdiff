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
#include <string>

void ShowUsage() {
    std::cerr << "用法:\n"
              << "  生成补丁: xdiff create <旧文件> <新文件> <补丁文件>\n"
              << "  应用补丁: xdiff apply <旧文件> <补丁文件> <新文件>\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        ShowUsage();
        return 1;
    }

    std::string mode = argv[1];
    if (mode == "create" && argc == 5) {
        auto old_data = ReadFileData(argv[2]);
        auto new_data = ReadFileData(argv[3]);
        if (old_data.empty() || new_data.empty()) return 1;

        auto records = GeneratePatch(old_data, new_data);
        if (!WritePatchFile(argv[4], records)) {
            std::cerr << "写入补丁文件失败" << std::endl;
            return 1;
        }
        std::cout << "成功生成补丁文件: " << argv[4] << std::endl;
    }
    else if (mode == "apply" && argc == 5) {
        auto old_data = ReadFileData(argv[2]);
        auto records = ReadPatchFile(argv[3]);
        if (old_data.empty() || records.empty()) return 1;

        if (!ApplyPatch(old_data, argv[4], records)) {
            std::cerr << "应用补丁失败" << std::endl;
            return 1;
        }
        std::cout << "成功生成新文件: " << argv[4] << std::endl;
    }
    else {
        ShowUsage();
        return 1;
    }

    return 0;
}