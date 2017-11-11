// asaClient.h: interface for the IasaClient class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__ASACLIENT_H__)
#define __ASACLIENT_H__

#include "ISAClient.h"

class IasaClient : public ISAClient  
{
public:
	IasaClient();
	virtual ~IasaClient();

	virtual ISAConnection *QueryConnectionInterface(
		SAConnection *pSAConnection);
};


#endif // !defined(__ASACLIENT_H__)
