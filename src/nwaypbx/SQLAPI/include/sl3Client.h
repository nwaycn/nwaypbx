// sl3Client.h: interface for the Isl3Client class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__SL3CLIENT_H__)
#define __SL3CLIENT_H__

#include "ISAClient.h"

class Isl3Client : public ISAClient  
{
public:
	Isl3Client();
	virtual ~Isl3Client();

	virtual ISAConnection *QueryConnectionInterface(
		SAConnection *pSAConnection);
};


#endif // !defined(__SL3CLIENT_H__)
