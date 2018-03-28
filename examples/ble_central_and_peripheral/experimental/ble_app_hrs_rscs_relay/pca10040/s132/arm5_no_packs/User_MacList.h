#ifndef __maclist_h
#define __maclist_h

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "nordic_common.h"
#include "ble_db_discovery.h"

#define  MAX_LIST_LENGTH  8      //�б�������
#define  MAC_ADDR_LENGTH  6      //MAC ��ַ����

typedef enum{
	E_01G = 0X00,
	E_01H = 0X01,
	E_03F = 0X02,
	E_03H = 0X03,
	E_06H = 0X04,
	E_06F = 0X05,
	E_08A = 0X06,
	E_08F = 0X07,
	E_09A = 0X08,
	E_09F = 0X0A
	
}_e_machine_model;



//�����豸��Ϣ
typedef struct{
	uint8_t 	mac_addr[MAC_ADDR_LENGTH];        	//�豸mac ��ַ
	uint16_t 	conn_handle;       					//���Ӿ��
	uint8_t 	connected_flag;   					//����״̬
	int     	rssi;             	  				//rssi �ź�ǿ��
	_e_machine_model model;             			//�豸�ͺ�
}_t_ble_status;


typedef struct{
	_t_ble_status ble_dev[MAX_LIST_LENGTH];  //�豸������Ϣ
    uint8_t       empty_flag;                //�豸��������־
	
}_t_dev_mac_match;





bool dev_check_empty(void);
void Device_Info_Reset(_t_ble_status *s_ble_info);
void Debug_Device_match_info(uint16_t  connected_handle);
void Device_Disconnected_handle(uint16_t  connected_handle);
void Debug_Device_match_connected_mac(ble_gap_addr_t mac_peer_addr,uint16_t handle);



#endif

