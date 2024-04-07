#include "hw4.h"

void initialize_game(ChessGame *game) {
    game->capturedCount = 0;
    game->moveCount = 0;
    game->currentPlayer = 0;

    for(int r = 0; r < 8; r++){
        for (int c = 0; c < 8; c++){
            if(r == 0){
                switch(c){
                    case 0: 
                        game->chessboard[r][c] = 'r';
                        continue;
                    case 1: 
                        game->chessboard[r][c] = 'n';
                        continue;
                    case 2: 
                        game->chessboard[r][c] = 'b';
                        continue;
                    case 3: 
                        game->chessboard[r][c] = 'q';
                        continue;
                    case 4: 
                        game->chessboard[r][c] = 'k';
                        continue;
                    case 5: 
                        game->chessboard[r][c] = 'b';
                        continue;
                    case 6: 
                        game->chessboard[r][c] = 'n';
                        continue;
                    case 7: 
                        game->chessboard[r][c] = 'r';
                        continue;      
                }
            }else if(r == 1){
                game->chessboard[r][c] = 'p';
            }else if(r == 6){
                game->chessboard[r][c] = 'P';
            }else if(r == 7){
                switch(c){
                    case 0: 
                        game->chessboard[r][c] = 'R';
                        continue;
                    case 1: 
                        game->chessboard[r][c] = 'N';
                        continue;
                    case 2: 
                        game->chessboard[r][c] = 'B';
                        continue;
                    case 3: 
                        game->chessboard[r][c] = 'Q';
                        continue;
                    case 4: 
                        game->chessboard[r][c] = 'K';
                        continue;
                    case 5: 
                        game->chessboard[r][c] = 'B';
                        continue;
                    case 6: 
                        game->chessboard[r][c] = 'N';
                        continue;
                    case 7: 
                        game->chessboard[r][c] = 'R';
                        continue;      
                }
            }else{
                game->chessboard[r][c] = '.';
            }
        }
    }
    display_chessboard(game);
}

void chessboard_to_fen(char fen[], ChessGame *game) {
    int fen_pos = 0;
    char empty_spaces = '0';
    for (int r = 0; r < 8; r++){
        for (int c = 0; c < 8; c++){
            if(game->chessboard[r][c] == '.'){
                empty_spaces++;
            }else{
                if(empty_spaces != '0'){
                    fen[fen_pos] = empty_spaces;
                    fen_pos++;
                    empty_spaces = '0';
                }
                fen[fen_pos] = game->chessboard[r][c];
                fen_pos++;
            }
        }
        if (empty_spaces != '0'){
            fen[fen_pos] = empty_spaces;
            fen_pos++;
            empty_spaces = '0';
        }
        if(r!=7){
            fen[fen_pos] = '/';
            fen_pos++;
        }
    }
    fen[fen_pos] = ' ';
    fen[fen_pos + 1] = (game->currentPlayer)? 'b' : 'w';
    fen[fen_pos + 2] = '\0';
}

bool isWhitePiece(char piece){
    if (piece == 'R' || piece == 'N' || piece == 'B' || piece == 'Q' || piece == 'K' || piece == 'P'){
        return true;
    }
    return false;
}

bool is_valid_pawn_move(char piece, int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    if (piece == 'P'){
        if(dest_row + 1 == src_row && src_col == dest_col){
            if(game->chessboard[dest_row][dest_col] == '.'){
                return true;
            }
        }else if(src_row == 6 && dest_row + 2 == src_row && src_col == dest_col){
            if(game->chessboard[dest_row][dest_col] == '.' && game->chessboard[dest_row+1][dest_col] == '.'){
                return true;
            }
        }else if(dest_row + 1 == src_row && dest_col + 1 == src_col){
            if(game->chessboard[dest_row][dest_col] != '.' && !isWhitePiece(game->chessboard[dest_row][dest_col])){
                return true;
            }
        }else if (dest_row + 1 == src_row && dest_col - 1 == src_col){
            if(game->chessboard[dest_row][dest_col] != '.' && !isWhitePiece(game->chessboard[dest_row][dest_col])){
                return true;
            }
        }
    }else if (piece == 'p'){
        if(dest_row - 1 == src_row && src_col == dest_col){
            if(game->chessboard[dest_row][dest_col] == '.'){
                return true;
            }
        }else if(src_row == 1 && dest_row - 2 == src_row && src_col == dest_col){
            if(game->chessboard[dest_row][dest_col] == '.' && game->chessboard[dest_row-1][dest_col] == '.'){
                return true;
            }
        }else if(dest_row - 1 == src_row && dest_col + 1 == src_col){
            if(game->chessboard[dest_row][dest_col] != '.' && isWhitePiece(game->chessboard[dest_row][dest_col])){
                return true;
            }
        }else if (dest_row - 1 == src_row && dest_col - 1 == src_col){
            if(game->chessboard[dest_row][dest_col] != '.' && isWhitePiece(game->chessboard[dest_row][dest_col])){
                return true;
            }
        }
    }
    return false;
}

