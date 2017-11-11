// ISAClient.h: interface for the ISAClient class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__ISACLIENT_H__)
#define __ISACLIENT_H__

#include <SQLAPI.h>

class ISAConnection;

// Helpers
// numeric support/conversions helpers
void LittleEndian10000BaseDivide(
	unsigned int LittleEndianSize,	// in shorts
	const unsigned short Devidend[],
	unsigned short Devisor,
	unsigned short Quotient[],
	unsigned short *pRemainder);
bool AllBytesAreZero(const void *pBytes, size_t nSize);
// Character sets conversion helpers
// returns number of Wide chars written
// can return character on which conversion was stopped
size_t SAMultiByteToWideChar(
	wchar_t *pchDataTgt,
	const char *lpch, size_t nLength, char **lppchStop = NULL);
// returns number of Multibyte chars written
// if pchDataTgt is NULL, function simply counts
size_t SAWideCharToMultiByte(
	char *pchDataTgt,
	const wchar_t *lpch, size_t nLength);

typedef void (SACommand::*DescribeFields_cb_t)(
	const SAString &sName,
	SADataType_t eFieldType,
	int nNativeType,
	size_t nFieldSize,
	int nFieldPrecision,
	int nFieldScale,
	bool bFieldRequired);

class ISACursor
{
	friend class SACommand;
	friend class SAField;
	friend class SAParam;

protected:
	ISAConnection *m_pISAConnection;
	SACommand *m_pCommand;
	PreHandleException_t m_fnPreHandleException;
	enum eFetchType {EFTYP_NONE, EFTYP_FIRST, EFTYP_NEXT, EFTYP_PRIOR, EFTYP_LAST} m_eLastFetchType;
	void *m_pParamBuffer;
	size_t m_nParamIndSize;
	size_t m_nParamSizeSize;
	void IncParamBuffer(
		void *&pBuf,
		void *&pInd,
		void *&pSize,
		size_t &nDataBufSize,
		void *&pValue);
	static bool isInputParam(const SAParam& Param);
	static bool isOutputParam(const SAParam& Param);

	void *m_pSelectBuffer;
	size_t m_nIndSize;
	size_t m_nSizeSize;
	int m_nBulkReadingBufSize;
	void IncFieldBuffer(
		void *&pBuf,
		void *&pInd,
		void *&pSize,
		size_t &nDataBufSize,
		void *&pValue);

	typedef enum eValueType {ISA_FieldValue, ISA_ParamValue} ValueType_t;
	// return false if target API has told ISA implementation
	// not to convert the value, but leave it as it is
	// (usually if it is a default parameter for SP, that has not been bound)
	virtual bool ConvertValue(
		int nPos,	// 1 - based
		int nNotConverted,
		size_t nIndSize,
		void *pNull,
		size_t nSizeSize,
		void *pSize,
		size_t nBufSize,
		void *pValue,
		ValueType_t eValueType,
		SAValueRead &vr,
		int nBulkReadingBufPos);
	void WriteLongOrLobToInternalValue(
		SAValue &value);

	virtual bool ConvertIndicator(
		int nPos,	// 1-based, can be field or param pos, 0-for return value from SP
		int nNotConverted,
		SAValueRead &vr,
		ValueType_t eValueType,
		void *pInd, size_t nIndSize,
		void *pSize, size_t nSizeSize,
		size_t &nRealSize,
		int nBulkReadingBufPos) const;
	virtual void ConvertString(
		SAString &String,
		const void *pData,
		size_t nRealSize);
	virtual void ConvertLongOrLOB(
		ValueType_t eValueType,
		SAValueRead &vr,
		void *pValue,
		size_t nFieldBufSize);
	// should return false if client binds output/return parameters
	// according to value type (coersing)
	// should return true if client binds output/return parameters
	// according to parameter type
	// defaults to false, should be overloaeded by client if this is not the intent
	virtual bool OutputBindingTypeIsParamType();

	virtual size_t InputBufferSize(
		const SAParam &Param) const;
	virtual size_t OutputBufferSize(
		SADataType_t eDataType,
		size_t nDataSize) const;

