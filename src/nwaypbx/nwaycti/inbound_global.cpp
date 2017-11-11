#include "inbound_global.h"

CInboundHandle* inboundHandle_instance = NULL;
CInboundHandle* CInboundHandle::GetInstance()
{
	if (inboundHandle_instance == NULL)
	{
		inboundHandle_instance = new CInboundHandle();
	}
	return inboundHandle_instance;
}

void CInboundHandle::Release()
{
	if (inboundHandle_instance)
	{
		delete inboundHandle_instance;
		inboundHandle_instance = NULL;
	}
}

CInboundHandle::CInboundHandle()
{

}

CInboundHandle::~CInboundHandle()
{

}

esl_handle_t* CInboundHandle::GetHandle()
{
	return pghandle;
}
