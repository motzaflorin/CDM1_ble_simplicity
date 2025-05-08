/*******************************************************************************
* @file  app.c
* @brief 
*******************************************************************************
* # License
* <b>Copyright 2023 Silicon Laboratories Inc. www.silabs.com</b>
*******************************************************************************
*
* The licensor of this software is Silicon Laboratories Inc. Your use of this
* software is governed by the terms of Silicon Labs Master Software License
* Agreement (MSLA) available at
* www.silabs.com/about-us/legal/master-software-license-agreement. This
* software is distributed to you in Source Code format and is governed by the
* sections of the MSLA applicable to Source Code.
*
******************************************************************************/
/*************************************************************************
 *
 */

/*================================================================================
 * @brief : This file contains example application for BLE Heart Rate Profile
 * @section Description :
 * This application demonstrates how to configure Heart rate as GATT server in
 * BLE peripheral mode and explains how to do indicate operation with GATT server
 * from connected remote device using GATT client.
 =================================================================================*/

/**
 * Include files
 * */
//! SL Wi-Fi SDK includes
#include "sl_board_configuration.h"
#include "sl_constants.h"
#include "sl_wifi.h"
#include "sl_wifi_callback_framework.h"
#include "cmsis_os2.h"
#include "sl_utility.h"
#include "FreeRTOSConfig.h"
//! BLE include file to refer BLE APIs
#include <string.h>
#include <stdio.h>

#include "ble_config.h"
#include "rsi_ble.h"
#include "rsi_ble_apis.h"
#include "rsi_ble_common_config.h"
#include "rsi_bt_common.h"
#include "rsi_bt_common_apis.h"
#include "rsi_common_apis.h"

#include <time.h>


//! BLE attribute service types uuid values
#define RSI_BLE_CHAR_SERV_UUID   0x2803
#define RSI_BLE_CLIENT_CHAR_UUID 0x2902

//! BLE characteristic service uuid
#define RSI_BLE_HEART_RATE_SERVICE_UUID       0x180D
#define RSI_BLE_HEART_RATE_MEASUREMENT_UUID   0x2A37
#define RSI_BLE_SENSOR_LOCATION_UUID          0x2A38
#define RSI_BLE_HEART_RATE_CONTROL_POINT_UUID 0x2A39

//! BLE characteristic service uuid - BATTERY SERVICE
//#define RSI_BLE_BATTERY_SERVICE_UUID          0x180F
//#define RSI_BLE_BATTERY_LEVEL_UUID            0x2A19
//#define RSI_BLE_BATTERY_CCCD_UUID             0x2902


//static const uint8_t battery_service_uuid[]     = { 0x0F, 0x18 };  // 0x180F
//static const uint8_t battery_level_uuid[]       = { 0x19, 0x2A };  // 0x2A19
//static const uint8_t cccd_uuid[]                = { 0x02, 0x29 };  // 0x2902

/******************************************************
 * *                     global battery char att value handle
 * ******************************************************/
// Handles
//static uint16_t battery_service_handle          = 0;
//static uint16_t battery_level_char_decl_handle  = 0;
//static uint16_t battery_char_val_att_handle      = 0;
//static bool notify_enabled = false;
//static uint16_t battery_level_cccd_handle       = 0;
//
//// State
//uint8_t battery_level_value              = 75;
//static bool notify_enabled                      = false;
//static uint8_t conn_handle                      = 0xFF;




//! max data length
#define RSI_BLE_MAX_DATA_LEN 20

//! local device name
#define RSI_BLE_HEART_RATE_PROFILE "BLE_CDM1"

//! Address type of the device to connect
#define RSI_BLE_DEV_ADDR_TYPE LE_PUBLIC_ADDRESS

//! Address of the device to connect
#define RSI_BLE_DEV_ADDR "00:23:A7:80:70:B9"

//! Remote Device Name to connect
#define RSI_REMOTE_DEVICE_NAME "SILABS"

//! attribute properties
#define RSI_BLE_ATT_PROPERTY_READ   0x02
#define RSI_BLE_ATT_PROPERTY_WRITE  0x08
#define RSI_BLE_ATT_PROPERTY_NOTIFY 0x10

//! application event list
#define RSI_APP_EVENT_ADV_REPORT              0x00
#define RSI_BLE_CONN_EVENT                    0x01
#define RSI_BLE_DISCONN_EVENT                 0x02
#define RSI_BLE_GATT_WRITE_EVENT              0x03
#define RSI_BLE_GATT_PROFILE_RESP_EVENT       0x04
#define RSI_BLE_GATT_CHAR_SERVICES_RESP_EVENT 0x05
#define RSI_BLE_GATT_CHAR_DESC_RESP_EVENT     0x06
#define RSI_BLE_GATT_PROFILES_RESP_EVENT      0x07
#define RSI_BLE_GATT_SEND_DATA                0x08
#define RSI_APP_EVENT_PHY_UPDATE_COMPLETE     0x09
#define RSI_BLE_COMMAND_SET_PHY               0x0A
#define RSI_BLE_GATT_SEND_DATA_custom               0x0B

#define BLE_2M_PHY 0x02
#define BLE_1M_PHY 0x01
#define IDLE       0x00
#define RUNNING    0x01

#define LOCAL_DEV_ADDR_LEN 18 // Length of the local device address

static uint8_t phy_value              = BLE_2M_PHY;
static uint8_t phy_update_in_progress = IDLE;
#define SERVER    0
#define CLIENT    1
#define GATT_ROLE SERVER

typedef struct heart_rate_s {
  int8_t flags;
  int8_t heart_rate_measure_8;
  int16_t heart_rate_measure_16;
  int8_t energy_expended_status;
  int8_t rr_interval;
} heart_rate_t;

uint8_t heartratefun(heart_rate_t, uint8_t *);




/******************************************************
 * *                     Heart rate  Macros
 * ******************************************************/

//VALUE_FORMATE_BIT    BIT(0)
#define RSI_BLE_HEART_RATE_VALUE_BYTE_FORMATE  0
#define RSI_BLE_HEART_RATE_VALUE_2BYTE_FORMATE 1

