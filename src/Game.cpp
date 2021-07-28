// game.cpp

#include <cstdlib>
#include <cstdio>
#include <iostream>

#include "glut.h"
#include "Game.h"

using namespace std;

/*
Called whenever the timer expires, but the game does not end or pause
*/
void Game::update() {
	// Check pieces collision
	if (moveCollision(0)) {					// If there was a collision
		if (activePiece.y <= 2) {			// checking if it ends the game
			killed = true;
		}
		else {								// If the game is still active
			updateActiveAfterCollision();	// The figure is fixed in place of the collision
			checkLine();					// Checking for filled lines
			if (deleteLines)				// If lines were found to be deleted,
				clearLine();				// filled lines are removed
			genNextPiece();					// Generating a new piece

			clearNextPieceGrid();
			updateNextPieceGrid();

			updateActivePiece();			// Its update in the game grid
		}
	}
	else {									// If there was no collision, the piece drops below
		fixActivePiece();
		activePiece.y++;
		updateActivePiece();
	}
}

/*
Starting a new game and initializing the required elements
*/
void Game::restart()
{
	clearMainGrid();		// Cleaning the main grid
	clearNextPieceGrid();	// Clearing the grid with the next figure
	linesCleared = 0;		// The player's score is zero
	shapesCount = 1;		// The counter of pieces per game is equal to one
	killed = false;
	paused = false;
	deleteLines = false;

	// Generating the current random piece
	activePiece = Piece(rand() % numPieces);
	activePiece.x = COLS/2;
	activePiece.y = 0;
	updateActivePiece();

	// Generating the next piece
	nextPiece = Piece(rand() % numPieces);
	nextPiece.x = COLS/2;
	nextPiece.y = 0;
	updateNextPieceGrid();
}

/*
Updating the game grid and the correct display of the active figure when falling
*/
void Game::fixActivePiece() {
	// Determining the data of the current piece by its type and position
	const int* trans = activePiece.rotations();
	for(int i = 0; i < 8; i += 2){
		Square &square = mainGrid[activePiece.y + trans[i + 1]][activePiece.x + trans[i]];
		// Setting active and inactive cells
		square.isFilled = false;
		square.isActive = false;
	}
}

/*
Create the next game piece
*/
void Game::genNextPiece() {
	activePiece = nextPiece;
	nextPiece = Piece(rand() % numPieces);
	nextPiece.x = COLS/2;
	nextPiece.y = 0;
	// Increase the piece counter per game
	shapesCount++;
}

/*
Move the active piece left and right
*/
void Game::move(int dir)
{
	if(moveCollision(dir))	// If there is a collision with one of the borders,
		return;				// nothing happens
	fixActivePiece();
	activePiece.x += dir;
	updateActivePiece();
}

/*
Cleaning the main grid
*/
void Game::clearMainGrid()
{
	for (int r=0; r<ROWS; r++) {
		for (int c=0; c<COLS; c++) {
			mainGrid[r][c].isFilled = false;
			mainGrid[r][c].isActive = false;
		}
	}
}

/*
Clearing the grid with the next piece
*/
void Game::clearNextPieceGrid()
{
	for (int r = 0; r < 5; r++) {	
		for (int c = 0; c < 5; c++) {
			nextPieceGrid[r][c].isFilled = false;
			nextPieceGrid[r][c].isActive = false;
		}
	}
}

/*
Updating the position of the active piece with rotation
*/
void Game::updateActivePiece() {
	// Pointer to an array that stores all conversions
	const int* trans = activePiece.rotations();
	for(int i = 0; i < 8; i += 2){
		// Find the active piece in the game grid
		Square &square = mainGrid[activePiece.y + trans[i + 1]][activePiece.x + trans[i]];
		// Convert the active piece to filled grid cells
		square.isFilled = true;
		square.isActive = true;
		square.red = activePiece.redVal;
		square.green = activePiece.blueVal;
		square.blue = activePiece.greenVal;
	}	
}

