#pragma once
#pragma comment(lib,"Version.lib")

#define WIN32_LEAN_AND_MEAN

#include <SDKDDKVer.h>
#include <cassert>
#include <Windows.h>
#include <winsock2.h>
#include <Shlobj.h>
#include <cstdint>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <inttypes.h>
#include "external/loguru/loguru.hpp"
#include "external/inih/INIReader.h"
#include "external/length-disassembler/headerOnly/ldisasm.h"