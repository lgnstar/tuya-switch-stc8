/**********************************Copyright (c)**********************************
**                       ��Ȩ���� (C), 2015-2020, Ϳѻ�Ƽ�
**
**                             http://www.tuya.com
**
*********************************************************************************/
/**
 * @file    protocol.c
 * @author  Ϳѻ�ۺ�Э�鿪����
 * @version v2.5.5
 * @date    2020.6.1
 * @brief
 *                       *******�ǳ���Ҫ��һ��Ҫ��Ŷ������********
 *          1. �û��ڴ��ļ���ʵ�������·�/�ϱ�����
 *          2. DP��ID/TYPE�����ݴ���������Ҫ�û�����ʵ�ʶ���ʵ��
 *          3. ����ʼĳЩ�궨�����Ҫ�û�ʵ�ִ���ĺ����ڲ���#err��ʾ,��ɺ�������ɾ����#err
 */

/******************************************************************************
                                ��ֲ��֪:
1:MCU������while��ֱ�ӵ���mcu_api.c�ڵ�wifi_uart_service()����
2:����������ʼ����ɺ�,���鲻���йش����ж�,�������ж�,���ж�ʱ������,���жϻ����𴮿����ݰ���ʧ
3:�������ж�/��ʱ���ж��ڵ����ϱ�����
******************************************************************************/

#include "wifi.h"

extern void usart1_SendByte(unsigned char dat);

#ifdef WEATHER_ENABLE
/**
 * @var    weather_choose
 * @brief  �������ݲ���ѡ������
 * @note   �û������Զ�����Ҫ�Ĳ�����ע�ͻ���ȡ��ע�ͼ��ɣ�ע�����
 */
const char *weather_choose[WEATHER_CHOOSE_CNT] = {
    "temp",
    "humidity",
    "condition",
    "pm25",
    /*"pressure",
    "realFeel",
    "uvi",
    "tips",
    "windDir",
    "windLevel",
    "windSpeed",
    "sunRise",
    "sunSet",
    "aqi",
    "so2 ",
    "rank",
    "pm10",
    "o3",
    "no2",
    "co",*/
};
#endif


/******************************************************************************
                              ��һ��:��ʼ��
1:����Ҫʹ�õ�wifi����ļ����ļ���include "wifi.h"
2:��MCU��ʼ���е���mcu_api.c�ļ��е�wifi_protocol_init()����
3:��MCU���ڵ��ֽڷ��ͺ�������protocol.c�ļ���uart_transmit_output������,��ɾ��#error
4:��MCU���ڽ��պ����е���mcu_api.c�ļ��ڵ�uart_receive_input����,�������յ����ֽ���Ϊ��������
5:��Ƭ������whileѭ�������mcu_api.c�ļ��ڵ�wifi_uart_service()����
******************************************************************************/

/******************************************************************************
                        1:dp���ݵ��������Ͷ��ձ�
          **��Ϊ�Զ����ɴ���,���ڿ���ƽ̨������޸�����������MCU_SDK**
******************************************************************************/
const DOWNLOAD_CMD_S download_cmd[] =
{
  {DPID_SWITCH_1, DP_TYPE_BOOL},
  {DPID_SWITCH_2, DP_TYPE_BOOL},
  {DPID_SWITCH_3, DP_TYPE_BOOL},
  {DPID_SWITCH_4, DP_TYPE_BOOL},
  {DPID_SWITCH_5, DP_TYPE_BOOL},
  {DPID_SWITCH_6, DP_TYPE_BOOL},
  {DPID_SWITCH_ALL, DP_TYPE_BOOL},
  {DPID_RELAY_STATUS, DP_TYPE_ENUM},
  {DPID_SWITCH_7, DP_TYPE_BOOL},
  {DPID_SWITCH_8, DP_TYPE_BOOL},
  {DPID_SWITCH_9, DP_TYPE_BOOL},
  {DPID_SWITCH_10, DP_TYPE_BOOL},
  {DPID_SWITCH_11, DP_TYPE_BOOL},
  {DPID_SWITCH_12, DP_TYPE_BOOL},
  {DPID_SWITCH_13, DP_TYPE_BOOL},
  {DPID_SWITCH_14, DP_TYPE_BOOL},
  {DPID_SWITCH_15, DP_TYPE_BOOL},
  {DPID_SWITCH_16, DP_TYPE_BOOL},
};



/******************************************************************************
                           2:���ڵ��ֽڷ��ͺ���
�뽫MCU���ڷ��ͺ�������ú�����,�������յ���������Ϊ�������봮�ڷ��ͺ���
******************************************************************************/

/**
 * @brief  ���ڷ�������
 * @param[in] {value} ����Ҫ���͵�1�ֽ�����
 * @return Null
 */
void uart_transmit_output(unsigned char value)
{
    //#error "�뽫MCU���ڷ��ͺ�������ú���,��ɾ������"

    //extern void usart1_SendByte(unsigned char dat);
    usart1_SendByte(value);

/*
    //Example:
    extern void Uart_PutChar(unsigned char value);
    Uart_PutChar(value);	                                //���ڷ��ͺ���
*/
}

/******************************************************************************
                           �ڶ���:ʵ�־����û�����
1:APP�·����ݴ���
2:�����ϱ�����
******************************************************************************/

/******************************************************************************
                            1:���������ϱ�����
��ǰ��������ȫ�������ϱ�(�������·�/���ϱ���ֻ�ϱ�)
  ��Ҫ�û�����ʵ�����ʵ��:
  1:��Ҫʵ�ֿ��·�/���ϱ����ݵ��ϱ�
  2:��Ҫʵ��ֻ�ϱ����ݵ��ϱ�
�˺���ΪMCU�ڲ��������
�û�Ҳ�ɵ��ô˺���ʵ��ȫ�������ϱ�
******************************************************************************/

//�Զ������������ϱ�����

/**
 * @brief  ϵͳ����dp����Ϣ�ϴ�,ʵ��APP��muc����ͬ��
 * @param  Null
 * @return Null
 * @note   �˺���SDK�ڲ�����ã�MCU����ʵ�ָú����������ϱ����ܣ�����ֻ�ϱ��Ϳ��ϱ����·�������
 */
