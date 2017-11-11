// db2Client.h: interface for the Idb2Client class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DB2CLIENT_H__11EAA72A_EFF5_4944_8B1F_44973D30EF73__INCLUDED_)
#define AFX_DB2CLIENT_H__11EAA72A_EFF5_4944_8B1F_44973D30EF73__INCLUDED_

#include "ISAClient.h"

class Idb2Client : public ISAClient  
{
public:
	Idb2Client();
	virtual ~Idb2Client();

	virtual ISAConnection *QueryConnectionInterface(
		SAConnection *pSAConnection);
};


#endif // !defined(AFX_DB2CLIENT_H__11EAA72A_EFF5_4944_8B1F_44973D30EF73__INCLUDED_)
