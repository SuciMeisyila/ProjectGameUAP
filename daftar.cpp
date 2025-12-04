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

    while (true) {
        drawBoard(board, revealed, cursorX, cursorY, moves, matchedCount);

        if (matchedCount == TOTAL) {
            mvprintw(4 + SIZE * 2, 0, "SELAMAT! Kamu menyelesaikan permainan dalam %d moves! Tekan q untuk keluar.", moves);
            refresh();
            int ch;
            while ((ch = getch()) != 'q' && ch != 'Q') { /* tunggu q */ }
            break;
        }