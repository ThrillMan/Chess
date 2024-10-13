#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string.h>
#include "functions.h"


char draggedPiece = NULL;
bool isDragging = false; // A flag to indicate if the user is currently dragging the object
bool isChecked[] = { false,false };
int kingCord[][2] = { {4,7},{4,0} };
int main(int argc, char* args[])
{
	init();
	loadPieces();
	
	bool quit = false;
	int xCordpre=0;
	int yCordpre=0;
	int xCordpost=0;
	int yCordpost=0;
	int promotionCordX=0, promotionCordY=0;
	
	SDL_Event e;
	loadBoard();
	defaultPieces();
	setPieces();
	while (!quit)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN) {
				//legalMoves();
				//printf("dol\n");
				xCordpre = e.motion.x / 100;
				yCordpre = e.motion.y / 100;
				if ((e.motion.y <= SCREEN_HEIGHT - 100)&&!isPromotion) {
					
					isDragging = true;
					draggedPiece = board[yCordpre][xCordpre];
					board[yCordpre][xCordpre] = NULL;
					clearSquare(xCordpre, yCordpre);
					check();
				}
				else if (isPromotion) {
					printf("xCordpromotion: %d y Cordpromotion %d\n",promotionCordX, promotionCordY);
					char choosenPiece = NULL;
					int size = SCREEN_WIDTH / 4;
					if (color == 'W') {
						if (e.motion.x < size) {
							choosenPiece = 'R';
						}
						else if (e.motion.x < 2 * size) {
							choosenPiece = 'N';
						}
						else if (e.motion.x < 3 * size) {
							choosenPiece = 'B';
						}
						else if (e.motion.x < 4 * size) {
							choosenPiece = 'Q';
						}
					}
					else {
						if (e.motion.x < size) {
							choosenPiece = 'r';
						}
						else if (e.motion.x < 2 * size) {
							choosenPiece = 'n';
						}
						else if (e.motion.x < 3 * size) {
							choosenPiece = 'b';
						}
						else if (e.motion.x < 4 * size) {
							choosenPiece = 'q';
						}
					}
					board[promotionCordY][promotionCordX] = choosenPiece;
					//printf("choosen piece %c\n", choosenPiece);
					//isPromotion = false;
				}
			}
			else if (e.type == SDL_MOUSEMOTION) {
				if (isDragging) {
					//printf("x:%d,y:%d\n", e.motion.x, e.motion.y);
					//printf("Dragged piece:%c\n", draggedPiece);
				}
			}
			else if (e.type == SDL_MOUSEBUTTONUP) {
				//legalMoves();
				isDragging = false;
				xCordpost = e.motion.x / 100;
				yCordpost = e.motion.y / 100;
				char attackedPiece = board[yCordpost][xCordpost];
				
				
				//numberOfLegalMoves--;
				check();
				
				bool wasEnpassantMove = isEnpassant;
				bool couldCastleKingSideMove[] = { canCastleKingSide[0],canCastleKingSide[1] };
				bool couldCastleQueenSideMove[] = { canCastleQueenSide[0],canCastleQueenSide[1] };
				if (yCordpost < 8 && move(yCordpre, xCordpre, yCordpost, xCordpost, draggedPiece) && !isPromotion) {
					board[yCordpost][xCordpost] = draggedPiece;
					setOnePiece(xCordpost, yCordpost, draggedPiece);
					//updating current cords of king
					if (draggedPiece == 'K') {
						kingCord[0][0] = xCordpost;
						kingCord[0][1] = yCordpost;
					}
					else if (draggedPiece == 'k') {
						kingCord[1][0] = xCordpost;
						kingCord[1][1] = yCordpost;
					}
					check();
					
					if ((isChecked[0] == true && color == 'W') || (isChecked[1] == true && color == 'B')) {
						//printf("szach w metodzie\n");
						board[yCordpre][xCordpre] = draggedPiece;
						setOnePiece(xCordpre, yCordpre, draggedPiece);

						board[yCordpost][xCordpost] = attackedPiece;
						clearSquare(xCordpost, yCordpost);
						setOnePiece(xCordpost, yCordpost, attackedPiece);
						//updating current cords of king
						if (draggedPiece == 'K') {
							kingCord[0][0] = xCordpre;
							kingCord[0][1] = yCordpre;
						}
						else if (draggedPiece == 'k') {
							kingCord[1][0] = xCordpre;
							kingCord[1][1] = yCordpre;
						}
						canCastleKingSide[0] = couldCastleKingSideMove[0];
						canCastleQueenSide[0] = couldCastleQueenSideMove[0];
						canCastleKingSide[1] = couldCastleKingSideMove[1];
						canCastleQueenSide[1] = couldCastleQueenSideMove[1];
						if (wasEnpassantMove&&!isEnpassant) {
							board[yCordpre][xCordpost] = enpassantPiece;
							setOnePiece(xCordpost, yCordpre, enpassantPiece);
						}
					}
					else {
						if ((draggedPiece == 'P' && yCordpost == 0) || (draggedPiece == 'p' && yCordpost == 7)) { //promotion of pawn
							promotion();
							promotionCordX = xCordpost;
							promotionCordY = yCordpost;
							board[yCordpost][xCordpost] = NULL;
							clearSquare(xCordpost, yCordpost);
						}
						else turn();
					}
				}
				else if (isPromotion) {
					//clearSquare(promotionCordX, promotionCordY);
					draggedPiece = board[promotionCordY][promotionCordX];
					setOnePiece(promotionCordX, promotionCordY, draggedPiece);
					isPromotion = false;
					turn();
				}
				else if (!isPromotion) {
					//printf("wykonalo sie acha\n");
					board[yCordpre][xCordpre] = draggedPiece;
					setOnePiece(xCordpre, yCordpre, draggedPiece);
				}
				bool couldCastleKingSide[] = { canCastleKingSide[0],canCastleKingSide[1] };
				bool couldCastleQueenSide[] = { canCastleQueenSide[0],canCastleQueenSide[1] };
				bool wasEnpassantCheck = isEnpassant;
				int previousEnpassant[2][2] = { {enpassant[0][0],enpassant[0][1]} ,{enpassant[1][0],enpassant[1][1]} };


				legalMoves();
				check();
				canCastleKingSide[0] = couldCastleKingSide[0];
				canCastleQueenSide[0] = couldCastleQueenSide[0];
				canCastleKingSide[1] = couldCastleKingSide[1];
				canCastleQueenSide[1] = couldCastleQueenSide[1];
				isEnpassant = wasEnpassantCheck;
				enpassant[0][0] = previousEnpassant[0][0];
				enpassant[0][1] = previousEnpassant[0][1];
				enpassant[1][0] = previousEnpassant[1][0];
				enpassant[1][1] = previousEnpassant[1][1];

				printf("number of legal moves: %d\n", numberOfLegalMoves);
				if (!numberOfLegalMoves) {
					if (isChecked[0] || isChecked[1]) mate();
					else stalemate();
				}
			}
			else if (e.type == SDL_KEYDOWN)
			{
				switch (e.key.keysym.sym)
				{
				case SDLK_UP:
					turn();
					break;
				case SDLK_DOWN:
					loadBoard();
					defaultPieces();
					setPieces();
					legalMoves();
					canCastleKingSide[0] = true;
					canCastleQueenSide[0] = true;
					canCastleKingSide[1] = true;
					canCastleQueenSide[1] = true;
					int kingCord[][2] = { {4,7},{4,0} };
					kingCord[0][0] = 4;
					kingCord[0][1] = 7;
					kingCord[1][0] = 4;
					kingCord[1][1] = 0;
					isChecked[0] = false;
					isChecked[1] = false;
					if (color == 'B') turn();
					break;
				case SDLK_LEFT:
					loadBoard();
					defaultPieces();
					//setPieces();
					break;

				}

			}
		}
		SDL_RenderPresent(renderer);
		
	}



	close();
	return 0;
}