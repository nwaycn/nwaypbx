// oraClient.h: interface for the IibClient class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ORACLIENT_H__11EAA72A_EFF5_4944_8B1F_44973D30EF73__INCLUDED_)
#define AFX_ORACLIENT_H__11EAA72A_EFF5_4944_8B1F_44973D30EF73__INCLUDED_

#include "ISAClient.h"

class IoraClient : public ISAClient  
{
public:
	IoraClient();
	virtual ~IoraClient();

	virtual ISAConnection *QueryConnectionInterface(
		SAConnection *pSAConnection);
};


#endif // !defined(AFX_ORACLIENT_H__11EAA72A_EFF5_4944_8B1F_44973D30EF73__INCLUDED_)
