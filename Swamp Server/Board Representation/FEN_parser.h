#include "Common.h"
#include "bitboards.h"

typedef enum
{
    PIECE_PLACEMENT,
    SIDE_TO_MOVE,
    CASTLING_ABILITY,
    ENPASSANT_TARGET,
    HALF_MOVE,
    FULL_MOVE,
    DONE
} FEN_STATE;


bool fenStringValidator(char *FEN_STRING)
{
    FEN_STATE _state = PIECE_PLACEMENT;

    for (int i = 0; i < MAX_UCI_QUERY_LENGTH && FEN_STRING[i] != '\0' && FEN_STRING[i] !='\n' ; i++)
    {
        if (_state == PIECE_PLACEMENT)
        {
            int _asciiArray[128] ={0} ;

            int _rankPieceCounter = 0;
            int _slashCounter = 0;

            for(int j =i ;j<MAX_UCI_QUERY_LENGTH && FEN_STRING[j] != '\0' && FEN_STRING[j] !='\n'; j++){

                if(FEN_STRING[j] == '/'){
                    if(_rankPieceCounter !=8){
                        return false;
                    }
                    _rankPieceCounter=0;
                    _slashCounter++;
                }
                else if(FEN_STRING[j] == ' '){
                    if(_rankPieceCounter !=8){
                        return false;
                    }
                    _state=SIDE_TO_MOVE;
                    i=j;
                    break;
                }   
                else{
                    switch (FEN_STRING[j]) {
                        case 'P': case 'N': case 'B': case 'R':
                        case 'Q': case 'K':
                        case 'p': case 'n': case 'b': case 'r':
                        case 'q': case 'k':
                        case '1' : case '2' : case '3' :
                        case '4' : case '5' : case '6' :
                        case '7' : case '8'  :
                            break;
                        default:
                            return false;
                }       
                        int _val = FEN_STRING[j];
                        _asciiArray[(_val)]+=1;
                        if(FEN_STRING[j] >='1' && FEN_STRING[j]<='8'){
                            _rankPieceCounter+=(FEN_STRING[j]-'0');
                        }else{
                            _rankPieceCounter+=1;
                        }
            }
        }
            int _blackPawn=_asciiArray['p'-0];
            int _whitePawn = _asciiArray['P'-0];
            if( _asciiArray['k'-0] != 1 || _asciiArray['K'-0] != 1 ||
                _asciiArray['p'-0] >8  || _asciiArray['P'-0] >8  ||
                _asciiArray['q'-0] >(9 - _blackPawn) || _asciiArray['Q'-0] > (9-_whitePawn)  ||
                _asciiArray['b'-0] >(10-_blackPawn)  || _asciiArray['B'-0] >(10- _whitePawn)  ||
                _asciiArray['r'-0] >(10-_blackPawn)  || _asciiArray['R'-0] >(10- _whitePawn) ||
                _asciiArray['n'-0]> (10-_blackPawn) || _asciiArray['N'-0] > (10- _whitePawn) ||
                _slashCounter !=7 ){

                return false;
            }
            continue;
    
    }
        else if(_state == SIDE_TO_MOVE){
            if(FEN_STRING[i+1] !=' '){
                return false;
            }
            if(FEN_STRING[i] == 'b' || FEN_STRING[i] == 'w' ){
                i++;
                _state = CASTLING_ABILITY;
                continue;
            }
            return false;
        }
        else if(_state == CASTLING_ABILITY){
            int _castlingCounter =0;
            
            char* _tempString ="KQkq";

            if(FEN_STRING[i]=='-'){
                i++;
                _state = ENPASSANT_TARGET;
                continue;
            }

            while(FEN_STRING[i]!=' '){
                if(_castlingCounter==4){
                    return false;
                }
                if(FEN_STRING[i] == _tempString[_castlingCounter] ){
                    i++;
                    _castlingCounter++;
                }
                else if(FEN_STRING[i] != _tempString[_castlingCounter] ){
                    _castlingCounter++;
                }
            }
            _state = ENPASSANT_TARGET;
            continue;
        }
        else if(_state ==  ENPASSANT_TARGET){
            if(FEN_STRING[i]=='-'){
                i++;
                _state = HALF_MOVE;
                continue;
            }

            if( FEN_STRING[i] ==' '|| FEN_STRING[i+1] == ' '){
                return false;
            }

            if(FEN_STRING[i] >= 'a' && FEN_STRING[i]<='h' && FEN_STRING[i+1]>='3' && FEN_STRING[i+1]<='6' && FEN_STRING[i+2] ==' ' ){
                i+=2;
                _state = HALF_MOVE;
                continue;
            }
        }
        else if(_state == HALF_MOVE){
            if(FEN_STRING[i]=='0'){
                i++;
                _state=FULL_MOVE;
                continue;
            }   
            while(FEN_STRING[i]!=' '){
                if(!(FEN_STRING[i]>='1' && FEN_STRING[i] <='9')){
                    return false;
                }
                i++;
            }   
            
            _state=FULL_MOVE;

        }   
        else if(_state == FULL_MOVE ){
            if(!(FEN_STRING[i]>='1' && FEN_STRING[i]<='9')){
                return false;
            }   

            while (FEN_STRING[i+1] >='0' && FEN_STRING[i+1]<='9' )
            {
                i++;
            }
            
            _state=DONE;
            continue;
        }



        return true;
    }
}

