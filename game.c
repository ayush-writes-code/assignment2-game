#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>   // if this errors, change to <curses.h>

#define TRACK_WIDTH 150     // wide horizontal road
#define LANES 15            // 5x height (15 lanes instead of 3)
#define MAX_OBSTACLES 30

typedef struct {
    int lane;
    int x;
    int active;
} Obstacle;

// Difficulty: how many obstacles
typedef enum {
    DIFF_EASY = 0,
    DIFF_MEDIUM = 1,
    DIFF_HARD = 2
} Difficulty;

// Speed: delay between frames
typedef enum {
    SPEED_SLOW = 0,
    SPEED_FAST = 1,
    SPEED_ULTRA = 2
} Speed;

typedef struct {
    int playerLane;
    int playerX;
    Obstacle obstacles[MAX_OBSTACLES];
    int numObstacles;
    int score;
} GameState;

// ---- Helpers ----
const char* difficultyName(Difficulty d) {
    if (d == DIFF_EASY) return "Easy";
    if (d == DIFF_MEDIUM) return "Medium";
    return "Hard";
}

const char* speedName(Speed s) {
    if (s == SPEED_SLOW) return "Slow";
    if (s == SPEED_FAST) return "Fast";
    return "Ultra";
}

int getDelayMs(Speed s) {
    if (s == SPEED_SLOW) return 120;
    if (s == SPEED_FAST) return 70;
    return 40;
}

int getObstacleCount(Difficulty d) {
    if (d == DIFF_EASY) return 3;
    if (d == DIFF_MEDIUM) return 8;
    return 15;
}


void drawTitleArt() {
    mvprintw(0, 0, " $$$$$$\\  $$$$$$$\\   $$$$$$\\   $$$$$$\\  $$$$$$$$\\       $$\\      $$\\  $$$$$$\\  $$$$$$$\\   $$$$$$\\  ");
    mvprintw(1, 0, "$$  __$$\\ $$  __$$\\ $$  __$$\\ $$  __$$\\ $$  _____|      $$ | $\\  $$ |$$  __$$\\ $$  __$$\\ $$  __$$\\ ");
    mvprintw(2, 0, "$$ /  \\__|$$ |  $$ |$$ /  $$ |$$ /  \\__|$$ |            $$ |$$$\\ $$ |$$ /  $$ |$$ |  $$ |$$ /  \\__|");
    mvprintw(3, 0, "\\$$$$$$\\  $$$$$$$  |$$$$$$$$ |$$ |      $$$$$\\          $$ $$ $$\\$$ |$$$$$$$$ |$$$$$$$  |\\$$$$$$\\  ");
    mvprintw(4, 0, " \\____$$\\ $$  ____/ $$  __$$ |$$ |      $$  __|         $$$$  _$$$$ |$$  __$$ |$$  __$$<  \\____$$\\ ");
    mvprintw(5, 0, "$$\\   $$ |$$ |      $$ |  $$ |$$ |  $$\\ $$ |            $$$  / \\$$$ |$$ |  $$ |$$ |  $$ |$$\\   $$ |");
    mvprintw(6, 0, "\\$$$$$$  |$$ |      $$ |  $$ |\\$$$$$$  |$$$$$$$$\\       $$  /   \\$$ |$$ |  $$ |$$ |  $$ |\\$$$$$$  |");
    mvprintw(7, 0, " \\______/ \\__|      \\__|  \\__| \\______/ \\________|      \\__/     \\__|\\__|  \\__|\\__|  \\__| \\______/ ");
}


// ---- Menu functions ----
int mainMenu(int hasSavedGame) {

    clear();
drawTitleArt();

int baseRow = 10;   // menu starts below title



    const char *items[] = {
        "Start New Game",
        "Continue Game",
        "Options",
        "Exit"
    };
    int nItems = 4;
    int choice = 0;
    int ch;

    while (1) {
        clear();
        drawTitleArt();  // 🔥 ASCII banner on top

        int baseRow = 10;  // menu starts below the art

        mvprintw(baseRow - 2, 0, "Use UP/DOWN arrows or W/S to move, ENTER to select.");

        for (int i = 0; i < nItems; i++) {
            if (i == choice) attron(A_REVERSE);

            if (i == 1 && !hasSavedGame) {
                mvprintw(baseRow + i, 0, "%s (no saved game)", items[i]);
            } else {
                mvprintw(baseRow + i, 0, "%s", items[i]);
            }

            if (i == choice) attroff(A_REVERSE);
        }

        refresh();

        ch = getch();
        if (ch == KEY_UP || ch == 'w' || ch == 'W') {
            if (choice > 0) choice--;
        } else if (ch == KEY_DOWN || ch == 's' || ch == 'S') {
            if (choice < nItems - 1) choice++;
        } else if (ch == '\n' || ch == KEY_ENTER || ch == 10 || ch == 13) {
            if (choice == 1 && !hasSavedGame) {
                mvprintw(baseRow + nItems + 2, 0, "No saved game to continue. Press any key...");
                nodelay(stdscr, FALSE);
                getch();
                nodelay(stdscr, TRUE);
            } else {
                return choice;
            }
        }

        napms(60);
    }
}