//SENSOR_CONTACT_BITS BIT(1), BIT(2)
#define RSI_BLE_HEART_RATE_SENSOR_CONTACT_NOT_SUPPORTED   0
#define RSI_SENSOR_CONTACT_SUPPORTED_CONNECT_NOT_DETECTED 2
#define RSI_SENSOR_CONTACT_SUPPORTED_CONNECT_DETECTED     3

//ENERGY_EXPENDED_FEILD_BIT    BIT(3)
#define RSI_ENERGY_EXPENDED_FEILD_NOT_PRESENT 0
#define RSI_ENERGY_EXPENDED_FEILD_PRESENT     1

//RR_INTEVAL_FEILD_BIT  BIT(4)
#define RSI_RR_INTERVAL_FEILD_NOT_PRESENT 0
#define RSI_RR_INTERVAL_FEILD_PRESENT     1

//sensor location
#define RSI_BLE_BODY_SENSOR_LOCATION_OTHER    BIT(0)
#define RSI_BLE_BODY_SENSOR_LOCATION_CHEST    BIT(1)
#define RSI_BLE_BODY_SENSOR_LOCATION_WRIST    BIT(2)
#define RSI_BLE_BODY_SENSOR_LOCATION_FINGER   BIT(3)
#define RSI_BLE_BODY_SENSOR_LOCATION_HAND     BIT(4)
#define RSI_BLE_BODY_SENSOR_LOCATION_EAR_LOBE BIT(5)
#define RSI_BLE_BODY_SENSOR_LOCATION_FOOT     BIT(6)

#define RSI_REM_DEV_ADDR_LEN 18


// Derfine a cusotm service and characteristic

static const uint8_t AFE_SERVICE_UUID[16] = { 0xAF, 0xEF, 0xAF, 0xEF, 0xAF, 0xEF, 0xAF, 0xEF,
                                              0xAF, 0xEF, 0xAF, 0xEF, 0xAF, 0xEF, 0xAF, 0xEF };

static const uint8_t ECG_CHAR_UUID[16] = { 0xEC, 0xEC, 0xEC, 0xEC, 0xEC, 0xEC, 0xEC, 0xEC,
                                           0xEC, 0xEC, 0xEC, 0xEC, 0xEC, 0xEC, 0xEC, 0xEC };

static const uint8_t ECG_READ_UUID[16] = { 0xEA, 0xFA, 0xEC, 0xEC, 0xEC, 0xEC, 0xEC, 0xEC,
                                           0xEC, 0xEC, 0xEC, 0xEC, 0xEC, 0xEC, 0xEC, 0xEC };

uint16_t ecg_char_val_handle = 0;

/*=======================================================================*/
//!    Powersave configurations
/*=======================================================================*/
#define ENABLE_POWER_SAVE 0 //! Set to 1 for powersave mode



//! global parameters list
static volatile uint32_t ble_app_event_map;
static volatile uint32_t ble_app_event_map1;
static volatile uint32_t ble_app_event_mask;
static uint8_t remote_addr_type = 0;
static uint8_t remote_name[31];
static uint8_t remote_dev_addr[18]   = { 0 };
static uint8_t remote_dev_bd_addr[6] = { 0 };
static uint8_t device_found          = 0;
static rsi_ble_event_conn_status_t conn_event_to_app;
static rsi_ble_event_disconnect_t disconn_event_to_app;
static rsi_ble_event_write_t app_ble_write_event;
static uint16_t rsi_ble_measurement_hndl;

static rsi_ble_event_read_by_type1_t char_servs;
rsi_ble_event_profile_by_uuid_t profiles_list;
rsi_ble_event_gatt_desc_t attr_desc_list;
int8_t notify_start            = false;
heart_rate_t rate              = { 0x00, 75, 73, 70, 0 };
uint8_t str_remote_address[18] = { '\0' };
osSemaphoreId_t ble_main_task_sem;

static const sl_wifi_device_configuration_t
  config = { .boot_option = LOAD_NWP_FW,
             .mac_address = NULL,
             .band        = SL_SI91X_WIFI_BAND_2_4GHZ,
             .region_code = US,
             .boot_config = {
               .oper_mode              = SL_SI91X_CLIENT_MODE,
               .coex_mode              = SL_SI91X_WLAN_BLE_MODE,
               .feature_bit_map        = (SL_SI91X_FEAT_WPS_DISABLE
                                   | (SL_SI91X_FEAT_ULP_GPIO_BASED_HANDSHAKE | SL_SI91X_FEAT_DEV_TO_HOST_ULP_GPIO_1)),
               .tcp_ip_feature_bit_map = (SL_SI91X_TCP_IP_FEAT_DHCPV4_CLIENT | SL_SI91X_TCP_IP_FEAT_EXTENSION_VALID),
               .custom_feature_bit_map = (SL_SI91X_CUSTOM_FEAT_EXTENTION_VALID),
               .ext_custom_feature_bit_map =
                 (SL_SI91X_EXT_FEAT_LOW_POWER_MODE | SL_SI91X_EXT_FEAT_XTAL_CLK | MEMORY_CONFIG
                  | SL_SI91X_EXT_FEAT_FRONT_END_SWITCH_PINS_ULP_GPIO_4_5_0 | (SL_SI91X_EXT_FEAT_BT_CUSTOM_FEAT_ENABLE)),
               .ext_tcp_ip_feature_bit_map = (SL_SI91X_CONFIG_FEAT_EXTENTION_VALID),
               .bt_feature_bit_map         = ((SL_SI91X_BT_RF_TYPE | SL_SI91X_ENABLE_BLE_PROTOCOL)),
               //!ENABLE_BLE_PROTOCOL in bt_feature_bit_map
               .ble_feature_bit_map =
                 ((SL_SI91X_BLE_MAX_NBR_PERIPHERALS(RSI_BLE_MAX_NBR_PERIPHERALS)
                   | SL_SI91X_BLE_MAX_NBR_CENTRALS(RSI_BLE_MAX_NBR_CENTRALS)
                   | SL_SI91X_BLE_MAX_NBR_ATT_SERV(RSI_BLE_MAX_NBR_ATT_SERV)
                   | SL_SI91X_BLE_MAX_NBR_ATT_REC(RSI_BLE_MAX_NBR_ATT_REC))
                  | SL_SI91X_FEAT_BLE_CUSTOM_FEAT_EXTENTION_VALID | SL_SI91X_BLE_PWR_INX(RSI_BLE_PWR_INX)
                  | SL_SI91X_BLE_PWR_SAVE_OPTIONS(RSI_BLE_PWR_SAVE_OPTIONS) | SL_SI91X_916_BLE_COMPATIBLE_FEAT_ENABLE

                  ),

               .ble_ext_feature_bit_map =
                 ((SL_SI91X_BLE_NUM_CONN_EVENTS(RSI_BLE_NUM_CONN_EVENTS)
                   | SL_SI91X_BLE_NUM_REC_BYTES(RSI_BLE_NUM_REC_BYTES))

                  ),
               .config_feature_bit_map = (SL_SI91X_FEAT_SLEEP_GPIO_SEL_BITMAP | SL_SI91X_ENABLE_ENHANCED_MAX_PSP) } };

