#include "./snake.cpp"
#include "./main.h"
#include <iostream>
#include <unistd.h> //시간
#include <cstdlib> //랜덤쓰기위해
#include <ctime>
#include <string>
int main() {
    srand((unsigned int)time(NULL));
    // ncurses 초기화
    initscr();
    curs_set(0);  // 커서 숨기기
    keypad(stdscr, true);  // 특수 키(화살표 등) 입력 활성화
    nodelay(stdscr, true);  // 입력 대기 없이 바로 반환


    start_color();  // 색상 초기화
    cbreak();
    noecho();

    // 게임 화면 크기 설정
    int width = 200;
    int height = 30;
    resize_term(height, width);
    refresh();

    //아이템과 워프 시간 생성
    int count = 0;
    // Snake 객체 생성
    Snake snake;
    bool winner_check = false; // 이겼는지 졌는지 판단 
    long long unsigned int time_count = 0; //시간초
    snake.setstage(0);
    int stage = 0; //1 스테이지
    // 게임 루프
    run(snake,stage,winner_check);
    if(winner_check){
        wait_run();
        winner_check = false;
        stage = 1; //다음 스테이지
        snake.reset();
        snake.setstage(stage);
        run(snake,stage,winner_check);
    }

    if(winner_check){
        wait_run();
        winner_check = false;
        stage = 2; //다음 스테이지
        snake.reset();
        snake.setstage(stage);
        run(snake,stage,winner_check);
    }

    if(winner_check){
        wait_run();
        winner_check = false;
        stage = 3; //다음 스테이지
        snake.reset();
        snake.setstage(stage);
        run(snake,stage,winner_check);
    }
    endwin(); // ncurses 종료
    if(winner_check == true){
        win(snake);
    }
    lose(snake);
    return 0;
}


void Color(int stage){
    init_pair(1,COLOR_BLUE,COLOR_BLUE);
    init_pair(4,COLOR_GREEN,COLOR_GREEN);
    init_pair(5,COLOR_YELLOW,COLOR_YELLOW);
    init_pair(6,COLOR_WHITE,COLOR_WHITE);
    init_pair(7,COLOR_CYAN,COLOR_CYAN);
    init_pair(8, COLOR_MAGENTA, COLOR_MAGENTA);

    for(int row= 0; row < 30; row++){
        for(int col = 0; col < 30; col++){
            if(map[stage][row][col] == 1 || map[stage][row][col] == 9){ //벽
                attron(COLOR_PAIR(1));
                mvprintw(row, col, "0");
                attroff(COLOR_PAIR(1));
            }
            if(map[stage][row][col] == 2){ // +아이템
                attron(COLOR_PAIR(4));
                mvprintw(row, col, "0");
                attroff(COLOR_PAIR(4));
            }
            if(map[stage][row][col] == 3){ //-아이템
                attron(COLOR_PAIR(5));
                mvprintw(row, col, "0");
                attroff(COLOR_PAIR(5));
            }
            if(map[stage][row][col] == 4){ // 게이트
                attron(COLOR_PAIR(6));
                mvprintw(row, col, "0");
                attroff(COLOR_PAIR(6));
            }
            if(map[stage][row][col] == 5){ //빨간아이템
                attron(COLOR_PAIR(3));
                mvprintw(row, col, "0");
                attroff(COLOR_PAIR(3));
            }
            if(map[stage][row][col] == 6){ //고드름아이템
                attron(COLOR_PAIR(7));
                mvprintw(row, col, "0");
                attroff(COLOR_PAIR(7));
            }
            if(map[stage][row][col] == 7){ //핑크아이템
                attron(COLOR_PAIR(8));
                mvprintw(row, col, "0");
                attroff(COLOR_PAIR(8));
            }
            if(map[stage][row][col] == 8){ //빨간벽
                attron(COLOR_PAIR(3));
                mvprintw(row, col, "0");
                attroff(COLOR_PAIR(3));
            }
        }
    }
    
    
}

