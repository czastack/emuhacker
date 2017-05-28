#pragma once
#include "ProcessHandler/ProcessHandler.h"
#include "gba.h"

class GbaEmuHandler : public ProcessHandler
{
	addr_t prepareAddr(addr_t addr, size_t size) override;

protected:
	u32	mPtrTable[9];
};