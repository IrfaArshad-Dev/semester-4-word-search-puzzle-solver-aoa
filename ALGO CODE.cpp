#include <iostream>
#include <string>
using namespace std;

// ============================================================
//  WORD SEARCH PUZZLE SOLVER
//  Algorithm : Trie (Prefix Tree) + DFS Backtracking
//  Course    : Design and Analysis of Algorithms
//  Instructor: Dr. Neelam Umbreen
//
//  Compile:  g++ main.cpp -o solver
//  Run:      ./solver
// ============================================================


// -----------------------------------------------------------
//  ANSI COLOR CODES  (terminal colors)
// -----------------------------------------------------------
#define RESET       "\033[0m"
#define BOLD        "\033[1m"
#define RED         "\033[31m"
#define GREEN       "\033[32m"
#define YELLOW      "\033[33m"
#define BLUE        "\033[34m"
#define MAGENTA     "\033[35m"
#define CYAN        "\033[36m"
#define WHITE       "\033[37m"
#define BG_GREEN    "\033[42m"
#define BG_BLUE     "\033[44m"
#define BG_YELLOW   "\033[43m"
#define BG_MAGENTA  "\033[45m"
#define BG_CYAN     "\033[46m"
#define BG_RED      "\033[41m"

// Up to 6 colors for different found words
const char* WORD_COLORS[6] = {
    "\033[1;42m",   // bright green bg
    "\033[1;44m",   // bright blue bg
    "\033[1;45m",   // bright magenta bg
    "\033[1;46m",   // bright cyan bg
    "\033[1;43m",   // bright yellow bg
    "\033[1;41m",   // bright red bg
};


// -----------------------------------------------------------
//  CONSTANTS
// -----------------------------------------------------------
const int MAX_ROWS  = 20;
const int MAX_COLS  = 20;
const int MAX_WORDS = 20;
const int MAX_LEN   = 20;
const int ALPHA     = 26;
const int POOL_SIZE = MAX_WORDS * MAX_LEN * 2;


// -----------------------------------------------------------
//  TRIE NODE
// -----------------------------------------------------------
struct TrieNode {
    int  children[ALPHA];
    bool isEnd;
    char word[MAX_LEN + 1];
};

TrieNode triePool[POOL_SIZE];
int      poolSize = 0;

int newNode() {
    for (int i = 0; i < ALPHA; i++)
        triePool[poolSize].children[i] = -1;
    triePool[poolSize].isEnd   = false;
    triePool[poolSize].word[0] = '\0';
    return poolSize++;
}

void insertWord(int root, const char* word) {
    int node = root;
    for (int i = 0; word[i] != '\0'; i++) {
        int idx = word[i] - 'A';
        if (triePool[node].children[idx] == -1)
            triePool[node].children[idx] = newNode();
        node = triePool[node].children[idx];
    }
    triePool[node].isEnd = true;
    int j = 0;
    while (word[j] != '\0') { triePool[node].word[j] = word[j]; j++; }
    triePool[node].word[j] = '\0';
}


// -----------------------------------------------------------
//  GRID AND GLOBAL STATE
// -----------------------------------------------------------
char grid[MAX_ROWS][MAX_COLS + 1];
int  ROWS, COLS;
bool visited[MAX_ROWS][MAX_COLS];


// -----------------------------------------------------------
//  RESULT
// -----------------------------------------------------------
struct Result {
    char word[MAX_LEN + 1];
    int  startRow, startCol;
    int  endRow,   endCol;
    char direction[20];
    bool found;
    int  colorIndex;   // which color to use for this word
};

Result results[MAX_WORDS];
int    wordCount = 0;

// 8 movement directions
int dr[8] = {  0,  0,  1, -1,  1,  1, -1, -1 };
int dc[8] = {  1, -1,  0,  0,  1, -1,  1, -1 };
const char* dirName[8] = {
    "RIGHT", "LEFT", "DOWN", "UP",
    "DOWN-RIGHT", "DOWN-LEFT", "UP-RIGHT", "UP-LEFT"
};


