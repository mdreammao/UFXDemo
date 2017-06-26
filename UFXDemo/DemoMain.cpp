#include "ufx_base_interface.h"
#include "ufx_entrust_interface.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

void ShowPacket(IF2UnPacker *lpUnPacker);

CUFXUserLogon * get_logon_info(const string &strOperNo,const string &strPasswd);
CUFTEntrust * get_uft_entrust_info(const string  &strUserToken);

CUFXEntrust * get_UFT_batch_entrust_info(const string  &strUserToken);
CUFXEntrust * get_O3_batch_entrust_info(const string  &strUserToken);

int Logon(const string &strOperNo,const string &strPasswd,string &strToken,CUFXSyncConnection *ufxCon);
int Logoff(const string &strOperNo,const string &strToken,CUFXSyncConnection *ufxCon);

int call_service(CUFXSyncConnection *ufxCon,CUFXBaseInterfaceParam *ufxParam);

int call_service(CUFXSyncConnection *ufxCon,CUFXBaseInterfaceParam *ufxParam,int iHintMode);

int main()
{
    CConfigInterface * lpConfig = NewConfig();

    //lpConfig->SetString("t2sdk", "servers", "10.88.114.18:9004");
    lpConfig->SetString("t2sdk", "servers", "192.168.54.57:9003");
    lpConfig->SetString("t2sdk", "license_file", "license.dat");
    lpConfig->SetString("t2sdk", "send_queue_size", "100");

    CUFXSyncConnection *ufxCon = new CUFXSyncConnection(lpConfig);
    string strToken = "";
    string strOperatorNo = "1000";
    string strPassword = "0";

    if (Logon(strOperatorNo,strPassword,strToken,ufxCon) == RET_OK)
    {
        CUFXBaseInterfaceParam * pO3BizParam = NULL;
        CUFXBaseInterfaceParam * pUFTBizParam = NULL;
        pO3BizParam = get_O3_batch_entrust_info(strToken);
        pUFTBizParam = get_UFT_batch_entrust_info(strToken);

        for(int i=0;i<1;i++)
        {
          printf("调用O3接口\n");
          call_service(ufxCon,pO3BizParam,2);

          printf("调用UFT接口\n");
          call_service(ufxCon,pUFTBizParam,2);
        }
        delete pUFTBizParam;
        delete pO3BizParam;

        if (Logoff(strOperatorNo,strToken,ufxCon) != RET_OK)
        {
          printf("退出登录连接失败！\n");
        }
    }
    else
    {
      printf("登录连接失败！\n");
    }

    delete ufxCon;
    lpConfig->Release();

    system("pause");    
    return 0;
}

int Logon(const string &strOperNo,const string &strPasswd,string &strToken,CUFXSyncConnection *ufxCon)
{
    CUFXUserLogon *userLogon = get_logon_info(strOperNo,strPasswd);
    strToken = "";
    
    int iRetCallUFX = call_service(ufxCon,userLogon);
    if(iRetCallUFX == RET_OK)
    {
        if (userLogon->m_out_ErrorCode != 0)
        {
          printf("登录失败:%s\n",userLogon->m_out_ErrorMsg);
          iRetCallUFX = userLogon->m_out_ErrorCode;
        }

        strToken = userLogon->m_out_user_token; 
    }
    delete userLogon;

    return iRetCallUFX;
}

int Logoff(const string &strOperNo,const string &strToken,CUFXSyncConnection *ufxCon)
{
  CUFXUserLogOff userLogoff;
  userLogoff.m_in_operator_no = strOperNo;
  userLogoff.m_in_user_token = strToken;

  int iRetCallUFX = call_service(ufxCon,&userLogoff);
  if(iRetCallUFX == RET_OK)
  {
     if (userLogoff.m_out_ErrorCode != 0)
     {
        printf("退出登录失败:%s\n",userLogoff.m_out_ErrorMsg);
        iRetCallUFX = userLogoff.m_out_ErrorCode;
     }
  }

  return iRetCallUFX;
}

int call_service(CUFXSyncConnection *ufxCon,CUFXBaseInterfaceParam *ufxParam)
{
    ufxCon->InitCallParam(ufxParam);
    int iRetCallUFX = ufxCon->SyncCallUFX();

    if (iRetCallUFX != RET_OK)
    {
        printf("连接UFX错误:%s\n",ufxCon->GetErrorMsg(iRetCallUFX));
    }

    return iRetCallUFX;
}

int call_service(CUFXSyncConnection *ufxCon,CUFXBaseInterfaceParam *ufxParam,int iHintMode)
{
  int iRetCallUFX = call_service(ufxCon,ufxParam);

  if (iRetCallUFX != RET_OK) return iRetCallUFX;

  //成功时，根据提示模式打印输出
  switch(iHintMode)
  {
  case 1: //只打印错误
    {
      if (ufxParam->m_out_ErrorCode != 0)
      {
        ufxCon->show_packet();    
      }

      break;
    }
  case 2: //打印所有
    {
      ufxCon->show_packet();
    }
  }

  return iRetCallUFX;
}

CUFXUserLogon * get_logon_info(const string &strOperNo,const string &strPasswd)
{
    CUFXUserLogon *userLogon = new CUFXUserLogon();

    userLogon->m_in_operator_no = strOperNo;
    userLogon->m_in_password = strPasswd;    
    userLogon->m_in_mac_address = "000000000001";
    userLogon->m_in_op_station = "111111";
    userLogon->m_in_ip_address = "127.0.0.1";

    return userLogon;
}

