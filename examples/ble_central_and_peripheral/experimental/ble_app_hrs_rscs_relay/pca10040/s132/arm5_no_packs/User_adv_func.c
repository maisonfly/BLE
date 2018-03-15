#include "User_adv_func.h"

#include "ble.h"
#include "ble_advdata.h"
#include "ble_advertising.h"
#include "ble_conn_params.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"


extern uint32_t adv_report_parse(uint8_t type, uint8_array_t * p_advdata, uint8_array_t * p_typedata);
extern ble_gap_conn_params_t const m_connection_param;


/**@brief Parameters used when scanning. */
static ble_gap_scan_params_t const m_scan_params =
{
    .active   = 1,
    .interval = 0xa0,
    .window   = 0x50,
    .timeout  = 0,
    #if (NRF_SD_BLE_API_VERSION <= 2)
        .selective   = 0,
        .p_whitelist = NULL,
    #endif
    #if (NRF_SD_BLE_API_VERSION >= 3)
        .use_whitelist = 0,
    #endif
};




//匹配mac 地址
bool User_Match_Adv_Addr(ble_gap_addr_t addr,uint8_t *dev_mac)
{
    uint8_t i = 0;
    
    if(dev_mac == NULL)return false;
    
    for( i = 0; i < BLE_GAP_ADDR_LEN;i++)
    {
        if(addr.addr[i] != dev_mac[i])
        {
            break;
        }
    }
    
    if(i < BLE_GAP_ADDR_LEN)
    {
       return false; 
    }
    else
    {
        return true;
    }
}



    
 //匹配绑定标志
bool ScanRsp_Match_Binding(ble_gap_evt_t  const *p_gap_evt)
{
    bool ret_value;
    
    if(p_gap_evt->params.adv_report.scan_rsp == USER_ADV_SCAN_RSP)
    {
        //解析广播包中的扫描应答
        //接收到的广播的类型
        NRF_LOG_INFO("\r\n广播类型:%02d", p_gap_evt->params.adv_report.type);
        //应答标志位  为1 是 扫描应答 0 为广播数据
        NRF_LOG_INFO("\r\n 应答标志位%d  \r\n",p_gap_evt->params.adv_report.scan_rsp); 

        NRF_LOG_INFO("\r\n  mac 地址匹配成功");
        //  扫描到的周围的mac 地址
        #if 1
        NRF_LOG_INFO("设备广播地址mac:%02x %02x %02x %02x %02x %02x ",p_gap_evt->params.adv_report.peer_addr.addr[0],\
        p_gap_evt->params.adv_report.peer_addr.addr[1],\
        p_gap_evt->params.adv_report.peer_addr.addr[2],\
        p_gap_evt->params.adv_report.peer_addr.addr[3],\
        p_gap_evt->params.adv_report.peer_addr.addr[4],\
        p_gap_evt->params.adv_report.peer_addr.addr[5]\
        );
        #endif
        //打印所有的广播数据包

        NRF_LOG_INFO("\r\n-----------------------start-------------------");

        for(uint8_t adv_len = 0;adv_len < p_gap_evt->params.adv_report.dlen; adv_len++)
        {
            NRF_LOG_INFO("%02x",p_gap_evt->params.adv_report.data[adv_len]);
        }
        NRF_LOG_INFO("\r\n-----------------------end-------------------");
               
        ret_value = true;
    } 
    else
    {
        //return false;
        ret_value = false;
    }
    
    return ret_value;
}
   
_e_match_dev  step;

