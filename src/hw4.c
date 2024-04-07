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
    (void)fen;
    (void)game;
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
            for (int r = dest_row - 1, c = dest_col + 1; r < src_row; r--,c++){
                if(game->chessboard[r][c] != '.'){
                    return false;
                }
            }
        }else if(dest_row > src_row && dest_col > src_col){
            for (int r = dest_row - 1, c = dest_col - 1; r < src_row; r--,c--){
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
    (void)src_row;
    (void)src_col;
    (void)dest_row;
    (void)dest_col;
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
    default:
        break;
    }
    
    (void)piece;
    (void)src_row;
    (void)src_col;
    (void)dest_row;
    (void)dest_col;
    (void)game;
    return false;
}

void fen_to_chessboard(const char *fen, ChessGame *game) {
    (void)fen;
    (void)game;
}

int parse_move(const char *move, ChessMove *parsed_move) {
    (void)move;
    (void)parsed_move;
    return -999;
}

int make_move(ChessGame *game, ChessMove *move, bool is_client, bool validate_move) {
    (void)game;
    (void)move;
    (void)is_client;
    (void)validate_move;
    return -999;
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