void optionsMenu(Difficulty *diff, Speed *speed) {
    int choice = 0; // 0 = difficulty, 1 = speed, 2 = back
    int ch;

    while (1) {
        clear();
        mvprintw(0, 0, "OPTIONS");
        mvprintw(1, 0, "Use UP/DOWN to select, LEFT/RIGHT to change, ENTER to go back.");

        for (int i = 0; i < 3; i++) {
            if (i == choice) attron(A_REVERSE);
            if (i == 0)
                mvprintw(3 + i, 0, "Difficulty: %s", difficultyName(*diff));
            else if (i == 1)
                mvprintw(3 + i, 0, "Speed: %s", speedName(*speed));
            else
                mvprintw(3 + i, 0, "Back");
            if (i == choice) attroff(A_REVERSE);
        }

        refresh();
        ch = getch();

        if (ch == KEY_UP || ch == 'w' || ch == 'W') {
            if (choice > 0) choice--;
        } else if (ch == KEY_DOWN || ch == 's' || ch == 'S') {
            if (choice < 2) choice++;
        } else if (ch == KEY_LEFT || ch == 'a' || ch == 'A') {
            if (choice == 0) {
                if (*diff > DIFF_EASY) (*diff)--;
            } else if (choice == 1) {
                if (*speed > SPEED_SLOW) (*speed)--;
            }
        } else if (ch == KEY_RIGHT || ch == 'd' || ch == 'D') {
            if (choice == 0) {
                if (*diff < DIFF_HARD) (*diff)++;
            } else if (choice == 1) {
                if (*speed < SPEED_ULTRA) (*speed)++;
            }
        } else if (ch == '\n' || ch == KEY_ENTER || ch == 10 || ch == 13) {
            if (choice == 2) return; // Back
        }

        napms(60);
    }
}

// ---- Game logic ----

void initGame(GameState *game, Difficulty diff) {
    game->playerLane = LANES / 2; // middle lane
    game->playerX = 5;
    game->score = 0;
    game->numObstacles = getObstacleCount(diff);

    for (int i = 0; i < MAX_OBSTACLES; i++) {
        game->obstacles[i].active = 0;
    }

    for (int i = 0; i < game->numObstacles; i++) {
        game->obstacles[i].active = 1;
        game->obstacles[i].lane = rand() % LANES;
        // spread them out on the road towards the right
        int segment = TRACK_WIDTH / game->numObstacles;
        int base = segment * i;
        game->obstacles[i].x = base + (rand() % (segment > 0 ? segment : TRACK_WIDTH));
        if (game->obstacles[i].x >= TRACK_WIDTH) game->obstacles[i].x = TRACK_WIDTH - 1;
    }
}

