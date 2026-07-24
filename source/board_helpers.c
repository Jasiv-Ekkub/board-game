#include <board_helpers.h>

void LoadFieldsDefault(uint16_t size, Field* fields)
{
	if(size > 256) size = 256;
	else size &= 0xFFFC;
	
	for(uint16_t i=0; i<size; ++i)
	{
		fields[i] = (Field){
			.name = "Wasteland",
			.value = 0,
			.buildingLevel = 0,
			.ownerId = 0,
			.color = (Color){16,16,16,255},
		};
	}
}
