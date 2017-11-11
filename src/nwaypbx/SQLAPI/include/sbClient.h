// ibClient.h: interface for the IibClient class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SBCLIENT_H__11EAA72A_EFF5_4944_8B1F_44973D30EF73__INCLUDED_)
#define AFX_SBCLIENT_H__11EAA72A_EFF5_4944_8B1F_44973D30EF73__INCLUDED_

#include "ISAClient.h"

class IsbClient : public ISAClient  
{
public:
	IsbClient();
	virtual ~IsbClient();

	virtual ISAConnection *QueryConnectionInterface(
		SAConnection *pSAConnection);
};


#endif // !defined(AFX_SBCLIENT_H__11EAA72A_EFF5_4944_8B1F_44973D30EF73__INCLUDED_)