const osThreadAttr_t thread_attributes = {
  .name       = "application_thread",
  .attr_bits  = 0,
  .cb_mem     = 0,
  .cb_size    = 0,
  .stack_mem  = 0,
  .stack_size = 3072,
  .priority   = osPriorityNormal,
  .tz_module  = 0,
  .reserved   = 0,
};

/*==============================================*/
/**
 * @fn         rsi_ble_add_char_serv_att
 * @brief      this function is used to add characteristic service attribute..
 * @param[in]  serv_handler, service handler.
 * @param[in]  handle, characteristic service attribute handle.
 * @param[in]  val_prop, characteristic value property.
 * @param[in]  att_val_handle, characteristic value handle
 * @param[in]  att_val_uuid, characteristic value uuid
 * @return     none.
 * @section description
 * This function is used at application to add characteristic attribute
 */
static void rsi_ble_add_char_serv_att(void *serv_handler,
                                      uint16_t handle,
                                      uint8_t val_prop,
                                      uint16_t att_val_handle,
                                      uuid_t att_val_uuid)
{
  rsi_ble_req_add_att_t new_att = { 0 };

  //! preparing the attribute service structure
  new_att.serv_handler       = serv_handler;
  new_att.handle             = handle;
  new_att.att_uuid.size      = 2;
  new_att.att_uuid.val.val16 = RSI_BLE_CHAR_SERV_UUID;
  new_att.property           = RSI_BLE_ATT_PROPERTY_READ;

  //! preparing the characteristic attribute value
  new_att.data_len = 6;
  new_att.data[0]  = val_prop;
  rsi_uint16_to_2bytes(&new_att.data[2], att_val_handle);
  rsi_uint16_to_2bytes(&new_att.data[4], att_val_uuid.val.val16);

  //! Add attribute to the service
  rsi_ble_add_attribute(&new_att);

  return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_add_char_val_att
 * @brief      this function is used to add characteristic value attribute.
 * @param[in]  serv_handler, new service handler.
 * @param[in]  handle, characteristic value attribute handle.
 * @param[in]  att_type_uuid, attribute uuid value.
 * @param[in]  val_prop, characteristic value property.
 * @param[in]  data, characteristic value data pointer.
 * @param[in]  data_len, characteristic value length.
 * @return     none.
 * @section description
 * This function is used at application to create new service.
 */

static void rsi_ble_add_char_val_att(void *serv_handler,
                                     uint16_t handle,
                                     uuid_t att_type_uuid,
                                     uint8_t val_prop,
                                     uint8_t *data,
                                     uint8_t data_len)
{
  rsi_ble_req_add_att_t new_att = { 0 };

  //! preparing the attributes
  new_att.serv_handler = serv_handler;
  new_att.handle       = handle;
  memcpy(&new_att.att_uuid, &att_type_uuid, sizeof(uuid_t));
  new_att.property = val_prop;

  //! preparing the attribute value
  new_att.data_len = RSI_MIN(sizeof(new_att.data), data_len);
  memcpy(new_att.data, data, new_att.data_len);

  //! add attribute to the service
  rsi_ble_add_attribute(&new_att);

  //! check the attribute property with notification
  if (val_prop & RSI_BLE_ATT_PROPERTY_NOTIFY) {
      printf("created CCCD because Notify\r\n");
    //! if notification property supports then we need to add client characteristic service.

    //! preparing the client characteristic attribute & values
    memset(&new_att, 0, sizeof(rsi_ble_req_add_att_t));
    new_att.serv_handler       = serv_handler;
    new_att.handle             = handle + 1;
    new_att.att_uuid.size      = 2;
    new_att.att_uuid.val.val16 = RSI_BLE_CLIENT_CHAR_UUID;
    new_att.property           = RSI_BLE_ATT_PROPERTY_READ | RSI_BLE_ATT_PROPERTY_WRITE;
    new_att.data_len           = 2;
    // added exclusively for testing
    new_att.data[0]            = 0x00;
    new_att.data[1]            = 0x00;

    //! add attribute to the service
    rsi_ble_add_attribute(&new_att);
  }

  return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_heart_rate_add_new_serv
 * @brief      this function is used to add new servcie i.e.., heart rate service.
 * @param[in]  none.
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used at application to create new service.
 */

static uint32_t rsi_ble_heart_rate_add_new_serv(void)
{
  uuid_t new_uuid                       = { 0 };
  rsi_ble_resp_add_serv_t new_serv_resp = { 0 };
  uint8_t sensor_data                   = RSI_BLE_BODY_SENSOR_LOCATION_WRIST;
  uint8_t control_data                  = 0;

  //! adding new service
  new_uuid.size      = 2;
  new_uuid.val.val16 = RSI_BLE_HEART_RATE_SERVICE_UUID;
  rsi_ble_add_service(new_uuid, &new_serv_resp);

  //! adding characteristic service attribute to the service
  new_uuid.size      = 2;
  new_uuid.val.val16 = RSI_BLE_HEART_RATE_MEASUREMENT_UUID;
  rsi_ble_add_char_serv_att(new_serv_resp.serv_handler,
                            new_serv_resp.start_handle + 1,
                            RSI_BLE_ATT_PROPERTY_NOTIFY,
                            new_serv_resp.start_handle + 2,
                            new_uuid);

  //! adding characteristic value attribute to the service
  rsi_ble_measurement_hndl = new_serv_resp.start_handle + 2;
  new_uuid.size            = 2;
  new_uuid.val.val16       = RSI_BLE_HEART_RATE_MEASUREMENT_UUID;
  rsi_ble_add_char_val_att(new_serv_resp.serv_handler,
                           new_serv_resp.start_handle + 2,
                           new_uuid,
                           RSI_BLE_ATT_PROPERTY_NOTIFY,
                           (uint8_t *)&rate,
                           sizeof(rate));

  //! adding characteristic service attribute to the service
  new_uuid.size      = 2;
  new_uuid.val.val16 = RSI_BLE_SENSOR_LOCATION_UUID;
  rsi_ble_add_char_serv_att(new_serv_resp.serv_handler,
                            new_serv_resp.start_handle + 4,
                            RSI_BLE_ATT_PROPERTY_READ,
                            new_serv_resp.start_handle + 5,
                            new_uuid);

  //! adding characteristic value attribute to the service
  new_uuid.size      = 2;
  new_uuid.val.val16 = RSI_BLE_SENSOR_LOCATION_UUID;
  rsi_ble_add_char_val_att(new_serv_resp.serv_handler,
                           new_serv_resp.start_handle + 5,
                           new_uuid,
                           RSI_BLE_ATT_PROPERTY_READ,
                           &sensor_data,
                           sizeof(sensor_data));

  //! adding characteristic service attribute to the service
  new_uuid.size      = 2;
  new_uuid.val.val16 = RSI_BLE_HEART_RATE_CONTROL_POINT_UUID;
  rsi_ble_add_char_serv_att(new_serv_resp.serv_handler,
                            new_serv_resp.start_handle + 6,
                            RSI_BLE_ATT_PROPERTY_WRITE,
                            new_serv_resp.start_handle + 7,
                            new_uuid);

  //! adding characteristic value attribute to the service
  new_uuid.size      = 2;
  new_uuid.val.val16 = RSI_BLE_HEART_RATE_CONTROL_POINT_UUID;
  rsi_ble_add_char_val_att(new_serv_resp.serv_handler,
                           new_serv_resp.start_handle + 7,
                           new_uuid,
                           RSI_BLE_ATT_PROPERTY_WRITE,
                           &control_data,
                           sizeof(control_data));

  return 0;
}


//// BATTERY SERVICE ADD
//static uint32_t rsi_ble_battery_add_new_serv(void)
//{
//
//  uuid_t new_uuid                       = { 0 };
//  rsi_ble_resp_add_serv_t new_serv_resp = { 0 };
//  uint8_t initial_battery_level                   = 100;
//
//
//  //! adding new service
//  new_uuid.size      = 2;
//  new_uuid.val.val16 = RSI_BLE_BATTERY_SERVICE_UUID;
//  rsi_ble_add_service(new_uuid, &new_serv_resp);
//
//
//  //! adding characteristic service attribute to the service
//  new_uuid.size      = 2;
//  new_uuid.val.val16 = RSI_BLE_BATTERY_LEVEL_UUID;
//  rsi_ble_add_char_serv_att(new_serv_resp.serv_handler,
//                            new_serv_resp.start_handle +5,
//                            RSI_BLE_ATT_PROPERTY_READ | RSI_BLE_ATT_PROPERTY_NOTIFY ,
//                            new_serv_resp.start_handle + 6,
//                            new_uuid);
//
//  //! adding characteristic value attribute to the service
//  new_uuid.size      = 2;
//  new_uuid.val.val16 = RSI_BLE_BATTERY_LEVEL_UUID;
//  battery_char_val_att_handle = new_serv_resp.start_handle + 6;
//  rsi_ble_add_char_val_att(new_serv_resp.serv_handler,
//                           battery_char_val_att_handle,
//                           new_uuid,
//                           RSI_BLE_ATT_PROPERTY_READ | RSI_BLE_ATT_PROPERTY_NOTIFY ,
//                           &initial_battery_level,
//                           sizeof(initial_battery_level));
//
//
//
//
//  printf("Battery Service Start Handle:        0x%04X\n", new_serv_resp.start_handle);
//  printf("Battery Level Decl Handle:           0x%04X\n", new_serv_resp.start_handle + 1);
//  printf("Battery Level Value Handle:          0x%04X\n", new_serv_resp.start_handle + 2);
//  printf("Expected CCCD Handle (value + 1):    0x%04X\n", new_serv_resp.start_handle + 3);
//
//  return 0;
//}


static uint32_t rsi_ble_afe_add_custom_service(void)
{
  uuid_t new_uuid = {0};
  rsi_ble_resp_add_serv_t new_serv_resp = {0};
  uint8_t ecg_initial_value[2] = {0x00, 0x00};
  uint8_t sensor_data = 55;
//  uint16_t handle_cursor = 0;
  int ret;

  // 1. Add custom AFE service
  new_uuid.size = 16;
  memcpy((uint8_t *)&new_uuid.val, AFE_SERVICE_UUID, 16);
  ret = rsi_ble_add_service(new_uuid, &new_serv_resp);
  printf("Add AFE service ret = 0x%04X\n", ret);


  // 2. Characteristic Declaration for ECG
  memcpy((uint8_t *)&new_uuid.val, ECG_CHAR_UUID, 16);
  rsi_ble_add_char_serv_att(new_serv_resp.serv_handler,
                            new_serv_resp.start_handle + 1,
                            RSI_BLE_ATT_PROPERTY_NOTIFY,
                            new_serv_resp.start_handle + 2,
                            new_uuid);

  // 3. Characteristic Value for ECG
  ecg_char_val_handle = new_serv_resp.start_handle + 2;
  rsi_ble_add_char_val_att(new_serv_resp.serv_handler,
                           new_serv_resp.start_handle + 2,
                           new_uuid,
                           RSI_BLE_ATT_PROPERTY_NOTIFY,
                           ecg_initial_value,
                           sizeof(ecg_initial_value));



  //! adding characteristic service attribute to the service
  new_uuid.size = 16;
  memcpy((uint8_t *)&new_uuid.val, ECG_READ_UUID, 16);
//  new_uuid.val.val16 = ECG_READ_UUID;
  rsi_ble_add_char_serv_att(new_serv_resp.serv_handler,
                            new_serv_resp.start_handle + 4,
                            RSI_BLE_ATT_PROPERTY_READ,
                            new_serv_resp.start_handle + 5,
                            new_uuid);

  //! adding characteristic value attribute to the service
  new_uuid.size = 16;
  memcpy((uint8_t *)&new_uuid.val, ECG_READ_UUID, 16);
//  new_uuid.val.val16 = ECG_READ_UUID;
  rsi_ble_add_char_val_att(new_serv_resp.serv_handler,
                           new_serv_resp.start_handle + 5,
                           new_uuid,
                           RSI_BLE_ATT_PROPERTY_READ,
                           &sensor_data,
                           sizeof(sensor_data));


  if (ret != RSI_SUCCESS) {
    printf("Failed to add CCCD. Error: 0x%X\n", ret);
    return ret;
  }

  printf("AFE Service created. Handles:\n");
  printf(" - ECG Declaration:  0x%04X\n", ecg_char_val_handle - 1);
  printf(" - ECG Value:        0x%04X\n", ecg_char_val_handle);
  printf(" - ECG CCCD (0x2902):0x%04X\n", ecg_char_val_handle + 1);

  return RSI_SUCCESS;
}


/*==============================================*/
/**
 * @fn         rsi_ble_app_init_events
 * @brief      initializes the event parameter.
 * @param[in]  none.
 * @return     none.
 * @section description
 * This function is used during BLE initialization.
 */
static void rsi_ble_app_init_events()
{
  ble_app_event_map  = 0;
  ble_app_event_mask = 0xFFFFFFFF;
  ble_app_event_mask = ble_app_event_mask; //To suppress warning while compiling

  return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_app_set_event
 * @brief      set the specific event.
 * @param[in]  event_num, specific event number.
 * @return     none.
 * @section description
 * This function is used to set/raise the specific event.
 */
void rsi_ble_app_set_event(uint32_t event_num)
{
  if (event_num < 32) {
    ble_app_event_map |= BIT(event_num);
  } else {
    ble_app_event_map1 |= BIT((event_num - 32));
  }
  osSemaphoreRelease(ble_main_task_sem);

  return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_app_clear_event
 * @brief      clears the specific event.
 * @param[in]  event_num, specific event number.
 * @return     none.
 * @section description
 * This function is used to clear the specific event.
 */
static void rsi_ble_app_clear_event(uint32_t event_num)
{
  if (event_num < 32) {
    ble_app_event_map &= ~BIT(event_num);
  } else {
    ble_app_event_map1 &= ~BIT((event_num - 32));
  }
  return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_app_get_event
 * @brief      returns the first set event based on priority
 * @param[in]  none.
 * @return     int32_t
 *             > 0  = event number
 *             -1   = not received any event
 * @section description
 * This function returns the highest priority event among all the set events
 */
static int32_t rsi_ble_app_get_event(void)
{
  uint32_t ix;

  for (ix = 0; ix < 64; ix++) {
    if (ix < 32) {
      if (ble_app_event_map & (1 << ix)) {
        return ix;
      }
    } else {
      if (ble_app_event_map1 & (1 << (ix - 32))) {
        return ix;
      }
    }
  }

  return (-1);
}
/**
 * @fn         rsi_ble_phy_update_complete_event
 * @brief      invoked when disconnection event is received
 * @param[in]  resp_disconnect, disconnected remote device information
 * @param[in]  reason, reason for disconnection.
 * @return     none.
 * @section description
 * This Callback function indicates disconnected device information and status
 */
void rsi_ble_phy_update_complete_event(rsi_ble_event_phy_update_t *rsi_ble_event_phy_update_complete)
{
  UNUSED_PARAMETER(rsi_ble_event_phy_update_complete);
  rsi_ble_app_set_event(RSI_APP_EVENT_PHY_UPDATE_COMPLETE);
}

/*==============================================*/
/**
 * @fn         rsi_ble_simple_central_on_adv_report_event
 * @brief      invoked when advertise report event is received
 * @param[in]  adv_report, pointer to the received advertising report
 * @return     none.
 * @section description
 * This callback function updates the scanned remote devices list
 */
void rsi_ble_simple_central_on_adv_report_event(rsi_ble_event_adv_report_t *adv_report)
{
  if (device_found == true) {
    return;
  }

  memset(remote_name, 0, sizeof(remote_name));
  BT_LE_ADPacketExtract(remote_name, adv_report->adv_data, adv_report->adv_data_len);

  remote_addr_type = adv_report->dev_addr_type;
  rsi_6byte_dev_address_to_ascii(remote_dev_addr, (uint8_t *)adv_report->dev_addr);
  memcpy(remote_dev_bd_addr, (uint8_t *)adv_report->dev_addr, sizeof(remote_dev_bd_addr));
  if ((device_found == false)
      && (((strcmp((const char *)remote_name, RSI_REMOTE_DEVICE_NAME)) == 0)
          || ((remote_addr_type == RSI_BLE_DEV_ADDR_TYPE)
              && ((strcmp((const char *)remote_dev_addr, RSI_BLE_DEV_ADDR) == 0))))) {
    device_found = true;
    rsi_ble_app_set_event(RSI_APP_EVENT_ADV_REPORT);
  }
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_enhance_conn_status_event
 * @brief      invoked when enhanced connection complete event is received
 * @param[out] resp_conn, connected remote device information
 * @return     none.
 * @section description
 * This callback function indicates the status of the connection
 */
void rsi_ble_on_enhance_conn_status_event(rsi_ble_event_enhance_conn_status_t *resp_enh_conn)
{
  conn_event_to_app.dev_addr_type = resp_enh_conn->dev_addr_type;
  memcpy(conn_event_to_app.dev_addr, resp_enh_conn->dev_addr, RSI_DEV_ADDR_LEN);
  conn_event_to_app.status = resp_enh_conn->status;
  rsi_ble_app_set_event(RSI_BLE_CONN_EVENT);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_connect_event
 * @brief      invoked when connection complete event is received
 * @param[out] resp_conn, connected remote device information
 * @return     none.
 * @section description
 * This callback function indicates the status of the connection
 */
static void rsi_ble_on_connect_event(rsi_ble_event_conn_status_t *resp_conn)
{
  memcpy(&conn_event_to_app, resp_conn, sizeof(rsi_ble_event_conn_status_t));
  rsi_ble_app_set_event(RSI_BLE_CONN_EVENT);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_disconnect_event
 * @brief      invoked when disconnection event is received
 * @param[in]  resp_disconnect, disconnected remote device information
 * @param[in]  reason, reason for disconnection.
 * @return     none.
 * @section description
 * This callback function indicates disconnected device information and status
 */
static void rsi_ble_on_disconnect_event(rsi_ble_event_disconnect_t *resp_disconnect, uint16_t reason)
{
  UNUSED_PARAMETER(reason); //This statement is added only to resolve compilation warning, value is unchanged
  memcpy(&disconn_event_to_app, resp_disconnect, sizeof(rsi_ble_event_disconnect_t));
  rsi_ble_app_set_event(RSI_BLE_DISCONN_EVENT);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_gatt_write_event
 * @brief      its invoked when write/notify/indication events are received.
 * @param[in]  event_id, it indicates write/notification event id.
 * @param[in]  rsi_ble_write, write event parameters.
 * @return     none.
 * @section description
 * This callback function is invoked when write/notify/indication events are received
 */
static void rsi_ble_on_gatt_write_event(uint16_t event_id, rsi_ble_event_write_t *rsi_ble_write)
{
  printf("write_event handle value is %d\r\n", *((uint16_t *)app_ble_write_event.handle));
  UNUSED_PARAMETER(event_id); //This statement is added only to resolve compilation warning, value is unchanged
  memcpy(&app_ble_write_event, rsi_ble_write, sizeof(rsi_ble_event_write_t));
  if ((rsi_ble_measurement_hndl + 1) == *((uint16_t *)app_ble_write_event.handle)) {
    if (app_ble_write_event.att_value[0] == 1) {
      notify_start = true;
      rsi_ble_app_set_event(RSI_BLE_GATT_SEND_DATA);
    } else if (app_ble_write_event.att_value[0] == 0) {
      notify_start = false;
      rsi_ble_app_clear_event(RSI_BLE_GATT_SEND_DATA);
    }
  }
  // our custom ecg service char handle
  if ((ecg_char_val_handle + 1) == *((uint16_t *)app_ble_write_event.handle)) {
     if (app_ble_write_event.att_value[0] == 1) {
       notify_start = true;
       rsi_ble_app_set_event(RSI_BLE_GATT_SEND_DATA_custom);
     } else if (app_ble_write_event.att_value[0] == 0) {
       notify_start = false;
       rsi_ble_app_clear_event(RSI_BLE_GATT_SEND_DATA_custom);
     }
   }
  rsi_ble_app_set_event(RSI_BLE_GATT_WRITE_EVENT);

  return;
}
///*==============================================*/
/**
 * @fn         ble_on_read_req_event
 * @brief      its invoked when read/ events are received.
 * @param[in]  event_id, it indicates read event id.
 * @param[in]
 * @return     none.
 * @section description
 * This callback function is invoked when write/notify/indication events are received
 */

static void ble_on_read_req_event(uint16_t event_id, rsi_ble_read_req_t *read_req)
{
  uint8_t sensor_read = 7;
  printf("Read req for handle %d and event id %d\n", read_req->handle, event_id);

  rsi_ble_set_local_att_value(read_req->handle, sizeof(sensor_read), &sensor_read);
}
/*==============================================*/
/**
 * @fn         rsi_ble_profiles_list
 * @brief      invoked when response is received for get list of services.
 * @param[out] p_ble_resp_profiles, profiles list details
 * @return     none
 * @section description
 */
static void rsi_ble_profiles_list_event(uint16_t event_status, rsi_ble_event_profiles_list_t *rsi_ble_event_profiles)
{
  UNUSED_PARAMETER(event_status); //This statement is added only to resolve compilation warning, value is unchanged
  UNUSED_PARAMETER(
    rsi_ble_event_profiles); //This statement is added only to resolve compilation warning, value is unchanged
  rsi_ble_app_set_event(RSI_BLE_GATT_PROFILES_RESP_EVENT);
  return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_profiles_event
 * @brief      invoked when the specific service details are received for
 *             supported specific services.
 * @param[out] rsi_ble_event_profile, specific profile details
 * @return     none
 * @section description
 * This is a callback function
 */
static void rsi_ble_profile_event(uint16_t event_status, rsi_ble_event_profile_by_uuid_t *rsi_ble_event_profile)
{
  UNUSED_PARAMETER(event_status); //This statement is added only to resolve compilation warning, value is unchanged
  memcpy(&profiles_list, rsi_ble_event_profile, sizeof(rsi_ble_event_profile_by_uuid_t));
  rsi_ble_app_set_event(RSI_BLE_GATT_PROFILE_RESP_EVENT);
  return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_char_services_event
 * @brief      invoked when response is received for characteristic services details
 * @param[out] p_ble_resp_char_services, list of characteristic services.
 * @return     none
 * @section description
 */
static void rsi_ble_on_char_services_event(uint16_t event_status,
                                           rsi_ble_event_read_by_type1_t *rsi_ble_event_read_type1)
{
  UNUSED_PARAMETER(event_status); //This statement is added only to resolve compilation warning, value is unchanged
  memcpy(&char_servs, rsi_ble_event_read_type1, sizeof(rsi_ble_event_read_by_type1_t));
  rsi_ble_app_set_event(RSI_BLE_GATT_CHAR_SERVICES_RESP_EVENT);
  return;
}

static void ble_on_att_desc_event(uint16_t event_status, rsi_ble_event_gatt_desc_t *rsi_ble_gatt_desc_val)
{
  UNUSED_PARAMETER(event_status); //This statement is added only to resolve compilation warning, value is unchanged
  memcpy(&attr_desc_list, rsi_ble_gatt_desc_val, sizeof(rsi_ble_event_gatt_desc_t));
  rsi_ble_app_set_event(RSI_BLE_GATT_CHAR_DESC_RESP_EVENT);
  return;
}
/*=============================================*/
/**
 * @fn         heartratefun 
 * @brief      Creates the  heart rate megurement data.
 * @param[in]  rate, it indicates heart rate mesurement strcture.
 * @param[out] p_data, heart rate mesurement strcture in buffer pointer.
 * @return     length of heart rate data buffer.
 * @section description
 */

uint8_t heartratefun(heart_rate_t rate, uint8_t *p_data)
{
  int len = 0;

  p_data[len++] = rate.flags;

  if (!(rate.flags & BIT(0))) {
    if (rate.heart_rate_measure_8 < 85) {
        rate.heart_rate_measure_8 = (rand() % 100); // 0–99%
//      rate.heart_rate_measure_8 = rate.heart_rate_measure_8 + 2;
    } else {
      rate.heart_rate_measure_8 = 75;
    }
    p_data[len++] = rate.heart_rate_measure_8;
  } else {
    *((uint16_t *)(p_data + len)) = rate.heart_rate_measure_16;
    len += 2;
  }

  if (rate.flags & BIT(3)) {
    *((uint16_t *)(p_data + len)) = rate.energy_expended_status;
    len += 2;
  }

  if (rate.flags & BIT(4)) {
    *((uint16_t *)(p_data + len)) = rate.rr_interval;
    len += 2;
  }

  return len;
}

uint8_t custom_service_data_function(uint8_t *p_data)
{

  *p_data =  (rand() % 100); // 0–99%
  return 0;
}

#ifdef SLI_SI91X_MCU_INTERFACE
#if (defined SL_SI91X_MCU_ALARM_BASED_WAKEUP && (GATT_ROLE == CLIENT))
/**
 * @fn         check_wakeup_source
 * @brief      interface to check wakeup source and perform task accrodingly.
 * @param[in]  none
 * @return     none.
 * @section description
 * This function is used to initial phyupdate.
 */
void check_wakeup_source()
{
  if (phy_update_in_progress == IDLE) {
    if (get_wakeUpSrc() & NPSS_TO_MCU_ALARM_INTR) {
      set_wakeUpSrc(~NPSS_TO_MCU_ALARM_INTR);
      DEBUGOUT("ALARM_IRQ\r\n");
      phy_update_in_progress = RUNNING;
      rsi_ble_app_set_event(RSI_BLE_COMMAND_SET_PHY);
    }
  }
}
#endif
#endif

/*==============================================*/
/**
 * @fn         ble_heart_rate_gatt_server
 * @brief      this is the application of heart rate profile.
 * @param[in]  event_id, it indicates write/notification event id.
 * @param[in]  rsi_ble_write, write event parameters.
 * @return     none.
 * @section description
 * This function is used to test the simple chat application.
 */
void ble_heart_rate_gatt_server(void *argument)
{
  UNUSED_PARAMETER(argument);

  int32_t status = 0;
  int32_t event_id;
  int8_t data[8] = { 0 };
  uint8_t len;
  uint8_t connected                                          = false;
  sl_wifi_firmware_version_t version                         = { 0 };
  static uint8_t rsi_app_resp_get_dev_addr[RSI_DEV_ADDR_LEN] = { 0 };
  uint8_t local_dev_addr[LOCAL_DEV_ADDR_LEN]                 = { 0 };

  uint8_t adv[31] = { 2, 1, 6 };


  status = sl_wifi_init(&config, NULL, sl_wifi_default_event_handler);
  if (status != SL_STATUS_OK) {
    LOG_PRINT("\r\nWi-Fi Initialization Failed, Error Code : 0x%lX\r\n", status);
    return;
  } else {
    printf("\r\n Wi-Fi Initialization Successful\n");
  }

  //! Firmware version Prints
  status = sl_wifi_get_firmware_version(&version);
  if (status != SL_STATUS_OK) {
    LOG_PRINT("\r\nFirmware version Failed, Error Code : 0x%lX\r\n", status);
  } else {
    print_firmware_version(&version);
  }

  //! get the local device MAC address.
  status = rsi_bt_get_local_device_address(rsi_app_resp_get_dev_addr);
  if (status != RSI_SUCCESS) {
    LOG_PRINT("\r\n Get local device address failed = %lx\r\n", status);
    return;
  } else {
    rsi_6byte_dev_address_to_ascii(local_dev_addr, rsi_app_resp_get_dev_addr);
    LOG_PRINT("\r\n Local device address %s \r\n", local_dev_addr);
  }

  //! adding simple BLE chat service
  rsi_ble_heart_rate_add_new_serv();

//   ADD BATTERY SERVICE
//  rsi_ble_battery_add_new_serv();

  // ADD CUSTOM srevice
  rsi_ble_afe_add_custom_service();
  //! registering the GAP callback functions
  rsi_ble_gap_register_callbacks(rsi_ble_simple_central_on_adv_report_event,
                                 rsi_ble_on_connect_event,
                                 rsi_ble_on_disconnect_event,
                                 NULL,
                                 rsi_ble_phy_update_complete_event,
                                 NULL,
                                 rsi_ble_on_enhance_conn_status_event,
                                 NULL,
                                 NULL,
                                 NULL);
  //! registering the GATT call back functions
  rsi_ble_gatt_register_callbacks(NULL,
                                  NULL,
                                  NULL,
                                  NULL,
                                  NULL,
                                  NULL,
                                  NULL,
                                  rsi_ble_on_gatt_write_event,
                                  NULL,
                                  NULL,
                                  ble_on_read_req_event, //read value of a characteristic
                                  NULL,
                                  NULL,
                                  ble_on_att_desc_event,
                                  rsi_ble_profiles_list_event,
                                  rsi_ble_profile_event,
                                  rsi_ble_on_char_services_event,
                                  NULL,
                                  NULL,
                                  NULL,
                                  NULL,
                                  NULL,
                                  NULL);

  //! create ble main task if ble protocol is selected
  ble_main_task_sem = osSemaphoreNew(1, 0, NULL);
  if (ble_main_task_sem == NULL) {
    LOG_PRINT("Failed to create ble_main_task_sem semaphore\r\n");
    return;
  }

  //!  initializing the application events map
  rsi_ble_app_init_events();

  //! Set local name
  rsi_bt_set_local_name((uint8_t *)RSI_BLE_HEART_RATE_PROFILE);

#if (GATT_ROLE == SERVER)
  //! prepare advertise data //local/device name
  adv[3] = strlen(RSI_BLE_HEART_RATE_PROFILE) + 1;
  adv[4] = 9;
  strcpy((char *)&adv[5], RSI_BLE_HEART_RATE_PROFILE);

  //! set advertise data
  rsi_ble_set_advertise_data(adv, strlen(RSI_BLE_HEART_RATE_PROFILE) + 5);
  LOG_PRINT("\r\n Starting advertising\r\n");
  //! set device in advertising mode.
  rsi_ble_start_advertising();
#endif





  //! waiting for events from controller.
  while (1) {
    //! checking for events list
    event_id = rsi_ble_app_get_event();
    if (event_id == -1) {


      osSemaphoreAcquire(ble_main_task_sem, osWaitForever);

      continue;
    }

    switch (event_id) {

      case RSI_BLE_CONN_EVENT: {
        //! event invokes when connection was completed

        //! clear the served event
        rsi_ble_app_clear_event(RSI_BLE_CONN_EVENT);
        rsi_6byte_dev_address_to_ascii(str_remote_address, conn_event_to_app.dev_addr);
        LOG_PRINT("\r\n Module connected to address : %s \r\n", str_remote_address);

        connected = true;

#if (GATT_ROLE == CLIENT)
        rsi_ble_app_set_event(RSI_BLE_GATT_PROFILES_RESP_EVENT);
#endif
      } break;

      case RSI_BLE_DISCONN_EVENT: {
        //! event invokes when disconnection was completed

        //! clear the served event
        notify_start = false;
        rsi_ble_app_clear_event(RSI_BLE_GATT_SEND_DATA);
        rsi_ble_app_clear_event(RSI_BLE_DISCONN_EVENT);
        LOG_PRINT("\r\nModule got Disconnected\r\n");


        connected = false;

        //! set device in advertising mode.
#if (GATT_ROLE == SERVER)
adv:
        status = rsi_ble_start_advertising();
        LOG_PRINT("\r\n module advertising \n");
        if (status != RSI_SUCCESS) {
          goto adv;
        }
#endif



      } break;

      case RSI_BLE_GATT_WRITE_EVENT: {
        //! event invokes when write/notification events received
        //! clear the served event
         printf("RSI_BLE_GATT_WRITE_EVENT happend\r\n");

        rsi_ble_app_clear_event(RSI_BLE_GATT_WRITE_EVENT);

      } break;

      case RSI_BLE_GATT_SEND_DATA: {
        if (connected == true) {
          if (notify_start == true) {
            len    = heartratefun(rate, (uint8_t *)data);
            status = rsi_ble_set_local_att_value(rsi_ble_measurement_hndl, len, (uint8_t *)data);
            if (status != RSI_SUCCESS) {
              LOG_PRINT("\n Set Local att value cmd failed = 0x%lX \n", status);
            }
          }
        }
      } break;
      case RSI_BLE_GATT_SEND_DATA_custom: {

              printf("reached case custom\r\n");
              if (connected == true) {
                if (notify_start == true) {
                  custom_service_data_function((uint8_t *)data);
                  status = rsi_ble_set_local_att_value(ecg_char_val_handle, sizeof(uint8_t), (uint8_t *)data);
                  if (status != RSI_SUCCESS) {
                    LOG_PRINT("\n Set Local att value cmd failed = 0x%lX \n", status);
                  }
                }
              }

      } break;
      case RSI_BLE_COMMAND_SET_PHY: {
        LOG_PRINT("\r\n set PHY");
        rsi_ble_app_clear_event(RSI_BLE_COMMAND_SET_PHY);
        if (connected == true) {
          if (BLE_2M_PHY == phy_value) {
            status    = rsi_ble_setphy((int8_t *)conn_event_to_app.dev_addr, BLE_2M_PHY, BLE_2M_PHY, CODDED_PHY_RATE);
            phy_value = BLE_1M_PHY;
          } else {
            status    = rsi_ble_setphy((int8_t *)conn_event_to_app.dev_addr, BLE_1M_PHY, BLE_1M_PHY, CODDED_PHY_RATE);
            phy_value = BLE_2M_PHY;
          }
          if (status != RSI_SUCCESS) {
            LOG_PRINT("\r\n Failed to cancel the connection request: 0x%lx \r\n - conn", status);
          }
        }
      } break;
      case RSI_APP_EVENT_PHY_UPDATE_COMPLETE: {
        //! phy update complete event
        LOG_PRINT("\r\n phy update completed ");
        phy_update_in_progress = IDLE;
        //! clear the phy updare complete event.
        rsi_ble_app_clear_event(RSI_APP_EVENT_PHY_UPDATE_COMPLETE);
      } break;
      default: {
        break;
      }
    }
  }
}

void app_init(void)
{
  osThreadNew((osThreadFunc_t)ble_heart_rate_gatt_server, NULL, &thread_attributes);
}
