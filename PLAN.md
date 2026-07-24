Board will be displayed in 3D

Board textures will be prerendered

Raygui usage

UI managed by layout engine, that uses GameContext for screen size and for UI scale factor from options slider

Board will be managed by tree of function calls:

- LoadBoard(int size, int scale):
	- LoadBoardLogic()
	- LoadBoardGraphics()

- DrawBoard(Vector3 position):
	- DrawBoardModel(Vector3 position)
	- DrawPlayers(Vector3 position)
