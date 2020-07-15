# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/hgfs/cs6210_AOS/non_omscs/cryptopals

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/hgfs/cs6210_AOS/non_omscs/cryptopals

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "No interactive CMake dialog available..."
	/usr/bin/cmake -E echo No\ interactive\ CMake\ dialog\ available.
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /mnt/hgfs/cs6210_AOS/non_omscs/cryptopals/CMakeFiles /mnt/hgfs/cs6210_AOS/non_omscs/cryptopals/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /mnt/hgfs/cs6210_AOS/non_omscs/cryptopals/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named hex_to_base64

# Build rule for target.
hex_to_base64: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 hex_to_base64
.PHONY : hex_to_base64

# fast build rule for target.
hex_to_base64/fast:
	$(MAKE) -f set1/hex_to_base64/CMakeFiles/hex_to_base64.dir/build.make set1/hex_to_base64/CMakeFiles/hex_to_base64.dir/build
.PHONY : hex_to_base64/fast

#=============================================================================
# Target rules for targets named test_hex_to_base64

# Build rule for target.
test_hex_to_base64: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 test_hex_to_base64
.PHONY : test_hex_to_base64

# fast build rule for target.
test_hex_to_base64/fast:
	$(MAKE) -f set1/hex_to_base64/CMakeFiles/test_hex_to_base64.dir/build.make set1/hex_to_base64/CMakeFiles/test_hex_to_base64.dir/build
.PHONY : test_hex_to_base64/fast

#=============================================================================
# Target rules for targets named hex_XOR

# Build rule for target.
hex_XOR: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 hex_XOR
.PHONY : hex_XOR

# fast build rule for target.
hex_XOR/fast:
	$(MAKE) -f set1/hex_XOR/CMakeFiles/hex_XOR.dir/build.make set1/hex_XOR/CMakeFiles/hex_XOR.dir/build
.PHONY : hex_XOR/fast

#=============================================================================
# Target rules for targets named test_hex_XOR

# Build rule for target.
test_hex_XOR: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 test_hex_XOR
.PHONY : test_hex_XOR

# fast build rule for target.
test_hex_XOR/fast:
	$(MAKE) -f set1/hex_XOR/CMakeFiles/test_hex_XOR.dir/build.make set1/hex_XOR/CMakeFiles/test_hex_XOR.dir/build
.PHONY : test_hex_XOR/fast

#=============================================================================
# Target rules for targets named decode_single_byte_XOR_cipher

# Build rule for target.
decode_single_byte_XOR_cipher: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 decode_single_byte_XOR_cipher
.PHONY : decode_single_byte_XOR_cipher

# fast build rule for target.
decode_single_byte_XOR_cipher/fast:
	$(MAKE) -f set1/single_byte_XOR_cipher/CMakeFiles/decode_single_byte_XOR_cipher.dir/build.make set1/single_byte_XOR_cipher/CMakeFiles/decode_single_byte_XOR_cipher.dir/build
.PHONY : decode_single_byte_XOR_cipher/fast

#=============================================================================
# Target rules for targets named test_decode_single_byte_XOR_cipher

# Build rule for target.
test_decode_single_byte_XOR_cipher: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 test_decode_single_byte_XOR_cipher
.PHONY : test_decode_single_byte_XOR_cipher

# fast build rule for target.
test_decode_single_byte_XOR_cipher/fast:
	$(MAKE) -f set1/single_byte_XOR_cipher/CMakeFiles/test_decode_single_byte_XOR_cipher.dir/build.make set1/single_byte_XOR_cipher/CMakeFiles/test_decode_single_byte_XOR_cipher.dir/build
.PHONY : test_decode_single_byte_XOR_cipher/fast

#=============================================================================
# Target rules for targets named test_crypto_lib

# Build rule for target.
test_crypto_lib: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 test_crypto_lib
.PHONY : test_crypto_lib

# fast build rule for target.
test_crypto_lib/fast:
	$(MAKE) -f crypto_lib/CMakeFiles/test_crypto_lib.dir/build.make crypto_lib/CMakeFiles/test_crypto_lib.dir/build
.PHONY : test_crypto_lib/fast

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... rebuild_cache"
	@echo "... edit_cache"
	@echo "... hex_to_base64"
	@echo "... test_hex_to_base64"
	@echo "... hex_XOR"
	@echo "... test_hex_XOR"
	@echo "... decode_single_byte_XOR_cipher"
	@echo "... test_decode_single_byte_XOR_cipher"
	@echo "... test_crypto_lib"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

