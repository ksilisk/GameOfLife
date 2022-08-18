#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <unistd.h>
#define X 80
#define Y 25

struct game_board {
    int speed;
    char **data;
    char **data_temp;
};

void clear_board(struct game_board *board);
void init_board(struct game_board *board, int speed, FILE * file);
void draw_board(struct game_board *board);
int live_die(struct game_board *board, int y_point, int x_point);
void update_board(struct game_board *board);
void change_speed(struct game_board *board, char key);
void init_curses(int k);
void menu(void);
int main_menu(struct game_board *board);

int main() {
    init_curses(1);
    struct game_board board;
    if (main_menu(&board)) {
        int stop_flag = -1; char key = 't';
        while (key != 'q' && key != 'Q') {
            key = getch();
            if (key == ' ')
                stop_flag *= -1;
            if (stop_flag == 1) {
                draw_board(&board);
                continue;
            }
            if (key == '+' || key == '-')
                change_speed(&board, key);
            update_board(&board);
            draw_board(&board);
        }
        clear_board(&board);
    } else {
        printw("\nError! File incorrect!\nPress q to exit...");
        char k;
        while (k != 'q')
            k = getch();
    }
    init_curses(0);
    return 0;
}

void update_board(struct game_board *board) {
    for (int i = 0; i < Y; i++) {
        for (int j = 0; j < X; j++) {
            board->data_temp[i][j] = board->data[i][j];
        }
    }
    for (int i = 0; i < Y; i++) {
        for (int j = 0; j < X; j++) {
            if (live_die(board, i, j))
                board->data[i][j] = '0';
            else
                board->data[i][j] = ' ';
        }
    }
}

void init_board(struct game_board *board, int speed, FILE * file) {
    char temp;
    board->speed = speed;
    board->data = malloc(Y * sizeof(char*));
    board->data_temp = malloc(Y * sizeof(char*));
    for (int i = 0; i < Y; i++) {
        board->data[i] = malloc(X * sizeof(char));
        board->data_temp[i] = malloc(X * sizeof(char));
    }
    for (int i = 0; i < Y; i++) {
        for (int j = 0; j < X; j++) {
            temp = getc(file);
            if (temp == '.') {
                board->data[i][j] = ' ';
            } else if (temp == '0') {
                board->data[i][j] = '0';
            } else {
                j--;
            }
        }
    }
}

void clear_board(struct game_board *board) {
    for (int i = 0; i < Y; i++) {
        free(board->data[i]);
        free(board->data_temp[i]);
    }
    free(board->data_temp);
    free(board->data);
}

int main_menu(struct game_board *board) {
    menu();
    int flag = 0;
    char key;
    while (key != '1' && key != '2' && key != '3'
           && key != '4' && key != '5' && key != '6') {
        key = getch(); }
    FILE * file;
    switch (key) {
        case '1':
            if ((file = fopen("1.txt", "r")) != NULL) {
                flag = 1; init_board(board, 75000, file);
                draw_board(board); }
            break;
        case '2':
            if ((file = fopen("2.txt", "r")) != NULL) {
                flag = 1; init_board(board, 75000, file);
                draw_board(board); }
            break;
        case '3':
            if ((file = fopen("3.txt", "r")) != NULL) {
                flag = 1; init_board(board, 75000, file);
                draw_board(board); }
            break;
        case '4':
            if ((file = fopen("4.txt", "r")) != NULL) {
                flag = 1; init_board(board, 75000, file);
                draw_board(board); }
            break;
        case '5':
            if ((file = fopen("5.txt", "r")) != NULL) {
                flag = 1; init_board(board, 75000, file);
                draw_board(board); }
            break;
        case '6':
            if ((file = fopen("6.txt", "r")) != NULL) {
                flag = 1; init_board(board, 75000, file);
                draw_board(board); }
            break; }
    fclose(file);
    return flag;
}

void draw_board(struct game_board *board) {
    clear();
    usleep(board->speed);
    printw("*");
    for (int i = 0; i < X; i++)
        printw("-");
    printw("*\n");
    for (int i = 0; i < Y; i++) {
        printw("|");
        for (int j = 0; j < X; j++) {
            if (j == X - 1) {
                printw("%c", board->data[i][j]);
                continue;
            }
            printw("%c", board->data[i][j]);
        }
        printw("|\n");
    }
    printw("*");
    for (int i = 0; i < X; i++)
        printw("-");
    printw("*");
}

int live_die(struct game_board *board, int y_point, int x_point) {
    int live_points = 0;
    if (board->data_temp[(y_point + 1 + Y) % Y]
        [x_point] == '0')
        live_points++;
    if (board->data_temp[(y_point - 1 + Y) % Y]
        [x_point] == '0')
        live_points++;
    if (board->data_temp[y_point]
        [(x_point + 1 + X) % X] == '0')
        live_points++;
    if (board->data_temp[y_point]
        [(x_point - 1 + X) % X] == '0')
        live_points++;
    if (board->data_temp[(y_point - 1 + Y) % Y]
        [(x_point - 1 + X) % X] == '0')
        live_points++;
    if (board->data_temp[(y_point + 1 + Y) % Y]
        [(x_point + 1 + X) % X] == '0')
        live_points++;
    if (board->data_temp[(y_point + 1 + Y) % Y]
        [(x_point - 1 + X) % X] == '0')
        live_points++;
    if (board->data_temp[(y_point - 1 + Y) % Y]
        [(x_point + 1 + X) % X] == '0')
        live_points++;
    if (board->data_temp[y_point][x_point] == '0'
        && (live_points == 2 || live_points == 3))
        return 1;
    else if (board->data_temp[y_point][x_point] == ' ' && live_points == 3)
        return 1;
    return 0;
}

void change_speed(struct game_board *board, char key) {
    if (key == '+' && board->speed >= 20000) {
        board->speed -= 10000;
    } else if (key == '-' && board->speed <= 200000) {
        board->speed += 10000;
    }
}

void init_curses(int k) {
    if (k) {
        initscr();
        cbreak();
        noecho();
        scrollok(stdscr, TRUE);
        nodelay(stdscr, TRUE);
    } else {
        nocbreak();
        echo();
        scrollok(stdscr, FALSE);
        nodelay(stdscr, FALSE);
        endwin();
    }
}

void menu(void) {
    printw(
            "[Space] to pause and unpause\n"
            "[1] picture 1 (symmetric oscillator)\n"
            "[2] picture 2 (rake)\n"
            "[3] picture 3 (hive)\n"
            "[4] picture 4 (diehard)\n"
            "[5] picture 5 (gun)\n"
            "[6] picture 6 (your custom picture)\n"
            "[-] slow\n"
            "[+] fast\n"
            "[q] exit\n"
            "Choice picture [1 - 6]:");
}
