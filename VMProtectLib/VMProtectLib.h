#pragma once

#ifdef _DEBUG
#ifdef _WIN64
#pragma comment(lib, "VMProtectLib64d.lib")
#else
#pragma comment(lib, "VMProtectLib32d.lib")
#endif
#else
#ifdef _WIN64
#pragma comment(lib, "VMProtectLib64.lib")
#else
#pragma comment(lib, "VMProtectLib32.lib")
#endif
#endif

#ifdef VMPROTECT
#include "VMProtectSDK.h"
#else
void VMProtectBegin(const char*);
void VMProtectBeginVirtualization(const char*);
void VMProtectBeginMutation(const char*);
void VMProtectBeginUltra(const char*);
void VMProtectEnd(void);
bool VMProtectIsProtected();
bool VMProtectIsDebuggerPresent(bool);
bool VMProtectIsVirtualMachinePresent(void);
bool VMProtectIsValidImageCRC(void);
const char* VMProtectDecryptStringA(const char*);
const wchar_t* VMProtectDecryptStringW(const wchar_t*);
bool VMProtectFreeString(const void*);
#endif

class VMProtect
{
	size_t maxCount;
	size_t count;
	void* strings[32];

	const void* Add(const void* input);
public:
	VMProtect();
	~VMProtect();
	void Clean();
	
	typedef enum
	{
		FlagAll = 4,
		FlagProtected = 8,
		FlagDebuggerUser = 16,
		FlagDebuggerKernel = 32,
		FlagVirtualMachine = 64,
		FlagValidImageCRC = 128
	} ProtectFlag;

	static __forceinline const char* String(VMProtect* _this, const char* input)
	{
		return (const char*)_this->Add(VMProtectDecryptStringA(input));
	}

	static __forceinline const wchar_t* String(VMProtect* _this, const wchar_t* input)
	{
		return (const wchar_t*)_this->Add(VMProtectDecryptStringW(input));
	}

	static __forceinline const char* String(VMProtect &_this, const char* input)
	{
		return (const char*)_this.Add(VMProtectDecryptStringA(input));
	}

	static __forceinline const wchar_t* String(VMProtect &_this, const wchar_t* input)
	{
		return (const wchar_t*)_this.Add(VMProtectDecryptStringW(input));
	}

	static __forceinline const char* String(const char* input)
	{
		return VMProtectDecryptStringA(input);
	}

	static __forceinline const wchar_t* String(const wchar_t* input)
	{
		return VMProtectDecryptStringW(input);
	}

	static bool IsProtected(unsigned char mask = 4)
	{
		bool result = true;

		if (mask & FlagAll)
			mask = FlagProtected | FlagDebuggerUser | FlagDebuggerKernel | FlagVirtualMachine | FlagValidImageCRC;

		if (mask & FlagProtected)
			result &= VMProtectIsProtected() == true;

		if (mask & FlagDebuggerUser)
			result &= VMProtectIsDebuggerPresent(false) == false;

		if (mask & FlagDebuggerKernel)
			result &= VMProtectIsDebuggerPresent(true) == false;

		if (mask & FlagVirtualMachine)
			result &= VMProtectIsVirtualMachinePresent() == false;

		if (mask & FlagValidImageCRC)
			result &= VMProtectIsValidImageCRC() == true;

		return result;
	}

	static __forceinline void Begin(const char* input)
	{
		VMProtectBegin(input);
	}

	static __forceinline void BeginVirtualization(const char* input)
	{
		VMProtectBeginVirtualization(input);
	}

	static __forceinline void BeginMutation(const char* input)
	{
		VMProtectBeginMutation(input);
	}

	static __forceinline void BeginUltra(const char* input)
	{
		VMProtectBeginUltra(input);
	}

	static __forceinline void End()
	{
		VMProtectEnd();
	}
};