void item(Snake& snake, int stage){
    for(int row= 0; row < 30; row++){
        for(int col = 0; col < 30; col++){
            if(map[stage][row][col] == 2 || map[stage][row][col] == 3 || map[stage][row][col] == 5 || map[stage][row][col] == 6 || map[stage][row][col] == 7) map[stage][row][col] = 0;
        }
    }
    int pos_x = std::rand() % 30;
    int pos_y = std::rand() % 30;
    int s = std::rand() % 5; // 좋은 아이템인지 나쁜 아이템인지
    while(true){ //snake 와 위치가 같거나 벽이랑 위치가 같으면 안되기 때문에 위치가 갖지 않을 때까지 반복
        if(map[stage][pos_y][pos_x] == 0){
            auto body = snake.getv();
            int cnt = 0;
            for(Position p : body ){
                if(p.x == pos_x && p.y == pos_y) cnt++;
            }
            if(cnt == 0) break;
        }
        pos_x = std::rand() % 30;
        pos_y = std::rand() % 30;
    }
    if(s == 0){
        map[stage][pos_y][pos_x] = 2;
    }
    else if(s == 1){
        map[stage][pos_y][pos_x] = 3;
    }
    else if(s == 2){
        map[stage][pos_y][pos_x] = 5;
    }
    else if(s ==3){
        map[stage][pos_y][pos_x] = 6;
    }
    else{
        map[stage][pos_y][pos_x] = 7;
    }
    
}


void gate(Snake& snake, int stage){
    for(int row= 0; row < 30; row++){
        for(int col = 0; col < 30; col++){
            if(map[stage][row][col] == 4 ||map[stage][row][col] == 8) map[stage][row][col] = 1;
        }
    }
    int start_pos_x = std::rand() % 30;
    int start_pos_y = std::rand() % 30;

    int end_pos_x = std::rand() % 30;
    int end_pos_y = std::rand() % 30;
    int s = std::rand() % 2;
    while(true){
        if( start_pos_x != end_pos_x || start_pos_y != end_pos_y ){ // 게이트 두 개가 서로 위치가 달라야되기 때문에 
            if(map[stage][start_pos_y][start_pos_x] == 1 && map[stage][end_pos_y][end_pos_x] == 1) { //게이트는 벽에 나타나야되기 때문에
                if(s == 0){
                    map[stage][start_pos_y][start_pos_x] = 4;
                    map[stage][end_pos_y][end_pos_x] = 4;
                }
                else{
                    map[stage][start_pos_y][start_pos_x] = 8;
                    map[stage][end_pos_y][end_pos_x] = 8;
                }
                snake.gateList[0] = start_pos_y;
                snake.gateList[1] = start_pos_x;
                snake.gateList[2] = end_pos_y;
                snake.gateList[3] = end_pos_x;
                break;
            }
        }
        start_pos_x = std::rand() % 30;
        start_pos_y = std::rand() % 30;

        end_pos_x = std::rand() % 30;
        end_pos_y = std::rand() % 30;
    }
}

