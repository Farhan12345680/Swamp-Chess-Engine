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
                if(!(FEN_STRING[i]>='1' || FEN_STRING[i] <='9')){
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
}
