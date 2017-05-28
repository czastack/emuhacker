#include "stdafx.h"
#include "gba/VbaHandler.h"
#include "gba/NogbaHandler.h"
#include "nds/NogbaNdsHandler.h"
#include "nds/DeSmuMEHandler.h"

#define ADDAPI __declspec(dllexport) __cdecl
#define SELF ((ProcessHandler*)self)

extern "C" {

	auto ADDAPI VbaHandler_new() {
		return new VbaHandler;
	}

	auto ADDAPI NogbaHandler_new() {
		return new NogbaHandler;
	}

	auto ADDAPI NogbaNdsHandler_new() {
		return new NogbaNdsHandler;
	}

	auto ADDAPI DeSmuMEHandler_new() {
		return new DeSmuMEHandler;
	}

	auto ADDAPI attach(void* self) {
		return SELF->attach();
	}

	void ADDAPI close(void* self) {
		delete SELF;
	}

	auto ADDAPI read8(void* self, u32 addr) {
		return SELF->read<u8>(addr);
	}

	auto ADDAPI read16(void* self, u32 addr) {
		return SELF->read<u16>(addr);
	}

	auto ADDAPI read32(void* self, u32 addr) {
		return SELF->read<u32>(addr);
	}

	auto ADDAPI write8(void* self, u32 addr, u8 data) {
		return SELF->write(addr, data);
	}

	auto ADDAPI write16(void* self, u32 addr, u16 data) {
		return SELF->write(addr, data);
	}

	auto ADDAPI write32(void* self, u32 addr, u32 data) {
		return SELF->write(addr, data);
	}

	auto ADDAPI read(void* self, u32 addr, size_t size, char* buf) {
		return SELF->read(addr, size, buf);
	}

	auto ADDAPI write(void* self, u32 addr, size_t size, char* buf) {
		return SELF->write(addr, size, buf);
	}

	auto ADDAPI add(void* self, u32 addr, int value) {
		return SELF->add(addr, value);
	}
}