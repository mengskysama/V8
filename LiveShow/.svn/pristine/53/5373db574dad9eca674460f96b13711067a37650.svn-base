#pragma once

#define CLIENT_COFIG_FILE							L"config\\config.xml"
#define CLIENT_COFIG_FILE_SERVER_TOKEN				L"server.token"
#define CLIENT_COFIG_FILE_SERVER_TOKEN_179_PORT		L"port_179"
#define CLIENT_COFIG_FILE_SERVER_TOKEN_VISITOR_PORT	L"port_visitor"
#define CLIENT_COFIG_FILE_SERVER_HALL_PORT			L"port_hall"
#define CLIENT_COFIG_FILE_SERVER_HALL				L"server.hall"
#define CLIENT_COFIG_FILE_SERVER_CNC_IP				L"ip_cnc"
#define CLIENT_COFIG_FILE_SERVER_CTC_IP				L"ip_ctc"

#define TOKEN_TIMEOUT			16
#define HALL_LOGIN_TIMEOUT		26
#define LOGIN_RETRY_CIRCLE		5
#define HALL_HEARTBEAT_TIMEOUT	60
#define CYCLE_TOKEN_TIMEER		1800

enum ENM_CONECTION_STATE
{
	enm_state_notbegin,
	enm_state_getting_ip,//正在域名解析
	enm_state_doing,
	enm_state_ok,
	enm_state_fail,
};

enum ENM_SERVER_TYPE
{
	enm_server_type_179,
	enm_server_type_visitor,
	enm_server_type_hall,
};

enum ENM_NET_TYPE
{
	enm_net_type_null,
	enm_net_type_cnc,
	enm_net_type_ctc,
};