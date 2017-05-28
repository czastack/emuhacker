#pragma once
#include "ProcessHandler/ProcessHandler.h"
#include "nds.h"

class NdsEmuHandler : public ProcessHandler
{
	addr_t prepareAddr(addr_t addr, size_t size) override;

protected:
	NDS_MEM_TABLE mPtrTable;
};