bool box(Snake& snake, int& time, int& min ,int stage){
    int height = 25;
    int width = 30;
    int start_y = 5;
    int start_x = 45;

    WINDOW* win = newwin(height, width, start_y, start_x);
    refresh();

    box(win, 0, 0);  // 테두리 그리기
    const std::string growth_count = std::to_string(snake.getgrowthcount());
    const std::string total_growth = std::to_string(snake.gettotalgrowthcount());
    bool growh_check = false;
    const std::string posion_count = std::to_string(snake.getposioncount());
    const std::string total_posion = std::to_string(snake.gettotalposioncount());
    bool posion_check = false;
    const std::string gate_count = std::to_string(snake.getgatecount());
    const std::string total_gate = std::to_string(snake.gettotalgatecount());
    bool gate_check = false;
    const std::string length = std::to_string(snake.getlength());
    const std::string max_length = std::to_string(snake.getMaxlength());
    bool length_check = false;
    const std::string Time = std::to_string(time);
    const std::string Min = std::to_string(min);
    const std::string Stage = std::to_string(stage+1);
    const std::string shield = std::to_string(snake.getshield());
    // 직사각형 내부에 텍스트 출력
    mvwprintw(win, 1, 9, "<Score Board>");
    mvwprintw(win, 2, 1, "  Length : ");
    mvwprintw(win, 2, 10, length.c_str());
    mvwprintw(win, 2, 11, "/");
    mvwprintw(win, 2, 12, max_length.c_str());
    mvwprintw(win, 4, 1, "+ item : ");
    mvwprintw(win, 4, 10, growth_count.c_str());
    mvwprintw(win, 4, 11, "/");
    mvwprintw(win, 4, 12, total_growth.c_str());
    mvwprintw(win, 6, 1, "- item : ");
    mvwprintw(win, 6, 10, posion_count.c_str());
    mvwprintw(win, 6, 11, "/");
    mvwprintw(win, 6, 12, total_posion.c_str());
    mvwprintw(win, 8, 1, "  gate : ");
    mvwprintw(win, 8, 10, gate_count.c_str());
    mvwprintw(win, 8, 11, "/");
    mvwprintw(win, 8, 12, total_gate.c_str());
    mvwprintw(win, 9, 1,  "  Time : ");
    mvwprintw(win, 9, 10, Min.c_str());
    mvwprintw(win, 9, 11, ":");
    mvwprintw(win, 9, 12, Time.c_str());
    

    mvwprintw(win, 10, 10, "<Mission>");
    if(snake.getlength() >= 6){
        mvwprintw(win, 11, 1, "  Length : 6 (V) ");
        length_check = true;
    }
    else{
        mvwprintw(win, 11, 1, "  Length : 6 ( ) ");
    }
    if(snake.getgrowthcount() >= 3){
        mvwprintw(win, 13, 1, "+ item : 3 (V) ");
        growh_check = true;
    }
    else{
        mvwprintw(win, 13, 1, "+ item : 3 ( ) ");
    }
    if(snake.getposioncount() >= 3){
        mvwprintw(win, 15, 1, "- item : 3 (V) ");
        posion_check = true;
    }
    else{
        mvwprintw(win, 15, 1, "- item : 3 ( ) ");
    }
    if(snake.getgatecount() >= 3){
        mvwprintw(win, 17, 1, "  gate : 3 (V) ");
        gate_check = true;
    }
    else{
        mvwprintw(win, 17, 1, "  gate : 3 ( ) ");
    }
    mvwprintw(win, 19, 1, "  stage : ");
    mvwprintw(win, 19, 10, Stage.c_str());
    mvwprintw(win, 21, 1, "  Shield : ");
    mvwprintw(win, 21, 12, shield.c_str());
    
    wrefresh(win);
    if(gate_check == true && growh_check == true && length_check == true && posion_check == true){
        return true;
    }
    return false;
}

void run(Snake& snake, int stage, bool& winner_check){
    //아이템과 워프 시간 생성
    int count = 0;
    while (true) {
        clear();  // 화면 지우기

        // 입력 처리
        int ch = getch();
        int change;
        switch (ch) {
            case KEY_LEFT:
                change = snake.changeDirection('l');
                break;
            case KEY_RIGHT:
                change = snake.changeDirection('r');
                break;
            case KEY_UP:
                change = snake.changeDirection('u');
                break;
            case KEY_DOWN:
                change = snake.changeDirection('d');
                break;
        }
        if(change == 0) break; // 방향이 반대면 종료
        Color(stage);
        snake.move();  // Snake 이동

        if (snake.checkCollision()) {
            break;  // 충돌 시 게임 종료
        }
        snake.eat();
        if(snake.die()) break;


        snake.draw();  // Snake 그리기

        count++;
        if(count % 10 == 0) {
            time_count++;
            if(time_count == 60){
                time_count = 0;
                min++;
            }
        }
        if(count == 50){ //먹이 구현
            item(snake,stage);
            gate(snake,stage);
            count = 0;
        }
        if(box(snake,time_count,min,stage)){
            winner_check = true;
            break;
        }

        refresh();
        usleep(100000);  // 0.1초 대기
    }
}

