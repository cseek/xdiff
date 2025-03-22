PACKAGE = xdiff
VERSION = 1.0.0
ARCH = amd64
PREFIX = /usr

# 编译相关配置
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Iinclude
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build
TARGET = $(BUILD_DIR)/xdiff

# 源文件列表
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))
DEPS = $(wildcard $(INC_DIR)/*.h)

# Debian打包配置
DEB_DIR = deb_pkg
DEB_BIN_DIR = $(DEB_DIR)/usr/bin
DEB_DOC_DIR = $(DEB_DIR)/usr/share/doc/$(PACKAGE)
DEB_CONTROL_FILE = $(DEB_DIR)/DEBIAN/control

.PHONY: all clean install uninstall pack

all: $(TARGET)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEPS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -rf $(BUILD_DIR) $(DEB_DIR) *.deb

install: $(TARGET)
	install -Dm 755 $(TARGET) $(DESTDIR)$(PREFIX)/bin/$(PACKAGE)

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/$(PACKAGE)

pack: $(TARGET)
	@echo "Building Debian package..."
	@mkdir -p $(DEB_BIN_DIR)
	@mkdir -p $(DEB_DOC_DIR)
	@mkdir -p $(DEB_DIR)/DEBIAN
	
# 复制二进制文件
	install -m 755 $(TARGET) $(DEB_BIN_DIR)/$(PACKAGE)
	
# 创建控制文件
	@echo "Package: $(PACKAGE)" > $(DEB_CONTROL_FILE)
	@echo "Version: $(VERSION)" >> $(DEB_CONTROL_FILE)
	@echo "Architecture: $(ARCH)" >> $(DEB_CONTROL_FILE)
	@echo "Maintainer: xiongyi <jassimxiong@gmail.com>" >> $(DEB_CONTROL_FILE)
	@echo "Description: file diff/patch utility" >> $(DEB_CONTROL_FILE)
	@echo " A tool for generating and applying binary diffs between files." >> $(DEB_CONTROL_FILE)
	
# 创建版权文件
	@echo "Copyright: $(shell date +%Y) xiongyi" > $(DEB_DOC_DIR)/copyright
	@echo "License: MIT" >> $(DEB_DOC_DIR)/copyright
	
# 构建deb包
	dpkg-deb --build $(DEB_DIR) $(PACKAGE)_$(VERSION)_$(ARCH).deb
	@echo "Package built: $(PACKAGE)_$(VERSION)_$(ARCH).deb"