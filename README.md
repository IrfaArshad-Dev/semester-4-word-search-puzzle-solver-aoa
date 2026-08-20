# 🔍 Word Search Puzzle Solver – Analysis of Algorithms

![AOA](https://img.shields.io/badge/Course-Analysis%20of%20Algorithms-blue) ![C++](https://img.shields.io/badge/Language-C++-00599C?logo=cplusplus) ![Trie](https://img.shields.io/badge/Algorithm-Trie%20%2B%20DFS%20%2B%20Backtracking-green) ![Semester](https://img.shields.io/badge/Semester-4th-orange) ![Status](https://img.shields.io/badge/Status-Complete-brightgreen) ![University](https://img.shields.io/badge/University-Riphah-lightgrey)

---

## 📖 Project Overview

A fully functional **Word Search Puzzle Solver** implemented in C++ using **Trie + DFS Backtracking**, developed as a final project for the **Analysis of Algorithms (AOA)** course during **Spring 2026 (4th Semester, BSSE-4)** at **Riphah International University**.

The solver accepts any M×N character grid and a list of words, then identifies the **exact location, direction, and path** of each word using an optimized algorithmic approach.

> **Supervisor:** Dr. Neelam Umbreen

---

## 🎯 Problem Statement

Given an M×N grid of characters and a list of K target words, determine whether each word exists in the grid. A word exists if its characters can be traced as a contiguous sequence in any of **8 directions** (horizontal, vertical, diagonal — forward and reverse). Each cell may be used at most once per word path.

| | |
|---|---|
| **Input** | M×N character grid + list of K words |
| **Output** | Start cell (row, col), direction, path — or `NOT FOUND` |
| **Directions** | RIGHT, LEFT, UP, DOWN, UP-RIGHT, UP-LEFT, DOWN-RIGHT, DOWN-LEFT |

---

## 💡 Proposed Solution

Two algorithmic techniques combined:

### 🌳 Trie (Prefix Tree)
- All K words inserted before search begins.
- O(1)-per-character prefix lookup during DFS.
- Enables simultaneous multi-word search in a single grid traversal.
- Immediately prunes any path not matching a valid prefix.

### 🔁 DFS + Backtracking
- Starts at every grid cell as a potential word origin.
- Explores all 8 directions recursively.
- Marks cells as visited during a path; unmarks on backtrack.
- Prunes on first character mismatch.

---

## ⚡ Why Not Other Approaches?

| Approach | Limitation | Chosen? |
|---|---|---|
| Brute Force | O(N·M·8·L·K) — restarts per word, too slow for large K | ❌ |
| KMP per line | Poor diagonal support, not naturally 2D | ❌ |
| Aho-Corasick | Theoretically optimal but complex implementation | ❌ |
| Regex | Opaque complexity, not algorithmically meaningful | ❌ |
| **Trie + DFS** | **O(N·M·8·L) — all words in one pass, correct, clear** | ✅ |

---

## 📊 Complexity Analysis

| Measure | Expression | Explanation |
|---|---|---|
| Trie Build Time | O(K × L) | K words of avg. length L |
| DFS Search Time | O(N × M × 8 × L) | Trie prunes non-matching paths |
| Space — Trie | O(K × L × 26) | Worst case: unique node per character |
| Space — Visited | O(N × M) | One boolean per cell |
| Space — Stack | O(L) | Max recursion depth = longest word |

**Speedup vs Brute Force:** K× faster (e.g., 4x for K=4 words, 6x for K=6 words)

---

## 🛠️ Key Data Structures

| Structure | Role |
|---|---|
| Trie (Prefix Tree) | 26-child nodes; O(L) insert, O(1)-per-char prefix lookup |
| 2D Visited Array | Boolean N×M grid; reset on backtrack |
| Result Map | Maps each found word → start (row, col) + direction string |
| Implicit Stack | Recursion call stack tracks DFS path up to depth L |

---

## 🚀 Features

- ✅ Interactive console menu with 3 modes
- ✅ **Mode 1:** Built-in dry-run example (5×5 grid with 4 words)
- ✅ **Mode 2:** Enter your own custom grid and word list
- ✅ Step-by-step Trie construction display
- ✅ Color-highlighted results grid in terminal
- ✅ Live complexity analysis output (brute force vs Trie+DFS comparison)
- ✅ Reports start cell, end cell, direction, and found/not found status

---

## 💻 How to Run

### Compile

```bash
g++ -o word_search ALGO_CODE.cpp
```

### Run

```bash
./word_search
```

### Menu Options

```
1 -> Dry-run example (5x5 grid: RIGHT, DOWN, DOWN-RIGHT, UP-RIGHT)
2 -> Enter your own puzzle
3 -> Exit
```

---

## 📂 Repository Structure

```
semester-4-word-search-puzzle-solver-aoa/
│
├── ALGO_CODE.cpp              # Main C++ source code
├── ALGO_presentation.pptx     # Project presentation slides
│
├── Documentation/
│   └── AOA_Final_Report.pdf   # Complete project report
│
├── README.md
└── LICENSE
```

---

## 📋 Sample Output

```
SEARCH RESULTS
================================================
Word        Status     Start    End      Direction
------------------------------------------------
ALGO        FOUND      (0,0)    (0,3)    RIGHT
RICE        FOUND      (0,4)    (3,4)    DOWN
OAK         FOUND      (1,1)    (3,3)    DOWN-RIGHT
SEA         FOUND      (4,0)    (2,2)    UP-RIGHT
================================================
Found: 4    Not Found: 0    Total: 4
```

---

## ⚠️ Limitations

- Grid size capped at 20×20 (fixed array allocation)
- Reports only the **first occurrence** of a word if duplicates exist
- Trie memory not explicitly freed after execution
- Input restricted to uppercase English letters (A–Z)
- Recursive DFS may stack overflow on very large grids
- No multi-threading / parallel processing

---

## 🔮 Future Improvements

- Dynamic memory for arbitrarily large grids
- Multiple occurrence detection and reporting
- Explicit memory cleanup / smart pointers
- Parallel DFS using multi-threading
- GUI or web-based visualization

---

## 📚 References

1. Cormen et al. — *Introduction to Algorithms*, 3rd ed. MIT Press (Chapters 12, 26)
2. Sedgewick & Wayne — *Algorithms*, 4th ed. (Chapter 5 — Strings: Tries)
3. Levitin — *Introduction to Design and Analysis of Algorithms*, 3rd ed. (Chapter 7)
4. LeetCode #212 — Word Search II (conceptual reference only)

---

## 🎓 Academic Context

| Field | Detail |
|---|---|
| Course | Analysis of Algorithms (AOA) |
| Program | BS Software Engineering |
| Semester | 4th Semester (BSSE-4) |
| Session | Spring 2026 |
| Institution | Riphah International University |
| Submission | 24 April 2026 |

---

## 👩‍💻 Project Team

| Name | SAP ID | Email |
|---|---|---|
| Irfa Arshad | 63662 | 63662@students.riphah.edu.pk |
| Nimra Atif | 64813 | 64813@students.riphah.edu.pk |
| Hafsa Saghir | 65314 | 65314@students.riphah.edu.pk |
| Amina Batool | 62461 | 62461@students.riphah.edu.pk |
| Mishal Asim | 62516 | 65315@students.riphah.edu.pk |
| Almarwah K.M. Shloun | 71120 | 71120@students.riphah.edu.pk |

---

## 📜 License

This project is licensed under the [MIT License](LICENSE).
