#include "stdafx.h"
#include "NdsEmuHandler.h"
#include "utils/utils.h"
#include <stdio.h>


addr_t NdsEmuHandler::prepareAddr(addr_t addr, size_t size)
{
	if (addr < 0x0A000000) {
		int index = (addr & 0x0F000000) >> 24;
		if (index > 8)
			index = 8;
		addr &= 0x00FFFFFF;

		if (addr + size <= NDS_MEMORY_SIZE[index])
			return addr;
	}
	return 0;
}