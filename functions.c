#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string.h>
#include "functions.h"
#include <stdlib.h>
char color = 'W';
bool isPromotion = false;
bool canCastleKingSide[] = { true,true };
bool canCastleQueenSide[] = { true,true };
int enpassant[2][2] = { {-1,-1} ,{-1,-1} };//left side then right side
int isEnpassant = false;
char enpassantPiece = NULL;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* texture = NULL;
SDL_Texture* piecesTexture[12];
//Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 900;
// [Y][X]
char board[8][8];
int numberOfLegalMoves = 0;
void init() {
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);

	window = SDL_CreateWindow("Chess", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
}
void defaultPieces() {
	for (int i = 0; i < 8; i++) {
		board[6][i] = 'P';
	}
	for (int i = 0; i < 8; i++) {
		board[1][i] = 'p';
	}
	board[0][0] = 'r';
	board[0][1] = 'n';
	board[0][2] = 'b';
	board[0][3] = 'q';
	board[0][4] = 'k';
	board[0][5] = 'b';
	board[0][6] = 'n';
	board[0][7] = 'r';

	board[7][0] = 'R';
	board[7][1] = 'N';
	board[7][2] = 'B';
	board[7][3] = 'Q';
	board[7][4] = 'K';
	board[7][5] = 'B';
	board[7][6] = 'N';
	board[7][7] = 'R';

	for (int i = 2; i < 6; i++) {
		for (int j = 0; j < 8; j++) {
			board[i][j] = NULL;
		}
	}

}
void loadPieces() {
	//SDL_Surface* imageSurface = IMG_Load("texture.png");
	texture = SDL_CreateTextureFromSurface(renderer, IMG_Load("texture.png"));
	//SDL_FreeSurface(imageSurface);

	//load pieces textures
	piecesTexture[P] = SDL_CreateTextureFromSurface(renderer, IMG_Load("textures/WPAWN.png"));
	piecesTexture[R] = SDL_CreateTextureFromSurface(renderer, IMG_Load("textures/WROOK.png"));
	piecesTexture[N] = SDL_CreateTextureFromSurface(renderer, IMG_Load("textures/WKNIGHT.png"));
	piecesTexture[B] = SDL_CreateTextureFromSurface(renderer, IMG_Load("textures/WBISHOP.png"));
	piecesTexture[Q] = SDL_CreateTextureFromSurface(renderer, IMG_Load("textures/WQUEEN.png"));
	piecesTexture[K] = SDL_CreateTextureFromSurface(renderer, IMG_Load("textures/WKING.png"));

	piecesTexture[p] = SDL_CreateTextureFromSurface(renderer, IMG_Load("textures/BPAWN.png"));
	piecesTexture[r] = SDL_CreateTextureFromSurface(renderer, IMG_Load("textures/BROOK.png"));
	piecesTexture[n] = SDL_CreateTextureFromSurface(renderer, IMG_Load("textures/BKNIGHT.png"));
	piecesTexture[b] = SDL_CreateTextureFromSurface(renderer, IMG_Load("textures/BBISHOP.png"));
	piecesTexture[q] = SDL_CreateTextureFromSurface(renderer, IMG_Load("textures/BQUEEN.png"));
	piecesTexture[k] = SDL_CreateTextureFromSurface(renderer, IMG_Load("textures/BKING.png"));
}
void loadBoard() {
	//SDL_Rect bar = {0,0,SCREEN_WIDTH,800};
	//SDL_RenderSetViewport(renderer, &bar);

	/*viewport seems to be utterly useless*/

	int size = SCREEN_WIDTH / 8;
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(renderer);
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			SDL_Rect square = { j * size, i * size, size , size };
			if ((i + j) % 2 == 0)  SDL_SetRenderDrawColor(renderer, 240, 217, 181, 255);//light squares
			else SDL_SetRenderDrawColor(renderer, 181, 136, 99, 255);
			SDL_RenderFillRect(renderer, &square);
		}
	}
	//SDL_RenderPresent(renderer);
	/*rendering is happening in the game loop*/

}
void turn() {
	SDL_Rect bar = { 0,800,SCREEN_WIDTH,100 };
	//SDL_RenderSetViewport(renderer, &bar);

	if (color == 'W') {
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		//printf("W\n");
		color = 'B';
	}
	else {
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		//printf("B\n");
		color = 'W';
	}
	SDL_RenderFillRect(renderer, &bar);
	
	/*for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			printf("%c ", board[i][j]);
			if (board[i][j]==NULL){
				printf("_");
			}
		}
		printf("\n");
	}
	printf("\n");*/
}
void setPieces() {
	int size = SCREEN_WIDTH / 8;
	//SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	//SDL_RenderClear(renderer);
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			SDL_Rect bar = {i*100,j*100,SCREEN_WIDTH/8,SCREEN_WIDTH/8};
			//SDL_RenderSetViewport(renderer, &bar);
			int index;
			switch (board[j][i]) {
			case 'P':
				index = P;
				break;
			case 'R':
				index = R;
				break;
			case 'N':
				index = N;
				break;
			case 'B':
				index = B;
				break;
			case 'Q':
				index = Q;
				break;
			case 'K':
				index = K;
				break;
			case 'p':
				index = p;
				break;
			case 'r':
				index = r;
				break;
			case 'n':
				index = n;
				break;
			case 'b':
				index = b;
				break;
			case 'q':
				index = q;
				break;
			case 'k':
				index = k;
				break;
			default:
				index = -1;
				break;
			}
			if (index>=0) {
				SDL_Texture* currentPiece = piecesTexture[index];
				SDL_SetTextureBlendMode(currentPiece, SDL_BLENDMODE_BLEND);
				SDL_SetTextureColorMod(currentPiece, 255, 255, 255, 128);
				SDL_RenderCopy(renderer, currentPiece, NULL, &bar);
			}
		}
	}
	//SDL_RenderPresent(renderer);
}
void setOnePiece(int cordX,int cordY,char draggedPiece) {
	SDL_Rect square = { cordX * 100,cordY * 100,SCREEN_WIDTH / 8,SCREEN_WIDTH / 8 };
	int index;
	switch (draggedPiece) {
	case 'P':
		index = P;
		break;
	case 'R':
		index = R;
		break;
	case 'N':
		index = N;
		break;
	case 'B':
		index = B;
		break;
	case 'Q':
		index = Q;
		break;
	case 'K':
		index = K;
		break;
	case 'p':
		index = p;
		break;
	case 'r':
		index = r;
		break;
	case 'n':
		index = n;
		break;
	case 'b':
		index = b;
		break;
	case 'q':
		index = q;
		break;
	case 'k':
		index = k;
		break;
	default:
		index = -1;
		break;
	}
	if (index >= 0) {
		SDL_Texture* currentPiece = piecesTexture[index];
		SDL_SetTextureBlendMode(currentPiece, SDL_BLENDMODE_BLEND);
		SDL_SetTextureColorMod(currentPiece, 255, 255, 255, 128);
		SDL_RenderCopy(renderer, currentPiece, NULL, &square);
	}
}
void clearSquare(int cordX, int cordY) {
	SDL_Rect square = { cordX * 100,cordY * 100,SCREEN_WIDTH / 8,SCREEN_WIDTH / 8 };
	if ((cordY + cordX) % 2 == 0)  SDL_SetRenderDrawColor(renderer, 240, 217, 181, 255);//light squares
	else SDL_SetRenderDrawColor(renderer, 181, 136, 99, 255);
	SDL_RenderFillRect(renderer, &square);
}
bool rookMove(int yCordpre, int xCordpre, int yCordpost, int xCordpost,char draggedPiece) {
	int from, to;
	//horizontal
	if (yCordpre == yCordpost) {
		if (xCordpre < xCordpost) {
			from = xCordpre;
			to = xCordpost;
		}
		else {
			from = xCordpost;
			to = xCordpre;
		}
		for (int i = from + 1; i < to; i++) {
			if (board[yCordpre][i] != NULL) return false;
		}
	}
	//vertical
	else if (xCordpre == xCordpost) {
		if (yCordpre < yCordpost) {
			from = yCordpre;
			to = yCordpost;
		}
		else {
			from = yCordpost;
			to = yCordpre;
		}
		for (int i = from + 1; i < to; i++) {
			if (board[i][xCordpre] != NULL) return false;
		}
	}
	if ((xCordpre != xCordpost && yCordpre == yCordpost) || (yCordpre != yCordpost && xCordpre == xCordpost)) {
		if (board[yCordpost][xCordpost] == NULL) return true;
		else if ((draggedPiece == 'R' || draggedPiece == 'Q') && board[yCordpost][xCordpost] >= 97) {
			if (board[yCordpost][xCordpost] == 'k') return true;
			clearSquare(xCordpost, yCordpost);
			board[yCordpost][xCordpost] = draggedPiece;
		}
		else if ((draggedPiece == 'r' || draggedPiece == 'q') && board[yCordpost][xCordpost] < 97) {
			if (board[yCordpost][xCordpost] == 'K') return true;
			clearSquare(xCordpost, yCordpost);
			board[yCordpost][xCordpost] = draggedPiece;
		}
		else return false;
	}
	else return false;
}
bool bishopMove(int yCordpre, int xCordpre, int yCordpost, int xCordpost, char draggedPiece) {
	int fromY, toY, fromX, toX;
	if (abs(xCordpre - xCordpost) == abs(yCordpre - yCordpost)) {
		if (yCordpre < yCordpost) {
			fromY = yCordpre;
			toY = yCordpost;
		}
		else {
			fromY = yCordpost;
			toY = yCordpre;
		}

		if (xCordpre < xCordpost) {
			fromX = xCordpre;
			toX = xCordpost;
		}
		else {
			fromX = xCordpost;
			toX = xCordpre;
		}

		//main diagonal
		//checking if it is on main diagonal helps to show whether the move on the minor diagonal is legal
		if (((xCordpost > xCordpre) && (yCordpost > yCordpre)) || ((xCordpost < xCordpre) && (yCordpost < yCordpre))) {

			for (int i = fromY + 1, j = fromX + 1; i < toY; i++, j++) {
				//printf("main: I:%d, J:%d\n", i, j);
				if (board[i][j] != NULL) return false;
			}
		}
		//minor diagonal
		else {
			for (int i = fromY + 1, j = toX - 1; i < toY; i++, j--) {
				//printf("minor: I:%d, J:%d\n", i, j);
				if (board[i][j] != NULL) return false;
			}
		}

		if (board[yCordpost][xCordpost] == NULL) return true;
		else if ((draggedPiece == 'B'|| draggedPiece == 'Q') && board[yCordpost][xCordpost] >= 97) {
			if (board[yCordpost][xCordpost] == 'k') return true;
			clearSquare(xCordpost, yCordpost);
			board[yCordpost][xCordpost] = draggedPiece;
		}
		else if ((draggedPiece == 'b' || draggedPiece == 'q') && board[yCordpost][xCordpost] < 97) {
			if (board[yCordpost][xCordpost] == 'K') return true;
			clearSquare(xCordpost, yCordpost);
			board[yCordpost][xCordpost] = draggedPiece;
		}
		else return false;
	}
	else return false;
}
bool castle(char king, int type) {
	if (type == 0) {
		if (king=='K') {
			kingCord[0][0] = 5;
			check();
			if (isChecked[0]) {
				kingCord[0][0] = 4;
				return false;
			}
			kingCord[0][0] = 6;
			check();
			if (isChecked[0]) {
				kingCord[0][0] = 4;
				return false;
			}
			return true;
		}
		else {
			kingCord[1][0] = 5;
			check();
			if (isChecked[1]) {
				kingCord[1][0] = 4;
				return false;
			}
			kingCord[1][0] = 6;
			check();
			if (isChecked[1]) {
				kingCord[1][0] = 4;
				return false;
			}
			return true;
		}
	}
	else {
		if (king == 'K') {
			kingCord[0][0] = 3;
			check();
			if (isChecked[0]) {
				kingCord[0][0] = 4;
				return false;
			}
			kingCord[0][0] = 2;
			check();
			if (isChecked[0]) {
				kingCord[0][0] = 4;
				return false;
			}
			return true;
		}
		else {
			kingCord[1][0] = 3;
			check();
			if (isChecked[1]) {
				kingCord[1][0] = 4;
				return false;
			}
			kingCord[1][0] = 2;
			check();
			if (isChecked[0]) {
				kingCord[1][1] = 4;
				return false;
			}
			return true;
		}
	}
}
bool move(int yCordpre, int xCordpre, int yCordpost, int xCordpost, char draggedPiece) {

	if (draggedPiece >= 97 && color == 'W') return false;
	else if (draggedPiece < 97 && color == 'B') return false;
	else if (xCordpre == xCordpost && yCordpre == yCordpost) return false;
	else {
		if (draggedPiece == 'P') {
			//printf("en passant left: y:%d x:%d right y:%d x:%d\n", enpassant[0][0], enpassant[0][1], enpassant[1][0], enpassant[1][1]);
			if ((yCordpost + 1) == yCordpre && xCordpost == xCordpre) {
				if (board[yCordpost][xCordpost] == NULL)
				{
					isEnpassant = false;
					return true;
				}
				else return false;
			}
			else if (yCordpre == 6 && yCordpost == 4 && xCordpost == xCordpre) {//two squares move
				if (board[yCordpost][xCordpost] == NULL && board[yCordpre - 1][xCordpre] == NULL) {
					if (xCordpre == 0) {
						enpassant[1][0] = yCordpost;
						enpassant[1][1] = xCordpost + 1;
					}
					else if (xCordpre == 7) {
						enpassant[0][0] = yCordpost;
						enpassant[0][1] = xCordpost - 1;
					}
					else {
						enpassant[0][0] = yCordpost;
						enpassant[0][1] = xCordpost - 1;

						enpassant[1][0] = yCordpost;
						enpassant[1][1] = xCordpost + 1;
					}
					isEnpassant = true;
					return true;
				}
				else return false;
			}
			else if ((yCordpost + 1) == yCordpre && (xCordpost - 1) == xCordpre) {
				if (board[yCordpost][xCordpost] != NULL && board[yCordpost][xCordpost] >= 97) {
					clearSquare(xCordpost, yCordpost);
					board[yCordpost][xCordpost] = draggedPiece;
					isEnpassant = false;
					return true;
				}
				else if (board[yCordpost][xCordpost] == NULL && enpassant[0][0] == yCordpre && enpassant[0][1] == xCordpre && isEnpassant) {//en passant
					clearSquare(xCordpost, yCordpost + 1);
					enpassantPiece = board[yCordpost + 1][xCordpost];
					board[yCordpost + 1][xCordpost] = NULL;
					board[yCordpost][xCordpost] = draggedPiece;
					isEnpassant = false;
					return true;
				}
				else return false;
			}
			else if ((yCordpost + 1) == yCordpre && (xCordpost + 1) == xCordpre) {
				if (board[yCordpost][xCordpost] != NULL && board[yCordpost][xCordpost] >= 97) {
					clearSquare(xCordpost, yCordpost);
					board[yCordpost][xCordpost] = draggedPiece;
					isEnpassant = false;
					return true;
				}
				else if (board[yCordpost][xCordpost] == NULL && enpassant[1][0] == yCordpre && enpassant[1][1] == xCordpre && isEnpassant) {//en passant
					clearSquare(xCordpost, yCordpost + 1);
					enpassantPiece = board[yCordpost + 1][xCordpost];
					board[yCordpost + 1][xCordpost] = NULL;
					board[yCordpost][xCordpost] = draggedPiece;
					isEnpassant = false;
					return true;
				}
				else return false;
			}
			else return false;
		}
		//pawn
		else if (draggedPiece == 'p') {
			//printf("en passant left: y:%d x:%d right y:%d x:%d\n", enpassant[0][0], enpassant[0][1], enpassant[1][0], enpassant[1][1]);
			if ((yCordpost - 1) == yCordpre && xCordpost == xCordpre) {
				if (board[yCordpost][xCordpost] == NULL)
				{
					isEnpassant = false;
					return true;
				}
				else return false;
			}
			else if (yCordpre == 1 && yCordpost == 3 && xCordpost == xCordpre) {//two squares
				if (board[yCordpost][xCordpost] == NULL && board[yCordpre + 1][xCordpre] == NULL) {
					if (xCordpre == 0) {
						enpassant[1][0] = yCordpost;
						enpassant[1][1] = xCordpost + 1;
					}
					else if (xCordpre == 7) {
						enpassant[0][0] = yCordpost;
						enpassant[0][1] = xCordpost - 1;
					}
					else {
						enpassant[0][0] = yCordpost;
						enpassant[0][1] = xCordpost - 1;

						enpassant[1][0] = yCordpost;
						enpassant[1][1] = xCordpost + 1;
					}
					isEnpassant = true;
					return true;
				}
				else return false;
			}
			else if ((yCordpost - 1) == yCordpre && (xCordpost - 1) == xCordpre) {
				if (board[yCordpost][xCordpost] != NULL && board[yCordpost][xCordpost] < 97) {
					clearSquare(xCordpost, yCordpost);
					board[yCordpost][xCordpost] = draggedPiece;
					isEnpassant = false;
					return true;
				}
				else if (board[yCordpost][xCordpost] == NULL && enpassant[0][0] == yCordpre && enpassant[0][1] == xCordpre && isEnpassant) {//en passant
					clearSquare(xCordpost, yCordpost - 1);
					enpassantPiece = board[yCordpost - 1][xCordpost];
					board[yCordpost - 1][xCordpost] = NULL;
					board[yCordpost][xCordpost] = draggedPiece;
					isEnpassant = false;
					return true;
				}
				else return false;
			}
			else if ((yCordpost - 1) == yCordpre && (xCordpost + 1) == xCordpre) {
				if (board[yCordpost][xCordpost] != NULL && board[yCordpost][xCordpost] < 97) {
					clearSquare(xCordpost, yCordpost);
					board[yCordpost][xCordpost] = draggedPiece;
					isEnpassant = false;
					return true;
				}
				else if (board[yCordpost][xCordpost] == NULL && enpassant[1][0] == yCordpre && enpassant[1][1] == xCordpre && isEnpassant) {//en passant
					clearSquare(xCordpost, yCordpost - 1);
					enpassantPiece = board[yCordpost - 1][xCordpost];
					board[yCordpost - 1][xCordpost] = NULL;
					board[yCordpost][xCordpost] = draggedPiece;
					isEnpassant = false;
					return true;
				}
				else return false;
			}
			else return false;
		}
		//rook
		else if (draggedPiece == 'R' || draggedPiece == 'r') {
			if (rookMove(yCordpre, xCordpre, yCordpost, xCordpost, draggedPiece)) {
				if (draggedPiece == 'R') {
					if (xCordpre == 7 && yCordpre == 7) {
						canCastleKingSide[0] = false;
					}
					else if (xCordpre == 0 && yCordpre == 7) {
						canCastleQueenSide[0] = false;
					}
				}
				else {
					if (xCordpre == 7 && yCordpre == 0) {
						canCastleKingSide[1] = false;
					}
					else if (xCordpre == 0 && yCordpre == 0) {
						canCastleQueenSide[1] = false;
					}
				}
				return true;
			}
			else return false;
		}
		//knight
		else if ((draggedPiece == 'N' || draggedPiece == 'n')) {
			if (((yCordpre - 2) == yCordpost && (xCordpre - 1) == xCordpost) || ((yCordpre - 2) == yCordpost && (xCordpre + 1) == xCordpost) || ((yCordpre - 1) == yCordpost && (xCordpre - 2) == xCordpost) || ((yCordpre - 1) == yCordpost && (xCordpre + 2) == xCordpost) || ((yCordpre + 1) == yCordpost && (xCordpre - 2) == xCordpost) || ((yCordpre + 1) == yCordpost && (xCordpre + 2) == xCordpost) || ((yCordpre + 2) == yCordpost && (xCordpre - 1) == xCordpost) || ((yCordpre + 2) == yCordpost && (xCordpre + 1) == xCordpost)) {
				if (board[yCordpost][xCordpost] == NULL) return true;
				else if (draggedPiece == 'N' && board[yCordpost][xCordpost] >= 97) {
					clearSquare(xCordpost, yCordpost);
					board[yCordpost][xCordpost] = draggedPiece;
				}
				else if (draggedPiece == 'n' && board[yCordpost][xCordpost] < 97) {
					clearSquare(xCordpost, yCordpost);
					board[yCordpost][xCordpost] = draggedPiece;
				}
				else return false;
			}
			else return false;
		}
		//bishop
		else if (draggedPiece == 'B' || draggedPiece == 'b') {
			if (bishopMove(yCordpre, xCordpre, yCordpost, xCordpost, draggedPiece)) return true;
			else return false;
		}
		else if (draggedPiece == 'Q' || draggedPiece == 'q') {
			if (xCordpre == xCordpost || yCordpre == yCordpost) {
				if (rookMove(yCordpre, xCordpre, yCordpost, xCordpost, draggedPiece)) return true;
				else return false;
			}
			else {
				if (bishopMove(yCordpre, xCordpre, yCordpost, xCordpost, draggedPiece)) return true;
				else return false;
			}
			return false;
		}
		else if (draggedPiece == 'K' || draggedPiece == 'k') {
			if ((isChecked[0] == false && canCastleKingSide[0] && draggedPiece == 'K' && yCordpost == 7 && xCordpost == 6 && board[7][7] == 'R') || (isChecked[1] == false && canCastleKingSide[1] && draggedPiece == 'k' && yCordpost == 0 && xCordpost == 6 && board[0][7] == 'r')) {
				if (draggedPiece == 'K') {
					if (board[7][5] == NULL && board[7][6] == NULL) {
						if (castle('K', 0)) {
							canCastleKingSide[0] = false;
								board[7][7] = NULL;//rook
								clearSquare(7, 7);
								board[7][5] = 'R';
								setOnePiece(5, 7, 'R');
								return true;
						}
						else return false;
					}
					else return false;
				}
				else {
					if (board[0][5] == NULL && board[0][6] == NULL) {
						if (castle('k', 0)) {
							canCastleKingSide[1] = false;
							board[0][7] = NULL;//rook
							clearSquare(7, 0);
							board[0][5] = 'r';
							setOnePiece(5, 0, 'r');

							return true;
						}
						else return false;
					}
					else return false;
				}
			}
			else if ((isChecked[0] == false && canCastleQueenSide[0] && draggedPiece == 'K' && yCordpost == 7 && xCordpost == 2 && board[7][0] == 'R') || (isChecked[1] == false && canCastleQueenSide[1] && draggedPiece == 'k' && yCordpost == 0 && xCordpost == 2 && board[0][0] == 'r')) {
				if (draggedPiece == 'K') {
					if (board[7][1] == NULL && board[7][2] == NULL && board[7][3] == NULL) {
						if (castle('K', 1)) {
							canCastleQueenSide[0] = false;
							board[7][0] = NULL;//rook
							clearSquare(0, 7);
							board[7][3] = 'R';
							setOnePiece(3, 7, 'R');

							return true;
						}
						return false;
					}
					else return false;
				}
				else {
					if (board[0][1] == NULL && board[0][2] == NULL && board[0][3] == NULL) {
						if (castle('k', 1)) {
							canCastleQueenSide[1] = false;
							board[0][0] = NULL;//rook
							clearSquare(0, 0);
							board[0][3] = 'r';
							setOnePiece(3, 0, 'r');

							return true;
						}
						else return false;
					}
					else return false;
				}
			}
			else if ((yCordpost + 1 == yCordpre && xCordpost == xCordpre) || (yCordpost + 1 == yCordpre && xCordpost - 1 == xCordpre) || (yCordpost + 1 == yCordpre && xCordpost + 1 == xCordpre) || (yCordpost == yCordpre && xCordpost - 1 == xCordpre) || (yCordpost == yCordpre && xCordpost + 1 == xCordpre) || (yCordpost - 1 == yCordpre && xCordpost == xCordpre) || (yCordpost - 1 == yCordpre && xCordpost - 1 == xCordpre) || (yCordpost - 1 == yCordpre && xCordpost + 1 == xCordpre)) {
				if (board[yCordpost][xCordpost] == NULL) {
					if (color == 'W') {
						canCastleKingSide[0] = false;
						canCastleQueenSide[0] = false;
					}
					else {
						canCastleKingSide[1] = false;
						canCastleQueenSide[1] = false;
					}
					return true;
				}
				else if (draggedPiece == 'K' && board[yCordpost][xCordpost] >= 97) {
					clearSquare(xCordpost, yCordpost);
					board[yCordpost][xCordpost] = draggedPiece;
					canCastleKingSide[0] = false;
					canCastleQueenSide[0] = false;
					return true;
				}
				else if (draggedPiece == 'k' && board[yCordpost][xCordpost] < 97) {
					clearSquare(xCordpost, yCordpost);
					board[yCordpost][xCordpost] = draggedPiece;
					canCastleKingSide[1] = false;
					canCastleQueenSide[1] = false;
					return true;
				}
				else return false;
			}
			return false;
		}
		else return false;
	}
}
bool moveCheck(int yCordpre, int xCordpre, int yCordpost, int xCordpost, char draggedPiece,char king) {
	if ((draggedPiece == 'P' && king == 'k') || (draggedPiece == 'p' && king == 'K')) {
		if (((yCordpost + 1) == yCordpre && (xCordpost - 1) == xCordpre) || ((yCordpost + 1) == yCordpre && (xCordpost + 1) == xCordpre)) {
			if (draggedPiece == 'P') return true;
			else return false;
		}
		else if (((yCordpost - 1) == yCordpre && (xCordpost - 1) == xCordpre) || ((yCordpost - 1) == yCordpre && (xCordpost + 1) == xCordpre)) {
			if (draggedPiece == 'p') return true;
			else return false;
		}
	}
	else if ((draggedPiece == 'R' && king == 'k') || (draggedPiece == 'r' && king == 'K')) {
		if (rookMove(yCordpre, xCordpre, yCordpost, xCordpost, draggedPiece)) {

			return true;
		}
		else return false;
	}
	else if ((draggedPiece == 'B' && king == 'k') || (draggedPiece == 'b' && king == 'K')) {
		if (bishopMove(yCordpre, xCordpre, yCordpost, xCordpost, draggedPiece)) {

			return true;
		}
		else return false;
	}
	else if ((draggedPiece == 'N' && king == 'k') || (draggedPiece == 'n' && king == 'K')) {
		if (((yCordpre - 2) == yCordpost && (xCordpre - 1) == xCordpost) || ((yCordpre - 2) == yCordpost && (xCordpre + 1) == xCordpost) || ((yCordpre - 1) == yCordpost && (xCordpre - 2) == xCordpost) || ((yCordpre - 1) == yCordpost && (xCordpre + 2) == xCordpost) || ((yCordpre + 1) == yCordpost && (xCordpre - 2) == xCordpost) || ((yCordpre + 1) == yCordpost && (xCordpre + 2) == xCordpost) || ((yCordpre + 2) == yCordpost && (xCordpre - 1) == xCordpost) || ((yCordpre + 2) == yCordpost && (xCordpre + 1) == xCordpost)) {
			return true;
		}
		else return false;
	}
	else if ((draggedPiece == 'Q' && king == 'k') || (draggedPiece == 'q' && king == 'K')) {
		
		if (xCordpre == xCordpost || yCordpre == yCordpost) {
			
			if (rookMove(yCordpre, xCordpre, yCordpost, xCordpost, draggedPiece))
			{

				return true;
			}
			else return false;
		}
		else {
			if (bishopMove(yCordpre, xCordpre, yCordpost, xCordpost,draggedPiece)) {

				return true;
			}
			else return false;
		}
	}
	if ((draggedPiece == 'K' && king == 'k') || (draggedPiece == 'k' && king == 'K')) {
		if ((yCordpost + 1 == yCordpre && xCordpost == xCordpre) || (yCordpost + 1 == yCordpre && xCordpost - 1 == xCordpre) || (yCordpost + 1 == yCordpre && xCordpost + 1 == xCordpre) || (yCordpost == yCordpre && xCordpost - 1 == xCordpre) || (yCordpost == yCordpre && xCordpost + 1 == xCordpre) || (yCordpost - 1 == yCordpre && xCordpost == xCordpre) || (yCordpost - 1 == yCordpre && xCordpost - 1 == xCordpre) || (yCordpost - 1 == yCordpre && xCordpost + 1 == xCordpre)) {
			return true;
		}
		else return false;
	}

	else return false;
}
bool isCheck(int cordX,int cordY,char king) {
	//white
	for (int i = 0; i < 8; i++) {//y
		for (int j = 0; j < 8; j++) {//x
			if (king=='K'&&board[i][j] >= 97 && moveCheck(i, j, cordY, cordX, board[i][j], king)) {
				return true;
			}
			else if (king == 'k' && board[i][j] < 97 && moveCheck(i, j, cordY, cordX, board[i][j], king)) {
				return true;
			}
		}
	}
	return false;
}
void check() {
	if (isCheck(kingCord[0][0], kingCord[0][1], 'K')) {
		isChecked[0] = true;
		//printf("szach biale\n");
	}
	else isChecked[0] = false;
	if (isCheck(kingCord[1][0], kingCord[1][1], 'k')) {
		isChecked[1] = true;
		//printf("szach czarne\n");
	}
	else isChecked[1] = false;
}
void promotion() {
	isPromotion = true;
	//rendering of the green bar
	SDL_Rect bar = { 0,800,SCREEN_WIDTH,100 };
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	SDL_RenderFillRect(renderer, &bar);
	int size = SCREEN_WIDTH/4;
	SDL_Texture* pieces[4];
	if (color == 'W') {
		pieces[0] = piecesTexture[R];
		pieces[1] = piecesTexture[N];
		pieces[2] = piecesTexture[B];
		pieces[3] = piecesTexture[Q];
	}
	else {
		pieces[0] = piecesTexture[r];
		pieces[1] = piecesTexture[n];
		pieces[2] = piecesTexture[b];
		pieces[3] = piecesTexture[q];
	}
	for (int i = 0; i < 4; i++) {
		SDL_Rect rectangle = { i * size,800,size,100 };
		SDL_SetTextureBlendMode(pieces[i], SDL_BLENDMODE_BLEND);
		SDL_SetTextureColorMod(pieces[i], 255, 255, 255, 128);
		SDL_RenderCopy(renderer, pieces[i], NULL, &rectangle);
	}

}
void legalMoves() {
	numberOfLegalMoves = 0;
	//bool wasEnpassant = isEnpassant;
	for (int a = 0; a < 8; a++) {
		for (int b = 0; b < 8; b++) {
			for (int c = 0; c < 8; c++) {
				for (int d = 0; d < 8; d++) {
					if (board[a][b] == 'P' && b-1==d) continue;

					int oneKing = 0;//crucial for checking whether there is no legal move
					if (board[a][b] == 'K' || board[a][b] == 'k') {
						if (abs(a - c) > 1 || abs(b - d) > 1) {
							//printf("dziala??\n");
							continue;
						}
						else if (a == c && b == d) {
							continue;
						}
					}
					if ((board[a][b] == 'K' && (b + 2 == d || b - 2 == d)) || (board[a][b] == 'k' && (b + 2 == d || b - 2 == d))) continue;


					char startPiece = board[a][b];
					char destination = board[c][d];
					if (move(a, b, c, d, board[a][b])) {
						board[a][b] = NULL;
						board[c][d] = startPiece;
						setOnePiece(d, c, startPiece);


						//check();
						if (startPiece == 'K') {
							kingCord[0][0] = d;
							kingCord[0][1] = c;
						}
						else if (startPiece == 'k') {
							kingCord[1][0] = d;
							kingCord[1][1] = c;
						}

						check();
						
						if ((isChecked[0] == false && color == 'W') || (isChecked[1] == false && color == 'B')) {
							printf("rusz sie %c\t", startPiece);
							printf("na te kordynaty y:%d x:%d\n", c, d);
							if ((startPiece == 'K' || startPiece == 'k')) oneKing++;
							numberOfLegalMoves++;
						}

						board[a][b] = startPiece;
						//if (oneKing==1)numberOfLegalMoves--;

						board[c][d] = destination;
						if (startPiece == 'K') {
							kingCord[0][0] = b;
							kingCord[0][1] = a;
						}
						else if (startPiece == 'k') {
							kingCord[1][0] = b;
							kingCord[1][1] = a;
						}
						clearSquare(d, c);
						setOnePiece(d, c, destination);
					}
				}
			}
		}
	}
}
void mate() {
	if (color == 'W') {
		printf("GAME OVER, BLACK WON");
	}
	else {
		printf("GAME OVER, WHITE WON");
	}
	/*SDL_Rect bar = {0,0,SCREEN_WIDTH,SCREEN_WIDTH};
	SDL_Texture* currentPiece = texture;
	SDL_SetTextureBlendMode(currentPiece, SDL_BLENDMODE_BLEND);
	SDL_SetTextureColorMod(currentPiece, 255, 255, 255, 128);
	SDL_RenderCopy(renderer, currentPiece, NULL, &bar);*/
}
void stalemate() {
	printf("ITS A STALEMATE");
}
void adversary() {

}
void close() {
	SDL_DestroyTexture(texture);
	for (int i = 0; i < 12; i++) {
		SDL_DestroyTexture(piecesTexture[i]);
	}
	IMG_Quit();
	SDL_Quit();
}