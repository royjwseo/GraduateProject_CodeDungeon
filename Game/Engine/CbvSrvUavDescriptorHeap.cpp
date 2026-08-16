#include "pch.h"
#include "CbvSrvUavDescriptorHeap.h"



CbvSrvUavDescriptorHeap::CbvSrvUavDescriptorHeap()
{
	m_d3dSrvCPUDescriptorStartHandle.ptr = NULL;
	m_d3dSrvGPUDescriptorStartHandle.ptr = NULL;
}

CbvSrvUavDescriptorHeap::~CbvSrvUavDescriptorHeap()
{
	
}