// -----------------------------------------------------------
//  HELPERS
// -----------------------------------------------------------
bool sameStr(const char* a, const char* b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) return false;
        i++;
    }
    return (a[i] == '\0' && b[i] == '\0');
}

void copyStr(char* dst, const char* src) {
    int i = 0;
    while (src[i] != '\0') { dst[i] = src[i]; i++; }
    dst[i] = '\0';
}

int strLen(const char* s) {
    int i = 0;
    while (s[i] != '\0') i++;
    return i;
}

// Print a horizontal line of given char and width
void printLine(char ch, int width) {
    for (int i = 0; i < width; i++) cout << ch;
    cout << "\n";
}

// Print centered text inside a box of given width
void printCentered(const char* text, int width, const char* color = "") {
    int len = strLen(text);
    int pad = (width - len) / 2;
    cout << color;
    for (int i = 0; i < pad; i++) cout << " ";
    cout << text;
    for (int i = 0; i < width - len - pad; i++) cout << " ";
    cout << RESET;
}


// -----------------------------------------------------------
//  DFS Backtracking
// -----------------------------------------------------------
void dfs(int startR, int startC,
         int curR,   int curC,
         int trieNode,
         int dirIndex,
         int colorIdx)
{
    if (triePool[trieNode].isEnd) {
        for (int i = 0; i < wordCount; i++) {
            if (sameStr(results[i].word, triePool[trieNode].word)) {
                if (!results[i].found) {
                    results[i].found      = true;
                    results[i].startRow   = startR;
                    results[i].startCol   = startC;
                    results[i].endRow     = curR;
                    results[i].endCol     = curC;
                    results[i].colorIndex = colorIdx;
                    copyStr(results[i].direction, dirName[dirIndex]);
                }
                break;
            }
        }
    }

    for (int d = 0; d < 8; d++) {
        if (dirIndex != -1 && d != dirIndex) continue;

        int nextR = curR + dr[d];
        int nextC = curC + dc[d];

        if (nextR < 0 || nextR >= ROWS) continue;
        if (nextC < 0 || nextC >= COLS) continue;
        if (visited[nextR][nextC]) continue;

        int idx = grid[nextR][nextC] - 'A';
        if (triePool[trieNode].children[idx] == -1) continue;

        int nextNode = triePool[trieNode].children[idx];

        visited[nextR][nextC] = true;
        dfs(startR, startC, nextR, nextC, nextNode, d, colorIdx);
        visited[nextR][nextC] = false;  // BACKTRACK
    }
}


// -----------------------------------------------------------
//  searchAll() - launches DFS from every cell
// -----------------------------------------------------------
void searchAll(int root) {
    int colorCounter = 0;
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            int idx = grid[r][c] - 'A';
            if (triePool[root].children[idx] == -1) continue;

            for (int i = 0; i < ROWS; i++)
                for (int j = 0; j < COLS; j++)
                    visited[i][j] = false;

            visited[r][c] = true;
            dfs(r, c, r, c, triePool[root].children[idx], -1, colorCounter % 6);
            visited[r][c] = false;
        }
    }
    // assign colors to found words in order
    int ci = 0;
    for (int i = 0; i < wordCount; i++)
        if (results[i].found)
            results[i].colorIndex = ci++;
}


// -----------------------------------------------------------
//  printHeader()
// -----------------------------------------------------------
void printHeader() {
    cout << "\n";
    cout << BOLD << CYAN;
    printLine('=', 60);
    printCentered("WORD SEARCH PUZZLE SOLVER", 60, "");
    cout << "\n";
    printCentered("Trie + DFS Backtracking Algorithm", 60, "");
    cout << "\n";
    printCentered("Analysis of Algorithms", 60, "");
    cout << "\n";
    printCentered("Instructor: Dr. Neelam Umbreen", 60, "");
    cout << "\n";
    printLine('=', 60);
    cout << RESET << "\n";
}


