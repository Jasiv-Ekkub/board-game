#include <board_config.h>
#include <raylib.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <json.h>

const BoardConfig defaultConfig = {
    .groupCount = 8,
    .groups = {
            {
                .color = BROWN,
                .cityCount = 4,
                .cities = {
                        { "Frankfurt", 300, 400 },
                        { "Berlin",    300, 400 },
                        { "Munchen",   300, 400 },
                        { "Hamburg",   300, 400 },
                    }
            },
            {
                .color = DARKBROWN,
                .cityCount = 4,
                .cities = {
                        { "Szczecin", 350, 450 },
                        { "Warszawa", 350, 450 },
                        { "Krakow",   350, 450 },
                        { "Katowice", 350, 450 },
                    }
            },
            {
                .color = DARKPURPLE,
                .cityCount = 4,
                .cities = {
                        { "Copenhagen", 400, 500 },
                        { "Stockholm",  400, 500 },
                        { "Oslo",       400, 500 },
                        { "Helsinki",   400, 500 },
                    }
            },
            {
                .color = PURPLE,
                .cityCount = 4,
                .cities = {
                        { "Dublin",     450, 550 },
                        { "London",     450, 550 },
                        { "Glasgow",    450, 550 },
                        { "Manchester", 450, 550 },
                    }
            },
            {
                .color = LIME,
                .cityCount = 4,
                .cities = {
                        { "Lyon",    500, 600 },
                        { "Paris",   500, 600 },
                        { "Rennes",  500, 600 },
                        { "Orleans", 500, 600 },
                    }
            },
            {
                .color = DARKGREEN,
                .cityCount = 4,
                .cities = {
                        { "Madrid",    550, 650 },
                        { "Lisbon",    550, 650 },
                        { "Seville",   550, 650 },
                        { "Barcelona", 550, 650 },
                    }
            },
            {
                .color = ORANGE,
                .cityCount = 4,
                .cities = {
                        { "Venice",   600, 700 },
                        { "Rome",     600, 700 },
                        { "Naples",   600, 700 },
                        { "Florence", 600, 700 },
                    }
            },
            {
                .color = YELLOW,
                .cityCount = 4,
                .cities = {
                        { "Athens",   650, 750 },
                        { "Zagreb",   650, 750 },
                        { "Sarajevo", 650, 750 },
                        { "Belgrad",  650, 750 },
                    }
            },
        }
    };

int GetGroupInfo(Group* group, json_value* json);

BoardConfig GetBoardConfig()
{
    char *text = LoadFileText("config.json");
    if(!text)
    {
        TraceLog(LOG_WARNING, "Couldn't load \"config.json\", loading default board configuration");
        return defaultConfig;
    }

    json_value* json = json_parse(text, strlen(text));
    free(text);
    if(!json)
    {
        TraceLog(LOG_WARNING, "Couldn't parse \"config.json\" properly, loading default board configuration");
        json_value_free(json);
        return defaultConfig;
    }

    if(json->type != json_array)
    {
        TraceLog(LOG_WARNING, "Configuration file must be an array of city groups, loading default board configuration");
        json_value_free(json);
        return defaultConfig;
    }
    
    BoardConfig config = {
        .groupCount = json->u.array.length,
        .groups = {},
        };
    
    if(config.groupCount < MIN_GROUP_COUNT)
    {
        TraceLog(LOG_WARNING, "Configuration file must contain at least %i city groups, loading default board configuration", MIN_GROUP_COUNT);
        json_value_free(json);
        return defaultConfig;
    }
    else if(config.groupCount > MAX_GROUP_COUNT)
        config.groupCount = MAX_GROUP_COUNT;
    
    for(unsigned int i=0; i<config.groupCount; ++i)
    {
        if(GetGroupInfo(&config.groups[i], json->u.array.values[i]))
        {
            TraceLog(LOG_WARNING, "Couldn't load group %i, loading default board configuration", i);
            json_value_free(json);
            return defaultConfig;
        }
    }

    json_value_free(json);
    TraceLog(LOG_INFO, "Loaded board configuration from file");
    return config;
}

int GetCityInfo(City* city, json_value* json);

#define COLOR_FLAG 1
#define CITIES_FLAG 2

