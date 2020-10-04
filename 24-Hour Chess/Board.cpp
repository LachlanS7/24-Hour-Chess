#include "Board.h"

Board::Board() {

}

Vec2 Board::position(int index) {
	int x = index % 8;
	int y = (index - x) / 8;

	return Vec2(x, y);
}

bool Board::vacant(unsigned int index) {
	return board[index].id == 0;
}

bool Board::clearPath(Vec2 direction, int initialIndex, int finalIndex) {

	initialIndex += direction.x;
	initialIndex += direction.y * 8;

	while (initialIndex != finalIndex) {
		if (board[initialIndex].id != 0) { return false; }
		initialIndex += direction.x;
		initialIndex += direction.y * 8;
	}

	return true;
}

Piece Board::getPiece(unsigned int index) {
	return board[index];
}

void Board::insertPiece(Vec2 position, Piece piece) {
	board[position.x + 8 * position.y] = piece;
}

void Board::insertPiece(Vec2 position, unsigned int pieceId) {
	board[position.x + 8 * position.y] = Piece(pieceId);
}

bool Board::isDoubleStepValid(Piece& selectedPiece, int distance) {
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

bool Board::isAttacking(Piece& selectedPiece, Piece& targetPiece) {
	if (targetPiece.id == 0) return false;
	return selectedPiece.get(options::colour) != selectedPiece.get(options::colour);
}

bool Board::move(int i1, int i2) {
	int numberOfDirections = 0;

	auto& selectedPiece = board[i1];
	auto& targetPiece = board[i2];

	Vec2 deltaPos = position(i2) - position(i1);

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
	if (selectedPiece.get(options::uniDirectional)) {

		if (deltaPos.y / abs(deltaPos.y) != -2 * selectedPiece.get(options::colour) + 1) { //Not moving in correct vertical direction
			return false;
		}

		else if (deltaPos.x != 0) {

			if (!selectedPiece.get(options::diagonalAttackOnly)) { //Not Pawn
				return false;
			} else { //Is Pawn

				if (!isAttacking(selectedPiece, targetPiece)) { //Not attacking
					return false;
				}

				else //Attacking! Oh shit! Watchout!

				{
					if (deltaPos.x != 1 || abs(deltaPos.y) != 1) {
						return false; //Phew, close call...
					}
				}
			}
		}
	}

	//Check to see if L-movement condition is held;
	if (deltaPos.length() == (float)sqrt(5)) {
		if (!(board[i1] & lMovement)) {
			return false;
		}
	}

	else {
		//Check to see if diagonal movement is held;
		if (abs(deltaPos.x) == abs(deltaPos.y)) {
			if ((board[i1] & diagonalMovement) || (board[i1] & diagonalAttackOnly)) {
				numberOfDirections++;
			}
			else {
				return false;
			}
		}
		else {
			//Check to see if horizontal movement is held;
			if (deltaPos.x) {
				if (board[i1] & horizontalMovement) {
					numberOfDirections++;
				}
				else {
					return false;
				}
			}

			//Check to see if vertical movement is held;
			if (deltaPos.y) {
				if (board[i1] & verticalMovement) {
					numberOfDirections++;
				}
				else {
					return false;
				}
			}
		}

		if (numberOfDirections == 2 || !clearPath(deltaPos.direction(), i1, i2)) { //This implies it is not diagonal but is moving both horizontally and vertically, this should not be possible
			return false;
		}
	}

	//Check for check!

	//Casteling
	if (board[i1] & castable && board[i2] & checkable) { //CHeck colour too
		if (board[i1] & moved || board[i2] & moved || !clearPath(deltaPos.direction(), i1, i2)) {
			return false;
		}
		else {
			Piece copy = board[i2] + 4096;
			board[i2] = board[i1] + 4096;
			board[i1] = copy;
			return true;
		}
	}
	else if ((board[i2] & colour == board[i1] & colour) && board[i2] != 0) {
		return false;
	}

	board[i2] = board[i1];
	board[i1] = 0;

	if (!(board[i2] & moved)) {
		board[i2] += 4096;
	}

	return true;
}