// -----------------------------------------------------------
//  printGrid() - with column/row labels, clean box
// -----------------------------------------------------------
void printGrid() {
    cout << "\n" << BOLD << BLUE;
    printLine('=', 60);
    printCentered("PUZZLE GRID", 60, "");
    cout << "\n";
    printLine('=', 60);
    cout << RESET;

    // Column headers
    cout << "       ";
    for (int c = 0; c < COLS; c++) {
        cout << BOLD << YELLOW << "C" << c << RESET;
        if (c < 9) cout << "  "; else cout << " ";
    }
    cout << "\n";

    // Top border
    cout << "      +";
    for (int c = 0; c < COLS; c++) cout << "---+";
    cout << "\n";

    // Rows
    for (int r = 0; r < ROWS; r++) {
        cout << BOLD << YELLOW;
        if (r < 10) cout << "   R" << r;
        else        cout << "  R" << r;
        cout << RESET << " |";
        for (int c = 0; c < COLS; c++) {
            cout << " " << BOLD << WHITE << grid[r][c] << RESET << " |";
        }
        cout << "\n";

        // Row separator
        cout << "      +";
        for (int c = 0; c < COLS; c++) cout << "---+";
        cout << "\n";
    }
    cout << "\n";
}


// -----------------------------------------------------------
//  printWordList() - shows words with numbered list
// -----------------------------------------------------------
void printWordList() {
    cout << BOLD << BLUE;
    printLine('=', 60);
    printCentered("WORDS TO SEARCH", 60, "");
    cout << "\n";
    printLine('=', 60);
    cout << RESET;

    for (int i = 0; i < wordCount; i++) {
        cout << "  " << BOLD << YELLOW << (i+1) << ". " << RESET
             << BOLD << WHITE << results[i].word << RESET << "\n";
    }
    cout << "\n";
}


// -----------------------------------------------------------
//  printTrieSteps() - shows trie building process
// -----------------------------------------------------------
void printTrieSteps() {
    cout << BOLD << BLUE;
    printLine('-', 60);
    cout << "  STEP 1: Building Trie (Prefix Tree)\n";
    printLine('-', 60);
    cout << RESET;

    int tempPool = 0;
    for (int i = 0; i < wordCount; i++) {
        int len = strLen(results[i].word);
        cout << "  Inserting " << BOLD << CYAN << "\"" << results[i].word << "\"" << RESET << "  ->  ";
        for (int j = 0; j < len; j++) {
            cout << BOLD << GREEN << results[i].word[j] << RESET;
            if (j < len - 1) cout << " -> ";
        }
        cout << " [END]\n";
    }
    cout << "  " << GREEN << "Trie built successfully. Total nodes: " << poolSize << RESET << "\n\n";
}


// -----------------------------------------------------------
//  printResults() - clean aligned table with colors
// -----------------------------------------------------------
void printResults() {
    int foundCount = 0;
    for (int i = 0; i < wordCount; i++)
        if (results[i].found) foundCount++;

    cout << "\n" << BOLD << BLUE;
    printLine('=', 70);
    printCentered("SEARCH RESULTS", 70, "");
    cout << "\n";
    printLine('=', 70);
    cout << RESET;

    // Header row
    cout << BOLD
         << "  " << "Word        "
         << "  " << "Status   "
         << "  " << "Start    "
         << "  " << "End      "
         << "  " << "Direction"
         << RESET << "\n";

    cout << "  ";
    printLine('-', 68);

    for (int i = 0; i < wordCount; i++) {
        // Word column
        int wlen = strLen(results[i].word);
        if (results[i].found)
            cout << "  " << WORD_COLORS[results[i].colorIndex] << results[i].word << RESET;
        else
            cout << "  " << BOLD << RED << results[i].word << RESET;

        for (int s = wlen; s < 12; s++) cout << " ";

        if (results[i].found) {
            // Status
            cout << "  " << BOLD << GREEN << "FOUND    " << RESET;
            // Start
            cout << "  (" << results[i].startRow << "," << results[i].startCol << ")";
            int sc = 2 + (results[i].startRow>9?2:1) + 1 + (results[i].startCol>9?2:1);
            for (int s = sc; s < 7; s++) cout << " ";
            // End
            cout << "  (" << results[i].endRow << "," << results[i].endCol << ")";
            int ec = 2 + (results[i].endRow>9?2:1) + 1 + (results[i].endCol>9?2:1);
            for (int s = ec; s < 7; s++) cout << " ";
            // Direction
            cout << "  " << BOLD << CYAN << results[i].direction << RESET;
        } else {
            cout << "  " << BOLD << RED << "NOT FOUND" << RESET;
        }
        cout << "\n";
    }

    cout << "  ";
    printLine('=', 68);

    cout << "  " << BOLD << GREEN << "Found: " << foundCount << RESET
         << "   " << BOLD << RED << "Not Found: " << (wordCount - foundCount) << RESET
         << "   " << BOLD << WHITE << "Total: " << wordCount << RESET << "\n\n";
}


