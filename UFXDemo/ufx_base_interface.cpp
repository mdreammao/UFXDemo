#include "ufx_base_interface.h"
#include "util_str.h"

void ShowPacket(IF2UnPacker *lpUnPacker)
{
    int i = 0, t = 0, j = 0, k = 0;

    for (i = 0; i < lpUnPacker->GetDatasetCount(); ++i)
    {
        // 设置当前结果集
        lpUnPacker->SetCurrentDatasetByIndex(i);

        printf("----------------[%d]-----------------\n",i);

        // 打印所有记录
        for (j = 0; j < (int)lpUnPacker->GetRowCount(); ++j)
        {
            // 打印每条记录
            for (k = 0; k < lpUnPacker->GetColCount(); ++k)
            {
                printf("%20s = ", lpUnPacker->GetColName(k));
                switch (lpUnPacker->GetColType(k))
                {
                case 'I':
                    printf("%20d\n", lpUnPacker->GetIntByIndex(k));
                    break;

                case 'C':
                    printf("%20c\n", lpUnPacker->GetCharByIndex(k));
                    break;

                case 'S':
                    printf("%20s\n", lpUnPacker->GetStrByIndex(k));
                    break;

                case 'F':
                    printf("%20f\n", lpUnPacker->GetDoubleByIndex(k));
                    break;

                case 'R':
                    {
                        int nLength = 0;
                        void *lpData = lpUnPacker->GetRawByIndex(k, &nLength);

                        // 对2进制数据进行处理
                        break;
                    }
                default:
                    // 未知数据类型
                    printf("未知数据类型。\n");
                    break;
                }
            }
            lpUnPacker->Next();
        }
    }
}

void CUFXSyncConnection::show_packet()
{
    if (NULL == m_pUnpacker)
    {
        printf("返回报文为空！！！\n");
        return;
    }

    ShowPacket((IF2UnPacker *)m_pUnpacker);
}

int CUFXSyncConnection::InitCallParam(CUFXBaseInterfaceParam *ufxParam)
{
    m_pUFXParam = ufxParam;

    return RET_OK;
}

CUFXSyncConnection::CUFXSyncConnection(CConfigInterface * lpConfig)
{
    int iRet = 0;
    m_pConnection = NewConnection(lpConfig);

    iRet = m_pConnection->Create(NULL);
    if(iRet != 0) printf("连接出错：%s",m_pConnection->GetErrorMsg(iRet));

    m_pPacker = NewPacker(2);
    m_pUnpacker = NULL;
}

CUFXSyncConnection::~CUFXSyncConnection()
{
    if (NULL != m_pConnection) m_pConnection->Release();
    if (NULL != m_pPacker) 
    {
        m_pPacker->FreeMem(m_pPacker->GetPackBuf());
        m_pPacker->Release();
    }
}

int CUFXSyncConnection::SyncCallUFX()
{
    if (NULL == m_pConnection)  return RET_ERR;
    if (NULL == m_pUFXParam) return RET_ERR;
    if (NULL == m_pPacker) return RET_ERR;

    m_pUFXParam->PackParam(m_pPacker);

    int hSend = m_pConnection->SendBiz(m_pUFXParam->GetFuncNo(),m_pPacker,0);

    int iRcv = m_pConnection->RecvBiz(hSend,&m_pUnpacker,m_pUFXParam->GetTimeOut());

    if (iRcv != 0)
    {
        printf("RecvBiz错误:%s\n",m_pConnection->GetErrorMsg(iRcv));
        return iRcv;
    }

    m_pUFXParam->UpackResult((IF2UnPacker *)m_pUnpacker);

    return RET_OK;
}

const char *CUFXSyncConnection::GetErrorMsg(int iErrCode)
{
    if (NULL == m_pConnection)  return "m_pConnection为空";

    return m_pConnection->GetErrorMsg(iErrCode);
}

