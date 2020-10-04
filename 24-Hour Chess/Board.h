#pragma once
#include <array>
#include <iostream>
#include <unordered_map>
#include "utilities.h"

enum options {
	colour,
	unitStep,
	uniDirectional,
	checkable,
	horizontalMovement,
	verticalMovement,
	diagonalMovement,
	lMovement,
	diagonalAttackOnly,
	promotable,
	doubleFirstStep,
	castable,
	moved
};

enum pieces {
	pawn = 1830,
	knight = 128,
	rook = 2096,
	bishop = 320,
	queen = 112,
	king = 122
};

struct Piece {
	unsigned short id = 0;
	bool get(int flag) { return id & (unsigned short)std::pow(2, flag); }
	Piece(unsigned int inID = 0) { id = inID; }
};

class Board {

private:
	std::array<Piece, 64> board;

	//Move methods
	bool isDoubleStepValid(Piece& selectedPiece, int distance);
	bool isAttacking(Piece& selectedPiece, Piece& targetPiece);

	bool clearPath(Vec2 direction, int initialIndex, int finalIndex);
	bool vacant(unsigned int index);

public:
	Board();

	Vec2 position(int index);
	Piece getPiece(unsigned int index);
	void insertPiece(Vec2 position, Piece piece);
	void insertPiece(Vec2 position, unsigned int pieceID);
	bool move(int i1, int i2);

};