#ifndef FIELD_H
#define FIELD_H

#include <board_assets.h>
#include <raylib.h>
#include <player.h>

#define FIELD_TEXT_LENGTH 32
#define MAX_BUILDING_LEVEL 4

typedef struct Board Board;

typedef enum FieldType { PROPERTY, ACTION, SUPERACTION} FieldType;

extern const char* buildingLevelNames[MAX_BUILDING_LEVEL];

typedef void(*FieldAction)(Board*);

typedef struct Field
{
	char name[FIELD_TEXT_LENGTH];
	FieldType type;
	union
	{
		struct {
			int value;
			int buildingLevel;
			int ownerId;
			Color color;
		};
		struct {
			char comment[FIELD_TEXT_LENGTH];
			BoardImageId imageId;
			FieldAction action;
		};
	};
} Field;

Field GetPropertyField(const char* name, int value, Color color);
Field GetActionField(const char* name, const char* comment, FieldAction action, BoardImageId imageId);
Field GetSuperactionField(const char* name, const char* comment, FieldAction action, BoardImageId imageId);

void PrintField(Field field);

int GetFeeValue(Field field);
int GetFieldValue(Field field);
int GetUpgradeValue(Field field);

#endif //FIELD_H