void  find_target_device(ble_evt_t const * p_ble_evt,ble_gap_addr_t  * device_addr)
{
    uint32_t      err_code;
    uint8_array_t adv_data;
    uint8_array_t dev_name;
    bool          do_connect = false;
    uint8_t       device_name[30];
    
    
    //For readibility.
    ble_gap_evt_t  const * p_gap_evt  = &p_ble_evt->evt.gap_evt;                    
    ble_gap_addr_t  const* peer_addr  = &p_gap_evt->params.adv_report.peer_addr;   //保存对等设备的地址
    
    
    //Prepare advertisement report for parsing.
    adv_data.p_data = (uint8_t *)p_gap_evt->params.adv_report.data;
    adv_data.size   = p_gap_evt->params.adv_report.dlen;

    //Search for advertising names.
    bool found_name = false;
    err_code = adv_report_parse(BLE_GAP_AD_TYPE_COMPLETE_LOCAL_NAME,
                                &adv_data,
                                &dev_name);

    
    if(err_code != NRF_SUCCESS)
    {
        // Look for the short local name if it was not found as complete.
        err_code = adv_report_parse(BLE_GAP_AD_TYPE_SHORT_LOCAL_NAME, &adv_data, &dev_name);
        if (err_code != NRF_SUCCESS)
        {
            // If we can't parse the data, then exit.
            return;
        }
        else
        {
            found_name = true;
        }
    }
    else
    {
        found_name = true;
    }
    
    if(found_name)
    {
        #if 0
        NRF_LOG_INFO("device name -------------start-------------------------");

        for(int i = 0; i< dev_name.size;i++)
        {
            NRF_LOG_INFO("name:%c",dev_name.p_data[i]);

        }
        NRF_LOG_INFO("device name -------------end-------------------------");
        #endif
        
        if(step == E_MATCH_NAME)
        {
            //比对名字匹配规则
            memset(device_name,0,sizeof(device_name));
            memcpy(device_name,dev_name.p_data,dev_name.size);
         
            uint8_t target_name_buff[20]={"SOMPUTON_08A"};
            
            #if 1
            if(strstr(device_name,target_name_buff) != NULL)   //找到目标设备名字
            {
                NRF_LOG_INFO("Name a match");   

               //peer_addr  = device_addr;
               //device_addr->addr[0] = peer_addr->addr[0];
                
				
				
                //memcpy(device_addr,peer_addr,sizeof(peer_addr));
                device_addr->addr[0]= 0x01;
                device_addr->addr[1]= 0x02;
                device_addr->addr[2]= 0x03;
                device_addr->addr[3]= 0x04;
                device_addr->addr[4]= 0x05;
                device_addr->addr[5]= 0x06;
                
                #if 0
                NRF_LOG_INFO("设备广播地址mac:%02x %02x %02x %02x %02x %02x ",device_addr->addr[0],\
                device_addr->addr[1],\
                device_addr->addr[2],\
                device_addr->addr[3],\
                device_addr->addr[4],\
                device_addr->addr[5]\
                );
                #endif
                
                
                #if 0
                if(ScanRsp_Match_Binding(p_gap_evt)== true)
                {
                    NRF_LOG_INFO("scan_rsp match");  
                
                     // Initiate connection.
                    err_code = sd_ble_gap_connect(peer_addr, &m_scan_params, &m_connection_param, 1);
                    if (err_code != NRF_SUCCESS)
                    {
                        NRF_LOG_ERROR("Connection Request Failed, reason %d", err_code);
                    }
                }
                #endif
            }
               
            #endif
            
            #if 0
            
            uint8_t sMacDevice[6]={0xF8,0X8B,0X29,0X51,0X60,0XF2}; //0XF2
            
            if( User_Match_Adv_Addr(p_gap_evt->params.adv_report.peer_addr,sMacDevice) == true)   //mac 地址过滤
            {
                //信号强度
                NRF_LOG_INFO("\r\n信号强度rssi:%02d",p_gap_evt->params.adv_report.rssi);
              
                //接收到的广播的类型

                NRF_LOG_INFO("\r\n广播类型:%02d", p_gap_evt->params.adv_report.type);
                //应答标志位  为1 是 扫描应答 0 为广播数据
                NRF_LOG_INFO("\r\n 应答标志位%d  \r\n",p_gap_evt->params.adv_report.scan_rsp); 
                
                NRF_LOG_INFO("\r\n  mac 地址匹配成功");
                //  扫描到的周围的mac 地址
                #if 1
                NRF_LOG_INFO("设备广播地址mac:%02x %02x %02x %02x %02x %02x ",p_gap_evt->params.adv_report.peer_addr.addr[0],\
                p_gap_evt->params.adv_report.peer_addr.addr[1],\
                p_gap_evt->params.adv_report.peer_addr.addr[2],\
                p_gap_evt->params.adv_report.peer_addr.addr[3],\
                p_gap_evt->params.adv_report.peer_addr.addr[4],\
                p_gap_evt->params.adv_report.peer_addr.addr[5]\
                );
                #endif
                //打印所有的广播数据包
                
                NRF_LOG_INFO("\r\n-----------------------start-------------------");
                
                for(uint8_t adv_len = 0;adv_len < p_gap_evt->params.adv_report.dlen; adv_len++)
                {
                    NRF_LOG_INFO("%02x",p_gap_evt->params.adv_report.data[adv_len]);
                }
                NRF_LOG_INFO("\r\n-----------------------end-------------------");
            }
            #endif     
        }  
    }
}








