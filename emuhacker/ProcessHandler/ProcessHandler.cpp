#include "stdafx.h"
#include "ProcessHandler.h"


ProcessHandler::ProcessHandler():mProcess(nullptr)
{
	HANDLE	hProcess;
	HANDLE	hToken;
	LUID	id;

	hProcess = GetCurrentProcess();
	if(OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		if(LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &id))
		{
			TOKEN_PRIVILEGES	TPEnableDebug;
			TPEnableDebug.PrivilegeCount = 1;
			TPEnableDebug.Privileges[0].Luid = id;
			TPEnableDebug.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

			AdjustTokenPrivileges(hToken, FALSE, &TPEnableDebug, 0, NULL, NULL);
		}
		CloseHandle(hToken);
	}
	CloseHandle(hProcess);
}

ProcessHandler::~ProcessHandler()
{
	close();
}

void ProcessHandler::close(){
	if(mProcess)
	{
		CloseHandle(mProcess);
		mProcess = nullptr;
	}
}

bool ProcessHandler::attachByWindowName(CSTR className, CSTR windowName){
	return attachByWindowHandle(FindWindow(className, windowName));
}

bool ProcessHandler::attachByWindowHandle(HWND hWnd){
	if(IsWindow(hWnd))
	{
		DWORD	dwProcessId;
		close();
		GetWindowThreadProcessId(hWnd, &dwProcessId);
		mProcess = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, dwProcessId);
		return mProcess != nullptr;
	}
	return false;
}

bool ProcessHandler::isValid()
{
	if (mProcess)
	{
		DWORD code;
		GetExitCodeProcess(mProcess, &code);
		return code == STILL_ACTIVE;
	}
	return false;
}

bool ProcessHandler::read(addr_t addr, size_t size, LPVOID buffer){
	if(isValid())
	{
		addr = prepareAddr(addr, size);
		if (addr) {
			return ReadProcessMemory(mProcess, (LPVOID)addr, buffer, size, NULL) != 0;
		}
	}
	return false;
}

bool ProcessHandler::write(addr_t addr, size_t size, LPCVOID buffer){
	if(isValid())
	{
		addr = prepareAddr(addr, size);
		if (addr) {
			return WriteProcessMemory(mProcess, (LPVOID)addr, buffer, size, NULL) != 0;
		}
	}
	return false;
}

bool ProcessHandler::add(addr_t addr, int value)
{
	u32 origin = read<u32>(addr);
	origin += value;
	return write(addr, origin);
}

bool ProcessHandler::ptrRead(addr_t addr, u32 offset, size_t size, LPVOID buffer){
	if(read(addr, sizeof(addr), &addr))
		return read(addr + offset, size, buffer);
	return false;
}

bool ProcessHandler::ptrWrite(addr_t addr, u32 offset, size_t size, LPCVOID buffer){
	if(read(addr, sizeof(addr), &addr))
		return write(addr + offset, size, buffer);
	return false;
}

bool ProcessHandler::readLastPtr(const PtrEntry &entry, addr_t *addrPtr){
	addr_t addr = entry.baseAddr;
	for (int i = 0; i < entry.ptrLevel - 1; i++){
		if(!read(addr, sizeof(addr), &addr))
			return false;
		addr = addr + entry.offsets[i];
	}
	*addrPtr = addr;
	return true;
}

bool ProcessHandler::ptrsRead(const PtrEntry &entry, size_t size, LPVOID buffer){
	addr_t addr;
	return readLastPtr(entry, &addr) && ptrRead(addr, entry.lastOffset(), size, buffer);
}

bool ProcessHandler::ptrsWrite(const PtrEntry &entry, size_t size, LPCVOID buffer){
	addr_t addr;
	return readLastPtr(entry, &addr) && ptrWrite(addr, entry.lastOffset(), size, buffer);
}
