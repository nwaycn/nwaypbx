// sybClient.h: interface for the IsybClient class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYBCLIENT_H__11EAA72A_EFF5_4944_8B1F_44973D30EF73__INCLUDED_)
#define AFX_SYBCLIENT_H__11EAA72A_EFF5_4944_8B1F_44973D30EF73__INCLUDED_

#include "ISAClient.h"

class IsybClient : public ISAClient  
{
public:
	IsybClient();
	virtual ~IsybClient();

	virtual ISAConnection *QueryConnectionInterface(
		SAConnection *pSAConnection);
};


#endif // !defined(AFX_SYBCLIENT_H__11EAA72A_EFF5_4944_8B1F_44973D30EF73__INCLUDED_)
