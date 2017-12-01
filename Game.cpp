#include <iostream>
#include <stdlib.h>
#include <ncurses.h>
#include <queue>
#include <fstream>
#include <string>
using namespace std;

const int width = 15;
const int height = 25;
queue<int> inputQueue;

FILE* file;
int scores[3];
char names[3][20];

struct pos {
    int x;
    int y;

    bool operator ==(pos other) {
        return (x == other.x && y == other.y);
    }
};

pos player;
vector<pos> enemies;
vector<pos> bullets;

int score = 0;

void input(){
    int x;
    timeout(20);
    x = getch();
    inputQueue.push(x);
}

void draw(){
    clear();
    attron(COLOR_PAIR(1));
    mvprintw(0, 0, " ");
    for (int i = 1; i < width - 1; i++) {
        printw(" ");
    }
    printw(" \n");

    for (int i = 1; i < height - 1; i++) {
        mvprintw(i, 0, " ");
        mvprintw(i, width - 1, " ");
    }

    mvprintw(height - 1, 0, " ");
    for (int i = 1; i < width - 1; i++) {
        printw(" ");
    }
    printw(" \n");

    attron(COLOR_PAIR(2));
    mvprintw(player.y, player.x, "^");
    attroff(COLOR_PAIR(2));

    attron(COLOR_PAIR(4));
    for (int i = 0; i < bullets.size(); i++) {
        mvprintw(bullets[i].y, bullets[i].x, "|");
    }
    attroff(COLOR_PAIR(4));

    attron(COLOR_PAIR(3));
    for (int i = 0; i < enemies.size(); i++) {
        mvprintw(enemies[i].y, enemies[i].x, "#");
    }
    attroff(COLOR_PAIR(3));

    mvprintw(height / 2, width + 5, "Score: %d", score);
}


bool run = true;
void stopGame() {
    run = false;
}

int counter = 0;
void physics() {
    for (int i = 0; i < bullets.size(); i++) {
        for(int x = 0; x < enemies.size(); x++){
            if(enemies[x] == bullets[i]){
                enemies.erase(enemies.begin() + x);
                x--;
                bullets.erase(bullets.begin() + i);
                i--;

                score+=10;
            }
        }
    }

    for (int i = 0; i < bullets.size(); i++) {
        bullets[i].y -= 1;
        if (bullets[i].y < 2) {
            bullets.erase(bullets.begin() + i);
            i--;
        }
    }

    if(counter % 25 == 0){
        for (int i = 0; i < enemies.size(); i++) {
            enemies[i].y += 1;
            if(enemies[i].y == height-1) {
                enemies.erase(enemies.begin() + i);
                stopGame();
                i--;
            }
        }
    }

    counter++;
    if(counter == 50) {
        counter = 0;
        pos ast;
        ast.x = (rand() % (width - 2)) + 1;
        ast.y = 1;
        enemies.push_back(ast);
    }

    while (!inputQueue.empty()) {
        int key = inputQueue.front();
        inputQueue.pop();

        if(key == -1) continue;
        
        if(key == 3) {
            stopGame();
        }

        if (key == 'a') {
            player.x -= 1;
            if (player.x < 1) player.x += 1;
        }

        else if (key == 'd') {
            player.x += 1;
            if (player.x >= width - 1) player.x -= 1;
        }

        else if(key == 'w'){
            pos bul;
            bul.x = player.x;
            bul.y = player.y-1;
            bullets.push_back(bul);
        }
    }

    draw();
}

void setup(){
    initscr();
    raw();
    curs_set(FALSE);
    noecho();

    start_color();
    init_pair(1, COLOR_BLACK, COLOR_GREEN); //Border
    init_pair(2, COLOR_WHITE, COLOR_BLACK); //Player
    init_pair(3, COLOR_RED, COLOR_BLACK); //Enemy
    init_pair(4, COLOR_YELLOW, COLOR_BLACK); //Bullet

    player.x = width / 2;
    player.y = height - 2;
}

int main(){
    file = fopen("scores", "r");
    for(int i = 0; i < 3; i++){
        fscanf(file, "%d", &scores[i]);
        fscanf(file, "%s", names[i]);
    }
    fclose(file);

    setup();

    while (run) {
        input();
        physics();
    }
    endwin();
    system("clear");

    for(int i = 0; i < 3; i++) {
        if (score > scores[i]) {            
            scores[i] = score;
            cout <<"Score: "<< score<<endl;
            cout <<"Enter Name: ";
            cin >> names[i];
            break;
        }
    }

    file = fopen("scores", "w");
    for(int i = 0; i < 3; i++) {
        fprintf(file,"%d ",scores[i]);
        fprintf(file,"%s",names[i]);
        fprintf(file,"\n");
    }
    fclose(file);
    system("clear");
    return 0;
}