// -----------------------------------------------------------
//  printColoredGrid() - highlights found words with colors
// -----------------------------------------------------------
void printColoredGrid() {
    // Build a color map: cellColor[r][c] = color index (-1 = not found)
    int cellColor[MAX_ROWS][MAX_COLS];
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++)
            cellColor[r][c] = -1;

    for (int i = 0; i < wordCount; i++) {
        if (!results[i].found) continue;

        int stepR = 0, stepC = 0;
        if      (results[i].endRow > results[i].startRow) stepR =  1;
        else if (results[i].endRow < results[i].startRow) stepR = -1;
        if      (results[i].endCol > results[i].startCol) stepC =  1;
        else if (results[i].endCol < results[i].startCol) stepC = -1;

        int r = results[i].startRow;
        int c = results[i].startCol;
        int len = strLen(results[i].word);

        for (int s = 0; s < len; s++) {
            cellColor[r][c] = results[i].colorIndex;
            r += stepR;
            c += stepC;
        }
    }

    cout << BOLD << BLUE;
    printLine('=', 60);
    printCentered("COLOR-HIGHLIGHTED GRID", 60, "");
    cout << "\n";
    printLine('=', 60);
    cout << RESET;

    // Legend
    cout << "  Legend:  ";
    for (int i = 0; i < wordCount; i++) {
        if (results[i].found) {
            cout << WORD_COLORS[results[i].colorIndex] << " " << results[i].word << " " << RESET << "  ";
        }
    }
    cout << "\n\n";

    // Column headers
    cout << "       ";
    for (int c = 0; c < COLS; c++) {
        cout << BOLD << YELLOW << "C" << c << RESET;
        if (c < 9) cout << "  "; else cout << " ";
    }
    cout << "\n";

    // Top border
    cout << "      +";
    for (int c = 0; c < COLS; c++) cout << "---+";
    cout << "\n";

    for (int r = 0; r < ROWS; r++) {
        cout << BOLD << YELLOW;
        if (r < 10) cout << "   R" << r;
        else        cout << "  R" << r;
        cout << RESET << " |";

        for (int c = 0; c < COLS; c++) {
            if (cellColor[r][c] >= 0) {
                cout << WORD_COLORS[cellColor[r][c]]
                     << " " << grid[r][c] << " "
                     << RESET << "|";
            } else {
                cout << " " << BOLD << WHITE << grid[r][c] << RESET << " |";
            }
        }
        cout << "\n";

        cout << "      +";
        for (int c = 0; c < COLS; c++) cout << "---+";
        cout << "\n";
    }
    cout << "\n";
}


