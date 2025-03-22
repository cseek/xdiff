# xdiff

[![Build Status](https://img.shields.io/badge/build-passing-brightgreen)]()
[![License](https://img.shields.io/badge/license-MIT-blue)]()

一个简易的差异处理工具

## 目录
- [项目结构](#项目结构)
- [快速开始](#快速开始)
- [编译安装](#编译安装)
- [Debian包使用](#debian包使用)
- [开发者指南](#开发者指南)
- [许可协议](#许可协议)

## 项目结构

```bash
xdiff/
├── Makefile          # 构建配置文件
├── include/          # 头文件目录
│   └── xdiff.h       # 核心头文件
├── src/              # 源代码目录
│   ├── main.cpp      # 主函数
│   └── xdiff.cpp     # 主实现文件
├── build/            # 构建输出目录（自动生成）
├── deb_pkg/          # 临时打包目录（自动生成）
└── README.md         # 项目文档
```

## 快速开始
### 依赖要求
编译工具链：g++ ≥9.3 或 clang++ ≥12
构建工具：make ≥4.2
运行时依赖：libstdc++6 ≥12.1
### 基础使用
```bash
# 生成补丁
./xdiff create old.bin new.bin update.patch
# 应用补丁
./xdiff apply old.bin update.patch patched.bin
```
## 编译安装
### 从源码构建
```bash
# 常规构建
make && sudo make install
# 调试构建
make CXXFLAGS="-DDEBUG -g3" clean all
```
### 安装选项
```bash
# 自定义安装路径
make PREFIX=/opt/xdiff install
```
## Debian包使用
### 生成deb包
```bash
make pack
# 生成文件：xdiff_1.0.0_amd64.deb
```
### 软件包管理
```bash
# 安装软件包
sudo dpkg -i xdiff_*.deb

# 查看包含文件
dpkg -L xdiff

# 卸载软件包
sudo apt purge xdiff
```
### 包文件结构
```bash
/usr/
└── bin/xdiff
```

## 许可协议
本项目采用[Apache](http://www.apache.org/licenses)许可证，允许自由使用、修改和分发，需保留原始版权声明。