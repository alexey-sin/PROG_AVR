// UTFT Memory Saver
// -----------------
//
// Поскольку у большинства людей есть только один или, возможно, два разных модуля дисплея, много
// памяти было потрачено впустую для поддержки многих ненужных микросхем контроллеров.
// Теперь у вас есть возможность удалить этот ненужный код из библиотеки с помощью
// этот файл.
// Отключив ненужные контроллеры, вы уменьшите объем памяти.
// библиотеки на несколько Кб.

// Раскомментируйте строки для контроллеров дисплея, которые вы не используете для сохранения
// некоторая флэш-память, не включая код инициализации для этого конкретного
// контроллер.

//#define DISABLE_CPLD		 		1	// CTE50CPLD / CTE70CPLD / EHOUSE50CPLD

//#define DISABLE_HX8340B_8 		1	// ITDB22 8bit mode
//#define DISABLE_HX8352A			1	// ITDB32WD / CTE32W
//#define DISABLE_HX8353C           1	// DMTFT18101

//#define DISABLE_ILI9325D  		1	// ITDB24D / ITDB24DWOT / ITDB28 / DMTFT24104 / DMTFT28103	- This single define will disable both 8bit and 16bit mode for this controller
//#define DISABLE_ILI9325D_ALT 		1	// CTE28
//#define DISABLE_ILI9341_S4P		1	// MI0283QT9
//#define DISABLE_ILI9341_S5P		1	// DMTFT28105
//#define DISABLE_ILI9341_8			1	// ITDB24E_V2_8 / ITDB28_V2
//#define DISABLE_ILI9341_16		1	// CTE32_R2 / ITDB24E_V2_16 / ITDB32S_V2
//#define DISABLE_ILI9481			1	// CTE32HR
//#define DISABLE_ILI9486			1	// CTE40

//#define DISABLE_PCF8833			1	// LPH9135

//#define DISABLE_R61581			1	// CTE35IPS

//#define DISABLE_S6D0164			1	// CTE22 / DMTFT22102
//#define DISABLE_S6D1121			1	// ITDB24E	- This single define will disable both 8bit and 16bit mode for this controller
//#define DISABLE_SSD1289			1	// ITDB32S / ELEE32_REVA / ELEE32_REVB / CTE32	- This single define will disable both 8bit, 16bit and latched mode for this controller
//#define DISABLE_SSD1963_480		1	// ITDB43
//#define DISABLE_SSD1963_800		1	// ITDB50 / CTE50 / EHOUSE50
//#define DISABLE_SSD1963_800_ALT	1	// CTE70 / EHOUSE70
//#define DISABLE_ST7735			1	// ITDB18SP

// ----------------------------------------------------------------------------
// The following TFT controllers are used by display modules and shields that 
// have been retired by their vendors. They are still supported but support may
// be removed in future versions of the library. These controllers are disabled 
// by default to save flash memory. To use them you must enable support for 
// them by commenting them out here.

#define DISABLE_HX8340B_S 			1	// ITDB22 Serial mode
#define DISABLE_HX8347A				1	// ITDB32
#define DISABLE_ILI9325C			1	// ITDB24
#define DISABLE_ILI9327				1	// ITDB32WC
#define DISABLE_S1D19122  			1	// ITDB25H
