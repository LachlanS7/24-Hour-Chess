#include "Board.h"

Board::Board(boardType bt) {
	if (bt == boardType::normal) {
		insertPiece({ 0, 0 }, rook);
		insertPiece({ 1, 0 }, knight);
		insertPiece({ 2, 0 }, bishop);
		insertPiece({ 3, 0 }, king);
		insertPiece({ 4, 0 }, queen);
		insertPiece({ 5, 0 }, bishop);
		insertPiece({ 6, 0 }, knight);
		insertPiece({ 7, 0 }, rook);

		insertPiece({ 0, 1 }, pawn);
		insertPiece({ 1, 1 }, pawn);
		insertPiece({ 2, 1 }, pawn);
		insertPiece({ 3, 1 }, pawn);
		insertPiece({ 4, 1 }, pawn);
		insertPiece({ 5, 1 }, pawn);
		insertPiece({ 6, 1 }, pawn);
		insertPiece({ 7, 1 }, pawn);

		insertPiece({ 0, 6 }, pawn + 1);
		insertPiece({ 1, 6 }, pawn + 1);
		insertPiece({ 2, 6 }, pawn + 1);
		insertPiece({ 3, 6 }, pawn + 1);
		insertPiece({ 4, 6 }, pawn + 1);
		insertPiece({ 5, 6 }, pawn + 1);
		insertPiece({ 6, 6 }, pawn + 1);
		insertPiece({ 7, 6 }, pawn + 1);

		insertPiece({ 0, 7 }, rook + 1);
		insertPiece({ 1, 7 }, knight + 1);
		insertPiece({ 2, 7 }, bishop + 1);
		insertPiece({ 3, 7 }, king + 1);
		insertPiece({ 4, 7 }, queen + 1);
		insertPiece({ 5, 7 }, bishop + 1);
		insertPiece({ 6, 7 }, knight + 1);
		insertPiece({ 7, 7 }, rook + 1);
	}
}

void Board::attachTexture(Piece piece, std::string address, bool rgba) {
	attachTexture(pieces(piece.getStaticID()), piece.get(options::colour), address, rgba);
}

void Board::attachTexture(pieces pieceID, bool colour, std::string address, bool rgba) {
	pieceTextureMap.insert_or_assign(pieceID + colour, 0);
	utilities::genTexture(address, &pieceTextureMap[pieceID + colour], rgba);
}

texture Board::getTexture(Piece piece) {
	return pieceTextureMap[pieces(piece.getStaticID())];
}

texture Board::getTexture(unsigned int index) {
	return pieceTextureMap[pieces(getPiece(index).getStaticID())];
}

utilities::Vec2 Board::position(int index) {
	int x = index % 8;
	int y = (index - x) / 8;

	return utilities::Vec2(x, y);
}

bool Board::vacant(unsigned int index) {
	return board[index].id == 0;
}

bool Board::clearPath(utilities::Vec2 direction, int initialIndex, int finalIndex) {

	initialIndex += direction.x;
	initialIndex += direction.y * 8;

	while (initialIndex != finalIndex) {
		if (board[initialIndex].id != 0) { return false; }
		initialIndex += direction.x;
		initialIndex += direction.y * 8;
	}

	return true;
}

Piece& Board::getPiece(unsigned int index) {
	return board[index];
}

void Board::insertPiece(utilities::Vec2 position, Piece piece) {
	board[position.x + 8 * position.y] = piece;
}

void Board::insertPiece(utilities::Vec2 position, unsigned int pieceId) {
	board[position.x + 8 * position.y] = Piece(pieceId);
}

bool Board::isDoubleStepValid(Piece& selectedPiece, float distance) {
	if (selectedPiece.get(options::unitStep)) {

		if (distance == 2) {

			//Ensuring can't double step
			if (!selectedPiece.get(options::doubleFirstStep) || selectedPiece.get(options::moved)) {
				return false;
			}

		}
		else if (distance >= 3) { return false; }
	}
	return true;
}