bool is_valid_rook_move(int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    if(src_row != dest_row && src_col != dest_col){
        return false;
    }
    if(src_row == dest_row){
        if(dest_col < src_col){
            for(int c = dest_col+1; c < src_col; c++){
                if(game->chessboard[src_row][c] != '.'){
                    return false;
                }
            }
        }else{
            for(int c = src_col+1; c < dest_col; c++){
                if(game->chessboard[src_row][c] != '.'){
                    return false;
                }
            }
        }
    }else if(src_col == dest_col){
        if(dest_row < src_row){
            for(int r = dest_row+1; r < src_row; r++){
                if(game->chessboard[r][src_col] != '.'){
                    return false;
                }
            }
        }else{
            for(int r = src_row+1; r < dest_row; r++){
                if(game->chessboard[r][src_col] != '.'){
                    return false;
                }
            }
        }
    }
    return true;
}

bool is_valid_knight_move(int src_row, int src_col, int dest_row, int dest_col) {
    if(src_row + 1 == dest_row || src_row - 1 == dest_row){
        if(src_col + 2 == dest_col || src_col - 2 == dest_col){
            return true;
        }
    }else if(src_col + 1 == dest_col || src_col - 1 == dest_col){
        if(src_row + 2 == dest_row || src_row - 2 == dest_row){
            return true;
        }
    }
    return false;
}

bool is_valid_bishop_move(int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    int row_diff = dest_row - src_row, col_diff = dest_col - src_col;
    if (row_diff < 0){
        row_diff *= -1;
    }
    if (col_diff < 0){
        col_diff *= -1;
    }
    //printf("%d %d\n", row_diff, col_diff);
    if(row_diff == col_diff){
        if(dest_row < src_row && dest_col < src_col){
            for (int r = dest_row + 1, c = dest_col + 1; r < src_row; r++,c++){
                if(game->chessboard[r][c] != '.'){
                    return false;
                }
            }
        }else if(dest_row < src_row && dest_col > src_col){
            //printf("rc: %d %d rc: %d %d\n",src_row,src_col,dest_row,dest_col );
            for (int r = dest_row + 1, c = dest_col - 1; r < src_row; r++,c--){
                if(game->chessboard[r][c] != '.'){
                    return false;
                }
            }
                        printf("rc: %d %d rc: %d %d\n",src_row,src_col,dest_row,dest_col );

        }else if(dest_row > src_row && dest_col < src_col){
            for (int r = dest_row - 1, c = dest_col + 1; r > src_row; r--,c++){
                if(game->chessboard[r][c] != '.'){
                    return false;
                }
            }
        }else if(dest_row > src_row && dest_col > src_col){
            for (int r = dest_row - 1, c = dest_col - 1; r > src_row; r--,c--){
                if(game->chessboard[r][c] != '.'){
                    return false;
                }
            }
        }
    }else{
        return false;
    }
    return true;
}

bool is_valid_queen_move(int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    return is_valid_rook_move(src_row, src_col, dest_row, dest_col, game) || is_valid_bishop_move(src_row, src_col, dest_row, dest_col, game);
}

bool is_valid_king_move(int src_row, int src_col, int dest_row, int dest_col) {
    if((dest_row == src_row + 1 || dest_row == src_row - 1) && (dest_col == src_col || dest_col == src_col+1 || dest_col == src_col-1))
        return true;
    else if(dest_row == src_row && (dest_col == src_col+1 || dest_col == src_col-1)){
        return true;
    }
    return false;
}