	void AllocBindBuffer(
		size_t nIndSize,
		size_t nSizeSize);
	void AllocBindBuffer(
		int nPlaceHolderCount,
		saPlaceHolder **ppPlaceHolders,
		size_t nIndSize,
		size_t nSizeSize,
		size_t nAddlSize = 0,
		void **ppAddl = NULL);
	short ParamDirCount(
		int nPlaceHolderCount,
		saPlaceHolder **ppPlaceHolders,
		int nCount, ...) const;
	void ConvertOutputParams();
	void GetParamBuffer(
		const SAParam &Param,
		void *&pValue, size_t &nParamBufSize);
	int OutputParamIndex(int nParamPos) const;

	void AllocSelectBuffer(
		size_t nIndSize,
		size_t nSizeSize,
		int nBulkReadingBufSize = 1);
	void ConvertSelectBufferToFields(int nBulkReadingBufPos);
	void GetFieldBuffer(
		int nPos,	// 1- based
		void *&pValue, size_t &nFieldBufSize);
	int FieldCount(int nCount, ...) const;

	static bool isLong(SADataType_t eDataType);
	static bool isLob(SADataType_t eDataType);
	static bool isLongOrLob(SADataType_t eDataType);

	bool isSetScrollable() const;
	bool getOptionValue(const SAString& sOption, bool bDefault) const;

public:
	ISACursor(
		ISAConnection *pISAConnection,
		SACommand *pCommand);
	virtual ~ISACursor();

	virtual bool IsOpened() = 0;
	virtual void Open() = 0;
	virtual void Close() = 0;
	virtual void Destroy() = 0;
	virtual void Reset() = 0;

	// prepare statement (also convert to native format if needed)
	virtual void Prepare(
		const SAString &sStmt,
		SACommandType_t eCmdType,
		int nPlaceHolderCount,
		saPlaceHolder **ppPlaceHolders) = 0;
	// executes statement (also binds parameters if needed)
	virtual void Execute(
		int nPlaceHolderCount,
		saPlaceHolder **ppPlaceHolders) = 0;
	// cleans up after execute if needed, so the statement can be reexecuted
	virtual void UnExecute() = 0;
	virtual void Cancel() = 0;

	virtual bool ResultSetExists() = 0;
	virtual void DescribeFields(
		DescribeFields_cb_t fn) = 0;
	virtual void SetSelectBuffers() = 0;
	virtual void SetFieldBuffer(
		int nCol,	// 1-based
		void *pInd,
		size_t nIndSize,
		void *pSize,
		size_t nSizeSize,
		void *pValue,
		size_t nValueSize) = 0;
	virtual bool FetchNext() = 0;
	virtual bool FetchPrior();
	virtual bool FetchFirst();
	virtual bool FetchLast();

	virtual long GetRowsAffected() = 0;

	virtual void ReadLongOrLOB(
		ValueType_t eValueType,
		SAValueRead &vr,
		void *pValue,
		size_t nFieldBufSize,
		saLongOrLobReader_t fnReader,
		size_t nReaderWantedPieceSize,
		void *pAddlData) = 0;

	virtual void DescribeParamSP() = 0;

	virtual saCommandHandles *NativeHandles() = 0;

	// Notifications
	virtual void OnConnectionAutoCommitChanged() {}

	// Options
	int BulkReadingBufSize() const;

	void setBatchExceptionPreHandler(PreHandleException_t fnHandler);
	bool PreHandleException(SAException &x);
};

class ISAConnection
{
protected:
	SAConnection *m_pSAConnection;

public:
	ISAConnection(SAConnection *pSAConnection);
	virtual ~ISAConnection();

	SAConnection *getSAConnection();
	void EnumCursors(EnumCursors_t fn, void *pAddlData);

	virtual void InitializeClient() = 0;
	virtual void UnInitializeClient() = 0;

	virtual long GetClientVersion() const = 0;
	virtual long GetServerVersion() const = 0;
	virtual SAString GetServerVersionString() const = 0;