// -----------------------------------------------------------
//  showComplexity() - complexity analysis with formatting
// -----------------------------------------------------------
void showComplexity() {
    int maxLen = 0;
    for (int i = 0; i < wordCount; i++) {
        int len = strLen(results[i].word);
        if (len > maxLen) maxLen = len;
    }

    int N = ROWS, M = COLS, K = wordCount, L = maxLen;
    long long bruteOps = (long long)N * M * 8 * L * K;
    long long trieOps  = (long long)N * M * 8 * L;

    cout << BOLD << BLUE;
    printLine('=', 60);
    printCentered("COMPLEXITY ANALYSIS", 60, "");
    cout << "\n";
    printLine('=', 60);
    cout << RESET;

    cout << "  " << BOLD << "Grid Size     : " << RESET << CYAN << N << " x " << M << RESET << "\n";
    cout << "  " << BOLD << "Words (K)     : " << RESET << CYAN << K << RESET << "\n";
    cout << "  " << BOLD << "Max Length (L): " << RESET << CYAN << L << " characters" << RESET << "\n";

    cout << "\n  " << BOLD << YELLOW << "BRUTE FORCE" << RESET << " - searches each word separately:\n";
    cout << "  Formula : O(N x M x 8 x L x K)\n";
    cout << "  = " << N << " x " << M << " x 8 x " << L << " x " << K
         << " = " << BOLD << RED << bruteOps << " operations" << RESET << "\n";

    cout << "\n  " << BOLD << GREEN << "TRIE + BACKTRACKING" << RESET << " - all words in one pass:\n";
    cout << "  Formula : O(N x M x 8 x L)\n";
    cout << "  = " << N << " x " << M << " x 8 x " << L
         << " = " << BOLD << GREEN << trieOps << " operations" << RESET << "\n";

    cout << "\n  " << BOLD << CYAN << "Speedup: " << K << "x faster than brute force" << RESET << "\n";

    cout << "\n  " << BOLD << "SPACE COMPLEXITY:" << RESET << "\n";
    cout << "  Trie    : O(K x L x 26) ~= " << CYAN << (K * L * 26) << " pointers" << RESET << "\n";
    cout << "  Visited : O(N x M)      = " << CYAN << (N * M) << " booleans" << RESET << "\n";
    cout << "  Stack   : O(L)          = " << CYAN << L << " recursive frames" << RESET << "\n";

    cout << "\n";
    printLine('=', 60);
    cout << "\n";
}


// -----------------------------------------------------------
//  resetState() - clears results and trie for a fresh run
// -----------------------------------------------------------
void resetState() {
    poolSize  = 0;
    wordCount = 0;
    for (int r = 0; r < MAX_ROWS; r++)
        for (int c = 0; c < MAX_COLS; c++)
            visited[r][c] = false;
    for (int i = 0; i < MAX_WORDS; i++) {
        results[i].found      = false;
        results[i].word[0]    = '\0';
        results[i].colorIndex = 0;
    }
}


// -----------------------------------------------------------
//  loadDryRun() - verified 5x5 grid showing all 4 directions
//
//  Grid:
//    C0  C1  C2  C3  C4
//  R0: A   L   G   O   R
//  R1: F   O   B   N   I
//  R2: M   M   A   G   C
//  R3: F   E   Z   K   E
//  R4: S   D   V   C   B
//
//  Words & verified directions:
//  ALGO  ->  RIGHT       (R0,C0) -> (R0,C3)
//  RICE  ->  DOWN        (R0,C4) -> (R3,C4)
//  OAK   ->  DOWN-RIGHT  (R1,C1) -> (R3,C3)
//  SEA   ->  UP-RIGHT    (R4,C0) -> (R2,C2)
// -----------------------------------------------------------
void loadDryRun() {
    ROWS = 5; COLS = 5;

    string g[5] = {"ALGOR",
                   "FOBNI",
                   "MMAGC",
                   "FEZKE",
                   "SDVCB"};
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++)
            grid[r][c] = g[r][c];
        grid[r][COLS] = '\0';
    }

    wordCount = 4;
    string w[4] = {"ALGO", "RICE", "OAK", "SEA"};
    for (int i = 0; i < wordCount; i++) {
        int j = 0;
        for (j = 0; j < (int)w[i].size(); j++)
            results[i].word[j] = w[i][j];
        results[i].word[j] = '\0';
        results[i].found   = false;
        results[i].colorIndex = i;
    }
}


// -----------------------------------------------------------
//  takeInput() - user enters their own puzzle
// -----------------------------------------------------------
void takeInput() {
    cout << BOLD << CYAN << "\n  Enter grid dimensions:\n" << RESET;
    cout << "  Rows (max 20): "; cin >> ROWS;
    cout << "  Cols (max 20): "; cin >> COLS;

    cout << BOLD << CYAN << "\n  Enter grid row by row (no spaces):\n" << RESET;
    for (int r = 0; r < ROWS; r++) {
        cout << "  Row " << r << ": ";
        string row; cin >> row;
        for (int c = 0; c < COLS; c++)
            grid[r][c] = toupper(row[c]);
        grid[r][COLS] = '\0';
    }

    cout << BOLD << CYAN << "\n  How many words to search? " << RESET;
    cin >> wordCount;
    cout << "  Enter the words:\n";
    for (int i = 0; i < wordCount; i++) {
        cout << "  Word " << (i+1) << ": ";
        string w; cin >> w;
        int j = 0;
        for (j = 0; j < (int)w.size(); j++)
            results[i].word[j] = toupper(w[j]);
        results[i].word[j] = '\0';
        results[i].found   = false;
        results[i].colorIndex = i;
    }
}


