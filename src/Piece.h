// piece.h

// Basic constants
const int numPieces = 7;	// Number of different pieces
const int numRotations = 4; // Number of turns for each piece
const int numSpaces = 8;	// Memory capacity for storing information about each piece

// Game piece class
class Piece {

public:

	// Constructors
	Piece();
	Piece(int newPiece);

	// Piece coordinates (calculated from the upper left corner of the piece, even if this corner is empty)
	int x;
	int y;

	// Color values
	float redVal, greenVal, blueVal;

	// Piece type and rotation
	int type;
	int rotation;

	// Rotating a piece
	void rotatePiece(int dir);

	// Setting the piece color in RGB format
	void color(float r, float g, float b);

	// Values for rotating pieces
	const int* rotations();
};

// All pieces with each option of their rotation in the grid in the format {x0, y0, x1, y1, x2, y2, x3, y3}
const int gamePieces[numPieces][numRotations][numSpaces] =
{
	{
		{0, 0, 1, 0, 0, 1, 1, 1}, // Square
		{0, 0, 1, 0, 0, 1, 1, 1},
		{0, 0, 1, 0, 0, 1, 1, 1},
		{0, 0, 1, 0, 0, 1, 1, 1},
	},
	{
		{0, 0, 0, 1, 0, 2, 0, 3}, // Vertical line
		{0, 0, 1, 0, 2, 0, 3, 0},
		{0, 0, 0, 1, 0, 2, 0, 3},
		{0, 0, 1, 0, 2, 0, 3, 0},
	},
	{
		{0, 0, 0, 1, 1, 1, 0, 2}, // T piece
		{1, 0, 0, 1, 1, 1, 2, 1},
		{0, 1, 1, 0, 1, 1, 1, 2}, 
		{0, 0, 1, 0, 2, 0, 1, 1}
	},
	{	{0, 0, 1, 0, 0, 1, 0, 2}, // L piece
		{0, 0, 0, 1, 1, 1, 2, 1},
		{1, 0, 1, 1, 0, 2, 1, 2},
		{0, 0, 1, 0, 2, 0, 2, 1}
	},
	{	{0, 0, 1, 0, 1, 1, 1, 2}, // Reverse L piece
		{0, 0, 1, 0, 2, 0, 0, 1},
		{0, 0, 0, 1, 0, 2, 1, 2},
		{2, 0, 0, 1, 1, 1, 2, 1}
	},
	{	{0, 0, 0, 1, 1, 1, 1, 2}, // Z piece
		{1, 0, 2, 0, 0, 1, 1, 1},
		{0, 0, 0, 1, 1, 1, 1, 2},
		{1, 0, 2, 0, 0, 1, 1, 1}
	},
	{	{1, 0, 0, 1, 1, 1, 0, 2}, // Reverse Z piece
		{0, 0, 1, 0, 1, 1, 2, 1},
		{1, 0, 0, 1, 1, 1, 0, 2},
		{0, 0, 1, 0, 1, 1, 2, 1}
	}
};
