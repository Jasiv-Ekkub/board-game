#include <field.h>
#include <string.h>
#include <stdio.h>

Field GetPropertyField(const char* name, int value, Color color)
{
	Field field = {
		.type = PROPERTY,
		.value = value,
		.color = color,
	};
	strncpy(field.name, name, FIELD_TEXT_LENGTH);
	return field;
}

Field GetActionField(const char* name, const char* comment, BoardImageId imageId)
{
	Field field = {
		.type = ACTION,
		.imageId = imageId,
	};
	strncpy(field.name, name, FIELD_TEXT_LENGTH);
	strncpy(field.comment, comment, FIELD_TEXT_LENGTH);
	return field;
}

void PrintField(Field field)
{
	switch(field.type)
	{
		case PROPERTY:
			printf("Property field: '%s' $%i buildingLevel:%i ownerId:%i\n", field.name, field.value, field.buildingLevel, field.ownerId);
			break;
		case ACTION:
			printf("Action field: '%s' '%s' imageId:%i\n", field.name, field.comment, field.imageId);
			break;
	}
}