void all_data_update(void)
{
    //#error "���ڴ˴�����·����ϱ����ݼ�ֻ�ϱ�����ʾ��,������ɺ�ɾ������"
    //�˴���Ϊƽ̨�Զ����ɣ��밴��ʵ�������޸�ÿ�����·����ϱ�������ֻ�ϱ�����
    mcu_dp_bool_update(DPID_SWITCH_1,0); //BOOL�������ϱ�;
    mcu_dp_bool_update(DPID_SWITCH_2,0); //BOOL�������ϱ�;
    mcu_dp_bool_update(DPID_SWITCH_3,0); //BOOL�������ϱ�;
    mcu_dp_bool_update(DPID_SWITCH_4,0); //BOOL�������ϱ�;
    mcu_dp_bool_update(DPID_SWITCH_5,0); //BOOL�������ϱ�;
    mcu_dp_bool_update(DPID_SWITCH_6,0); //BOOL�������ϱ�;
    mcu_dp_bool_update(DPID_SWITCH_ALL,0); //BOOL�������ϱ�;
    mcu_dp_enum_update(DPID_RELAY_STATUS,0); //ö���������ϱ�;
    mcu_dp_bool_update(DPID_SWITCH_7,0); //BOOL�������ϱ�;
    mcu_dp_bool_update(DPID_SWITCH_8,0); //BOOL�������ϱ�;
    mcu_dp_bool_update(DPID_SWITCH_9,0); //BOOL�������ϱ�;
    mcu_dp_bool_update(DPID_SWITCH_10,0); //BOOL�������ϱ�;
    mcu_dp_bool_update(DPID_SWITCH_11,0); //BOOL�������ϱ�;
    mcu_dp_bool_update(DPID_SWITCH_12,0); //BOOL�������ϱ�;
    mcu_dp_bool_update(DPID_SWITCH_13,0); //BOOL�������ϱ�;
    mcu_dp_bool_update(DPID_SWITCH_14,0); //BOOL�������ϱ�;
    mcu_dp_bool_update(DPID_SWITCH_15,0); //BOOL�������ϱ�;
    mcu_dp_bool_update(DPID_SWITCH_16,0); //BOOL�������ϱ�;
}


