
#include "app_bt_cfg.h"
#include "sparcommon.h"
#include "wiced_bt_dev.h"
#include "wiced_bt_trace.h"
#include "wiced_bt_stack.h"
#include "wiced_platform.h"
#include "wiced_transport.h"
#include "wiced_rtos.h"
#include "cycfg.h"
#include "cycfg_gatt_db.h"
#include "wiced_timer.h"

/* Convenient defines for thread sleep times */
#define SLEEP_10MS		(10)
#define SLEEP_100MS		(100)
#define SLEEP_250MS		(250)
#define SLEEP_1000MS	(1000)

/* Globally define wiced_timer_t structure */
wiced_timer_t led_timer;

/*******************************************************************
 * Function Prototypes
 ******************************************************************/
wiced_bt_dev_status_t  app_bt_management_callback( wiced_bt_management_evt_t event, wiced_bt_management_evt_data_t *p_event_data );

/* Timer callback function */
void timer_cback( uint32_t data );


/*******************************************************************************
* Function Name: void application_start(void)
********************************************************************************
* Summary: Entry point to the application. Initialize transport configuration
*          and register BLE management event callback. The actual application
*          initialization will happen when stack reports that BT device is ready
********************************************************************************/
void application_start(void)
{
    /* Initialize Stack and Register Management Callback */
    wiced_bt_stack_init( app_bt_management_callback, &wiced_bt_cfg_settings, wiced_bt_cfg_buf_pools );
}


/**************************************************************************************************
* Function Name: wiced_bt_dev_status_t app_bt_management_callback(wiced_bt_management_evt_t event,
*                                                  wiced_bt_management_evt_data_t *p_event_data)
***********************************************************************************************    ***
* Summary:
*   This is a Bluetooth stack management event handler function to receive events from
*   BLE stack and process as per the application.
*
* Parameters:
*   wiced_bt_management_evt_t event             : BLE event code of one byte length
*   wiced_bt_management_evt_data_t *p_event_data: Pointer to BLE management event structures
*
* Return:
*  wiced_result_t: Error code from WICED_RESULT_LIST or BT_RESULT_LIST
*
***********************************************************************************************/
wiced_result_t app_bt_management_callback( wiced_bt_management_evt_t event, wiced_bt_management_evt_data_t *p_event_data )
{
    wiced_result_t status = WICED_BT_SUCCESS;

    switch( event )
    {
    case BTM_ENABLED_EVT:						// Bluetooth Controller and Host Stack Enabled

        if( WICED_BT_SUCCESS == p_event_data->enabled.status )
        {
        	/* Start a periodic timer */
        	wiced_init_timer( &led_timer, timer_cback, 0, WICED_MILLI_SECONDS_PERIODIC_TIMER );
        	wiced_start_timer( &led_timer, SLEEP_250MS );
        }
        break;

    default:
        break;
    }

    return status;
}


/*******************************************************************************
* Function Name: void timer_cback( uint32_t *data )
********************************************************************************/
void timer_cback( uint32_t data )
{
	wiced_hal_gpio_set_pin_output(LED1, !wiced_hal_gpio_get_pin_output(LED1) );
}
