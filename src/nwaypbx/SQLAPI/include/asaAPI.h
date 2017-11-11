//////////////////////////////////////////////////////////////////////
// asaAPI.h
//////////////////////////////////////////////////////////////////////

#if !defined(__ASAAPI_H__)
#define __ASAAPI_H__

#include <SQLAPI.h>
#include <sqlca.h>
#include <sacapi.h>

extern void AddSQLAnywhereSupport(const SAConnection * pCon);
extern void ReleaseSQLAnywhereSupport();

typedef sacapi_bool (*sqlany_init_t)( const char * app_name, sacapi_u32 api_version, sacapi_u32 * max_version );
typedef void (*sqlany_fini_t)();
typedef a_sqlany_connection * (*sqlany_new_connection_t)( );
typedef void (*sqlany_free_connection_t)( a_sqlany_connection *sqlany_conn );
typedef a_sqlany_connection * (*sqlany_make_connection_t)( void * arg );
typedef sacapi_bool (*sqlany_connect_t)( a_sqlany_connection * sqlany_conn, const char * str );
typedef sacapi_bool (*sqlany_disconnect_t)( a_sqlany_connection * sqlany_conn );
typedef sacapi_bool (*sqlany_execute_immediate_t)( a_sqlany_connection * sqlany_conn, const char * sql );
typedef a_sqlany_stmt * (*sqlany_prepare_t)( a_sqlany_connection * sqlany_conn, const char * sql_str );
typedef void (*sqlany_free_stmt_t)( a_sqlany_stmt * sqlany_stmt );
typedef sacapi_i32 (*sqlany_num_params_t)( a_sqlany_stmt * sqlany_stmt );
typedef sacapi_bool (*sqlany_describe_bind_param_t)( a_sqlany_stmt * sqlany_stmt, sacapi_u32 index, a_sqlany_bind_param * params );
typedef sacapi_bool (*sqlany_bind_param_t)( a_sqlany_stmt * sqlany_stmt, sacapi_u32 index, a_sqlany_bind_param * params );
typedef sacapi_bool (*sqlany_send_param_data_t)( a_sqlany_stmt * sqlany_stmt, sacapi_u32 index, char * buffer, size_t size );
typedef sacapi_bool (*sqlany_reset_t)( a_sqlany_stmt * sqlany_stmt );
typedef sacapi_bool (*sqlany_get_bind_param_info_t)( a_sqlany_stmt * sqlany_stmt, sacapi_u32 index, a_sqlany_bind_param_info * info );
typedef sacapi_bool (*sqlany_execute_t)( a_sqlany_stmt * sqlany_stmt );
typedef a_sqlany_stmt * (*sqlany_execute_direct_t)( a_sqlany_connection * sqlany_conn, const char * sql_str );
typedef sacapi_bool (*sqlany_fetch_absolute_t)( a_sqlany_stmt * sqlany_result, sacapi_i32 row_num );
typedef sacapi_bool (*sqlany_fetch_next_t)( a_sqlany_stmt * sqlany_stmt );
typedef sacapi_bool (*sqlany_get_next_result_t)( a_sqlany_stmt * sqlany_stmt );
typedef sacapi_i32 (*sqlany_affected_rows_t)( a_sqlany_stmt * sqlany_stmt );
typedef sacapi_i32 (*sqlany_num_cols_t)( a_sqlany_stmt * sqlany_stmt );
typedef sacapi_i32 (*sqlany_num_rows_t)( a_sqlany_stmt * sqlany_stmt );
typedef sacapi_bool (*sqlany_get_column_t)( a_sqlany_stmt * sqlany_stmt, sacapi_u32 col_index, a_sqlany_data_value * buffer );
typedef sacapi_i32 (*sqlany_get_data_t)( a_sqlany_stmt * sqlany_stmt, sacapi_u32 col_index, size_t offset, void * buffer, size_t size );
typedef sacapi_bool (*sqlany_get_data_info_t)( a_sqlany_stmt * sqlany_stmt, sacapi_u32 col_index, a_sqlany_data_info * buffer );
typedef sacapi_bool (*sqlany_get_column_info_t)( a_sqlany_stmt * sqlany_stmt, sacapi_u32 col_index, a_sqlany_column_info * buffer );
typedef sacapi_bool (*sqlany_commit_t)( a_sqlany_connection * sqlany_conn );
typedef sacapi_bool (*sqlany_rollback_t)( a_sqlany_connection * sqlany_conn );
typedef sacapi_bool (*sqlany_client_version_t)( char * buffer, size_t len );
typedef sacapi_i32 (*sqlany_error_t)( a_sqlany_connection * sqlany_conn, char * buffer, size_t size );
typedef size_t (*sqlany_sqlstate_t)( a_sqlany_connection * sqlany_conn, char * buffer, size_t size );
typedef void (*sqlany_clear_error_t)( a_sqlany_connection * sqlany_conn );
// newer API
typedef a_sqlany_interface_context *(*sqlany_init_ex_t)( const char *app_name, sacapi_u32 api_version, sacapi_u32 *max_version );
typedef void (*sqlany_fini_ex_t)( a_sqlany_interface_context *context );
typedef a_sqlany_connection *(*sqlany_new_connection_ex_t)( a_sqlany_interface_context *context );
typedef a_sqlany_connection *(*sqlany_make_connection_ex_t)( a_sqlany_interface_context *context, void *arg );
typedef sacapi_bool (*sqlany_client_version_ex_t)( a_sqlany_interface_context *context, char *buffer, size_t len );
typedef void (*sqlany_cancel_t)( a_sqlany_connection * sqlany_conn );

