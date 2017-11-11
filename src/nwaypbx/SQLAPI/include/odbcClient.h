// odbcClient.h: interface for the IodbcClient class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ODBCCLIENT_H__11EAA72A_EFF5_4944_8B1F_44973D30EF73__INCLUDED_)
#define AFX_ODBCCLIENT_H__11EAA72A_EFF5_4944_8B1F_44973D30EF73__INCLUDED_

#include "ISAClient.h"

class IodbcClient : public ISAClient  
{
public:
	IodbcClient();
	virtual ~IodbcClient();

	virtual ISAConnection *QueryConnectionInterface(
		SAConnection *pSAConnection);
};


#endif // !defined(AFX_ODBCCLIENT_H__11EAA72A_EFF5_4944_8B1F_44973D30EF73__INCLUDED_)
