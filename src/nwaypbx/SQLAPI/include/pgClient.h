// pgClient.h: interface for the IpgClient class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__PGCLIENT_H__)
#define __PGCLIENT_H__

#include "ISAClient.h"

class IpgClient : public ISAClient  
{
public:
	IpgClient();
	virtual ~IpgClient();

	virtual ISAConnection *QueryConnectionInterface(
		SAConnection *pSAConnection);
};


#endif // !defined(__PGCLIENT_H__)