// API declarations
class SQLAPI_API asaAPI : public saAPI
{
public:
	asaAPI();

	sqlany_init_t sqlany_init;
	sqlany_fini_t sqlany_fini;
	sqlany_new_connection_t sqlany_new_connection;
	sqlany_free_connection_t sqlany_free_connection;
	sqlany_make_connection_t sqlany_make_connection;
	sqlany_connect_t sqlany_connect;
	sqlany_disconnect_t sqlany_disconnect;
	sqlany_execute_immediate_t sqlany_execute_immediate;
	sqlany_prepare_t sqlany_prepare;
	sqlany_free_stmt_t sqlany_free_stmt;
	sqlany_num_params_t sqlany_num_params;
	sqlany_describe_bind_param_t sqlany_describe_bind_param;
	sqlany_bind_param_t sqlany_bind_param;
	sqlany_send_param_data_t sqlany_send_param_data;
	sqlany_reset_t sqlany_reset;
	sqlany_get_bind_param_info_t sqlany_get_bind_param_info;
	sqlany_execute_t sqlany_execute;
	sqlany_execute_direct_t sqlany_execute_direct;
	sqlany_fetch_absolute_t sqlany_fetch_absolute;
	sqlany_fetch_next_t sqlany_fetch_next;
	sqlany_get_next_result_t sqlany_get_next_result;
	sqlany_affected_rows_t sqlany_affected_rows;
	sqlany_num_cols_t sqlany_num_cols;
	sqlany_num_rows_t sqlany_num_rows;
	sqlany_get_column_t sqlany_get_column;
	sqlany_get_data_t sqlany_get_data;
	sqlany_get_data_info_t sqlany_get_data_info;
	sqlany_get_column_info_t sqlany_get_column_info;
	sqlany_commit_t sqlany_commit;
	sqlany_rollback_t sqlany_rollback;
	sqlany_client_version_t sqlany_client_version;
	sqlany_error_t sqlany_error;
	sqlany_sqlstate_t sqlany_sqlstate;
	sqlany_clear_error_t sqlany_clear_error;
// newer API
	sqlany_init_ex_t sqlany_init_ex;
	sqlany_fini_ex_t sqlany_fini_ex;
	sqlany_new_connection_ex_t sqlany_new_connection_ex;
	sqlany_make_connection_ex_t sqlany_make_connection_ex;
	sqlany_client_version_ex_t sqlany_client_version_ex;
	sqlany_cancel_t sqlany_cancel;
};

class SQLAPI_API asaConnectionHandles : public saConnectionHandles
{
public:
	asaConnectionHandles();
	a_sqlany_connection *pDb;
};

class SQLAPI_API asaCommandHandles : public saCommandHandles
{
public:
	asaCommandHandles();
	a_sqlany_stmt *pStmt;
};

extern asaAPI g_asaAPI;

#endif //__ASAAPI_H__
