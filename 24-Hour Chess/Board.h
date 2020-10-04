#pragma once
#include <array>
#include <vector>
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

enum boardType {
	normal,
	custom,
};

struct Piece {
	unsigned short id = 0;
	bool get(int flag) { return id & (unsigned short)std::pow(2, flag); }
	unsigned int getStaticID() {
		if (id >= 4096) {
			return id - 4096;
		}
		return id;
	}
	Piece(unsigned int inID = 0) { id = inID; }
};

class Board {

private:
	std::array<Piece, 64> board;
	std::unordered_map<unsigned int, texture> pieceTextureMap = {};

private:
	//Move methods
	bool isDoubleStepValid(Piece& selectedPiece, float distance);
	bool isAttacking(Piece& selectedPiece, Piece& targetPiece);
	bool isUniDirectionalityValid(Piece& selectedPiece, Piece& targetPiece, utilities::Vec2 deltaPosition);
	bool isLmovementValid(Piece& selectedPiece, float distance);
	bool isOtherMovementValid(Piece& selectedPiece, Piece& targetPiece, utilities::Vec2 deltaPos, unsigned int intialIndex, unsigned int finalIndex);
	bool isCastling(Piece& selectedPiece, Piece& targetPiece, utilities::Vec2 deltaPos, int initialIndex, int finalIndex);

	bool clearPath(utilities::Vec2 direction, int initialIndex, int finalIndex);
	bool vacant(unsigned int index);

public:
	Board(boardType bt = boardType::normal);
	void attachTexture(Piece piece, std::string address, bool rgba = true);
	void attachTexture(pieces pieceID, bool colour, std::string address, bool rgba = true);
	texture getTexture(Piece piece);
	texture getTexture(unsigned int index);
	void insertPiece(utilities::Vec2 position, unsigned int pieceID);
	void insertPiece(utilities::Vec2 position, Piece piece);
	utilities::Vec2 position(int index);
	Piece& getPiece(unsigned int index);
	bool move(int i1, int i2);
};