	virtual bool IsConnected() const = 0;
	virtual bool IsAlive() const = 0;
	virtual void Connect(
		const SAString &sDBString,
		const SAString &sUserID,
		const SAString &sPassword,
		saConnectionHandler_t fHandler) = 0;
	virtual void Disconnect() = 0;
	virtual void Destroy() = 0;
	virtual void Reset() = 0;

	virtual void setIsolationLevel(
		SAIsolationLevel_t eIsolationLevel) = 0;
	virtual void setAutoCommit(
		SAAutoCommit_t eAutoCommit) = 0;
	virtual void Commit() = 0;
	virtual void Rollback() = 0;

	virtual saAPI *NativeAPI() const = 0;
	virtual saConnectionHandles *NativeHandles() = 0;

	virtual ISACursor *NewCursor(SACommand *pCommand) = 0;

	virtual void CnvtInternalToNumeric(
		SANumeric &numeric,
		const void *pInternal,
		int nInternalSize) = 0;
	virtual void CnvtInternalToDateTime(
		SADateTime &date_time,
		const void *pInternal,
		int nInternalSize) = 0;
	virtual void CnvtInternalToInterval(
		SAInterval &interval,
		const void *pInternal,
		int nInternalSize) = 0;
	virtual void CnvtInternalToCursor(
		SACommand *pCursor,
		const void *pInternal) = 0;
};

class ISAClient
{
public:
	ISAClient();
	virtual ~ISAClient();

	virtual ISAConnection *QueryConnectionInterface(
		SAConnection *pSAConnection) = 0;
};

class ISADataConverter
{
public:
	virtual ~ISADataConverter() {};

public:
	// puts a piece of external data into converter
	// pExternalData is guaranteed to be valid on the next GetStream call
	virtual void PutStream(unsigned char *pExternalData, size_t nExternalDataSize, SAPieceType_t eExternalPieceType) = 0;
	// gets a piece of data (converted if necessary) from converter
	// returns false if converter is empty or it doesn't have enough data to return nWantedSize block
	// pData should be the same pointer that was used in PutStream (pExternalData)
	virtual bool GetStream(unsigned char *pData, size_t nWantedSize, size_t &nDataSize, SAPieceType_t &eCnvtPieceType) = 0;
	// checks if no data is waiting in converter
	virtual bool IsEmpty() const = 0;
};

// Does nothing, simply works as a repeater,
// no buffering/no conversions are made.
class SADummyConverter : public ISADataConverter
{
	unsigned char *m_pExternalData;
	size_t m_nExternalDataSize;
	SAPieceType_t m_eExternalPieceType;

	SAPieceType_t m_eCnvtPieceType;
	// converter should return true at least once
	// for the final external piece, even if there is no data
	bool m_bFinalPiecePending;

protected:
	bool StreamIsEnough(size_t nWantedSize, size_t nExternalDataSize) const;
	void FlushExternalData(unsigned char *pData, size_t &nDataSize);
	void SetExternalData(unsigned char *pExternalData, size_t nExternalDataSize);

public:
	SADummyConverter();
	virtual ~SADummyConverter() {};

	virtual void PutStream(unsigned char *pExternalData, size_t nExternalDataSize, SAPieceType_t eExternalPieceType);
	virtual bool GetStream(unsigned char *pData, size_t nWantedSize, size_t &nDataSize, SAPieceType_t &eCnvtPieceType);
	virtual bool IsEmpty() const;
};

// Does the same as SADummyConverter (no conversion)
// but can also provide buffering if necessary
class SABufferConverter : public SADummyConverter
{
	size_t m_nBufferSizeAtGetAppendBufferCall;
	SAString m_Buffer;

protected:
	unsigned char *GetAppendBuffer(size_t nAppendBytesMaxCount);
	void ReleaseAppendBuffer(size_t nAppendBytesCount);

public:
	SABufferConverter();
	virtual ~SABufferConverter();

	virtual bool GetStream(unsigned char *pData, size_t nWantedSize, size_t &nDataSize, SAPieceType_t &eCnvtPieceType);
	virtual bool IsEmpty() const;
};

