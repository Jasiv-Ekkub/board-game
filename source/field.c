#include <field.h>
#include <string.h>
#include <stdio.h>

const char* buildingLevelNames[MAX_BUILDING_LEVEL] = {
	"Construction site",
	"Family house",
	"Villa",
	"Apartament"
};

Field GetPropertyField(const char* name, int value, Color color)
{
	Field field = {
		.type = PROPERTY,
		.value = value,
		.color = color,
		.ownerId = -1,
		.buildingLevel = 0,
	};
	strncpy(field.name, name, FIELD_TEXT_LENGTH);
	return field;
}

Field GetActionField(const char* name, const char* comment, FieldAction action, BoardImageId imageId)
{
	Field field = {
		.type = ACTION,
		.imageId = imageId,
		.action = action,
	};
	strncpy(field.name, name, FIELD_TEXT_LENGTH);
	strncpy(field.comment, comment, FIELD_TEXT_LENGTH);
	return field;
}

Field GetSuperactionField(const char* name, const char* comment, FieldAction action, BoardImageId imageId)
{
	Field field = {
		.type = SUPERACTION,
		.imageId = imageId,
		.action = action,
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
		case SUPERACTION:
			printf("Superaction field: '%s' '%s' imageId:%i\n", field.name, field.comment, field.imageId);
			break;
		case ACTION:
			printf("Action field: '%s' '%s' imageId:%i\n", field.name, field.comment, field.imageId);
			break;
	}
}

int GetFeeValue(Field field)
{
	return field.value * (1 + 2*field.buildingLevel) / 5;
}

int GetFieldValue(Field field)
{
	return field.value * (2 + field.buildingLevel) / 2;
}

int GetUpgradeValue(Field field)
{
	return field.value * (field.buildingLevel + 1) / 4;
}