bool Board::isUniDirectionalityValid(Piece& selectedPiece, Piece& targetPiece, utilities::Vec2 deltaPos) {
	if (selectedPiece.get(options::uniDirectional)) {
		if (deltaPos.y / abs(deltaPos.y) != -2 * selectedPiece.get(options::colour) + 1) { //Not moving in correct vertical direction
			return false;
		}

		else if (deltaPos.x != 0) {

			if (!selectedPiece.get(options::diagonalAttackOnly)) { //Not Pawn
				return false;
			}
			else { //Is Pawn

				if (!isAttacking(selectedPiece, targetPiece)) { //Not attacking
					return false;
				}

				else if (abs(deltaPos.x) != 1 || abs(deltaPos.y) != 1) {
						return false; //Phew, close call...
				}
			}
		}
	}

	return true;
}

bool Board::isLmovementValid(Piece& selectedPiece, float distance) {

	
	if (selectedPiece.get(lMovement) && !selectedPiece.get(horizontalMovement) && !selectedPiece.get(diagonalMovement) && !selectedPiece.get(verticalMovement) && !selectedPiece.get(diagonalAttackOnly)) {
		if (distance == sqrtf(5)) {
			return true;
		}
	}

	return false;
}

bool Board::isOtherMovementValid(Piece& selectedPiece, Piece& targetPiece, utilities::Vec2 deltaPos, unsigned int intialIndex, unsigned int finalIndex) {

	//Diagonal Movement
	if (selectedPiece.get(diagonalMovement) || selectedPiece.get(diagonalAttackOnly)) {
		if (abs(deltaPos.x) == abs(deltaPos.y) && clearPath(deltaPos.direction(), intialIndex, finalIndex)) {
			return true;
		}
	}

	if (deltaPos.y != 0 && deltaPos.x != 0) {
		return false;
	}

	if ((((deltaPos.y != 0) > selectedPiece.get(options::verticalMovement)) xor ((deltaPos.x != 0) > selectedPiece.get(options::horizontalMovement))) == 0) {
		if (clearPath(deltaPos.direction(), intialIndex, finalIndex)) {
			return true;
		}
	}

	return false;
}

bool Board::isCastling(Piece& selectedPiece, Piece& targetPiece, utilities::Vec2 deltaPos, int initialIndex, int finalIndex) {
	//Casteling
	if (!isAttacking(selectedPiece, targetPiece)) {
		if (selectedPiece.get(options::castable) && targetPiece.get(options::checkable)) {
			if (!selectedPiece.get(options::moved) && !targetPiece.get(options::moved) && clearPath(deltaPos.direction(), initialIndex, finalIndex)) {
				return true;
			}
		}
	}

	return false;
}

bool Board::isAttacking(Piece& selectedPiece, Piece& targetPiece) {
	if (targetPiece.id == 0) return false;
	return selectedPiece.get(options::colour) != targetPiece.get(options::colour);
}

bool Board::move(int i1, int i2) {
	auto& selectedPiece = board[i1];
	auto& targetPiece = board[i2];
	utilities::Vec2 deltaPos = position(i2) - position(i1);

	//Ensure a peice is selected
	if (vacant(i1)) {
		return false;
	}

	//Check to ensure a move is actually made
	if (i1 == i2) {
		return false;
	}

	if (!isDoubleStepValid(selectedPiece, deltaPos.length())) {
		return false;
	}

	//Check to see if uni-direction condition is held(make sure it is traveling in the direction of attack as accordance to colour);
	if (!isUniDirectionalityValid(selectedPiece, targetPiece, deltaPos)) {
		return false;
	}

	//Check to see if L-movement condition is held;
	if (!isLmovementValid(selectedPiece, deltaPos.length())) {
		if (!isOtherMovementValid(selectedPiece, targetPiece, deltaPos, i1, i2)) {
			return false;
		}
	}

	if (isCastling(selectedPiece, targetPiece, deltaPos, i1, i2)) {
		Piece placeholder = board[i1];
		board[i1] = board[i2];
		board[i2] = placeholder;
		board[i1].id += pow(2, (int)options::moved);
		board[i2].id += pow(2, (int)options::moved);
	}
	else {

		if (isAttacking(selectedPiece, targetPiece) || targetPiece.id == 0) {
			board[i2] = board[i1];
			board[i1].id = 0;
			if (!board[i2].get(options::moved)) {
				board[i2].id += pow(2, (int)options::moved);
			}
		}
		else { return false; }
	}

	if (position(i2).y == 7 * (-board[i2].get(options::colour) + 1) && board[i2].get(options::promotable)) {
		board[i2].id = queen + board[i2].get(options::colour);
	}

	return true;
}