#include "databaseconnect.h"
#include <comdef.h>
#include <xlocbuf>
DataBaseConnect::DataBaseConnect()
    :connector_flag_(false)
{
        /// 初始化 COM 环境
        CoInitialize(NULL);

        pConn_ = NULL;
        hr_ = pConn_.CreateInstance(__uuidof(Connection));
        if (SUCCEEDED(hr_)){
            // 连接字符串
            _bstr_t strConnect = "Provider=SQLOLEDB;Data Source=127.0.0.1;Initial Catalog=qtqq;User ID=sa;Password=123456;";

            // 打开数据库连接
            hr_ = pConn_->Open(strConnect, "", "", adModeUnknown);
            if (SUCCEEDED(hr_)) {
                connector_flag_ = true;
            }
        }
        else {
            // std::cerr << "Failed to create connection" << std::endl;
        }
}

DataBaseConnect::~DataBaseConnect()
{
    // 释放 COM 对象
    pConn_.Release();
    // 释放 COM 环境
    CoUninitialize();
}

DataBaseConnect& DataBaseConnect::getInstance()
{
    static DataBaseConnect instance; // 在首次调用时初始化，确保唯一实例
    return instance;
}

std::string DataBaseConnect::SQLQuery(std::string sql_table, std::string sql_field)
{
    if (SUCCEEDED(hr_))
    {
        //   std::cout << "Connected to database" << std::endl;
        _RecordsetPtr pRS = NULL;
        HRESULT hr = pRS.CreateInstance(__uuidof(Recordset));

        if (SUCCEEDED(hr))
        { 
            try
            {
                _variant_t vRecordsAffected;
                pRS = pConn_->Execute(_bstr_t(sql_table.c_str()), &vRecordsAffected, adCmdText);
            }
            catch (...)
            {
                return std::string(" ");
            }
            
           // while (!pRS->EndOfFile)
          //  {
                // 处理查询结果
                _variant_t vID = pRS->Fields->GetItem(_variant_t(sql_field.c_str()))->Value;

                if (vID.vt == VT_BSTR) // 如果是字符串类型
                {
                    std::string strID = (const char*)(_bstr_t)vID.bstrVal;
                    int a = 10;
                    return strID; // 返回 std::string 类型的值
                }
                else if (vID.vt == VT_I4) // 如果是整数类型
                {
                    int intValue = vID.intVal;
                    return std::to_string(intValue); // 转换为 std::string 类型并返回
                }
          //  }      
            pRS->Close();
            return std::string("");
        }
        else
        {
            pRS->Close();
            return std::string("");
        }
        pConn_->Close();
        return std::string("");
      
    }
    else
    {
        return std::string("");
    }
}

bool DataBaseConnect::GetConnectorFlag()
{
    return connector_flag_;
}
