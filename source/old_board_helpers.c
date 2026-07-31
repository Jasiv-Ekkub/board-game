#include <old_board_helpers.h>
#include <stdio.h>
#include <string.h>

/*

void DrawBoardEdge(Image* image, BoardGraphics graphics, BoardRenderData renderData, Rectangle rectangle, Field field)
{
	char commentBuffer[FIELD_NAME_LENGTH];
	switch(field.type)
	{
		case PROPERTY:
			ImageDrawRectangleRec(image, (Rectangle){
				rectangle.x,
				rectangle.y,
				rectangle.width,
				graphics.dividerOffset,
			}, field.color);
			float offset = graphics.dividerOffset + graphics.borderWidth;
			ImageDrawRectangleRec(image, (Rectangle){
				rectangle.x,
				rectangle.y + offset,
				rectangle.width,
				rectangle.height - offset,
			}, renderData.colorPalette.light);
			snprintf(commentBuffer, FIELD_NAME_LENGTH, "$%i", field.value);
			break;
		case ACTION:
			ImageDrawRectangleRec(image, rectangle, renderData.colorPalette.light);

			Image icon = renderData.images[field.imageId];
			ImageDrawImagePro(image, icon,
				(Rectangle){0, 0, (float)icon.width, (float)icon.height},
				(Rectangle){
					rectangle.x + (rectangle.width - renderData.imageSizeEdge)/2,
					rectangle.y + (rectangle.height - renderData.imageSizeEdge)/2,
					renderData.imageSizeEdge,
					renderData.imageSizeEdge,
				},
				(Vector2){0}, 0,
				WHITE
			);
			
			strncpy(commentBuffer, field.comment, FIELD_NAME_LENGTH);
			break;
		default:
			break;
	}
	ImageDrawTextSpec(image,
		renderData.font,
		field.name,
		(Vector2){
			rectangle.x + rectangle.width/2,
			rectangle.y + rectangle.height - renderData.nameOffsetEdge,
		},
		0,
		renderData.fontSize,
		renderData.fontSpacing,
		renderData.colorPalette.dark
		);
	ImageDrawTextSpec(image,
		renderData.font,
		commentBuffer,
		(Vector2){
			rectangle.x + rectangle.width/2,
			rectangle.y + rectangle.height - renderData.commentOffsetEdge,
		},
		0,
		renderData.fontSize,
		renderData.fontSpacing,
		renderData.colorPalette.dark
		);
}

Vector2 CalculateFieldCenter(Board board, int fieldNumber)
{
	BoardGraphics graphics = board.graphics;
	int quarter = board.fieldCount/4;
	float cornerOffset = graphics.borderWidth + graphics.cornerSize/2;
	
	Vector2 position = {0};

	int sideId = fieldNumber/quarter;
	int sideFieldId = fieldNumber%quarter;
	if(sideFieldId == 0)
	{
		switch(sideId)
		{
			case 0:
				position = (Vector2){
					cornerOffset,
					graphics.pixelSize - cornerOffset,
				};
				break;
			case 1:
				position = (Vector2){
					cornerOffset,
					cornerOffset,
				};
				break;
			case 2:
				position = (Vector2){
					graphics.pixelSize - cornerOffset,
					cornerOffset,
				};
				break;
			case 3:
				position = (Vector2){
					graphics.pixelSize - cornerOffset,
					graphics.pixelSize - cornerOffset,
				};
				break;
		}
	}
	else
	{
		float edgeOffsetBase = cornerOffset*2 + graphics.fieldWidth/2;
		float edgeOffset = graphics.fieldWidth + graphics.borderWidth;

		float edgeOffsetFinal =  edgeOffsetBase + edgeOffset*(float)(sideFieldId - 1);
		switch(sideId)
		{
			case 0:
				position = (Vector2){
					cornerOffset,
					graphics.pixelSize - edgeOffsetFinal,
				};
				break;
			case 1:
				position = (Vector2){
					edgeOffsetFinal,
					cornerOffset,
				};
				break;
			case 2:
				position = (Vector2){
					graphics.pixelSize - cornerOffset,
					edgeOffsetFinal,
				};
				break;
			case 3:
				position = (Vector2){
					graphics.pixelSize - edgeOffsetFinal,
					graphics.pixelSize - cornerOffset,
				};
				break;
		}
	}

	return position;
}

Vector3 CalculatePlayerPosition(Board board, int playerId)
{
	Player player = board.players[playerId];
	float pawnOffset = board.graphics.pawnOffset;

	Vector2 pixelPosition = CalculateFieldCenter(board, player.position);
	
	pixelPosition.x += (playerId&1 ? pawnOffset : -pawnOffset);
	pixelPosition.y += (playerId&2 ? pawnOffset : -pawnOffset);

	return (Vector3){
		board.position.x + (pixelPosition.x / board.graphics.pixelSize * 2 - 1),
		board.position.y,
		board.position.z + (pixelPosition.y / board.graphics.pixelSize * 2 - 1),
	};
}

Vector3 CalculateHousePosition(Board board, int fieldId)
{
	int half = board.fieldCount/2;
	int quarter = half/2;

	BoardGraphics graphics = board.graphics;

	Vector2 pixelPosition = CalculateFieldCenter(board, fieldId);

	if(fieldId%quarter == 0)
	{
		pixelPosition.x += (graphics.cornerSize - graphics.dividerOffset)/2;
		pixelPosition.y -= (graphics.cornerSize - graphics.dividerOffset)/2;
	}
	else
	{
		if(fieldId%half > quarter)
		{
			pixelPosition.y -= (graphics.cornerSize - graphics.dividerOffset)/2;
		}
		else
		{
			pixelPosition.x += (graphics.cornerSize - graphics.dividerOffset)/2;
		}
	}

	return (Vector3){
		board.position.x + (pixelPosition.x / board.graphics.pixelSize * 2 - 1),
		board.position.y,
		board.position.z + (pixelPosition.y / board.graphics.pixelSize * 2 - 1),
	};
}
*/