// -----------------------------------------------------------
//  runSolver() - full pipeline for one puzzle
// -----------------------------------------------------------
void runSolver() {
    printWordList();
    printGrid();

    // Build Trie
    cout << BOLD << BLUE;
    printLine('-', 60);
    cout << "  STEP 1: Building Trie...\n";
    printLine('-', 60);
    cout << RESET;
    int root = newNode();
    for (int i = 0; i < wordCount; i++) {
        insertWord(root, results[i].word);
        cout << "  " << GREEN << "Inserted" << RESET
             << " \"" << BOLD << results[i].word << RESET << "\"  ->  ";
        int len = strLen(results[i].word);
        for (int j = 0; j < len; j++) {
            cout << BOLD << CYAN << results[i].word[j] << RESET;
            if (j < len-1) cout << BOLD << " -> " << RESET;
        }
        cout << BOLD << " [END]\n" << RESET;
    }
    cout << "  " << GREEN << "Done. Total Trie nodes: " << poolSize << RESET << "\n\n";

    // Search
    cout << BOLD << BLUE;
    printLine('-', 60);
    cout << "  STEP 2: DFS Backtracking Search...\n";
    printLine('-', 60);
    cout << RESET;
    searchAll(root);
    cout << "  " << GREEN << "Search complete!\n" << RESET << "\n";

    // Output
    printResults();
    printColoredGrid();
    showComplexity();
}


// -----------------------------------------------------------
//  clearScreen() - clears the terminal
// -----------------------------------------------------------
void clearScreen() {
    cout << "\033[2J\033[H";   // ANSI: clear screen + move cursor to top
}


// -----------------------------------------------------------
//  MAIN  - menu loop (no restart needed)
// -----------------------------------------------------------
int main() {
    clearScreen();
    printHeader();

    while (true) {
        cout << BOLD << BLUE;
        printLine('=', 60);
        printCentered("MAIN MENU", 60, "");
        cout << "\n";
        printLine('=', 60);
        cout << RESET;

        cout << "  " << BOLD << YELLOW << "1" << RESET << "  ->  Dry-run example (5x5 grid): \n";
        cout << "  " << BOLD << YELLOW << "2" << RESET << "  ->  Enter your own puzzle\n";
        cout << "  " << BOLD << YELLOW << "3" << RESET << "  ->  Exit\n";
        cout << "\n  " << BOLD << "Your choice: " << RESET;

        int choice;
        cin >> choice;
        cout << "\n";

        if (choice == 1) {
            clearScreen();
            resetState();
            loadDryRun();
            cout << BOLD << GREEN << "  [Dry-run: 5x5 grid — RIGHT, DOWN, DOWN-RIGHT, UP-RIGHT]\n\n" << RESET;
            runSolver();
        }
        else if (choice == 2) {
            clearScreen();
            resetState();
            takeInput();
            runSolver();
        }
        else if (choice == 3) {
            clearScreen();
            cout << BOLD << CYAN;
            printLine('=', 60);
            printCentered("Thank you! Goodbye.", 60, "");
            cout << "\n";
            printLine('=', 60);
            cout << RESET << "\n";
            break;
        }
        else {
            cout << BOLD << RED << "  Invalid choice. Please enter 1, 2, or 3.\n" << RESET << "\n";
        }

        // After each run, pause before showing menu again
        cout << BOLD << YELLOW << "\n  Press ENTER to return to menu..." << RESET;
        cin.ignore();
        cin.get();
        clearScreen();
        printHeader();
    }

    return 0;
}
