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
			printf("Property field: '%s' $%i buildingLevel:%i ownerId:%i groupId:%i\n", field.name, field.value, field.buildingLevel, field.ownerId, field.groupId);
			break;
		case SUPERACTION:
			printf("Superaction field: '%s' '%s' imageId:%i\n", field.name, field.comment, field.imageId);
			break;
		case ACTION:
			printf("Action field: '%s' '%s' imageId:%i\n", field.name, field.comment, field.imageId);
			break;
	}
}

const static int valueMultiplier[4] = { 2, 4, 7, 10 };
const static int feeMultiplier[4] = { 1, 5, 8, 14 };
const static int upgradeMultiplier[4] = { 6, 9, 12, 15 };

int GetFieldValue(Field field)
{
	return field.value * valueMultiplier[field.buildingLevel] / 2;
}

int GetFeeValue(Field field)
{
	return field.value * feeMultiplier[field.buildingLevel] / 10;
}

int GetUpgradeValue(Field field)
{
	return field.value * upgradeMultiplier[field.buildingLevel] / 10;
}
