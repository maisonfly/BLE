#include "User_MacList.h"

#define  MAX_LIST_LENGTH  8      //�б�������
#define  MAC_ADDR_LENGTH  6      //MAC ��ַ����




typedef struct{
	uint8_t 	mac_addr[MAC_ADDR_LENGTH];        	//�豸mac ��ַ
	uint8_t 	conn_handle;       					//���Ӿ��
	uint8_t 	connected_flag;   					//����״̬
	int     	rssi;             	  				//rssi �ź�ǿ��
	_e_machine_model model;             			//�豸�ͺ�
}_t_ble_status;


typedef struct{
	_t_ble_status ble_dev[MAX_LIST_LENGTH];
	

}_t_dev_mac_match;








