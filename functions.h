#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string.h>
//bool
typedef int bool;
enum { false, true };

//enum fen notation
enum pieces{P,R,N,B,Q,K,p,r,n,b,q,k};
//current turn
extern char color;

//board with pieces according to fen notation
extern char board[8][8];

extern char draggedPiece;
extern bool isDragging; 
extern bool isPromotion;
extern int promotionCordX, promotionCordY;
extern bool canCastleKingSide[];//0 white 1 black
extern bool canCastleQueenSide[];//0 white 1 black
extern int enpassant[][2];//cords of pawns which can enpassant
extern int enpassantCounter;
extern bool isEnpassant;
extern char enpassantPiece;
extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern SDL_Texture* texture;
extern SDL_Texture* piecesTexture[];
//Screen dimension constants
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern bool isChecked[];//0 white 1 black
extern int kingCord[][2];
extern int numberOfLegalMoves;
void init();
//Default placement of pieces
void defaultPieces();
//Loads textures of the pieces
void loadPieces();
//Renders chessboard
void loadBoard();
//Keeps track of whose turn it is
void turn();
//Place pieces on the chessboard
void setPieces();
//Place one piece
void setOnePiece(int,int,char);
//clear square before placing new piece there
void clearSquare(int , int);
//using rookMove will help in checking legality of queen move
bool rookMove(int, int, int, int,char);
//using bishopMove will help in checking legality of queen move
bool bishopMove(int, int, int, int,char);
//king, type of castle: 0 king 1 queen
bool castle(char,int);
//Checks legality of a move and clears the square which the piece have left
bool move(int,int,int,int,char);
//Checks whether certain pieces gives check
bool moveCheck(int, int, int, int, char);
//white king cord black king cord
bool isCheck(int, int, char);
//faster way of checking check
void check();
//promotion of a pawn
void promotion();
void legalMoves();
void mate();
void stalemate();
//generates moves for black
void adversary();
void close();