CUFXUserLogon::CUFXUserLogon()
{
    m_in_operator_no = "";
    m_in_password = "";
    m_in_mac_address = "";
    m_in_op_station = "";
    m_in_ip_address = "";

    m_out_user_token = "";
}

CUFXUserLogon::~CUFXUserLogon()
{
}

int CUFXUserLogon::CheckParamValid()
{
    if (m_in_operator_no.length() > MAX_OPERATOR_LEN) return RET_ERR;
    if (m_in_password.length() > MAX_PASSWORD_LEN) return RET_ERR;
    
    return RET_OK;
}

int CUFXUserLogon::PackParam(IF2Packer *lpPacker)
{
   if (NULL == lpPacker) return RET_ERR;

   lpPacker->BeginPack();

   lpPacker->AddField("operator_no");
   lpPacker->AddField("password");
   lpPacker->AddField("mac_address");
   lpPacker->AddField("op_station");
   lpPacker->AddField("ip_address");

   lpPacker->AddStr(m_in_operator_no.c_str());
   lpPacker->AddStr(m_in_password.c_str());
   lpPacker->AddStr(m_in_mac_address.c_str());
   lpPacker->AddStr(m_in_op_station.c_str());
   lpPacker->AddStr(m_in_ip_address.c_str());
  
   lpPacker->EndPack();

   return RET_OK;
}

int CUFXUserLogon::UpackResult(IF2UnPacker *lpUnpacker)
{
    if (NULL == lpUnpacker) return RET_ERR;
    
    int iErrorCode = UpackHeader(lpUnpacker);
    if (iErrorCode != 0) return iErrorCode;
    
    if (lpUnpacker->GetDatasetCount() > 1)
      lpUnpacker->SetCurrentDatasetByIndex(1);
    
    m_out_user_token = charToStr(lpUnpacker->GetStr("user_token"));

    return RET_OK;
}

void CUFXUserLogon::LogParam()
{
    printf("operator_no=%s\n",m_in_operator_no.c_str());	
    printf("password=%s\n",m_in_password.c_str());
    printf("token=%s\n",m_out_user_token.c_str());
}

int CUFXBaseInterfaceParam::ClearInfo()
{
  m_out_ErrorCode = 0;
  m_out_ErrorMsg = "";
  m_out_MsgDetail = "";
  m_out_DataCount = 0;

  return 0;
}

int CUFXBaseInterfaceParam::UpackHeader( IF2UnPacker *lpUnpacker )
{
  m_out_ErrorCode = lpUnpacker->GetInt("ErrorCode");
  m_out_ErrorMsg = charToStr(lpUnpacker->GetStr("ErrorMsg"));
  m_out_MsgDetail = charToStr(lpUnpacker->GetStr("MsgDetail"));
  m_out_DataCount = lpUnpacker->GetInt("DataCount");  

  return m_out_ErrorCode;
}

int CUFXUserLogOff::PackParam( IF2Packer *lpPacker )
{
  if (NULL == lpPacker) return RET_ERR;

  lpPacker->BeginPack();

  lpPacker->AddField("user_token");
  lpPacker->AddField("operator_no");

  lpPacker->AddStr(m_in_user_token.c_str());
  lpPacker->AddStr(m_in_operator_no.c_str());

  lpPacker->EndPack();

  return RET_OK;  
}

int CUFXUserLogOff::UpackResult( IF2UnPacker *lpUnpacker )
{
  if (NULL == lpUnpacker) return RET_ERR;

  int iErrorCode = UpackHeader(lpUnpacker);
  if (iErrorCode != 0) return iErrorCode;

  return RET_OK;
}

CUFXUserLogOff::CUFXUserLogOff()
{
  m_in_user_token = "";
  m_in_operator_no = "";
}

CUFXUserLogOff::~CUFXUserLogOff()
{

}

int CUFXUserLogOff::CheckParamValid()
{
  
  return 0;
}

void CUFXUserLogOff::LogParam()
{

}
