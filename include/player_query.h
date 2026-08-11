#ifndef PLAYER_QUERY
#define PLAYER_QUERY

#include <field.h>

typedef struct PlayerQuery
{
	const char* message;
	const char* options;
	FieldAction handler;

} PlayerQuery;

#endif //PLAYER_QUERY

