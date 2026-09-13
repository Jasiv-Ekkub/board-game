#include <field.h>
#include <string.h>
#include <stdio.h>

const char* buildingLevelNames[MAX_BUILDING_LEVEL] = {
	"Construction site",
	"Family house",
	"Villa",
	"Apartament"
};

Field GetPropertyField(const char* name, int value, int groupId, Color color)
{
	Field field = {
		.type = PROPERTY,
		.value = value,
		.color = color,
		.ownerId = -1,
		.groupId = groupId,
		.buildingLevel = 0,
	};
	strncpy(field.name, name, FIELD_TEXT_LENGTH);
	return field;
}

Field GetActionField(const char* name, const char* comment, FieldAction action, ImageId imageId)
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

Field GetSuperactionField(const char* name, const char* comment, FieldAction action, ImageId imageId)
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
	int level = field.buildingLevel;
	return field.value * (1 + level*level) / 2;
}

int GetFieldValue(Field field)
{
	int level = field.buildingLevel;
	return field.value * (2 + level*level) / 2;
}

int GetUpgradeValue(Field field)
{
	int level = field.buildingLevel;
	return field.value * (1 + level*level) / 4;
}
