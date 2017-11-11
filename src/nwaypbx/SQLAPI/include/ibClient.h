// ibClient.h: interface for the IibClient class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IBCLIENT_H__11EAA72A_EFF5_4944_8B1F_44973D30EF73__INCLUDED_)
#define AFX_IBCLIENT_H__11EAA72A_EFF5_4944_8B1F_44973D30EF73__INCLUDED_

#include "ISAClient.h"

class IibClient : public ISAClient  
{
public:
	IibClient();
	virtual ~IibClient();

	virtual ISAConnection *QueryConnectionInterface(
		SAConnection *pSAConnection);
};


#endif // !defined(AFX_IBCLIENT_H__11EAA72A_EFF5_4944_8B1F_44973D30EF73__INCLUDED_)
