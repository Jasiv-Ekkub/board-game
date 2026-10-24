#include <board_config.h>
#include <raylib.h>

/*
	{
		BROWN,
		{ "Frankfurt", "Berlin", "Munchen", "Hamburg" },
	},
	{
		DARKBROWN,
		{ "Szczecin", "Warszawa", "Krakow", "Katowice" }
	},
	{
		DARKPURPLE,
		{ "Copenhagen", "Stockholm", "Oslo", "Helsinki" },
	},
	{
		PURPLE,
		{ "Dublin", "London", "Glasgow", "Manchester" },
	},
	{
		LIME,
		{ "Lyon", "Paris", "Rennes", "Orleans" },
	},
	{
		DARKGREEN,
		{ "Madrid", "Lisbon", "Seville", "Barcelona" },
	},
	{
		ORANGE,
		{ "Venice", "Rome", "Naples", "Florence" },
	},
	{
		YELLOW,
		{ "Athens", "Zagreb", "Sarajevo", "Belgrad" },
	},
*/
const BoardConfig defaultBoardConfig = {
    .groupDataCount = 8,
    .groupDatas = {
            {
                .color = BROWN,
                .fieldDataCount = 4,
                .fieldDatas = {
                        { "Frankfurt", 300, 400 },
                        { "Berlin",    300, 400 },
                        { "Munchen",   300, 400 },
                        { "Hamburg",   300, 400 },
                    }
            },
            {
                .color = DARKBROWN,
                .fieldDataCount = 4,
                .fieldDatas = {
                        { "Szczecin", 350, 450 },
                        { "Warszawa", 350, 450 },
                        { "Krakow",   350, 450 },
                        { "Katowice", 350, 450 },
                    }
            },
            {
                .color = DARKPURPLE,
                .fieldDataCount = 4,
                .fieldDatas = {
                        { "Copenhagen", 400, 500 },
                        { "Stockholm",  400, 500 },
                        { "Oslo",       400, 500 },
                        { "Helsinki",   400, 500 },
                    }
            },
            {
                .color = PURPLE,
                .fieldDataCount = 4,
                .fieldDatas = {
                        { "Dublin",     450, 550 },
                        { "London",     450, 550 },
                        { "Glasgow",    450, 550 },
                        { "Manchester", 450, 550 },
                    }
            },
            {
                .color = LIME,
                .fieldDataCount = 4,
                .fieldDatas = {
                        { "Lyon",    500, 600 },
                        { "Paris",   500, 600 },
                        { "Rennes",  500, 600 },
                        { "Orleans", 500, 600 },
                    }
            },
            {
                .color = DARKGREEN,
                .fieldDataCount = 4,
                .fieldDatas = {
                        { "Madrid", 550, 650 },
                        { "Lisbon",    550, 650 },
                        { "Seville",   550, 650 },
                        { "Barcelona",   550, 650 },
                    }
            },
            {
                .color = ORANGE,
                .fieldDataCount = 4,
                .fieldDatas = {
                        { "Venice",   600, 700 },
                        { "Rome",     600, 700 },
                        { "Naples",   600, 700 },
                        { "Florence", 600, 700 },
                    }
            },
            {
                .color = YELLOW,
                .fieldDataCount = 4,
                .fieldDatas = {
                        { "Athens",   650, 750 },
                        { "Zagreb",   650, 750 },
                        { "Sarajevo", 650, 750 },
                        { "Belgrad",  650, 750 },
                    }
            },
        }
    };

BoardConfig GetBoardConfig()
{
    return defaultBoardConfig;
}
