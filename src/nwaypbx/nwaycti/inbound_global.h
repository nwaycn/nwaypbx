#ifndef __NWAY_INBOUND_GLOBAL_H
#define __NWAY_INBOUND_GLOBAL_H
#include <esl.h>
class CInboundHandle
{
public:
	static CInboundHandle* GetInstance();
	esl_handle_t* GetHandle();
	void Release();
protected:
private:
	CInboundHandle();
	~CInboundHandle();
	esl_handle_t* pghandle;
};
#endif