#include <ncurses/curses.h>
#include <cstdlib>
#include <ctime>

#define SIZE 4
#define TOTAL (SIZE * SIZE)

// Untuk mengacak array sederhana
void shuffleArray(int arr[], int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

void drawBoard(int board[SIZE][SIZE], bool revealed[SIZE][SIZE], int cursorX, int cursorY, int moves, int matchedCount) {
    clear();
    mvprintw(0, 0, "=== MEMORY CARD GAME ===");
    mvprintw(1, 0, "Gunakan panah untuk bergerak, Enter/Spasi untuk membuka, q untuk keluar");
    mvprintw(2, 0, "Moves: %d    Matched: %d/%d", moves, matchedCount, TOTAL);

    // koordinat start
    int startY = 4;
    int startX = 2;
    for (int y = 0; y < SIZE; y++) {
        for (int x = 0; x < SIZE; x++) {
            int px = startX + x * 4;
            int py = startY + y * 2;

            if (cursorY == y && cursorX == x) {
                attron(A_REVERSE);
            }

            if (revealed[y][x]) {
                // gunakan width 2 untuk menjaga aligned
                mvprintw(py, px, "%2d", board[y][x]);
            } else {
                mvprintw(py, px, " *");
            }

            if (cursorY == y && cursorX == x) {
                attroff(A_REVERSE);
            }
        }
    }
    refresh();
}

int main() {
    srand((unsigned)time(0));

    initscr();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    // Membuat daftar angka pasangan (2 kali masing-masing)
    int numbers[TOTAL];
    for (int i = 0; i < TOTAL; i++) {
        numbers[i] = i / 2 + 1; // 1..(TOTAL/2)
    }

    // Acak pasangan
    shuffleArray(numbers, TOTAL);

    // Masukkan ke board SIZE x SIZE
    int board[SIZE][SIZE];
    bool revealed[SIZE][SIZE];
    bool matched[SIZE][SIZE]; // track kartu yang sudah permanently matched

    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++) {
            board[i][j] = numbers[i * SIZE + j];
            revealed[i][j] = false;
            matched[i][j] = false;
        }

    int cursorX = 0, cursorY = 0;

    int openCount = 0;
    int ox1 = -1, oy1 = -1, ox2 = -1, oy2 = -1;
    int matchedCount = 0;
    int moves = 0;

    while (true){
        drawBoard(board, revealed, cursorX, cursorY, moves, matchedCount);

        if (matchedCount == TOTAL) {
            mvprintw(4 + SIZE * 2, 0, "SELAMAT! Kamu menyelesaikan permainan dalam %d moves! Tekan q untuk keluar.", moves);
            refresh();
            int ch;
            while ((ch = getch()) != 'q' && ch != 'Q') { /* tunggu q */ }
            break;
        }