int playGame(GameState *game, Difficulty diff, Speed speed, int *hasSavedGame) {
    int ch;
    int delay = getDelayMs(speed);

    while (1) {
        // ---- INPUT ----
        ch = getch();
        if (ch != ERR) {
            if ((ch == 'a' || ch == 'A' || ch == KEY_LEFT) && game->playerX > 0) {
                game->playerX--;
            } else if ((ch == 'd' || ch == 'D' || ch == KEY_RIGHT) && game->playerX < TRACK_WIDTH - 6) {
                game->playerX++;
            } else if ((ch == 'w' || ch == 'W' || ch == KEY_UP) && game->playerLane > 0) {
                game->playerLane--;
            } else if ((ch == 's' || ch == 'S' || ch == KEY_DOWN) && game->playerLane < LANES - 1) {
                game->playerLane++;
            } else if (ch == 'q' || ch == 'Q') {
                // quit to menu but keep state = Continue Game
                *hasSavedGame = 1;
                return 0;
            }
        }

        // ---- COLLISION ----
        for (int i = 0; i < game->numObstacles; i++) {
            if (!game->obstacles[i].active) continue;
            if (game->obstacles[i].lane == game->playerLane &&
                (game->obstacles[i].x >= game->playerX &&
                game->obstacles[i].x <= game->playerX + 5)) {
                clear();
                mvprintw(0, 0, "GAME OVER!");
                mvprintw(1, 0, "Final Score: %d", game->score);
                mvprintw(3, 0, "Press any key to return to menu...");
                refresh();
                nodelay(stdscr, FALSE);
                getch();
                nodelay(stdscr, TRUE);
                *hasSavedGame = 0;
                return 0;
            }
        }

        // ---- DRAW ----
        clear();
        mvprintw(0, 0, "Controls: W/S (lanes), A/D (left/right), Q (back to menu)");
        mvprintw(1, 0, "Score: %d   |   Difficulty: %s   |   Speed: %s",
                 game->score, difficultyName(diff), speedName(speed));

        // top border
        mvprintw(2, 0, "+");
        for (int i = 0; i < TRACK_WIDTH; i++) printw("-");
        printw("+");

        // lanes
        for (int lane = 0; lane < LANES; lane++) {
            char line[TRACK_WIDTH + 1];
            for (int i = 0; i < TRACK_WIDTH; i++) line[i] = ' ';
            line[TRACK_WIDTH] = '\0';

            // obstacles
            for (int i = 0; i < game->numObstacles; i++) {
                if (!game->obstacles[i].active) continue;
                if (game->obstacles[i].lane == lane &&
                    game->obstacles[i].x >= 0 && game->obstacles[i].x < TRACK_WIDTH) {
                    line[game->obstacles[i].x] = 'X';
                }
            }

            // player
            // draw BIG SPACESHIP (3 rows tall)
if (lane == game->playerLane) {
    // Middle body
    if (game->playerX >= 0 && game->playerX + 4 < TRACK_WIDTH) {
        line[game->playerX]     = '=';
        line[game->playerX + 1] = '=';
        line[game->playerX + 2] = '|';
        line[game->playerX + 3] = '=';
        line[game->playerX + 4] = '=';
        if (game->playerX + 5 < TRACK_WIDTH) line[game->playerX + 5] = '>';
    }
}

// TOP WING
if (lane == game->playerLane - 1) {
    if (game->playerX + 2 < TRACK_WIDTH) {
        line[game->playerX + 2] = '/';
        line[game->playerX + 3] = '\\';
    }
}

// BOTTOM WING
if (lane == game->playerLane + 1) {
    if (game->playerX + 2 < TRACK_WIDTH) {
        line[game->playerX + 2] = '\\';
        line[game->playerX + 3] = '/';
    }
}

            mvprintw(3 + lane, 0, "|%s|", line);
        }

        // bottom border
        mvprintw(3 + LANES, 0, "+");
        for (int i = 0; i < TRACK_WIDTH; i++) printw("-");
        printw("+");

        refresh();
        napms(delay);

        // ---- UPDATE OBSTACLES ----
        for (int i = 0; i < game->numObstacles; i++) {
            if (!game->obstacles[i].active) continue;
            game->obstacles[i].x--; // move right -> left

            if (game->obstacles[i].x < 0) {
                // respawn on right in random lane
                game->obstacles[i].lane = rand() % LANES;
                game->obstacles[i].x = TRACK_WIDTH - 1;
                game->score++;
            }
        }
    }
}

// ---- main ----

int main() {
    // ncurses init
    initscr();
    noecho();
    cbreak();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    srand((unsigned int)time(NULL));

    Difficulty diff = DIFF_EASY;
    Speed speed = SPEED_SLOW;
    GameState game;
    int hasSavedGame = 0;

    while (1) {
        int menuChoice = mainMenu(hasSavedGame);

        if (menuChoice == 0) {         // Start New Game
            initGame(&game, diff);
            hasSavedGame = 1;
            playGame(&game, diff, speed, &hasSavedGame);
        } else if (menuChoice == 1) {  // Continue Game
            if (hasSavedGame) {
                playGame(&game, diff, speed, &hasSavedGame);
            }
        } else if (menuChoice == 2) {  // Options
            optionsMenu(&diff, &speed);
        } else if (menuChoice == 3) {  // Exit
            break;
        }
    }

    endwin();
    return 0;
}
