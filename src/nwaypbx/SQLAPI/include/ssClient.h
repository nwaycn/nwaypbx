// ssClient.h: interface for the IssClient class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SSCLIENT_H__11EAA72A_EFF5_4944_8B1F_44973D30EF73__INCLUDED_)
#define AFX_SSCLIENT_H__11EAA72A_EFF5_4944_8B1F_44973D30EF73__INCLUDED_

#include "ISAClient.h"

class IssClient : public ISAClient  
{
public:
	IssClient();
	virtual ~IssClient();

	virtual ISAConnection *QueryConnectionInterface(
		SAConnection *pSAConnection);
};


#endif // !defined(AFX_SSCLIENT_H__11EAA72A_EFF5_4944_8B1F_44973D30EF73__INCLUDED_)
