#ifndef INC_LCD_H_
#define INC_LCD_H_

#include <stdint.h>

#include "FreeRTOS.h"

#include "task.h"
#include "queue.h"

#define LCD_CLEAR_DISPLAY_INSTRUCTION		0x01

#define LCD_RETURN_HOME_INSTRUCTION			0x02

#define LCD_ENTRY_MODE_INSTRUCTION			0x04
#define LCD_ENTRY_MODE_INC_ADR				0x02
#define LCD_ENTRY_MODE_DEC_ADR				0x00
#define LCD_ENTRY_MODE_SHIFT_ON				0x01
#define LCD_ENTRY_MODE_SHIFT_OFF			0x00

#define LCD_CONTROL_INSTRUCTION				0x08
#define LCD_CONTROL_DISPLAY_ON				0x04
#define LCD_CONTROL_DISPLAY_OFF				0x00
#define LCD_CONTROL_CURSOR_ON				0x02
#define LCD_CONTROL_CURSOR_OFF				0x00
#define LCD_CONTROL_BLINK_ON				0x01
#define LCD_CONTROL_BLINK_OFF				0x00

#define LCD_FUNCTION_SET_INSTRUCTION		0x20
#define LCD_FUNCTION_SET_8_BIT_INTERFACE	0x10
#define LCD_FUNCTION_SET_4_BIT_INTERFACE	0x00
#define LCD_FUNCTION_SET_2_LINE				0x08
#define LCD_FUNCTION_SET_1_LINE				0x00
#define LCD_FUNCTION_SET_5x10_DOTS			0x04
#define LCD_FUNCTION_SET_5x7_DOTS			0x00

#define LCD_SHIFT_INSTRUCTION				0x10
#define LCD_SHIFT_CURSOR					0x00
#define LCD_SHIFT_DISPLAY					0x08
#define LCD_SHIFT_DIRECTION_RIGHT			0x04
#define LCD_SHIFT_DIRECTION_LEFT			0x00

#define LCD_SET_DD_RAM_ADDRESS_INSTRUCTION	0x80

#define LCD_BUSY_FLAG			0x80

#define LCD_ENABLE_BIT			0x40

typedef enum { INSTRUCTION = 0, DATA } LCD_CommandAddress;
typedef uint8_t LCD_CommandValue;

typedef struct {
	LCD_CommandAddress address;
	LCD_CommandValue value;
} LCD_Command;

extern TaskHandle_t lcdTaskHandle;
extern QueueHandle_t lcdQueueHandle;

extern void LCD_Init();
extern void LCD_EnqueueCommand(LCD_CommandAddress address, LCD_CommandValue value);
extern void LCD_EnqueueCommandFromISR(LCD_CommandAddress address, LCD_CommandValue value, BaseType_t* t);
extern void LCD_Task(void* parameter);
extern void LCD_Write(LCD_CommandAddress address, LCD_CommandValue value);

#endif /* INC_LCD_H_ */
