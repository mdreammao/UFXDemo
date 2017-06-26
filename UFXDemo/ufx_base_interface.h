#ifndef _UFX_BASE_INTERFACE_H
#define _UFX_BASE_INTERFACE_H

#include "t2sdk_interface.h"
#include <string>

using namespace std;

const int RET_OK = 0;
const int RET_ERR = -1;

const int UFX_FUNCNO_USER_LOGON = 10001;
const int UFX_FUNCNO_USER_LOGOFF = 10002;
const int UFX_FUNCNO_ENTRUST = 91090;

const int UFX_UFT_ENTRUST = 21001;

const int PACKER_VIRSION = 0x20;
const int DEFAULT_TIME_OUT = 10000;

const int MAX_OPERATOR_LEN = 18;
const int MAX_PASSWORD_LEN = 10;

class CUFXBaseInterfaceParam
{
public:
    int m_out_ErrorCode;
    std::string m_out_ErrorMsg;
    std::string m_out_MsgDetail;
    int m_out_DataCount;
    
    CUFXBaseInterfaceParam(){ClearInfo();};
    virtual ~CUFXBaseInterfaceParam(){};
    
    virtual int ClearInfo();
    virtual int CheckParamValid()=0;
    virtual int PackParam(IF2Packer *lpPacker)=0;
    virtual int UpackResult(IF2UnPacker *lpUnpacker)=0;
    virtual int UpackHeader(IF2UnPacker *lpUnpacker); //解压头部的错误信息
    virtual void LogParam()=0;
    virtual int GetFuncNo()=0;
    virtual int GetTimeOut() { return DEFAULT_TIME_OUT; };
};

class CUFXUserLogon:public CUFXBaseInterfaceParam
{
public:
    CUFXUserLogon();
    ~CUFXUserLogon();

    string m_in_operator_no;
    string m_in_mac_address;
    string m_in_op_station;
    string m_in_ip_address;
    string m_in_password;

    string m_out_user_token;

    int CheckParamValid();
    int PackParam(IF2Packer *lpPacker);
    int UpackResult(IF2UnPacker *lpUnpacker);
    void LogParam();
    int GetFuncNo() { return UFX_FUNCNO_USER_LOGON; };
    int GetTimeOut() { return DEFAULT_TIME_OUT; };
};

class CUFXUserLogOff:public CUFXBaseInterfaceParam
{
public:
  CUFXUserLogOff();
  ~CUFXUserLogOff();

  string m_in_user_token;
  string m_in_operator_no;

  int CheckParamValid();
  int PackParam(IF2Packer *lpPacker);
  int UpackResult(IF2UnPacker *lpUnpacker);
  void LogParam();
  int GetFuncNo() { return UFX_FUNCNO_USER_LOGOFF; };
  int GetTimeOut() { return DEFAULT_TIME_OUT; };
};

class CUFXSyncConnection
{
private:
    IF2Packer *m_pPacker;
    void *m_pUnpacker;
    CUFXBaseInterfaceParam *m_pUFXParam;
    CConnectionInterface *m_pConnection;

    int ParseCallUFXRet();
public:
    ~CUFXSyncConnection();
    CUFXSyncConnection(CConfigInterface * lpConfig);
    
    int InitCallParam(CUFXBaseInterfaceParam *ufxParam);
    int SyncCallUFX();
    const char *GetErrorMsg(int iErrCode);

    void show_packet();
};

void ShowPacket(IF2UnPacker *lpUnPacker);

#endif