void wait_run(){
    int count = 0;
    int n = 0;
    clear();
    while(count != 3){
        for(int row= 0; row < 30; row++){
            for(int col = 0; col < 30; col++){
                if(Wait[count][row][col] == 1){
                    attron(COLOR_PAIR(1));
                    mvprintw(row, col, "0");
                    attroff(COLOR_PAIR(1));
                }
            }
        }
        n++;
        if(n % 20 == 0) {
            count++; //2초 카운트 다운
            clear();
        }
        refresh();
        usleep(100000);  // 0.1초 대기
    }
}

void win(Snake& snake){
    int height = 20;
    int width = 30;
    int start_y = 5;
    int start_x = 45;

    WINDOW* winner = newwin(height, width, start_y, start_x);
    refresh();

    int count = 0;
    int n = 0;

    const std::string total_growth = std::to_string(snake.gettotalgrowthcount());
    const std::string total_posion = std::to_string(snake.gettotalposioncount());
    const std::string total_gate = std::to_string(snake.gettotalgatecount());
    const std::string max_length = std::to_string(snake.getMaxlength());
    const std::string Time = std::to_string(time_count);
    const std::string Min = std::to_string(min);
    clear();
    while(count != 3){
        box(winner, 0, 0);
        mvwprintw(winner, 1, 9, "<Score Board>");
        mvwprintw(winner, 2, 1, "  Max Length   : ");
        mvwprintw(winner, 2, 18, max_length.c_str());
        mvwprintw(winner, 4, 1, "  total + item : ");
        mvwprintw(winner, 4, 18, total_growth.c_str());
        mvwprintw(winner, 6, 1, "  total - item : ");
        mvwprintw(winner, 6, 18, total_posion.c_str());
        mvwprintw(winner, 8, 1, "  total gate   : ");
        mvwprintw(winner, 8, 18, total_gate.c_str());
        mvwprintw(winner, 9, 1,  "     Time     : ");
        mvwprintw(winner, 9, 18, Min.c_str());
        mvwprintw(winner, 9, 19, ":");
        mvwprintw(winner, 9, 20, Time.c_str());
        mvwprintw(winner, 11, 10, "!!!You Win!!!");
        wrefresh(winner);
        n++;
        if(n % 10 == 0) {
            count++; //1초 카운트 다운
        }
        refresh();
        usleep(100000);  // 0.1초 대기
    }
}

void lose(Snake& snake){
    int height = 20;
    int width = 30;
    int start_y = 5;
    int start_x = 45;

    WINDOW* loser = newwin(height, width, start_y, start_x);
    refresh();

    int count = 0;
    int n = 0;
    const std::string total_growth = std::to_string(snake.gettotalgrowthcount());
    const std::string total_posion = std::to_string(snake.gettotalposioncount());
    const std::string total_gate = std::to_string(snake.gettotalgatecount());
    const std::string max_length = std::to_string(snake.getMaxlength());
    const std::string Time = std::to_string(time_count);
    const std::string Min = std::to_string(min);
    clear();
    while(count != 3){
        box(loser, 0, 0);
        mvwprintw(loser, 1, 9, "<Score Board>");
        mvwprintw(loser, 2, 1, "  Max Length   : ");
        mvwprintw(loser, 2, 18, max_length.c_str());
        mvwprintw(loser, 4, 1, "  total + item : ");
        mvwprintw(loser, 4, 18, total_growth.c_str());
        mvwprintw(loser, 6, 1, "  total - item : ");
        mvwprintw(loser, 6, 18, total_posion.c_str());
        mvwprintw(loser, 8, 1, "  total gate   : ");
        mvwprintw(loser, 8, 18, total_gate.c_str());
        mvwprintw(loser, 9, 1,  "     Time     : ");
        mvwprintw(loser, 9, 18, Min.c_str());
        mvwprintw(loser, 9, 19, ":");
        mvwprintw(loser, 9, 20, Time.c_str());
        mvwprintw(loser, 11, 10, "You lose~~~");
        wrefresh(loser);
        n++;
        if(n % 10 == 0) {
            count++; //1초 카운트 다운
        }
        refresh();
        usleep(100000);  // 0.1초 대기
    }
}