/******************************************************************************
                                WARNING!!!
                            2:���������ϱ�����
�Զ�������ģ�庯��,�������û�����ʵ�����ݴ���
******************************************************************************/
/*****************************************************************************
�������� : dp_download_switch_1_handle
�������� : ���DPID_SWITCH_1�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERROR
ʹ��˵�� : ���·����ϱ�����,��Ҫ�ڴ��������ݺ��ϱ���������app
*****************************************************************************/
static unsigned char dp_download_switch_1_handle(const unsigned char value[], unsigned short length)
{
    //ʾ��:��ǰDP����ΪBOOL
    unsigned char ret;
    //0:��/1:��
    unsigned char switch_1;

    switch_1 = mcu_get_dp_download_bool(value,length);
    if(switch_1 == 0) {
        //���ع�
    }else {
        //���ؿ�
    }

    //������DP���ݺ�Ӧ�з���
    ret = mcu_dp_bool_update(DPID_SWITCH_1,switch_1);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
�������� : dp_download_switch_2_handle
�������� : ���DPID_SWITCH_2�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERROR
ʹ��˵�� : ���·����ϱ�����,��Ҫ�ڴ��������ݺ��ϱ���������app
*****************************************************************************/
static unsigned char dp_download_switch_2_handle(const unsigned char value[], unsigned short length)
{
    //ʾ��:��ǰDP����ΪBOOL
    unsigned char ret;
    //0:��/1:��
    unsigned char switch_2;

    switch_2 = mcu_get_dp_download_bool(value,length);
    if(switch_2 == 0) {
        //���ع�
    }else {
        //���ؿ�
    }

    //������DP���ݺ�Ӧ�з���
    ret = mcu_dp_bool_update(DPID_SWITCH_2,switch_2);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
�������� : dp_download_switch_3_handle
�������� : ���DPID_SWITCH_3�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERROR
ʹ��˵�� : ���·����ϱ�����,��Ҫ�ڴ��������ݺ��ϱ���������app
*****************************************************************************/
static unsigned char dp_download_switch_3_handle(const unsigned char value[], unsigned short length)
{
    //ʾ��:��ǰDP����ΪBOOL
    unsigned char ret;
    //0:��/1:��
    unsigned char switch_3;

    switch_3 = mcu_get_dp_download_bool(value,length);
    if(switch_3 == 0) {
        //���ع�
    }else {
        //���ؿ�
    }

    //������DP���ݺ�Ӧ�з���
    ret = mcu_dp_bool_update(DPID_SWITCH_3,switch_3);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
�������� : dp_download_switch_4_handle
�������� : ���DPID_SWITCH_4�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERROR
ʹ��˵�� : ���·����ϱ�����,��Ҫ�ڴ��������ݺ��ϱ���������app
*****************************************************************************/
static unsigned char dp_download_switch_4_handle(const unsigned char value[], unsigned short length)
{
    //ʾ��:��ǰDP����ΪBOOL
    unsigned char ret;
    //0:��/1:��
    unsigned char switch_4;

    switch_4 = mcu_get_dp_download_bool(value,length);
    if(switch_4 == 0) {
        //���ع�
    }else {
        //���ؿ�
    }

    //������DP���ݺ�Ӧ�з���
    ret = mcu_dp_bool_update(DPID_SWITCH_4,switch_4);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
�������� : dp_download_switch_5_handle
�������� : ���DPID_SWITCH_5�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERROR
ʹ��˵�� : ���·����ϱ�����,��Ҫ�ڴ��������ݺ��ϱ���������app
*****************************************************************************/
static unsigned char dp_download_switch_5_handle(const unsigned char value[], unsigned short length)
{
    //ʾ��:��ǰDP����ΪBOOL
    unsigned char ret;
    //0:��/1:��
    unsigned char switch_5;

    switch_5 = mcu_get_dp_download_bool(value,length);
    if(switch_5 == 0) {
        //���ع�
    }else {
        //���ؿ�
    }

    //������DP���ݺ�Ӧ�з���
    ret = mcu_dp_bool_update(DPID_SWITCH_5,switch_5);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
�������� : dp_download_switch_6_handle
�������� : ���DPID_SWITCH_6�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERROR
ʹ��˵�� : ���·����ϱ�����,��Ҫ�ڴ��������ݺ��ϱ���������app
*****************************************************************************/
static unsigned char dp_download_switch_6_handle(const unsigned char value[], unsigned short length)
{
    //ʾ��:��ǰDP����ΪBOOL
    unsigned char ret;
    //0:��/1:��
    unsigned char switch_6;

    switch_6 = mcu_get_dp_download_bool(value,length);
    if(switch_6 == 0) {
        //���ع�
    }else {
        //���ؿ�
    }

    //������DP���ݺ�Ӧ�з���
    ret = mcu_dp_bool_update(DPID_SWITCH_6,switch_6);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
�������� : dp_download_switch_all_handle
�������� : ���DPID_SWITCH_ALL�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERROR
ʹ��˵�� : ���·����ϱ�����,��Ҫ�ڴ��������ݺ��ϱ���������app
*****************************************************************************/
static unsigned char dp_download_switch_all_handle(const unsigned char value[], unsigned short length)
{
    //ʾ��:��ǰDP����ΪBOOL
    unsigned char ret;
    //0:��/1:��
    unsigned char switch_all;

    switch_all = mcu_get_dp_download_bool(value,length);
    if(switch_all == 0) {
        //���ع�
    }else {
        //���ؿ�
    }

    //������DP���ݺ�Ӧ�з���
    ret = mcu_dp_bool_update(DPID_SWITCH_ALL,switch_all);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
�������� : dp_download_relay_status_handle
�������� : ���DPID_RELAY_STATUS�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERROR
ʹ��˵�� : ���·����ϱ�����,��Ҫ�ڴ��������ݺ��ϱ���������app
*****************************************************************************/
static unsigned char dp_download_relay_status_handle(const unsigned char value[], unsigned short length)
{
    //ʾ��:��ǰDP����ΪENUM
    unsigned char ret;
    unsigned char relay_status;

    relay_status = mcu_get_dp_download_enum(value,length);
    switch(relay_status) {
        case 0:
        break;

        case 1:
        break;

        case 2:
        break;

        default:

        break;
    }

    //������DP���ݺ�Ӧ�з���
    ret = mcu_dp_enum_update(DPID_RELAY_STATUS, relay_status);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
�������� : dp_download_switch_7_handle
�������� : ���DPID_SWITCH_7�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERROR
ʹ��˵�� : ���·����ϱ�����,��Ҫ�ڴ��������ݺ��ϱ���������app
*****************************************************************************/
static unsigned char dp_download_switch_7_handle(const unsigned char value[], unsigned short length)
{
    //ʾ��:��ǰDP����ΪBOOL
    unsigned char ret;
    //0:��/1:��
    unsigned char switch_7;

    switch_7 = mcu_get_dp_download_bool(value,length);
    if(switch_7 == 0) {
        //���ع�
    }else {
        //���ؿ�
    }

    //������DP���ݺ�Ӧ�з���
    ret = mcu_dp_bool_update(DPID_SWITCH_7,switch_7);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
�������� : dp_download_switch_8_handle
�������� : ���DPID_SWITCH_8�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERROR
ʹ��˵�� : ���·����ϱ�����,��Ҫ�ڴ��������ݺ��ϱ���������app
*****************************************************************************/
static unsigned char dp_download_switch_8_handle(const unsigned char value[], unsigned short length)
{
    //ʾ��:��ǰDP����ΪBOOL
    unsigned char ret;
    //0:��/1:��
    unsigned char switch_8;

    switch_8 = mcu_get_dp_download_bool(value,length);
    if(switch_8 == 0) {
        //���ع�
    }else {
        //���ؿ�
    }

    //������DP���ݺ�Ӧ�з���
    ret = mcu_dp_bool_update(DPID_SWITCH_8,switch_8);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
�������� : dp_download_switch_9_handle
�������� : ���DPID_SWITCH_9�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERROR
ʹ��˵�� : ���·����ϱ�����,��Ҫ�ڴ��������ݺ��ϱ���������app
*****************************************************************************/
static unsigned char dp_download_switch_9_handle(const unsigned char value[], unsigned short length)
{
    //ʾ��:��ǰDP����ΪBOOL
    unsigned char ret;
    //0:��/1:��
    unsigned char switch_9;

    switch_9 = mcu_get_dp_download_bool(value,length);
    if(switch_9 == 0) {
        //���ع�
    }else {
        //���ؿ�
    }

    //������DP���ݺ�Ӧ�з���
    ret = mcu_dp_bool_update(DPID_SWITCH_9,switch_9);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
�������� : dp_download_switch_10_handle
�������� : ���DPID_SWITCH_10�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERROR
ʹ��˵�� : ���·����ϱ�����,��Ҫ�ڴ��������ݺ��ϱ���������app
*****************************************************************************/
static unsigned char dp_download_switch_10_handle(const unsigned char value[], unsigned short length)
{
    //ʾ��:��ǰDP����ΪBOOL
    unsigned char ret;
    //0:��/1:��
    unsigned char switch_10;

    switch_10 = mcu_get_dp_download_bool(value,length);
    if(switch_10 == 0) {
        //���ع�
    }else {
        //���ؿ�
    }

    //������DP���ݺ�Ӧ�з���
    ret = mcu_dp_bool_update(DPID_SWITCH_10,switch_10);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
�������� : dp_download_switch_11_handle
�������� : ���DPID_SWITCH_11�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERROR
ʹ��˵�� : ���·����ϱ�����,��Ҫ�ڴ��������ݺ��ϱ���������app
*****************************************************************************/
static unsigned char dp_download_switch_11_handle(const unsigned char value[], unsigned short length)
{
    //ʾ��:��ǰDP����ΪBOOL
    unsigned char ret;
    //0:��/1:��
    unsigned char switch_11;

    switch_11 = mcu_get_dp_download_bool(value,length);
    if(switch_11 == 0) {
        //���ع�
    }else {
        //���ؿ�
    }

    //������DP���ݺ�Ӧ�з���
    ret = mcu_dp_bool_update(DPID_SWITCH_11,switch_11);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
�������� : dp_download_switch_12_handle
�������� : ���DPID_SWITCH_12�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERROR
ʹ��˵�� : ���·����ϱ�����,��Ҫ�ڴ��������ݺ��ϱ���������app
*****************************************************************************/
static unsigned char dp_download_switch_12_handle(const unsigned char value[], unsigned short length)
{
    //ʾ��:��ǰDP����ΪBOOL
    unsigned char ret;
    //0:��/1:��
    unsigned char switch_12;

    switch_12 = mcu_get_dp_download_bool(value,length);
    if(switch_12 == 0) {
        //���ع�
    }else {
        //���ؿ�
    }

    //������DP���ݺ�Ӧ�з���
    ret = mcu_dp_bool_update(DPID_SWITCH_12,switch_12);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
�������� : dp_download_switch_13_handle
�������� : ���DPID_SWITCH_13�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERROR
ʹ��˵�� : ���·����ϱ�����,��Ҫ�ڴ��������ݺ��ϱ���������app
*****************************************************************************/
static unsigned char dp_download_switch_13_handle(const unsigned char value[], unsigned short length)
{
    //ʾ��:��ǰDP����ΪBOOL
    unsigned char ret;
    //0:��/1:��
    unsigned char switch_13;

    switch_13 = mcu_get_dp_download_bool(value,length);
    if(switch_13 == 0) {
        //���ع�
    }else {
        //���ؿ�
    }

    //������DP���ݺ�Ӧ�з���
    ret = mcu_dp_bool_update(DPID_SWITCH_13,switch_13);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
�������� : dp_download_switch_14_handle
�������� : ���DPID_SWITCH_14�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERROR
ʹ��˵�� : ���·����ϱ�����,��Ҫ�ڴ��������ݺ��ϱ���������app
*****************************************************************************/
static unsigned char dp_download_switch_14_handle(const unsigned char value[], unsigned short length)
{
    //ʾ��:��ǰDP����ΪBOOL
    unsigned char ret;
    //0:��/1:��
    unsigned char switch_14;

    switch_14 = mcu_get_dp_download_bool(value,length);
    if(switch_14 == 0) {
        //���ع�
    }else {
        //���ؿ�
    }

    //������DP���ݺ�Ӧ�з���
    ret = mcu_dp_bool_update(DPID_SWITCH_14,switch_14);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
�������� : dp_download_switch_15_handle
�������� : ���DPID_SWITCH_15�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERROR
ʹ��˵�� : ���·����ϱ�����,��Ҫ�ڴ��������ݺ��ϱ���������app
*****************************************************************************/
static unsigned char dp_download_switch_15_handle(const unsigned char value[], unsigned short length)
{
    //ʾ��:��ǰDP����ΪBOOL
    unsigned char ret;
    //0:��/1:��
    unsigned char switch_15;

    switch_15 = mcu_get_dp_download_bool(value,length);
    if(switch_15 == 0) {
        //���ع�
    }else {
        //���ؿ�
    }

    //������DP���ݺ�Ӧ�з���
    ret = mcu_dp_bool_update(DPID_SWITCH_15,switch_15);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
�������� : dp_download_switch_16_handle
�������� : ���DPID_SWITCH_16�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERROR
ʹ��˵�� : ���·����ϱ�����,��Ҫ�ڴ��������ݺ��ϱ���������app
*****************************************************************************/
static unsigned char dp_download_switch_16_handle(const unsigned char value[], unsigned short length)
{
    //ʾ��:��ǰDP����ΪBOOL
    unsigned char ret;
    //0:��/1:��
    unsigned char switch_16;

    switch_16 = mcu_get_dp_download_bool(value,length);
    if(switch_16 == 0) {
        //���ع�
    }else {
        //���ؿ�
    }

    //������DP���ݺ�Ӧ�з���
    ret = mcu_dp_bool_update(DPID_SWITCH_16,switch_16);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}




/******************************************************************************
                                WARNING!!!
�˲��ֺ����û������޸�!!
******************************************************************************/

/**
 * @brief  dp�·�������
 * @param[in] {dpid} dpid ���
 * @param[in] {value} dp���ݻ�������ַ
 * @param[in] {length} dp���ݳ���
 * @return dp������
 * -           0(ERROR): ʧ��
 * -           1(SUCCESS): �ɹ�
 * @note   �ú����û������޸�
 */
unsigned char dp_download_handle(unsigned char dpid,const unsigned char value[], unsigned short length)
{
    /*********************************
    ��ǰ����������·�/���ϱ����ݵ���
    ���庯������Ҫʵ���·����ݴ���
    �������Ҫ��������������APP��,����APP����Ϊ�·�ʧ��
    ***********************************/
    unsigned char ret;
    switch(dpid) {
        case DPID_SWITCH_1:
            //���ش�����
            ret = dp_download_switch_1_handle(value,length);
        break;
        case DPID_SWITCH_2:
            //���� 2������
            ret = dp_download_switch_2_handle(value,length);
        break;
        case DPID_SWITCH_3:
            //���� 3������
            ret = dp_download_switch_3_handle(value,length);
        break;
        case DPID_SWITCH_4:
            //���� 4������
            ret = dp_download_switch_4_handle(value,length);
        break;
        case DPID_SWITCH_5:
            //���� 5������
            ret = dp_download_switch_5_handle(value,length);
        break;
        case DPID_SWITCH_6:
            //���� 6������
            ret = dp_download_switch_6_handle(value,length);
        break;
        case DPID_SWITCH_ALL:
            //�ܿ��ش�����
            ret = dp_download_switch_all_handle(value,length);
        break;
        case DPID_RELAY_STATUS:
            //�ϵ�״̬���ô�����
            ret = dp_download_relay_status_handle(value,length);
        break;
        case DPID_SWITCH_7:
            //����7������
            ret = dp_download_switch_7_handle(value,length);
        break;
        case DPID_SWITCH_8:
            //����8������
            ret = dp_download_switch_8_handle(value,length);
        break;
        case DPID_SWITCH_9:
            //����9������
            ret = dp_download_switch_9_handle(value,length);
        break;
        case DPID_SWITCH_10:
            //����10������
            ret = dp_download_switch_10_handle(value,length);
        break;
        case DPID_SWITCH_11:
            //����11������
            ret = dp_download_switch_11_handle(value,length);
        break;
        case DPID_SWITCH_12:
            //����12������
            ret = dp_download_switch_12_handle(value,length);
        break;
        case DPID_SWITCH_13:
            //����13������
            ret = dp_download_switch_13_handle(value,length);
        break;
        case DPID_SWITCH_14:
            //����14������
            ret = dp_download_switch_14_handle(value,length);
        break;
        case DPID_SWITCH_15:
            //����15������
            ret = dp_download_switch_15_handle(value,length);
        break;
        case DPID_SWITCH_16:
            //����16������
            ret = dp_download_switch_16_handle(value,length);
        break;


        default:
        break;
    }
    return ret;
}

/**
 * @brief  ��ȡ����dp�����ܺ�
 * @param[in] Null
 * @return �·������ܺ�
 * @note   �ú����û������޸�
 */
unsigned char get_download_cmd_total(void)
{
    return(sizeof(download_cmd) / sizeof(download_cmd[0]));
}


/******************************************************************************
                                WARNING!!!
�˴���ΪSDK�ڲ�����,�밴��ʵ��dp����ʵ�ֺ����ڲ�����
******************************************************************************/

#ifdef SUPPORT_MCU_FIRM_UPDATE
/**
 * @brief  ��������Сѡ��
 * @param[in] {package_sz} ��������С
 * @ref           0x00: 256byte (Ĭ��)
 * @ref           0x01: 512byte
 * @ref           0x02: 1024byte
 * @return Null
 * @note   MCU��Ҫ����ʵ�ָù���
 */
void upgrade_package_choose(unsigned char package_sz)
{
    #error "������ʵ��������ʵ����������Сѡ�����,��ɺ���ɾ������"
    unsigned short send_len = 0;
    send_len = set_wifi_uart_byte(send_len, package_sz);
    wifi_uart_write_frame(UPDATE_START_CMD, MCU_TX_VER, send_len);
}

/**
 * @brief  MCU����̼�����ģʽ
 * @param[in] {value} �̼�������
 * @param[in] {position} ��ǰ���ݰ����ڹ̼�λ��
 * @param[in] {length} ��ǰ�̼�������(�̼�������Ϊ0ʱ,��ʾ�̼����������)
 * @return Null
 * @note   MCU��Ҫ����ʵ�ָù���
 */
unsigned char mcu_firm_update_handle(const unsigned char value[],unsigned long position,unsigned short length)
{
    #error "���������MCU�̼���������,��ɺ���ɾ������"
    if(length == 0) {
        //�̼����ݷ������

    }else {
        //�̼����ݴ���

    }

    return SUCCESS;
}
#endif

#ifdef SUPPORT_GREEN_TIME
/**
 * @brief  ��ȡ���ĸ���ʱ��
 * @param[in] {time} ��ȡ���ĸ���ʱ������
 * @return Null
 * @note   MCU��Ҫ����ʵ�ָù���
 */
void mcu_get_greentime(unsigned char time[])
{
    #error "�����������ش���,��ɾ������"
    /*
    time[0] Ϊ�Ƿ��ȡʱ��ɹ���־��Ϊ 0 ��ʾʧ�ܣ�Ϊ 1��ʾ�ɹ�
    time[1] Ϊ��ݣ�0x00 ��ʾ 2000 ��
    time[2] Ϊ�·ݣ��� 1 ��ʼ��12 ����
    time[3] Ϊ���ڣ��� 1 ��ʼ��31 ����
    time[4] Ϊʱ�ӣ��� 0 ��ʼ��23 ����
    time[5] Ϊ���ӣ��� 0 ��ʼ��59 ����
    time[6] Ϊ���ӣ��� 0 ��ʼ��59 ����
    */
    if(time[0] == 1) {
        //��ȷ���յ�wifiģ�鷵�صĸ�������

    }else {
        //��ȡ����ʱ�����,�п����ǵ�ǰwifiģ��δ����
    }
}
#endif

#ifdef SUPPORT_MCU_RTC_CHECK
/**
 * @brief  MCUУ�Ա���RTCʱ��
 * @param[in] {time} ��ȡ���ĸ���ʱ������
 * @return Null
 * @note   MCU��Ҫ����ʵ�ָù���
 */
void mcu_write_rtctime(unsigned char time[])
{
    #error "���������RTCʱ��д�����,��ɾ������"
    /*
    Time[0] Ϊ�Ƿ��ȡʱ��ɹ���־��Ϊ 0 ��ʾʧ�ܣ�Ϊ 1��ʾ�ɹ�
    Time[1] Ϊ��ݣ�0x00 ��ʾ 2000 ��
    Time[2] Ϊ�·ݣ��� 1 ��ʼ��12 ����
    Time[3] Ϊ���ڣ��� 1 ��ʼ��31 ����
    Time[4] Ϊʱ�ӣ��� 0 ��ʼ��23 ����
    Time[5] Ϊ���ӣ��� 0 ��ʼ��59 ����
    Time[6] Ϊ���ӣ��� 0 ��ʼ��59 ����
    Time[7] Ϊ���ڣ��� 1 ��ʼ�� 7 ������1��������һ
   */
    if(time[0] == 1) {
        //��ȷ���յ�wifiģ�鷵�صı���ʱ������

    }else {
        //��ȡ����ʱ�����ݳ���,�п����ǵ�ǰwifiģ��δ����
    }
}
#endif

#ifdef WIFI_TEST_ENABLE
/**
 * @brief  wifi���ܲ��Է���
 * @param[in] {result} wifi���ܲ��Խ��
 * @ref       0: ʧ��
 * @ref       1: �ɹ�
 * @param[in] {rssi} ���Գɹ���ʾwifi�ź�ǿ��/����ʧ�ܱ�ʾ��������
 * @return Null
 * @note   MCU��Ҫ����ʵ�ָù���
 */
void wifi_test_result(unsigned char result,unsigned char rssi)
{
    //#error "������ʵ��wifi���ܲ��Գɹ�/ʧ�ܴ���,��ɺ���ɾ������"
    if(result == 0) {
        //����ʧ��
        if(rssi == 0x00) {
            //δɨ�赽����Ϊtuya_mdev_test·����,����
        }else if(rssi == 0x01) {
            //ģ��δ��Ȩ
        }
    }else {
        //���Գɹ�
        //rssiΪ�ź�ǿ��(0-100, 0�ź���100�ź���ǿ)
    }
}
#endif

#ifdef WEATHER_ENABLE
/**
* @brief  mcu����������
 * @param  Null
 * @return Null
 */
void mcu_open_weather(void)
{
    int i = 0;
    char buffer[13] = {0};
    unsigned char weather_len = 0;
    unsigned short send_len = 0;

    weather_len = sizeof(weather_choose) / sizeof(weather_choose[0]);

    for(i=0;i<weather_len;i++) {
        buffer[0] = sprintf(buffer+1,"w.%s",weather_choose[i]);
        send_len = set_wifi_uart_buffer(send_len, (unsigned char *)buffer, buffer[0]+1);
    }

    #error "�������ʾ���������ƴ�����������룬��ɺ���ɾ������"
    /*
    //����ȡ�Ĳ����к�ʱ���йصĲ���ʱ(��:�ճ�����)����Ҫ����t.unix����t.localʹ�ã���Ҫ��ȡ�Ĳ��������ǰ��ո���ʱ�仹�Ǳ���ʱ��
    buffer[0] = sprintf(buffer+1,"t.unix"); //����ʱ��   ��ʹ��  buffer[0] = sprintf(buffer+1,"t.local"); //����ʱ��
    send_len = set_wifi_uart_buffer(send_len, (unsigned char *)buffer, buffer[0]+1);
    */

    buffer[0] = sprintf(buffer+1,"w.date.%d",WEATHER_FORECAST_DAYS_NUM);
    send_len = set_wifi_uart_buffer(send_len, (unsigned char *)buffer, buffer[0]+1);

    wifi_uart_write_frame(WEATHER_OPEN_CMD, MCU_TX_VER, send_len);
}

/**
 * @brief  ���������ܷ����û��Դ�����
 * @param[in] {res} ���������ܷ��ؽ��
 * @ref       0: ʧ��
 * @ref       1: �ɹ�
 * @param[in] {err} ������
 * @return Null
 * @note   MCU��Ҫ����ʵ�ָù���
 */
void weather_open_return_handle(unsigned char res, unsigned char err)
{
    #error "��������ɴ��������ܷ������ݴ������,��ɺ���ɾ������"
    unsigned char err_num = 0;

    if(res == 1) {
        //���������سɹ�
    }else if(res == 0) {
        //����������ʧ��
        //��ȡ������
        err_num = err;
    }
}

/**
 * @brief  ���������û��Դ�����
 * @param[in] {name} ������
 * @param[in] {type} ��������
 * @ref       0: int ��
 * @ref       1: string ��
 * @param[in] {data} ����ֵ�ĵ�ַ
 * @param[in] {day} ��һ�������  0:��ʾ���� ȡֵ��Χ: 0~6
 * @ref       0: ����
 * @ref       1: ����
 * @return Null
 * @note   MCU��Ҫ����ʵ�ָù���
 */
void weather_data_user_handle(char *name, unsigned char type, const unsigned char *data, char day)
{
    #error "���������ʾ���������������������ݴ������,��ɺ���ɾ������"
    int value_int;
    char value_string[50];//�����еĲ������ݽ϶࣬����Ĭ��Ϊ50�������Ը��ݶ���Ĳ����������ʵ����ٸ�ֵ

    my_memset(value_string, '\0', 50);

    //���Ȼ�ȡ��������
    if(type == 0) { //������INT��
        value_int = data[0] << 24 | data[1] << 16 | data[2] << 8 | data[3];
    }else if(type == 1) {
        my_strcpy(value_string, data);
    }

    //ע��Ҫ������ѡ������������ò���ֵ������
    if(my_strcmp(name, "temp") == 0) {
        printf("day:%d temp value is:%d\r\n", day, value_int);          //int ��
    }else if(my_strcmp(name, "humidity") == 0) {
        printf("day:%d humidity value is:%d\r\n", day, value_int);      //int ��
    }else if(my_strcmp(name, "pm25") == 0) {
        printf("day:%d pm25 value is:%d\r\n", day, value_int);          //int ��
    }else if(my_strcmp(name, "condition") == 0) {
        printf("day:%d condition value is:%s\r\n", day, value_string);  //string ��
    }
}
#endif

#ifdef MCU_DP_UPLOAD_SYN
/**
 * @brief  ״̬ͬ���ϱ����
 * @param[in] {result} ���
 * @ref       0: ʧ��
 * @ref       1: �ɹ�
 * @return Null
 * @note   MCU��Ҫ����ʵ�ָù���
 */
void get_upload_syn_result(unsigned char result)
{
    #error "���������״̬ͬ���ϱ��������,��ɾ������"

    if(result == 0) {
        //ͬ���ϱ�����
    }else {
        //ͬ���ϱ��ɹ�
    }
}
#endif

#ifdef GET_WIFI_STATUS_ENABLE
/**
 * @brief  ��ȡ WIFI ״̬���
 * @param[in] {result} ָʾ WIFI ����״̬
 * @ref       0x00: wifi״̬ 1 smartconfig ����״̬
 * @ref       0x01: wifi״̬ 2 AP ����״̬
 * @ref       0x02: wifi״̬ 3 WIFI �����õ�δ����·����
 * @ref       0x03: wifi״̬ 4 WIFI ������������·����
 * @ref       0x04: wifi״̬ 5 ������·���������ӵ��ƶ�
 * @ref       0x05: wifi״̬ 6 WIFI �豸���ڵ͹���ģʽ
 * @ref       0x06: wifi״̬ 7 WIFI �豸����smartconfig&AP����״̬
 * @return Null
 * @note   MCU��Ҫ����ʵ�ָù���
 */
void get_wifi_status(unsigned char result)
{
  #error "��������ɻ�ȡ WIFI ״̬�������,��ɾ������"

    switch(result) {
        case 0:
            //wifi����״̬1
        break;

        case 1:
            //wifi����״̬2
        break;

        case 2:
            //wifi����״̬3
        break;

        case 3:
            //wifi����״̬4
        break;

        case 4:
            //wifi����״̬5
        break;

        case 5:
            //wifi����״̬6
        break;

        case 6:
            //wifi����״̬7
        break;

        default:break;
    }
}
#endif

#ifdef WIFI_STREAM_ENABLE
/**
 * @brief  �������ͽ��
 * @param[in] {result} ���
 * @ref       0x00: �ɹ�
 * @ref       0x01: ��������δ����
 * @ref       0x02: ��������δ���ӳɹ�
 * @ref       0x03: �������ͳ�ʱ
 * @ref       0x04: ��������ݳ��ȴ���
 * @return Null
 * @note   MCU��Ҫ����ʵ�ָù���
 */
void stream_trans_send_result(unsigned char result)
{
    #error "���������ʾ���������������������ͽ���������,��ɺ���ɾ������"
    switch(result) {
        case 0x00:
            //�ɹ�
        break;

        case 0x01:
            //��������δ����
        break;

        case 0x02:
            //��������δ���ӳɹ�
        break;

        case 0x03:
            //�������ͳ�ʱ
        break;

        case 0x04:
            //��������ݳ��ȴ���
        break;

        default:break;
    }
}

/**
 * @brief  ���ͼ�������ͽ��
 * @param[in] {result} ���
 * @ref       0x00: �ɹ�
 * @ref       0x01: ʧ��
 * @return Null
 * @note   MCU��Ҫ����ʵ�ָù���
 */
void maps_stream_trans_send_result(unsigned char result)
{
    #error "���������ʾ�������������ƶ��ͼ�������ͽ���������,��ɺ���ɾ������"
    switch(result) {
        case 0x00:
            //�ɹ�
        break;

        case 0x01:
            //ʧ��
        break;

        default:break;
    }
}
#endif

#ifdef WIFI_CONNECT_TEST_ENABLE
/**
 * @brief  ·����Ϣ���ս��֪ͨ
 * @param[in] {result} ģ���Ƿ�ɹ����յ���ȷ��·����Ϣ
 * @ref       0x00: ʧ��
 * @ref       0x01: �ɹ�
 * @return Null
 * @note   MCU��Ҫ����ʵ�ָù���
 */
void wifi_connect_test_result(unsigned char result)
{
    #error "������ʵ��wifi���ܲ��Գɹ�/ʧ�ܴ���,��ɺ���ɾ������"
    if(result == 0) {
        //·����Ϣ����ʧ�ܣ����鷢����·����Ϣ���Ƿ���������JSON���ݰ�
    }else {
        //·����Ϣ���ճɹ�����������ע��WIFI_STATE_CMDָ���wifi����״̬
    }
}
#endif

#ifdef GET_MODULE_MAC_ENABLE
/**
 * @brief  ��ȡģ��mac���
 * @param[in] {mac} ģ�� MAC ����
 * @ref       mac[0]: Ϊ�Ƿ��ȡmac�ɹ���־��0x00 ��ʾ�ɹ���0x01 ��ʾʧ��
 * @ref       mac[1]~mac[6]: ����ȡ MAC��ַ��־λ���mac[0]Ϊ�ɹ������ʾģ����Ч��MAC��ַ
 * @return Null
 * @note   MCU��Ҫ����ʵ�ָù���
 */
void mcu_get_mac(unsigned char mac[])
{
    #error "���������mac��ȡ����,��ɾ������"
    /*
    mac[0]Ϊ�Ƿ��ȡmac�ɹ���־��0x00 ��ʾ�ɹ���Ϊ0x01��ʾʧ��
    mac[1]~mac[6]:����ȡ MAC��ַ��־λ���mac[0]Ϊ�ɹ������ʾģ����Ч��MAC��ַ
   */

    if(mac[0] == 1) {
        //��ȡmac����
    }else {
        //��ȷ���յ�wifiģ�鷵�ص�mac��ַ
    }
}
#endif

#ifdef GET_IR_STATUS_ENABLE
/**
 * @brief  ��ȡ����״̬���
 * @param[in] {result} ָʾ����״̬
 * @ref       0x00: ����״̬ 1 ���ڷ��ͺ�����
 * @ref       0x01: ����״̬ 2 ���ͺ��������
 * @ref       0x02: ����״̬ 3 ����ѧϰ��ʼ
 * @ref       0x03: ����״̬ 4 ����ѧϰ����
 * @return Null
 * @note   MCU��Ҫ����ʵ�ָù���
 */
void get_ir_status(unsigned char result)
{
    #error "��������ɺ���״̬����,��ɾ������"
    switch(result) {
        case 0:
            //����״̬ 1
        break;

        case 1:
            //����״̬ 2
        break;

        case 2:
            //����״̬ 3
        break;

        case 3:
            //����״̬ 4
        break;

        default:break;
    }

    wifi_uart_write_frame(GET_IR_STATUS_CMD, MCU_TX_VER, 0);
}
#endif

#ifdef IR_TX_RX_TEST_ENABLE
/**
 * @brief  ��������շ�������֪ͨ
 * @param[in] {result} ģ���Ƿ�ɹ����յ���ȷ����Ϣ
 * @ref       0x00: ʧ��
 * @ref       0x01: �ɹ�
 * @return Null
 * @note   MCU��Ҫ����ʵ�ָù���
 */
void ir_tx_rx_test_result(unsigned char result)
{
    #error "������ʵ�ֺ�������շ����⹦�ܲ��Գɹ�/ʧ�ܴ���,��ɺ���ɾ������"
    if(result == 0) {
        //��������շ�����ɹ�
    }else {
        //��������շ�����ʧ�ܣ����鷢�������ݰ�
    }
}
#endif

#ifdef FILE_DOWNLOAD_ENABLE
/**
 * @brief  �ļ����ذ���Сѡ��
 * @param[in] {package_sz} �ļ����ذ���С
 * @ref       0x00: 256 byte (Ĭ��)
 * @ref       0x01: 512 byte
 * @ref       0x02: 1024 byte
 * @return Null
 * @note   MCU��Ҫ����ʵ�ָù���
 */
void file_download_package_choose(unsigned char package_sz)
{
    #error "������ʵ��������ʵ���ļ����ذ���Сѡ�����,��ɺ���ɾ������"
    unsigned short send_len = 0;
    send_len = set_wifi_uart_byte(send_len, package_sz);
    wifi_uart_write_frame(FILE_DOWNLOAD_START_CMD, MCU_TX_VER, send_len);
}

/**
 * @brief  �ļ�������ģʽ
 * @param[in] {value} ���ݻ�����
 * @param[in] {position} ��ǰ���ݰ������ļ�λ��
 * @param[in] {length} ��ǰ�ļ�������(����Ϊ0ʱ,��ʾ�ļ����������)
 * @return ���ݴ�����
 * -           0(ERROR): ʧ��
 * -           1(SUCCESS): �ɹ�
 * @note   MCU��Ҫ����ʵ�ָù���
 */
unsigned char file_download_handle(const unsigned char value[],unsigned long position,unsigned short length)
{
    #error "����������ļ������ش���,��ɺ���ɾ������"
    if(length == 0) {
        //�ļ������ݷ������

    }else {
        //�ļ������ݴ���

    }

    return SUCCESS;
}
#endif

#ifdef MODULE_EXPANDING_SERVICE_ENABLE
/**
 * @brief  ��ģ��ʱ�����֪ͨ���
 * @param[in] {value} ���ݻ�����
 * @param[in] {length} ���ݳ���
 * @return Null
 * @note   MCU��Ҫ����ʵ�ָù���
 */
void open_module_time_serve_result(const unsigned char value[], unsigned short length)
{
    #error "������ʵ��ģ��ʱ�����֪ͨ�������,��ɺ���ɾ������"
    unsigned char sub_cmd = value[0];

    switch(sub_cmd) {
        case 0x01: { //������  ��ģ��ʱ�����֪ͨ
            if(0x02 != length) {
                //���ݳ��ȴ���
                return;
            }

            if(value[1] == 0) {
                //�������ɹ�
            }else {
                //������ʧ��
            }
        }
        break;

        case 0x02: {  //������  ģ��ʱ�����֪ͨ
            if(0x09 != length) {
                //���ݳ��ȴ���
                return;
            }

            unsigned char time_type = value[1]; //0x00:����ʱ��  0x01:����ʱ��
            unsigned char time_data[7];

            my_memcpy(time_data, value + 2, length - 2);
            /*
            Data[0]Ϊ���, 0x00��ʾ2000��
            Data[1]Ϊ�·ݣ���1��ʼ��12����
            Data[2]Ϊ���ڣ���1��ʼ��31����
            Data[3]Ϊʱ�ӣ���0��ʼ��23����
            Data[4]Ϊ���ӣ���0��ʼ��59����
            Data[5]Ϊ���ӣ���0��ʼ��15����
            Data[6]Ϊ���ڣ���1��ʼ��7������1��������һ
            */

            //�ڴ˴����ʱ�����ݴ�����룬time_typeΪʱ������

            unsigned short send_len = 0;
            send_len = set_wifi_uart_byte(send_len,sub_cmd);
            wifi_uart_write_frame(MODULE_EXTEND_FUN_CMD, MCU_TX_VER, send_len);
        }
        break;

        case 0x03: {  //������  ��������������������
            if(0x02 != length) {
                //���ݳ��ȴ���
                return;
            }

            if(value[1] == 0) {
                //�ɹ�
            }else {
                //ʧ��
            }
        }
        break;

        case 0x04: {  //������  ��ģ������״̬֪ͨ
            if(0x02 != length) {
                //���ݳ��ȴ���
                return;
            }

            if(value[1] == 0) {
                //�ɹ�
            }else {
                //ʧ��
            }
        }
        break;

        case 0x05: {  //������  ģ������״̬֪ͨ
            if(0x02 != length) {
                //���ݳ��ȴ���
                return;
            }

            switch(value[1]) {
                case 0x00:
                    //ģ�鱾������

                break;
                case 0x01:
                    //APPԶ������

                break;
                case 0x02:
                    //APP�ָ���������

                break;
                default:break;
            }

            unsigned short send_len = 0;
            send_len = set_wifi_uart_byte(send_len, sub_cmd);
            wifi_uart_write_frame(MODULE_EXTEND_FUN_CMD, MCU_TX_VER, send_len);
        }
        break;

        default:break;
    }
}
#endif

#ifdef BLE_RELATED_FUNCTION_ENABLE
/**
 * @brief  ���������Բ��Խ��
 * @param[in] {value} ���ݻ�����
 * @param[in] {length} ���ݳ���
 * @return Null
 * @note   MCU��Ҫ����ʵ�ָù���
 */
void BLE_test_result(const unsigned char value[], unsigned short length)
{
    #error "������ʵ�����������Բ��Խ������,��ɺ���ɾ������"
    unsigned char sub_cmd = value[0];

    if(0x03 != length) {
        //���ݳ��ȴ���
        return;
    }

    if(0x01 != sub_cmd) {
        //���������
        return;
    }

    unsigned char result = value[1];
    unsigned char rssi = value[2];

    if(result == 0) {
        //����ʧ��
        if(rssi == 0x00) {
            //δɨ�赽����Ϊ ty_mdev�����ű�,����
        }else if(rssi == 0x01) {
            //ģ��δ��Ȩ
        }
    }else if(result == 0x01) {
        //���Գɹ�
        //rssiΪ�ź�ǿ��(0-100, 0�ź���100�ź���ǿ)
    }
}
#endif

#ifdef VOICE_MODULE_PROTOCOL_ENABLE
/**
 * @brief  ��ȡ����״̬����
 * @param[in] {result} ����״̬��
 * @ref       0x00: ����
 * @ref       0x01: mic����״̬
 * @ref       0x02: ����
 * @ref       0x03: ����¼��
 * @ref       0x04: ����ʶ��
 * @ref       0x05: ʶ��ɹ�
 * @ref       0x06: ʶ��ʧ��
 * @return Null
 * @note   MCU��Ҫ����ʵ�ָù���
 */
void get_voice_state_result(unsigned char result)
{
    #error "������ʵ�ֻ�ȡ����״̬�����������,��ɺ���ɾ������"
    switch(result) {
        case 0:
            //����
        break;

        case 1:
            //mic����״̬
        break;

        case 2:
            //����
        break;

        case 3:
            //����¼��
        break;

        case 4:
            //����ʶ��
        break;

        case 5:
            //ʶ��ɹ�
        break;

        case 6:
            //ʶ��ʧ��
        break;

      default:break;
    }
}

/**
 * @brief  MIC�������ý��
 * @param[in] {result} ����״̬��
 * @ref       0x00: mic ����
 * @ref       0x01: mic ����
 * @return Null
 * @note   MCU��Ҫ����ʵ�ָù���
 */
void set_voice_MIC_silence_result(unsigned char result)
{
    #error "������ʵ��MIC�������ô������,��ɺ���ɾ������"
    if(result == 0) {
        //mic ����
    }else {
        //mic ����
    }
}

/**
 * @brief  speaker�������ý��
 * @param[in] {result} ����ֵ
 * @ref       0~10: ������Χ
 * @return Null
 * @note   MCU��Ҫ����ʵ�ָù���
 */
void set_speaker_voice_result(unsigned char result)
{
    #error "������ʵ��speaker�������ý���������,��ɺ���ɾ������"

}

/**
 * @brief  ��Ƶ������
 * @param[in] {result} ��Ƶ����״̬
 * @ref       0x00: �ر���Ƶ����
 * @ref       0x01: mic1��Ƶ��·����
 * @ref       0x02: mic2��Ƶ��·����
 * @return Null
 * @note   MCU��Ҫ����ʵ�ָù���
 */
void voice_test_result(unsigned char result)
{
    #error "������ʵ����Ƶ�������������,��ɺ���ɾ������"
    if(result == 0x00) {
        //�ر���Ƶ����
    }else if(result == 0x01) {
        //mic1��Ƶ��·����
    }else if(result == 0x02) {
        //mic2��Ƶ��·����
    }
}

/**
 * @brief  ���Ѳ�����
 * @param[in] {result} ���ѷ���ֵ
 * @ref       0x00: ���ѳɹ�
 * @ref       0x01: ����ʧ��(10s��ʱʧ��)
 * @return Null
 * @note   MCU��Ҫ����ʵ�ָù���
 */
void voice_awaken_test_result(unsigned char result)
{
    #error "������ʵ�ֻ��Ѳ������������,��ɺ���ɾ������"
    if(result == 0x00) {
        //���ѳɹ�
    }else if(result == 0x01) {
        //����ʧ��
    }
}

/**
 * @brief  ����ģ����չ����
 * @param[in] {value} ���ݻ�����
 * @param[in] {length} ���ݳ���
 * @return Null
 * @note   MCU��Ҫ����ʵ�ָù���
 */
void voice_module_extend_fun(const unsigned char value[], unsigned short length)
{
    unsigned char sub_cmd = value[0];
    unsigned char play;
    unsigned char bt_play;
    unsigned short send_len = 0;

    switch(sub_cmd) {
        case 0x00: { //������  MCU��������
            if(0x02 != length) {
                //���ݳ��ȴ���
                return;
            }

            if(value[1] == 0) {
                //�ɹ�
            }else {
                //ʧ��
            }
        }
        break;

        case 0x01: {  //������  ״̬֪ͨ
            if(0x02 > length) {
                //���ݳ��ȴ���
                return;
            }

            unsigned char play = 0xff;
            unsigned char bt_play = 0xff;

            const char *str_buff = (const char *)&value[1];
            const char *str_result = NULL;

            str_result = strstr(str_buff,"play") + my_strlen("play") + 2;
            if(NULL == str_result) {
                //���ݴ���
                goto ERR_EXTI;
            }

            if(0 == memcmp(str_result, "true", my_strlen("true"))) {
                play = 1;
            }else if(0 == memcmp(str_result, "false", my_strlen("false"))) {
                play = 0;
            }else {
                //���ݴ���
                goto ERR_EXTI;
            }

            str_result = strstr(str_buff,"bt_play") + my_strlen("bt_play") + 2;
            if(NULL == str_result) {
                //���ݴ���
                goto ERR_EXTI;
            }

            if(0 == memcmp(str_result, "true", my_strlen("true"))) {
                bt_play = 1;
            }else if(0 == memcmp(str_result, "false", my_strlen("false"))) {
                bt_play = 0;
            }else {
                //���ݴ���
                goto ERR_EXTI;
            }

            #error "������ʵ������ģ��״̬֪ͨ�������,��ɺ���ɾ������"
            //MCU�����ݽ�֧�֡�����/��ͣ�� ���������ء�
            //play    ����/��ͣ����  1(����) / 0(��ͣ)
            //bt_play �������ع���   1(��)   / 0(��)



            send_len = 0;
            send_len = set_wifi_uart_byte(send_len, sub_cmd);
            send_len = set_wifi_uart_byte(send_len, 0x00);
            wifi_uart_write_frame(MODULE_EXTEND_FUN_CMD, MCU_TX_VER, send_len);
        }
        break;

        default:break;
    }

    return;

ERR_EXTI:
    send_len = 0;
    send_len = set_wifi_uart_byte(send_len, sub_cmd);
    send_len = set_wifi_uart_byte(send_len, 0x01);
    wifi_uart_write_frame(MODULE_EXTEND_FUN_CMD, MCU_TX_VER, send_len);
    return;
}
#endif