int GetGroupInfo(Group* group, json_value* json)
{
    if(json->type != json_object)
    {
        TraceLog(LOG_WARNING, "City group must an object");
        return 1;
    }

    unsigned char flags = 0;
    for(unsigned int i=0; i<json->u.object.length; ++i)
    {
        json_object_entry entry = json->u.object.values[i];
        if(strncmp(entry.name, "color", entry.name_length) == 0)
        {
            if(entry.value->type != json_integer)
            {
                TraceLog(LOG_WARNING, "Property \"color\" must be an integer");
                return 1;
            }
            unsigned int colorVal = entry.value->u.integer;

            if(colorVal < 0)
            {
                TraceLog(LOG_WARNING, "Color value must more or equal to 0");
                colorVal = 0;
            }
            else if(colorVal > 16777214)
            {
                TraceLog(LOG_WARNING, "Color value must more or equal to 16777214");
                colorVal = 16777214;
            }
            colorVal <<= 16;
            colorVal |= 0xff;
            group->color = GetColor(colorVal);
            flags |= COLOR_FLAG;
        }
        else if(strncmp(entry.name, "cities", entry.name_length) == 0)
        {
            if(entry.value->type != json_array)
            {
                TraceLog(LOG_WARNING, "Property \"cities\" must be an array");
                return 1;
            }

            group->cityCount = entry.value->u.array.length;
            if(group->cityCount < MIN_CITY_COUNT)
            {
                TraceLog(LOG_WARNING, "City group must contain at least %i cities", MIN_CITY_COUNT);
                return 1;
            }
            else if(group->cityCount > MAX_CITY_COUNT)
                group->cityCount = MAX_CITY_COUNT;

            for(unsigned int j=0; j<group->cityCount; ++j)
            {
                if(GetCityInfo(&group->cities[j], entry.value->u.array.values[j]))
                {
                    TraceLog(LOG_WARNING, "Couldn't load city %i", j);
                    return 1;
                }
            }
            flags |= CITIES_FLAG;
        }
    }
    if(flags != 3)
    {
        TraceLog(LOG_WARNING, "Couldn't all necessary group data");
        return 1;
    }
    return 0;
}

#define NAME_FLAG 1
#define MIN_COST_FLAG 2
#define MAX_COST_FLAG 4
int GetCityInfo(City* city, json_value* json)
{
    if(json->type != json_object)
    {
        TraceLog(LOG_WARNING, "City must an object");
        return 1;
    }
    
    unsigned char flags = 0;
    for(unsigned int i=0; i<json->u.object.length; ++i)
    {
        json_object_entry entry = json->u.object.values[i];
        
        if(strncmp(entry.name, "name", entry.name_length) == 0)
        {
            if(entry.value->type != json_string)
            {
                TraceLog(LOG_WARNING, "Property \"name\" must be a string");
                return 1;
            }
            strncpy(city->name, entry.value->u.string.ptr, FIELD_TEXT_LENGTH);
            flags |= NAME_FLAG;
        }
        else if(strncmp(entry.name, "minCost", entry.name_length) == 0)
        {
            if(entry.value->type != json_integer)
            {
                TraceLog(LOG_WARNING, "Property \"minCost\" must be an integer");
                return 1;
            }
            city->minCost = entry.value->u.integer;
            flags |= MIN_COST_FLAG;
        }
        else if(strncmp(entry.name, "maxCost", entry.name_length) == 0)
        {
            if(entry.value->type != json_integer)
            {
                TraceLog(LOG_WARNING, "Property \"maxCost\" must be an integer");
                return 1;
            }
            city->maxCost = entry.value->u.integer;
            flags |= MAX_COST_FLAG;
        }
    }
    if(!(flags & NAME_FLAG))
    {
        TraceLog(LOG_WARNING, "City must have a name");
        return 1;
    }

    if(!(flags & MIN_COST_FLAG)) city->minCost = 300;
    if(!(flags & MAX_COST_FLAG)) city->maxCost = city->minCost;

    return 0;
}

void PrintConfig(BoardConfig config)
{
    printf("START CONFIGURATION %i\n", config.groupCount);
    for(unsigned int i=0; i<config.groupCount; ++i)
    {
        Group group = config.groups[i];
        printf("START GROUP %i %i\n", i, group.cityCount);
        printf("COLOR %u %u %u %u\n", group.color.r, group.color.g, group.color.b, group.color.a);

        for(unsigned int j=0; j<group.cityCount; ++j)
        {
            City city = group.cities[j];
            printf("CITY %i %s %i %i\n", j, city.name, city.minCost, city.maxCost);
        }

        printf("END GROUP %i\n", i);
    }
    printf("END CONFIGURATION\n");
}
