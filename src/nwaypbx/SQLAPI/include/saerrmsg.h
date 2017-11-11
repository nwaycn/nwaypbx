#ifndef __SAERRMSG_H__
#define __SAERRMSG_H__

#define IDS_CLIENT_NOT_SET				_TSA("API client not set")
#define IDS_CLIENT_NOT_SUPPORTED		_TSA("API client not supported")

#define IDS_LOAD_LIBRARY_FAILS \
	_TSA("%") SA_FMT_STR _TSA("\n\n") \
    _TSA("DBMS API Library loading fails\n") \
    _TSA("This library is a part of DBMS client installation, not SQLAPI++\n") \
    _TSA("Make sure DBMS client is installed and\n") \
    _TSA("this required library is available for dynamic loading\n")

#define IDS_GET_LIBRARY_VERSION_FAILS	_TSA("API Library '%") SA_FMT_STR _TSA("' quering version fails")
#define IDS_CLIENT_VERSION_OLD			_TSA("This version of client (%d.%d) is too old and not supported")
#define IDS_CLIENT_INIT_FAILS			_TSA("API Library initialization fails")

#define IDS_BIND_VAR_NOT_FOUND			_TSA("Bind variable/parameter '%") SA_FMT_STR _TSA("' not found")
#define IDS_FIELD_NOT_FOUND				_TSA("Field '%") SA_FMT_STR _TSA("' not found")
#define IDS_UNKNOWN_DATA_TYPE			_TSA("Unknown data type")
#define IDS_UNKNOWN_PARAMETER_TYPE		_TSA("Unknown parameter data type ('%") SA_FMT_STR _TSA("')")
#define IDS_UNKNOWN_DIRECTION_TYPE		_TSA("Unknown parameter direction type ('%") SA_FMT_STR _TSA("')")
#define IDS_UNKNOWN_COLUMN_TYPE			_TSA("Unknown column data type ('%") SA_FMT_STR _TSA("')")

#define IDS_CONVERTION_FROM_STRING_TO_SHORT_ERROR	_TSA("Can't convert from string '%") SA_FMT_STR _TSA("' to short")
#define IDS_CONVERTION_FROM_STRING_TO_LONG_ERROR	_TSA("Can't convert from string '%") SA_FMT_STR _TSA("' to long")
#define IDS_CONVERTION_FROM_STRING_TO_DOUBLE_ERROR	_TSA("Can't convert from string '%") SA_FMT_STR _TSA("' to double")
#define IDS_CONVERTION_FROM_STRING_TO_NUMERIC_ERROR	_TSA("Can't convert from string '%") SA_FMT_STR _TSA("' to numeric")

#define IDS_DATETIME_WRONG_ERROR	_TSA("Invalid date time detected")

#endif // !__SAERRMSG_H__
