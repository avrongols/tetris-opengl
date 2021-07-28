// game.h

#include "Square.h"
#include "Piece.h"

// Main field rows and columns
const int ROWS = 20;
const int COLS = 10;

// Main game class
class Game {
public:

	// Constructor
	Game();

	// Main game class methods
	bool canRotate(Piece p);
	bool moveCollision(int dir);
	bool rotationCollision();
	void clearMainGrid();
	void clearNextPieceGrid();
	void genNextPiece();
	void restart();
	void move(int dir);
	void rotateShape(int rot);
	void updateActivePiece();
	void updateNextPieceGrid();
	void fixActivePiece();
	void update();
	void updateActiveAfterCollision();
	void checkLine();
	void clearLine();
	//bool gameOver();
	
	// Game pieces
	Piece activePiece;
	Piece nextPiece;
	Piece activePieceCopy;

	// Current game grid
	Square mainGrid[ROWS][COLS];

	// Grid with the next piece
	Square nextPieceGrid[5][5];

	// Game data
	bool killed;	
	bool paused;
	bool deleteLines;
	int linesCleared;
	int shapesCount;
	int timer;		
};
