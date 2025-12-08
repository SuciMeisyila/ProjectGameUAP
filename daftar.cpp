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

        int ch = getch();

        if (ch == 'q' || ch == 'Q') break;

        if (ch == KEY_UP && cursorY > 0) cursorY--;
        else if (ch == KEY_DOWN && cursorY < SIZE - 1) cursorY++;
        else if (ch == KEY_LEFT && cursorX > 0) cursorX--;
        else if (ch == KEY_RIGHT && cursorX < SIZE - 1) cursorX++;
        else if (ch == 10 || ch == ' ') { // Enter atau Spasi
            // jika sel sudah matched (permanent) abaikan
            if (matched[cursorY][cursorX]) {
                // do nothing
            } else if (!revealed[cursorY][cursorX]) {
                // jika belum dibuka
                if (openCount == 0) {
                    // buka kartu pertama
                    ox1 = cursorX; oy1 = cursorY;
                    revealed[oy1][ox1] = true;
                    openCount = 1;
                } else if (openCount == 1) {
                    // jika pemain memilih koordinat yang sama dengan kartu pertama, abaikan
                    if (ox1 == cursorX && oy1 == cursorY) {
                        // tidak melakukan apa-apa, biarkan terbuka
                    } else {
                        // buka kartu kedua
                        ox2 = cursorX; oy2 = cursorY;
                        revealed[oy2][ox2] = true;
                        openCount = 2;

                        // gambar board dengan kedua kartu terbuka
                        drawBoard(board, revealed, cursorX, cursorY, moves, matchedCount);
                        napms(700); // delay 700ms

                        // cek pasangan
                        if (board[oy1][ox1] == board[oy2][ox2]) {
                            // tandai matched permanent
                            matched[oy1][ox1] = true;
                            matched[oy2][ox2] = true;
                            matchedCount += 2;
                        } else {
                            // sembunyikan kembali
                            revealed[oy1][ox1] = false;
                            revealed[oy2][ox2] = false;
                        }
                        openCount = 0;
                        moves++;
                    }
                }
            } else {
                // jika sudah terbuka (tapi belum matched) dan user tekan lagi -> abaikan
            }
        }
    }

    endwin();
    return 0;
}