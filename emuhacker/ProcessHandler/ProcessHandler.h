#pragma once

typedef struct
{
	addr_t baseAddr;
	int ptrLevel;
	u32 offsets[4];
	u32 lastOffset() const{
		return offsets[ptrLevel - 1];
	}
} PtrEntry;


class ProcessHandler
{
public:
	ProcessHandler();
	virtual ~ProcessHandler();

	virtual bool attach() = 0;
	virtual addr_t prepareAddr(addr_t addr, size_t size) = 0;

	/*
	 * close the handle
	 */
	void close();

	/**
	 * find the first process whose main window has the specified class and window name,
	 * the handle will be automatically closed, unless it is detached
	 */
	bool attachByWindowName(CSTR className, CSTR windowName);

	/*
	 * attach the process whose main window is the specified window
	 * the handle will be automatically closed, unless it is detached
	 */
	bool attachByWindowHandle(HWND hWnd);

	/*
	 * return value is TRUE, if this object has attached to a valid process handle
	 */

	bool isValid();

	bool read(addr_t addr, size_t size, LPVOID buffer);
	bool write(addr_t addr, size_t size, LPCVOID buffer);

	bool add(addr_t addr, int value);

	bool ptrRead(addr_t addr, u32 offset, size_t size, LPVOID buffer);
	bool ptrWrite(addr_t addr, u32 offset, size_t size, LPCVOID buffer);

	bool readLastPtr(const PtrEntry &entry, addr_t *addrPtr);

	bool ptrsRead(const PtrEntry &entry, size_t size, LPVOID buffer);
	bool ptrsWrite(const PtrEntry &entry, size_t size, LPCVOID buffer);

	/**
	 * ��ȡ���ݵ�����
	 */
	template<size_t size, typename TYPE>
	bool read(addr_t addr, TYPE(&arr)[size]) {
		return read(addr, size * sizeof(TYPE), arr);
	}

	/**
	 * д�������е�����
	 */
	template<size_t size, typename TYPE>
	bool write(addr_t addr, TYPE(&arr)[size]) {
		return write(addr, size * sizeof(TYPE), arr);
	}

	/**
	 * ��ȡ����
	 */
	template<typename TYPE>
	bool read(addr_t addr, TYPE &buff) {
		return read(addr, sizeof(TYPE), &buff);
	}

	/**
	 * д������
	 */
	template<typename TYPE>
	bool write(addr_t addr, TYPE &buff) {
		return write(addr, sizeof(TYPE), &buff);
	}

	/**
	 * ��ȡ����
	 */
	template<typename T=u8>
	T read(addr_t addr) {
		T data;
		read(addr, data);
		return data;
	}

private:
	HANDLE		mProcess;
};
