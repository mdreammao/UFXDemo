#ifndef _UFX_ENTRUST_INTERFACE_H
#define _UFX_ENTRUST_INTERFACE_H

#include "t2sdk_interface.h"
#include "ufx_base_interface.h"
#include <string>
#include <vector>

class CUFXEntrustStock
{
public:
    CUFXEntrustStock();
    ~CUFXEntrustStock();
    string m_market_no;
    string m_stock_code;
    string m_entrust_direction;
    string m_futures_direction;
    string m_price_type;
    double m_entrust_price;
    int m_entrust_amount;
    string m_stockholder_id;
    string m_report_seat;
    string m_invest_type;
    int m_extsystem_id;
    string m_third_reff;
};

class CUFXEntrust:public CUFXBaseInterfaceParam
{
public:
    CUFXEntrust();
    ~CUFXEntrust();

    string m_in_user_token;
    string m_in_account_code;
    string m_in_combi_no;
    int m_in_batch_no;
    string m_instance_no;
    double m_in_max_cancel_ratio;

    vector<CUFXEntrustStock *> m_in_stock_list;

    int m_out_batch_no;

    void ClearEntrustInfo();
    void ClearEntrustStock();
    int AppendEntrustStock(CUFXEntrustStock *pEntrustStock);

    int CheckParamValid();
    int PackParam(IF2Packer *lpPacker);
    int UpackResult(IF2UnPacker *lpUnpacker);
    void LogParam();
    int GetFuncNo() { return UFX_FUNCNO_ENTRUST; };
    int GetTimeOut() { return DEFAULT_TIME_OUT * 10; };
};


class CUFTEntrust:public CUFXBaseInterfaceParam
{
public:
    CUFTEntrust();
    ~CUFTEntrust();

    string m_in_user_token;
    string  m_in_operator_no;
    string m_in_combine_id;
    string m_in_exchange_type;
    string m_in_stock_code;
    string m_in_entrust_direction;
    string m_in_amprice_type;
    double m_in_entrust_price;
    int m_in_entrust_amount;
    string m_in_instance_id;
    string m_in_stock_account;
    string m_in_seat_no;
    string m_in_invest_type;

    int m_out_error_no;
    string m_out_error_msg;
    string m_out_msg_detail;
    int m_out_l_entrust_no;
    int m_out_l_batch_serial_no;
    int m_out_l_report_serial_no;

    void ClearEntrustInfo();

    int CheckParamValid();
    int PackParam(IF2Packer *lpPacker);
    int UpackResult(IF2UnPacker *lpUnpacker);
    void LogParam();
    int GetFuncNo() { return UFX_UFT_ENTRUST; };
    int GetTimeOut() { return DEFAULT_TIME_OUT * 10; };
};


#endif