GameState initializeNewGameFromString(char *FEN_STRING)
{   
    _globalZorbistHashing= generateZorbistNumbers();
    
    GameState _gameState;
    _gameState._blackPawns = 0;
    _gameState._whitePawns = 0;
    _gameState._blackRooks = 0;
    _gameState._blackKnights = 0;
    _gameState._blackBishops = 0;
    _gameState._blackKing = 0;
    _gameState._blackQueens = 0;
    _gameState._whiteRooks = 0;
    _gameState._whiteKnights = 0;
    _gameState._whiteBishops = 0;
    _gameState._whiteQueens = 0;
    _gameState._whiteKing = 0;


    if(!fenStringValidator(FEN_STRING)){
        return _gameState;
    }
    int _rankCounter = 8;
    int _currPosition= 0;
    int _i=0;

    for(int i =_i; i < MAX_UCI_QUERY_LENGTH && FEN_STRING[i] != '\0' && FEN_STRING[i] != '\n' ; i++, _i++){
        switch (FEN_STRING[i])
        {
        case '/':
            _rankCounter--;
            _currPosition=0;
            break;
        case ' ':
            _i++;
            i=MAX_UCI_QUERY_LENGTH;
            break;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
            _currPosition+=(FEN_STRING[i] -'0');
            break;
        case 'k':
            SET_BIT_PIECE(_gameState._blackKing , _rankCounter , _currPosition);
            break;
        case 'K':
            SET_BIT_PIECE(_gameState._whiteKing , _rankCounter , _currPosition);
            break;
        case 'q':
            SET_BIT_PIECE(_gameState._blackQueens , _rankCounter , _currPosition);
            break;
        case 'Q':
            SET_BIT_PIECE(_gameState._whiteQueens , _rankCounter ,_currPosition);
            break;
        case 'p':
            SET_BIT_PIECE(_gameState._blackPawns , _rankCounter , _currPosition);
            break;
        case 'P':
            SET_BIT_PIECE(_gameState._whitePawns , _rankCounter , _currPosition);
            break;
        case 'b':
            SET_BIT_PIECE(_gameState._blackBishops, _rankCounter , _currPosition);
            break;
        case 'B':
            SET_BIT_PIECE(_gameState._whiteBishops, _rankCounter , _currPosition);
            break;        
        case 'n':
            SET_BIT_PIECE(_gameState._blackKnights, _rankCounter , _currPosition);
            break;
        case 'N':
            SET_BIT_PIECE(_gameState._whiteKnights, _rankCounter , _currPosition);
            break;
        case 'r':
            SET_BIT_PIECE(_gameState._blackRooks, _rankCounter , _currPosition);
            break;
        case 'R':
            SET_BIT_PIECE(_gameState._whiteRooks, _rankCounter , _currPosition);
            break;
        }
    }
    
    {
        #define _newState _gameState

        _newState._occupancy =
            _newState._blackPawns ^ _newState._blackRooks ^
            _newState._blackKnights ^ _newState._blackBishops ^ _newState._blackQueens ^
            _newState._blackKing ^ _newState._whitePawns ^
            _newState._whiteRooks ^ _newState._whiteKnights ^
            _newState._whiteQueens ^ _newState._whiteKing ^
            _newState._whiteBishops;

        _newState._blackOccupancy =
            _newState._blackPawns ^ _newState._blackRooks ^
            _newState._blackKnights ^ _newState._blackBishops ^ _newState._blackQueens ^
            _newState._blackKing;

        _newState._whiteOccupancy = _newState._whitePawns ^
                                    _newState._whiteRooks ^ _newState._whiteKnights ^
                                    _newState._whiteQueens ^ _newState._whiteKing ^
                                    _newState._whiteBishops; 



        _newState._prevStates = (__uint64_t *)malloc(sizeof(__uint64_t) * 100);
        _newState._stateIndex = 0;
        _newState._totalState = 100;
        _newState._castlingAvailable = (char*)malloc(sizeof(char) * 4 );

        #undef _newState 
    }

    _gameState._pieceToMove =FEN_STRING[_i];
    _i++;
    _i++;

    char string1[4]="KQkq";
    int i =0;



    while(i<4){
        if(string1[i]== FEN_STRING[_i]){
            _gameState._castlingAvailable[i]=FEN_STRING[_i];
            i++;
            _i++;
        }else{
            _gameState._castlingAvailable[i]=' ';
            i++;
            _i++;
        }
    }
    _i++;

    _gameState._enpassantFile= FEN_STRING[_i];
    _i++;
    
    _gameState._zobristHash = generateZorbistHashFromAGameState(_gameState);
    
    _gameState._numberHalfMoves=0;

    while(FEN_STRING[_i] >= '0' && FEN_STRING[_i] <=9){
        _gameState._numberHalfMoves=0 +(FEN_STRING[_i++]-'0');
    }

    _i++;

    _gameState._numberMoves=0;

    while(FEN_STRING[_i] >= '0' && FEN_STRING[_i] <=9){
        _gameState._numberMoves=0 +(FEN_STRING[_i++]-'0');
    }


    return _gameState;
}

