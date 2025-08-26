#pragma once
#define NOMINMAX

#include <Windows.h>

#define MOD_NAME		STR("UnrealSignatureBypass")
#define MOD_VERSION		STR("1.0.0")
#define MOD_DESCRIPTION STR("Unreal Engine signature bypass.")
#define MOD_AUTHOR		STR("Syahmi")

#if UE_BUILD_DEBUG
#define LOG_SEPARATOR STR("|")
#define LOG(level, format, ...) \
    RC::Output::send<level>(MOD_NAME LOG_SEPARATOR STR(format) STR("\n") , ##__VA_ARGS__)
#define LOG_FUNCTION(level, format, ...) \
    LOG(level, "{}| " STR(format) , __FUNCTIONW__ , ##__VA_ARGS__)
#else
#define LOG_SEPARATOR
#define LOG(level, format, ...)
#define LOG_FUNCTION(level, format, ...)
#endif

#define GET_STACK_PARAMS_VALUE(stack, property_name, type) \
    stack.Node()->GetPropertyByNameInChain(STR(#property_name))->ContainerPtrToValuePtr<type>(stack.Locals())

#include "include/Patch/Patch.hpp"
#include "include/Hooks/Hooks.hpp"

#include <DynamicOutput/DynamicOutput.hpp>

namespace Globals {

	inline uintptr_t g_MainExeAddress = reinterpret_cast<uintptr_t>(GetModuleHandle(NULL));

	extern Patch g_BitfixPatch;
	extern Patch g_FPakFile_CreatePakReader_Patch;

	extern std::optional<Hooks> g_FPakFileConstructor_Hook;
	extern std::optional<Hooks> g_FPakPlatformFile__Mount_Hook;
	
	bool Initialize();
	bool Uninitialize();
	static bool Execute();
	static void Register();

	void RegisterBitfixPatch();
	void RegisterFPakFileCreatePakReaderPatch();
	void RegisterFPakFileConstructorHook();
	void RegisterFPakPlatformFile__MountHook();

}