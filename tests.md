--2048 Test Evidence--

The tests were run against the current 5x5 game. In each board, rows
are listed from top to bottom, values in a row are listed from left to right,
and 0 means an empty cell. Unlisted board rows are 0 0 0 0 0.

--Deterministic movement and game state tests--

| Test | Starting board | Input | Expected result | Actual result | Status |
|---|---|---|---|---|---|
| move left through a gap | Row 1: 0 2 0 4 0 | Left | Row 1 becomes 2 4 0 0 0; changed; score stays 0 | Row 1 became 2 4 0 0 0; changed; score stayed 0 | PASS |
| move right through a gap | Row 1: 0 2 0 4 0 | Right | Row 1 becomes 0 0 0 2 4; changed; score stays 0 | Row 1 became 0 0 0 2 4; changed; score stayed 0 | PASS |
| move up through a gap | Column 1, top to bottom: 0, 2, 0, 4, 0 | Up | Column 1 becomes 2, 4, 0, 0, 0; changed; score stays 0 | Column 1 became 2, 4, 0, 0, 0; changed; score stayed 0 | PASS |
| move down through a gap | Column 1, top to bottom: 0, 2, 0, 4, 0 | Down | Column 1 becomes 0, 0, 0, 2, 4; changed; score stays 0 | Column 1 became 0, 0, 0, 2, 4; changed; score stayed 0 | PASS |

| three equal tiles merge once | Row 1: 2 2 2 0 0 | Left | Row 1 becomes 4 2 0 0 0; score increases by 4 | Row 1 became 4 2 0 0 0; score increased by 4 | PASS |
| four equal tiles merge once per pair | Row 1: 2 2 2 2 0 | Left | Row 1 becomes 4 4 0 0 0; score increases by 8 | Row 1 became 4 4 0 0 0; score increased by 8 | PASS |

| successful slide without a merge | Row 1: 0 2 0 0 0; starting score 7 | Left | Row 1 becomes 2 0 0 0 0; move succeeds; score remains 7 | Row 1 became 2 0 0 0 0; move succeeded; score remained 7 | PASS |
| unchanged move | Row 1: 2 0 0 0 0; starting score 7 | Left | Board unchanged; move reports unchanged; score remains 7 | Board remained unchanged; move reported unchanged; score remained 7 | PASS |
| several merges and score change | Row 1: 2 2 4 4 0; starting score 0 | Left | Row 1 becomes 4 8 0 0 0; score becomes 12 | Row 1 became 4 8 0 0 0; score became 12 | PASS |
| reach 2048 | Row 1: 1024 1024 0 0 0; starting score 0 | Left | Row 1 becomes 2048 0 0 0 0; highest tile and score become 2048 | Row 1 became 2048 0 0 0 0; highest tile and score became 2048 | PASS |

| full board with possible moves | Every cell contains 2 | Check game-over state | Not game over because matching neighbors can merge | Reported not game over | PASS |
| full board without possible moves | Every row alternates 2 4 2 4 2 and 4 2 4 2 4 | Check game over state | Game over because there are no empty cells or matching neighbors | Reported game over | PASS |

--Separate random insertion record--

Random cell selection is nondeterministic; these results record one run using
srand(17). The insertion checks verify the number and value of tiles rather
than requiring a particular random location.

| Test | Starting board and state | Input | Expected result | Actual result | Status |
|---|---|---|---|---|---|
| insert one random tile | Empty 5x5 board | Call insert_random | Exactly one cell contains 2; all other cells remain empty | One 2 was inserted at row 5, column 5; 24 cells remained empty | PASS |
| initialize a new game | Empty 5x5 board; score 99; successful moves 88 | Call start_new_game with srand(17) | Score and moves reset to 0; two distinct cells contain 2; 23 cells remain empty | Score and moves reset to 0; tiles were at row 3, column 3 and row 5, column 5; 23 cells remained empty | PASS |