CUFTEntrust * get_uft_entrust_info(const string  &strUserToken)
{
    CUFTEntrust *uftEntrust = new CUFTEntrust();

    //国泰君安用例
    //uftEntrust->m_in_user_token = strUserToken;
    //uftEntrust->m_in_operator_no = "1000";
    //uftEntrust->m_in_combine_id = "8501";
    //uftEntrust->m_in_exchange_type = "1";
    //uftEntrust->m_in_stock_code = "600000";
    //uftEntrust->m_in_entrust_direction = "1";
    //uftEntrust->m_in_amprice_type = "0";
    //uftEntrust->m_in_entrust_price = 10;
    //uftEntrust->m_in_entrust_amount = 1000;
    //uftEntrust->m_in_instance_id = "";
    //uftEntrust->m_in_stock_account = "D890333263";
    //uftEntrust->m_in_seat_no = "23670";
    //uftEntrust->m_in_invest_type = "1";

    //54.57用例
    uftEntrust->m_in_user_token = strUserToken;
    uftEntrust->m_in_operator_no = "1000";
    uftEntrust->m_in_combine_id = "8888_000";
    uftEntrust->m_in_exchange_type = "1";
    uftEntrust->m_in_stock_code = "600000";
    uftEntrust->m_in_entrust_direction = "1";
    uftEntrust->m_in_amprice_type = "0";
    uftEntrust->m_in_entrust_price = 13;
    uftEntrust->m_in_entrust_amount = 100;
    uftEntrust->m_in_instance_id = "";
    uftEntrust->m_in_stock_account = "D890021442";
    uftEntrust->m_in_seat_no = "22392";
    uftEntrust->m_in_invest_type = "1";

    return uftEntrust;
}


CUFXEntrust * get_UFT_batch_entrust_info(const string  &strUserToken)
{
  CUFXEntrust *pBatchEntrust = new CUFXEntrust();

  pBatchEntrust->m_in_user_token = strUserToken;
  pBatchEntrust->m_in_account_code = "8888";
  pBatchEntrust->m_in_combi_no = "8888_000";
  pBatchEntrust->m_in_batch_no = 0;
  pBatchEntrust->m_instance_no = "";
  pBatchEntrust->m_in_max_cancel_ratio = 0;

  CUFXEntrustStock *pFutureStock = new CUFXEntrustStock;

  pFutureStock->m_market_no = "7";
  pFutureStock->m_stock_code = "IF1109";
  pFutureStock->m_entrust_direction = "1";
  pFutureStock->m_futures_direction = "1";
  pFutureStock->m_price_type = "2";
  pFutureStock->m_entrust_price = 2820;
  pFutureStock->m_entrust_amount = 2;
  pFutureStock->m_stockholder_id = "";
  pFutureStock->m_report_seat = "";
  pFutureStock->m_invest_type = "c";
  pFutureStock->m_extsystem_id = 30;
  pFutureStock->m_third_reff = "wuch";

  pBatchEntrust->AppendEntrustStock(pFutureStock);

  CUFXEntrustStock *pSpotStock = new CUFXEntrustStock;

  pSpotStock->m_market_no = "1";
  pSpotStock->m_stock_code = "600000";
  pSpotStock->m_entrust_direction = "1";
  pSpotStock->m_futures_direction = "";
  pSpotStock->m_price_type = "0";
  pSpotStock->m_entrust_price = 130.2;
  pSpotStock->m_entrust_amount = 200;
  pSpotStock->m_stockholder_id = "";
  pSpotStock->m_report_seat = "";
  pSpotStock->m_invest_type = "1";
  pSpotStock->m_extsystem_id = 30;
  pSpotStock->m_third_reff = "wuch";

  pBatchEntrust->AppendEntrustStock(pSpotStock);

  return pBatchEntrust;

}


CUFXEntrust * get_O3_batch_entrust_info(const string  &strUserToken)
{
  CUFXEntrust *pBatchEntrust = new CUFXEntrust();

  pBatchEntrust->m_in_user_token = strUserToken;
  pBatchEntrust->m_in_account_code = "184693";
  pBatchEntrust->m_in_combi_no = "1000005";
  pBatchEntrust->m_in_batch_no = 0;
  pBatchEntrust->m_instance_no = "";
  pBatchEntrust->m_in_max_cancel_ratio = 0;

  CUFXEntrustStock *pFutureStock = new CUFXEntrustStock;

  pFutureStock->m_market_no = "7";
  pFutureStock->m_stock_code = "IF1109";
  pFutureStock->m_entrust_direction = "1";
  pFutureStock->m_futures_direction = "1";
  pFutureStock->m_price_type = "2";
  pFutureStock->m_entrust_price = 2820;
  pFutureStock->m_entrust_amount = 2;
  pFutureStock->m_stockholder_id = "";
  pFutureStock->m_report_seat = "";
  pFutureStock->m_invest_type = "c";
  pFutureStock->m_extsystem_id = 30;
  pFutureStock->m_third_reff = "wuch O3";

  pBatchEntrust->AppendEntrustStock(pFutureStock);

  CUFXEntrustStock *pSpotStock = new CUFXEntrustStock;

  pSpotStock->m_market_no = "1";
  pSpotStock->m_stock_code = "600001";
  pSpotStock->m_entrust_direction = "1";
  pSpotStock->m_futures_direction = "";
  pSpotStock->m_price_type = "0";
  pSpotStock->m_entrust_price = 13.2;
  pSpotStock->m_entrust_amount = 200;
  pSpotStock->m_stockholder_id = "";
  pSpotStock->m_report_seat = "";
  pSpotStock->m_invest_type = "1";
  pSpotStock->m_extsystem_id = 30;
  pSpotStock->m_third_reff = "wuch O3";

  pBatchEntrust->AppendEntrustStock(pSpotStock);

  return pBatchEntrust;

}