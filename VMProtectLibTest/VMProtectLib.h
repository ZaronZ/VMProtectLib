#pragma once

#ifdef VMPROTECT
#include "VMProtectSDK.h"
#else
static void __stdcall VMProtectBegin(const char*)
{

}

static void __stdcall VMProtectBeginVirtualization(const char*)
{

}

static void __stdcall VMProtectBeginMutation(const char*)
{

}

static void __stdcall VMProtectBeginUltra(const char*)
{

}

static void __stdcall VMProtectEnd(void)
{

}

static bool __stdcall VMProtectIsProtected()
{
	return true;
}

static bool __stdcall VMProtectIsDebuggerPresent(bool)
{
	return false;
}

static bool __stdcall VMProtectIsVirtualMachinePresent(void)
{
	return false;
}

static bool __stdcall VMProtectIsValidImageCRC(void)
{
	return true;
}

static const char* __stdcall VMProtectDecryptStringA(const char* value)
{
	return value;
}

static const wchar_t* __stdcall VMProtectDecryptStringW(const wchar_t* value)
{
	return value;
}

static bool __stdcall VMProtectFreeString(const void*)
{
	return true;
}
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

inline VMProtect::VMProtect()
{
	this->maxCount = sizeof(strings) / sizeof(void*);
	this->count = 0;
}

inline VMProtect::~VMProtect()
{
	this->Clean();
}

inline const void* VMProtect::Add(const void* input)
{
	this->strings[this->count] = (void*)input;
	this->count++;

	if (this->count == this->maxCount)
	{
		VMProtectFreeString(this->strings[0]);
		for (size_t i = 1; i < this->count; i++)
			this->strings[i - 1] = this->strings[i];
		this->count--;
	}
	return input;
}

inline void VMProtect::Clean()
{
	for (size_t i = 0; i < this->count; i++)
		VMProtectFreeString(this->strings[i]);
	this->count = 0;
}