// Does the same as SABufferConverter (buffering)
// but also converts input data from Unicode to Multibyte
class SAUnicode2MultibyteConverter : public SABufferConverter
{
	// we assume that sizeof(wchar_t) > 1
	// if PutStream() size is not a multiply of sizeof(wchar_t)
	// (for example user reads Unicode data in sizeof(wchar_t)*n+1 pieces)
	// we will save the reminder here
	unsigned char m_chReminderBytes[sizeof(wchar_t)];
	size_t m_ReminderBytesCount;
	unsigned int m_MB_CUR_MAX;
#ifdef SA_UNICODE
	bool m_bUseUTF;
#endif

public:
	SAUnicode2MultibyteConverter();
	virtual ~SAUnicode2MultibyteConverter();
#ifdef SA_UNICODE
	void SetUseUTF8(bool bUseUTF8 = true);
#endif

public:
	virtual bool GetStream(unsigned char *pData, size_t nWantedSize, size_t &nDataSize, SAPieceType_t &eDataPieceType);
	virtual bool IsEmpty() const;
};

// Does the same as SABufferConverter (buffering)
// but also converts input data from Unicode to Multibyte
class SAMultibyte2UnicodeConverter : public SABufferConverter
{
	// multibyte sequence is invalid in current locale
	// no way to continue
	bool m_bFatalError;
	// we will save the reminder here
	char *m_chReminderBytes;	// = malloc(MB_CUR_MAX)
	size_t m_ReminderBytesCount;
	unsigned int m_MB_CUR_MAX;
#ifdef SA_UNICODE
	bool m_bUseUTF;
#endif

public:
	SAMultibyte2UnicodeConverter();
	virtual ~SAMultibyte2UnicodeConverter();
#ifdef SA_UNICODE
	void SetUseUTF8(bool bUseUTF8 = true);
#endif

public:
	virtual bool GetStream(unsigned char *pData, size_t nWantedSize, size_t &nDataSize, SAPieceType_t &eDataPieceType);
	virtual bool IsEmpty() const;
};

#if defined(SA_UNICODE) && !defined(SQLAPI_WINDOWS)
class SAUnicodeUTF16Converter : public SABufferConverter
{
	// we assume that sizeof(wchar_t) > 1
	// if PutStream() size is not a multiply of sizeof(wchar_t)
	// (for example user reads Unicode data in sizeof(wchar_t)*n+1 pieces)
	// we will save the reminder here
	unsigned char m_chReminderBytes[sizeof(wchar_t)];
	size_t m_ReminderBytesCount;

public:
	SAUnicodeUTF16Converter();
	virtual ~SAUnicodeUTF16Converter();

public:
	virtual bool GetStream(unsigned char *pData, size_t nWantedSize, size_t &nDataSize, SAPieceType_t &eDataPieceType);
	virtual bool IsEmpty() const;
};

class SAUTF16UnicodeConverter : public SABufferConverter
{
	// we will save the reminder here
	unsigned char *m_chReminderBytes;
	size_t m_ReminderBytesCount;
	bool m_bFatalError;

public:
	SAUTF16UnicodeConverter();
	virtual ~SAUTF16UnicodeConverter();

public:
	virtual bool GetStream(unsigned char *pData, size_t nWantedSize, size_t &nDataSize, SAPieceType_t &eDataPieceType);
	virtual bool IsEmpty() const;
};
#endif


// Tracing feature
extern SATraceFunction_t saTraceFunc;
extern SATraceInfo_t saTraceInfo;
extern void* saTraceData;

void SATrace(SATraceInfo_t traceInfo, SAConnection* pCon, SACommand* pCmd, const SAChar* szInfo);

#define SA_TRACE_CMD(c)		SATrace(SA_Trace_QueryText, NULL, this->m_pCommand, c)
#define SA_TRACE_CMDC(c)	SATrace(SA_Trace_QueryText, this->m_pSAConnection, NULL, c)

#define SA_DefaultMaxLong	0x7FFFFFFC

#endif // !defined(__ISACLIENT_H__)