bool is_valid_move(char piece, int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    if(game->chessboard[src_row][src_col] == '.'){
        return false;
    }
    switch (piece){
    case 'p':
        return is_valid_pawn_move(piece, src_row, src_col, dest_row, dest_col, game);
        break;
    case 'P':
        return is_valid_pawn_move(piece, src_row, src_col, dest_row, dest_col, game);
        break;
    case 'r':
        return is_valid_rook_move(src_row, src_col, dest_row, dest_col, game);
        break;
    case 'R':
        return is_valid_rook_move(src_row, src_col, dest_row, dest_col, game);
        break;
    case 'n':
        return is_valid_knight_move(src_row, src_col, dest_row, dest_col);
        break;
    case 'N':
        return is_valid_knight_move(src_row, src_col, dest_row, dest_col);
        break;
    case 'b':
        return is_valid_bishop_move(src_row, src_col, dest_row, dest_col, game);
        break;
    case 'B':
        return is_valid_bishop_move(src_row, src_col, dest_row, dest_col, game);
        break;
    case 'q':
        return is_valid_queen_move(src_row, src_col, dest_row, dest_col, game);
        break;
    case 'Q':
        return is_valid_queen_move(src_row, src_col, dest_row, dest_col, game);
        break;
    case 'k':
        return is_valid_king_move(src_row, src_col, dest_row, dest_col);
        break;
    case 'K':
        return is_valid_king_move(src_row, src_col, dest_row, dest_col);
        break;
    default:
        break;
    }
    
    return false;
}

void fen_to_chessboard(const char *fen, ChessGame *game) {
    const char *fen_pos = fen;
    char empty_spaces = '0';
    for(int r = 0; r < 8; r++){
        for (int c = 0; c < 8; c++){
            if(empty_spaces == '0'){
                if(*fen_pos >= '1' && *fen_pos <= '9'){
                    game->chessboard[r][c] = '.';
                    empty_spaces = *fen_pos - 1;
                    fen_pos++;
                }else{
                    game->chessboard[r][c] = *fen_pos;
                    fen_pos++;
                }
                continue;
            }
            game->chessboard[r][c] = '.';
            empty_spaces -= 1;
        }
        fen_pos++;
    }
    if(*fen_pos == 'w'){
        game->currentPlayer = 0;
    }else{
        game->currentPlayer = 1;
    }
}

int parse_move(const char *move, ChessMove *parsed_move) {
    const char *move_pos = move;
    char startSqaure[3], endSquare[4];
    int move_length = 0;
    bool invalid_col = false;
    while (*move_pos != '\0'){
        if((move_length == 0 || move_length == 2)){
            if((*move_pos < 'a' || *move_pos > 'h')){
                return PARSE_MOVE_INVALID_FORMAT;
            }
            else{
                if (move_length == 0){
                    startSqaure[0] = *move_pos;
                }else{
                    endSquare[0] = *move_pos;
                }
            }
        }
        if((move_length == 1 || move_length == 3)){
            if((*move_pos < '1' || *move_pos > '8')){
                invalid_col = true;
            }
            else{
                if (move_length == 1){
                    startSqaure[1] = *move_pos;
                }else{
                    endSquare[1] = *move_pos;
                }
            }
        }
        if (move_length == 4){
            endSquare[2] = *move_pos;
        }
        move_pos++;
        move_length++;    
    }
    if (move_length < 4 || move_length > 5)
        return PARSE_MOVE_INVALID_FORMAT;
    if (invalid_col)
        return PARSE_MOVE_OUT_OF_BOUNDS;
    if (move_length == 5){
        if (!(startSqaure[1] == '7' && endSquare[1] == '8') && !(startSqaure[1] == '2' && endSquare[1] == '1'))
            return PARSE_MOVE_INVALID_DESTINATION;
        if(endSquare[2]!= 'q' && endSquare[2] != 'r' && endSquare[2] != 'b' && endSquare[2] != 'n')
            return PARSE_MOVE_INVALID_PROMOTION;
    }
    parsed_move->startSquare[0] = startSqaure[0];
    parsed_move->startSquare[1] = startSqaure[1];
    parsed_move->startSquare[2] = '\0';
    parsed_move->endSquare[0] = endSquare[0];
    parsed_move->endSquare[1] = endSquare[1];
    if(move_length == 5){
        parsed_move->endSquare[2] = endSquare[2];
    }else{
        parsed_move->endSquare[2] = '\0';
    }
    parsed_move->endSquare[3] = '\0';

    return 0;
}
int get_pos_col(char *pos){
    switch (*pos){
    case 'a':
        return 0;
    case 'b':
        return 1;
    case 'c':
        return 2;
    case 'd':
        return 3;
    case 'e':
        return 4;
    case 'f':
        return 5;
    case 'g':
        return 6;
    case 'h':
        return 7;
    default:
        return -1;
    }
}

