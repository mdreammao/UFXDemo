#include "ufx_entrust_interface.h"
#include "util_str.h"

CUFXEntrustStock::CUFXEntrustStock()
{
  m_market_no = "";
  m_stock_code = "";
  m_entrust_direction = "";
  m_futures_direction = "";
  m_price_type = "";
  m_entrust_price = 0;
  m_entrust_amount = 0;
  m_stockholder_id = "";
  m_report_seat = "";
  m_invest_type = "";
  m_extsystem_id = 0;
  m_third_reff = "";
}

CUFXEntrustStock::~CUFXEntrustStock()
{

}

CUFXEntrust::CUFXEntrust()
{
    ClearEntrustInfo();
}

CUFXEntrust::~CUFXEntrust()
{
    ClearEntrustStock();
}

void CUFXEntrust::ClearEntrustInfo()
{
  m_in_user_token = "";
  m_in_account_code = "";
  m_in_combi_no = "";
  m_in_batch_no = 0;
  m_instance_no = "";
  m_in_max_cancel_ratio = 0;

  ClearEntrustStock();

  m_out_batch_no=0;
}

void CUFXEntrust::ClearEntrustStock()
{
    for(vector<CUFXEntrustStock *>::iterator it = m_in_stock_list.begin(); it != m_in_stock_list.end();it++)
    {
        delete *it;
    }

    m_in_stock_list.clear();
}

int CUFXEntrust::CheckParamValid()
{
    return RET_OK;
}

int CUFXEntrust::PackParam(IF2Packer *lpPacker)
{
    if (NULL == lpPacker) return RET_ERR;

    lpPacker->BeginPack();

    lpPacker->AddField("user_token");
    lpPacker->AddField("account_code");
    lpPacker->AddField("combi_no");
    lpPacker->AddField("batch_no");
    lpPacker->AddField("instance_no");
    lpPacker->AddField("max_cancel_ratio");

    lpPacker->AddField("market_no");
    lpPacker->AddField("stock_code");
    lpPacker->AddField("entrust_direction");
    lpPacker->AddField("futures_direction");
    lpPacker->AddField("price_type");
    lpPacker->AddField("entrust_price");

    lpPacker->AddField("entrust_amount");
    lpPacker->AddField("stockholder_id");
    lpPacker->AddField("report_seat");
    lpPacker->AddField("invest_type");
    lpPacker->AddField("extsystem_id");
    lpPacker->AddField("third_reff");

    for(vector<CUFXEntrustStock *>::iterator it = m_in_stock_list.begin(); it != m_in_stock_list.end();it++)
    {
        lpPacker->AddStr(m_in_user_token.c_str());
        lpPacker->AddStr(m_in_account_code.c_str());
        lpPacker->AddStr(m_in_combi_no.c_str());
        lpPacker->AddInt(m_in_batch_no);
        lpPacker->AddStr(m_instance_no.c_str());
        lpPacker->AddDouble(m_in_max_cancel_ratio);

        lpPacker->AddStr((*it)->m_market_no.c_str());
        lpPacker->AddStr((*it)->m_stock_code.c_str());
        lpPacker->AddStr((*it)->m_entrust_direction.c_str());
        lpPacker->AddStr((*it)->m_futures_direction.c_str());
        lpPacker->AddStr((*it)->m_price_type.c_str());
        lpPacker->AddDouble((*it)->m_entrust_price);

        lpPacker->AddInt((*it)->m_entrust_amount);
        lpPacker->AddStr((*it)->m_stockholder_id.c_str());
        lpPacker->AddStr((*it)->m_report_seat.c_str());
        lpPacker->AddStr((*it)->m_invest_type.c_str());
        lpPacker->AddInt((*it)->m_extsystem_id);
        lpPacker->AddStr((*it)->m_third_reff.c_str());
    }

    lpPacker->EndPack();

    return RET_OK;
}

int CUFXEntrust::UpackResult(IF2UnPacker *lpUnpacker)
{
    if (NULL == lpUnpacker) return RET_ERR;

    m_out_batch_no = lpUnpacker->GetInt("batch_no");

    return RET_OK;
}