/*
Updating the grid with the next piece
*/
void Game::updateNextPieceGrid() {
	// Pointer to an array that stores all conversions
	const int* transNext = nextPiece.rotations();
	for (int i = 0; i < 8; i += 2) {
		// Find the active piece in the game grid
		Square& squareNext = nextPieceGrid[nextPiece.y + transNext[i + 1]][nextPiece.x + transNext[i]];
		// Convert the active piece to filled grid cells
		squareNext.isFilled = true;
		squareNext.isActive = true;
		squareNext.red = nextPiece.redVal;
		squareNext.green = nextPiece.blueVal;
		squareNext.blue = nextPiece.greenVal;
	}
}

/*
Constructor
*/
Game::Game()
{
	restart();
	timer = 500;
}

/*
Rotate the current piece and check if it can be rotated
*/
void Game::rotateShape(int dir) {
	// Create a copy of the active piece and check if it can be rotated
	activePieceCopy = Piece(rand() % numPieces);
	activePieceCopy.x = activePiece.x;
	activePieceCopy.y = activePiece.y;
	activePieceCopy.rotation = activePiece.rotation;
	activePieceCopy.type = activePiece.type;
	activePieceCopy.rotatePiece(dir);
	
	// If the active piece can be rotated, it is rotated and displayed
	if(canRotate(activePieceCopy)) {
		fixActivePiece();
		activePiece.rotatePiece(dir);
		updateActivePiece();
	}
}

/*
Checking whether a piece can be rotated
*/
bool Game::canRotate(Piece activeP) {
	if(rotationCollision()) {
		return false;
	}
	else
		return true;
}

/*
Checking for collisions when rotating a piece
*/
bool Game::rotationCollision() {
	int x, y;
	const int* trans = activePieceCopy.rotations();
	for (int i = 0; i < 8; i += 2) {
		x = activePieceCopy.x + trans[i];
		y = activePieceCopy.y + trans[i + 1];

		if (x >= COLS || y >= ROWS || x < 0 || (mainGrid[y][x].isFilled && !mainGrid[y][x].isActive))
			return true;
	}
	return false;
}

/*
Checking for collisions when the piece is moving
*/
bool Game::moveCollision(int dir) {
	int x, y;
	const int* trans = activePiece.rotations();
	for (int i = 0; i < 8; i += 2) {
		x = activePiece.x + trans[i];
		y = activePiece.y + trans[i + 1];
		if (dir == 0)
			y += 1;
		else
			x += dir;
		if (x >= COLS || y >= ROWS || x < 0 || (mainGrid[y][x].isFilled && !mainGrid[y][x].isActive))
			return true;
	}
	return false;
}

/*
Updating the location of the active piece after a collision
*/
void Game::updateActiveAfterCollision() {
	const int* trans = activePiece.rotations();
	for(int i = 0; i < 8; i += 2){
		Square &square = mainGrid[activePiece.y + trans[i + 1]][activePiece.x + trans[i]];
		square.isActive = false;
	}
}

/*
Checking lines for filling and setting filled lines for deleting
*/
void Game::checkLine() {
	int fullRows = 0;
	for (int r=0; r<ROWS; r++) {
		bool fullRow = false;
			for (int c=0; c<COLS; c++) {
				Square &square = mainGrid[r][c];
				if (square.isFilled){
					fullRow = true;
				}
				else {
					fullRow = false;
					break;
				}
			}
		if(fullRow) {
			for ( int c =0; c < COLS; c++){
				mainGrid[r][c].toBeDeleted = true;
			}
			deleteLines = true;
			linesCleared++;
		}
	}
}

/*
Remove a filled row and move all pieces up one cell down
*/
void Game::clearLine() {
	for (int r = ROWS-1; r > 0; r--){ // Checking each line
		int linesDeleted = 0;
		if (mainGrid[r][0].toBeDeleted){
			for (int r2 = r; r2>0; r2--){ // Move all rows down one cell
				for (int c = 0; c < COLS; c++){
					mainGrid[r2][c].isFilled=mainGrid[r2-1][c].isFilled;
					mainGrid[r2][c].isActive=mainGrid[r2-1][c].isActive;
					mainGrid[r2][c].toBeDeleted=mainGrid[r2-1][c].toBeDeleted;
					mainGrid[r2][c].red=mainGrid[r2-1][c].red;
					mainGrid[r2][c].green=mainGrid[r2-1][c].green;
					mainGrid[r2][c].blue=mainGrid[r2-1][c].blue;
				}
			}
			r++;
		}
	}
	deleteLines = false;

}
