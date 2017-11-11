// myClient.h: interface for the IssClient class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__MYSQLCLIENT_H__)
#define __MYSQLCLIENT_H__

#include "ISAClient.h"

class ImyClient : public ISAClient  
{
public:
	ImyClient();
	virtual ~ImyClient();

	virtual ISAConnection *QueryConnectionInterface(
		SAConnection *pSAConnection);
};


#endif // !defined(__MYSQLCLIENT_H__)