int CUFXEntrust::AppendEntrustStock(CUFXEntrustStock *pEntrustStock)
{
    if (NULL == pEntrustStock) return RET_ERR;

    m_in_stock_list.push_back(pEntrustStock);

    return RET_OK;
}

void CUFXEntrust::LogParam()
{
    
}




CUFTEntrust::CUFTEntrust()
{
    ClearEntrustInfo();
}

CUFTEntrust::~CUFTEntrust()
{
}

void CUFTEntrust::ClearEntrustInfo()
{
    m_in_user_token = "";
    m_in_operator_no = "";
    m_in_combine_id = "";
    m_in_exchange_type = "";
    m_in_stock_code = "";
    m_in_entrust_direction = "";
    m_in_amprice_type = "";
    m_in_entrust_price = 0;
    m_in_entrust_amount = 0;
    m_in_instance_id = "";
    m_in_stock_account = "";
    m_in_seat_no = "";
    m_in_invest_type = "";

    m_out_error_no = 0;
    m_out_error_msg = "";
    m_out_msg_detail = "";
    m_out_l_entrust_no = 0;
    m_out_l_batch_serial_no = 0;
    m_out_l_report_serial_no = 0;
}

int CUFTEntrust::CheckParamValid()
{
    return RET_OK;
}

int CUFTEntrust::PackParam(IF2Packer *lpPacker)
{
    if (NULL == lpPacker) return RET_ERR;

    lpPacker->BeginPack();

    lpPacker->AddField("user_token");
    lpPacker->AddField("operator_no");
    lpPacker->AddField("combine_id");
    lpPacker->AddField("exchange_type");
    lpPacker->AddField("stock_code");
    lpPacker->AddField("entrust_direction");
    lpPacker->AddField("amprice_type");
    lpPacker->AddField("entrust_price");
    lpPacker->AddField("entrust_amount");
    lpPacker->AddField("instance_id");
    lpPacker->AddField("stock_account");
    lpPacker->AddField("seat_no");
    lpPacker->AddField("invest_type");

    lpPacker->AddStr(m_in_user_token.c_str());
    lpPacker->AddStr(m_in_operator_no.c_str());
    lpPacker->AddStr(m_in_combine_id.c_str());
    lpPacker->AddStr(m_in_exchange_type.c_str());
    lpPacker->AddStr(m_in_stock_code.c_str());
    lpPacker->AddStr(m_in_entrust_direction.c_str());
    lpPacker->AddStr(m_in_amprice_type.c_str());
    lpPacker->AddDouble(m_in_entrust_price);
    lpPacker->AddInt(m_in_entrust_amount);
    lpPacker->AddStr(m_in_instance_id.c_str());
    lpPacker->AddStr(m_in_stock_account.c_str());
    lpPacker->AddStr(m_in_seat_no.c_str());
    lpPacker->AddStr(m_in_invest_type.c_str());
       
    lpPacker->EndPack();

    return RET_OK;
}

int CUFTEntrust::UpackResult(IF2UnPacker *lpUnpacker)
{
    if (NULL == lpUnpacker) return RET_ERR;

    //m_out_error_no = lpUnpacker->GetInt("error_no");
    //m_out_error_msg = charToStr(lpUnpacker->GetStr("error_msg"));
    //m_out_msg_detail = charToStr(lpUnpacker->GetStr("msg_detail"));
    //m_out_entrust_no = lpUnpacker->GetInt("entrust_no");
    //m_out_batch_no = lpUnpacker->GetInt("batch_no");

    //return RET_OK;


    if (NULL == lpUnpacker) return RET_ERR;

    int iErrorCode = UpackHeader(lpUnpacker);
    if (iErrorCode != 0) return iErrorCode;

    if (lpUnpacker->GetDatasetCount() > 1)
      lpUnpacker->SetCurrentDatasetByIndex(1);
    
    m_out_error_no = lpUnpacker->GetInt("error_no");
    m_out_error_msg = charToStr(lpUnpacker->GetStr("error_msg"));
        
    m_out_l_entrust_no = lpUnpacker->GetInt("l_entrust_no");
    m_out_l_batch_serial_no = lpUnpacker->GetInt("l_batch_serial_no");
    m_out_l_report_serial_no = lpUnpacker->GetInt("l_report_serial_no");

    return RET_OK;
}

void CUFTEntrust::LogParam()
{

}