#pragma once
#include <string>
#import "C:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename("EOF", "EndOfFile")
class DataBaseConnect {
public:
	static DataBaseConnect& getInstance();
	std::string SQLQuery(std::string sql_table, std::string sql_field);
	bool GetConnectorFlag();
private:
	DataBaseConnect();
	~DataBaseConnect();
	DataBaseConnect(const DataBaseConnect&) = delete; // 禁用拷贝构造函数
	void operator=(const DataBaseConnect&) = delete; // 禁用赋值操作符
	_ConnectionPtr pConn_;
	HRESULT hr_;
	bool connector_flag_;
};