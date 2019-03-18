#ifndef _ICONS_H
#define _ICONS_H

#include <c_types.h>
#include <pgmspace.h>

#define NBR_ICONS    61
#define ICON_WIDTH    9
#define ICON_HEIGHT   9

typedef struct PROGMEM
{
	int            icon_number;
	const uint8_t *icon_day;
	const uint8_t *icon_night;  
}icon_list_t;

extern const icon_list_t icon_list[NBR_ICONS] PROGMEM;

#endif