int get_pos_row(char *pos){
    switch (*(pos+1)){
    case '1':
        return 7;
    case '2':
        return 6;
    case '3':
        return 5;
    case '4':
        return 4;
    case '5':
        return 3;
    case '6':
        return 2;
    case '7':
        return 1;
    case '8':
        return 0;
    default:
        return -1;
    }
}
int make_move(ChessGame *game, ChessMove *move, bool is_client, bool validate_move) {
    int src_row = get_pos_row(move->startSquare);
    int src_col = get_pos_col(move->startSquare);
    int dest_row= get_pos_row(move->endSquare);
    int dest_col= get_pos_col(move->endSquare);
    if(validate_move){
        if (game->currentPlayer == 1 && is_client) 
            return MOVE_OUT_OF_TURN;
        if (game->currentPlayer == 0 && !is_client) 
            return MOVE_OUT_OF_TURN;
        if(game->chessboard[src_row][src_col] == '.')
            return MOVE_NOTHING;
        if(is_client){
            display_chessboard(game);
            if(!(isWhitePiece(game->chessboard[src_row][src_col]))){
                return MOVE_WRONG_COLOR;
            }
            if(game->chessboard[dest_row][dest_col] != '.' && (isWhitePiece(game->chessboard[dest_row][dest_col])))
                return MOVE_SUS;
            if(move->endSquare[3]!= '\0' && game->chessboard[src_row][src_col] != 'P')
                return MOVE_NOT_A_PAWN;
            if(game->chessboard[src_row][src_col] == 'P' && dest_row == 0 && move->endSquare[2] == '\0')
                return MOVE_MISSING_PROMOTION;
        }else{
            if((isWhitePiece(game->chessboard[src_row][src_col])))
                return MOVE_WRONG_COLOR;
            if(game->chessboard[dest_row][dest_col] != '.' && !(isWhitePiece(game->chessboard[dest_row][dest_col])))
                return MOVE_SUS;
            if(move->endSquare[3]!= '\0' && game->chessboard[src_row][src_col] != 'p')
                return MOVE_NOT_A_PAWN;
            if(game->chessboard[src_row][src_col] == 'p' && dest_row == 7 && move->endSquare[2] == '\0')
                return MOVE_MISSING_PROMOTION;
        }
        if(!is_valid_move(game->chessboard[src_row][src_col], src_row, src_col, dest_row, dest_col, game))
            return MOVE_WRONG;
    }
    if (game->chessboard[src_row][src_col] == 'P' && dest_row == 0){
        game->chessboard[src_row][src_col] = toupper(move->endSquare[2]);
    }
    if (game->chessboard[src_row][src_col] == 'p' && dest_row == 7){
        game->chessboard[src_row][src_col] = move->endSquare[2];
    }
    if(game->chessboard[dest_row][dest_col] != '.'){
        game->capturedPieces[game->capturedCount] = game->chessboard[dest_row][dest_col];
        game->capturedCount++;
    }
    game->chessboard[dest_row][dest_col] = game->chessboard[src_row][src_col];
    game->chessboard[src_row][src_col] = '.';
    game->moves[game->moveCount] = *move;
    game->moveCount++;
    if(game->currentPlayer == 1){
        game->currentPlayer = 0;
    }else{
        game->currentPlayer = 1;
    }
    printf("%d\n", game->currentPlayer);
    return 0;
}

int send_command(ChessGame *game, const char *message, int socketfd, bool is_client) {
    (void)game;
    (void)message;
    (void)socketfd;
    (void)is_client;
    return -999;
}

int receive_command(ChessGame *game, const char *message, int socketfd, bool is_client) {
    (void)game;
    (void)message;
    (void)socketfd;
    (void)is_client;
    return -999;
}

int save_game(ChessGame *game, const char *username, const char *db_filename) {
    (void)game;
    (void)username;
    (void)db_filename;
    return -999;
}

int load_game(ChessGame *game, const char *username, const char *db_filename, int save_number) {
    (void)game;
    (void)username;
    (void)db_filename;
    (void)save_number;
    return -999;
}

void display_chessboard(ChessGame *game) {
    printf("\nChessboard:\n");
    printf("  a b c d e f g h\n");
    for (int i = 0; i < 8; i++) {
        printf("%d ", 8 - i);
        for (int j = 0; j < 8; j++) {
            printf("%c ", game->chessboard[i][j]);
        }
        printf("%d\n", 8 - i);
    }
    printf("  a b c d e f g h\n");
}
