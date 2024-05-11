#include <vector>
#include <ncurses.h>
#include "./map.cpp"
// Snake의 좌표 구조체
struct Position {
    int x, y;
};

// Snake 클래스 정의
class Snake {
public:
    int gateList[4]; //gate 위치 저장
    Snake(){
        // 초기 Snake의 길이와 방향 설정
        width = 30;
        height = 30;
        length = 5;
        max_length = length;
        gate_count = 0; //게이트 탔을 때 카운트
        gate_total_count = 0;
        growth_item_count = 0; //+먹었을 때 카운트
        growwth_total_count = 0;
        poison_item_count = 0; //-먹었을 때 카운트
        posion_total_count = 0;
        shield = 0;
        direction = 'r'; //시작 방향
        for(int i = 0 ; i < 4; i++){
            gateList[i] = 0;
        }
        // 초기 Snake의 위치 설정
        for (int i = 0; i < length; ++i) {
            body.push_back({width/2 - i, height/2}); //시작지점
        }
    }
    void reset(){
        body.clear();
        gate_count = 0; //게이트 탔을 때 카운트
        growth_item_count = 0; //+먹었을 때 카운트
        poison_item_count = 0; //-먹었을 때 카운트
        direction = 'r'; //시작방향
        for (int i = 0; i < 3; ++i) {
            body.push_back({width/2 - i, 5}); //시작지점
        }

    }
    void setstage(int s){
        stage = s;
    }
    auto getv(){
        return body; 
    }
    int getshield(){
        return shield;
    }
    int getlength(){
        return length;
    }
    int getMaxlength(){
        return max_length;
    }
    int getgrowthcount(){
        return growth_item_count;
    }
    int gettotalgrowthcount(){
        return growwth_total_count;
    }
    int getposioncount(){
        return poison_item_count;
    }
    int gettotalposioncount(){
        return posion_total_count;
    }
    int getgatecount(){
        return gate_count;
    }
    int gettotalgatecount(){
        return gate_total_count;
    }

    int changeDirection(char newDirection) {
        // 반대 방향으로 진행할 수 없도록 방향 변경
        if ((newDirection == 'l' && direction != 'r') ||
            (newDirection == 'r' && direction != 'l') ||
            (newDirection == 'u' && direction != 'd') ||
            (newDirection == 'd' && direction != 'u')) {
            direction = newDirection;
            return 1;
        }
        else{
            return 0;
        }
    }

    void move() {
        // Snake의 머리 위치 계산
        int x = body.front().x;
        int y = body.front().y;

        // 방향에 따라 머리 위치 업데이트
        switch (direction) {
            case 'l':
                x--;
                break;
            case 'r':
                x++;
                break;
            case 'u':
                y--;
                break;
            case 'd':
                y++;
                break;
        }

        // 머리 위치를 Snake 몸통의 앞에 추가
        body.insert(body.begin(), {x, y});

         // Snake가 길어지면 꼬리를 삭제하여 길이 유지
        if (body.size() > length) {
            body.pop_back();
        }
    }

    bool checkCollision() {
        // 벽과의 충돌 여부 확인
        int x = body.front().x;
        int y = body.front().y;
        if(map[stage][y][x] == 1){
            return true;
        }
        if(map[stage][y][x] == 4 || map[stage][y][x] == 8 ){ //벽이 아니라 게이트인지 확인
            go_gate();
        }

        // 자신의 몸과의 충돌 여부 확인
        for (size_t i = 1; i < body.size(); ++i) {
            if (body[i].x == x && body[i].y == y) {
                return true;
            }
        }

        return false;
    }


    void eat(){ //방향에 따라 먹는 위치 갱신
        if( map[stage][body[0].y][body[0].x] == 7){ //쉴드 생성
            shield++;
            map[stage][body[0].y][body[0].x] = 0;
        }
        //+물약
        if(direction == 'l' && ( map[stage][body[0].y][body[0].x] == 2 )){
            body.push_back({body[0].x-1,body[0].y});
            map[stage][body[0].y][body[0].x] = 0;
            length++;
            growth_item_count++;
            growwth_total_count++;
        }
        else if(direction == 'r' && ( map[stage][body[0].y][body[0].x] == 2 )){
            body.push_back({body[0].x+1,body[0].y});
            map[stage][body[0].y][body[0].x] = 0;
            length++;
            growth_item_count++;
            growwth_total_count++;
        }
        else if(direction == 'u' && ( map[stage][body[0].y][body[0].x] == 2 )){
            body.push_back({body[0].x,body[0].y-1});
            map[stage][body[0].y][body[0].x] = 0;
            length++;
            growth_item_count++;
            growwth_total_count++;
        }
        else if(direction == 'd' && ( map[stage][body[0].y][body[0].x] == 2 )){
            body.push_back({body[0].x,body[0].y+1});
            map[stage][body[0].y][body[0].x] = 0;
            length++;
            growth_item_count++;
            growwth_total_count++;
        }
        //-물약
        else if(direction == 'l' && ( map[stage][body[0].y][body[0].x] == 3 )){
            if(shield > 0){
                shield--;
                poison_item_count++;
                posion_total_count++;
                map[stage][body[0].y][body[0].x] = 0;
            }
            else{
                body.pop_back();
                map[stage][body[0].y][body[0].x] = 0;
                length--;
                poison_item_count++;
                posion_total_count++;
            }
        }
        else if(direction == 'r' && ( map[stage][body[0].y][body[0].x] == 3 )){
            if(shield > 0){
                shield--;
                poison_item_count++;
                posion_total_count++;
                map[stage][body[0].y][body[0].x] = 0;
            }
            else{
                body.pop_back();
                map[stage][body[0].y][body[0].x] = 0;
                length--;
                poison_item_count++;
                posion_total_count++;
            }
        }
        else if(direction == 'u' && ( map[stage][body[0].y][body[0].x] == 3 )){
            if(shield > 0){
                shield--;
                poison_item_count++;
                posion_total_count++;
                map[stage][body[0].y][body[0].x] = 0;
            }
            else{
                body.pop_back();
                map[stage][body[0].y][body[0].x] = 0;
                length--;
                poison_item_count++;
                posion_total_count++;
            }
        }
        else if(direction == 'd' && ( map[stage][body[0].y][body[0].x] == 3 )){
            if(shield > 0){
                shield--;
                poison_item_count++;
                posion_total_count++;
                map[stage][body[0].y][body[0].x] = 0;
            }
            else{
                body.pop_back();
                map[stage][body[0].y][body[0].x] = 0;
                length--;
                poison_item_count++;
                posion_total_count++;
            }
        }
        //빨간물약
        else if(direction == 'l' && ( map[stage][body[0].y][body[0].x] == 5 )){
            int cnt = 0;
            while(length != 3){
                if(shield > 0){
                    shield--;
                }
                else{
                    length--;
                    body.pop_back();
                }
                cnt++;
            }
            map[stage][body[0].y][body[0].x] = 0;
            poison_item_count += cnt;
            posion_total_count += cnt;
        }
        else if(direction == 'r' && ( map[stage][body[0].y][body[0].x] == 5 )){
            int cnt = 0;
            while(length != 3){
                if(shield > 0){
                    shield--;
                }
                else{
                    length--;
                    body.pop_back();
                }
                cnt++;
            }
            map[stage][body[0].y][body[0].x] = 0;
            poison_item_count += cnt;
            posion_total_count += cnt;
        }
        else if(direction == 'u' && ( map[stage][body[0].y][body[0].x] == 5 )){
            int cnt = 0;
            while(length != 3){
                if(shield > 0){
                    shield--;
                }
                else{
                    length--;
                    body.pop_back();
                }
                cnt++;
            }
            map[stage][body[0].y][body[0].x] = 0;
            poison_item_count += cnt;
            posion_total_count += cnt;
        }
        else if(direction == 'd' && ( map[stage][body[0].y][body[0].x] == 5 )){
            int cnt = 0;
            while(length != 3){
                if(shield > 0){
                    shield--;
                }
                else{
                    length--;
                    body.pop_back();
                }
                cnt++;
            }
            map[stage][body[0].y][body[0].x] = 0;
            poison_item_count += cnt;
            posion_total_count += cnt;
        } 
        //고드름물약
        else if(direction == 'l' && ( map[stage][body[0].y][body[0].x] == 6 )){
            body.pop_back();
            body.pop_back();
            map[stage][body[0].y][body[0].x] = 0;
            length -= 2; poison_item_count += 2; posion_total_count +=2;
            growth_item_count++; growwth_total_count++;
            gate_count++; gate_total_count++;

        }
        else if(direction == 'r' && ( map[stage][body[0].y][body[0].x] == 6 )){
            body.pop_back();
            body.pop_back();
            map[stage][body[0].y][body[0].x] = 0;
            length -= 2; poison_item_count += 2; posion_total_count +=2;
            growth_item_count++; growwth_total_count++;
            gate_count++; gate_total_count++;
        }
        else if(direction == 'u' && ( map[stage][body[0].y][body[0].x] == 6 )){
            body.pop_back();
            body.pop_back();
            map[stage][body[0].y][body[0].x] = 0;
            length -= 2; poison_item_count += 2; posion_total_count +=2;
            growth_item_count++; growwth_total_count++;
            gate_count++; gate_total_count++;
        }
        else if(direction == 'd' && ( map[stage][body[0].y][body[0].x] == 6 )){
            body.pop_back();
            body.pop_back();
            map[stage][body[0].y][body[0].x] = 0;
            length -= 2; poison_item_count += 2; posion_total_count +=2;
            growth_item_count++; growwth_total_count++;
            gate_count++; gate_total_count++;
        }
        if(length > max_length ) max_length = length;
    }
    void go_gate(){ //시계방향 순서로 게이트 탈 수 있게 동작
        if(body[0].y == this->gateList[0] && body[0].x == this->gateList[1]){
            if(direction == 'l'){
                if(  gateList[3]-1 > 0 && map[stage][gateList[2]][gateList[3]-1] == 0 ){
                        body[0].x = gateList[3]-1;
                        body[0].y = gateList[2];
                }
                else if(  gateList[2]-1 > 0 && map[stage][gateList[2]-1][gateList[3]] == 0 ){
                        body[0].x = gateList[3];
                        body[0].y = gateList[2]-1;
                        direction = 'u';
                }
                else if(  gateList[3]+1 < 29 && map[stage][gateList[2]][gateList[3]+1] == 0 ){
                        body[0].x = gateList[3]+1;
                        body[0].y = gateList[2];
                        direction = 'r';
                }
                else if(  gateList[2]+1 < 29 && map[stage][gateList[2]+1][gateList[3]] == 0 ){
                        body[0].x = gateList[3];
                        body[0].y = gateList[2]+1;
                        direction = 'd';
                }

            }
            if(direction == 'd'){
                if(  gateList[2]+1 < 29 && map[stage][gateList[2]+1][gateList[3]] == 0 ){
                        body[0].x = gateList[3];
                        body[0].y = gateList[2]+1;
                }

                else if(  gateList[3]-1 > 0 && map[stage][gateList[2]][gateList[3]-1] == 0 ){
                        body[0].x = gateList[3]-1;
                        body[0].y = gateList[2];
                        direction = 'l';
                }
                else if(  gateList[2]-1 > 0 && map[stage][gateList[2]-1][gateList[3]] == 0 ){
                        body[0].x = gateList[3];
                        body[0].y = gateList[2]-1;
                        direction = 'u';
                }
                else if(  gateList[3]+1 < 29 && map[stage][gateList[2]][gateList[3]+1] == 0 ){
                        body[0].x = gateList[3]+1;
                        body[0].y = gateList[2];
                        direction = 'r';
                }
            }
            if(direction == 'r'){
                if(  gateList[3]+1 < 29 && map[stage][gateList[2]][gateList[3]+1] == 0 ){
                        body[0].x = gateList[3]+1;
                        body[0].y = gateList[2];
                }
                else if(  gateList[2]+1 < 29 && map[stage][gateList[2]+1][gateList[3]] == 0 ){
                        body[0].x = gateList[3];
                        body[0].y = gateList[2]+1;
                        direction = 'd';
                }
                else if(  gateList[3]-1 > 0 && map[stage][gateList[2]][gateList[3]-1] == 0 ){
                        body[0].x = gateList[3]-1;
                        body[0].y = gateList[2];
                        direction = 'l';
                }
                else if(  gateList[2]-1 > 0 && map[stage][gateList[2]-1][gateList[3]] == 0 ){
                        body[0].x = gateList[3];
                        body[0].y = gateList[2]-1;
                        direction = 'u';
                }
            }
            if(direction == 'u'){
                if(  gateList[2]-1 > 0 && map[stage][gateList[2]-1][gateList[3]] == 0 ){
                        body[0].x = gateList[3];
                        body[0].y = gateList[2]-1;       
                }
                if(  gateList[3]+1 < 29 && map[stage][gateList[2]][gateList[3]+1] == 0 ){
                        body[0].x = gateList[3]+1;
                        body[0].y = gateList[2];
                        direction = 'r';
                }    
                else if(  gateList[2]+1 < 29 && map[stage][gateList[2]+1][gateList[3]] == 0 ){
                        body[0].x = gateList[3];
                        body[0].y = gateList[2]+1;
                        direction = 'd';
                }
                else if(  gateList[3]-1 > 0 && map[stage][gateList[2]][gateList[3]-1] == 0 ){
                        body[0].x = gateList[3]-1;
                        body[0].y = gateList[2];
                        direction = 'l';
                }
            }
            gate_count++;
            gate_total_count++;
            if(map[stage][gateList[0]][gateList[1]] == 8){
                if(shield > 0){
                    shield--;
                }
                else{
                    body.pop_back();
                    length--;
                }
                gate_count++;
                gate_total_count++;
            }
        }
        else if(body[0].y == this->gateList[2] && body[0].x == this->gateList[3]){
            if(direction == 'l'){
                if(  gateList[1]-1 > 0 && map[stage][gateList[0]][gateList[1]-1] == 0 ){
                        body[0].x = gateList[1]-1;
                        body[0].y = gateList[0];
                }
                else if(  gateList[0]-1 > 0 && map[stage][gateList[0]-1][gateList[1]] == 0 ){
                        body[0].x = gateList[1];
                        body[0].y = gateList[0]-1;
                        direction = 'u';
                }
                else if(  gateList[1]+1 < 29 && map[stage][gateList[0]][gateList[1]+1] == 0 ){
                        body[0].x = gateList[1]+1;
                        body[0].y = gateList[0];
                        direction = 'r';
                }
                else if(  gateList[1]+1 < 29 && map[stage][gateList[0]+1][gateList[1]] == 0 ){
                        body[0].x = gateList[1];
                        body[0].y = gateList[0]+1;
                        direction = 'd';
                }

            }
            if(direction == 'd'){
                if(  gateList[0]+1 < 29 && map[stage][gateList[0]+1][gateList[1]] == 0 ){
                        body[0].x = gateList[1];
                        body[0].y = gateList[0]+1;
                }

                else if(  gateList[1]-1 > 0 && map[stage][gateList[0]][gateList[1]-1] == 0 ){
                        body[0].x = gateList[1]-1;
                        body[0].y = gateList[0];
                        direction = 'l';
                }
                else if(  gateList[0]-1 > 0 && map[stage][gateList[0]-1][gateList[1]] == 0 ){
                        body[0].x = gateList[1];
                        body[0].y = gateList[0]-1;
                        direction = 'u';
                }
                else if(  gateList[1]+1 < 29 && map[stage][gateList[0]][gateList[1]+1] == 0 ){
                        body[0].x = gateList[1]+1;
                        body[0].y = gateList[0];
                        direction = 'r';
                }
            }
            if(direction == 'r'){
                if(  gateList[0]+1 < 29 && map[stage][gateList[0]][gateList[1]+1] == 0 ){
                        body[0].x = gateList[1]+1;
                        body[0].y = gateList[0];
                }
                else if(  gateList[0]+1 < 29 && map[stage][gateList[0]+1][gateList[1]] == 0 ){
                        body[0].x = gateList[1];
                        body[0].y = gateList[0]+1;
                        direction = 'd';
                }
                else if(  gateList[1]-1 > 0 && map[stage][gateList[0]][gateList[1]-1] == 0 ){
                        body[0].x = gateList[1]-1;
                        body[0].y = gateList[0];
                        direction = 'l';
                }
                else if(  gateList[0]-1 > 0 && map[stage][gateList[0]-1][gateList[1]] == 0 ){
                        body[0].x = gateList[1];
                        body[0].y = gateList[0]-1;
                        direction = 'u';
                }
            }
            if(direction == 'u'){
                if(  gateList[0]-1 > 0 && map[stage][gateList[0]-1][gateList[1]] == 0 ){
                        body[0].x = gateList[1];
                        body[0].y = gateList[0]-1;       
                }
                if(  gateList[1]+1 < 29 && map[stage][gateList[0]][gateList[1]+1] == 0 ){
                        body[0].x = gateList[1]+1;
                        body[0].y = gateList[0];
                        direction = 'r';
                }    
                else if(  gateList[0]+1 < 29 && map[stage][gateList[0]+1][gateList[1]] == 0 ){
                        body[0].x = gateList[1];
                        body[0].y = gateList[0]+1;
                        direction = 'd';
                }
                else if(  gateList[1]-1 > 0 && map[stage][gateList[0]][gateList[1]-1] == 0 ){
                        body[0].x = gateList[1]-1;
                        body[0].y = gateList[0];
                        direction = 'l';
                }
            }
            gate_count++;
            gate_total_count++;
            if(map[stage][gateList[2]][gateList[3]] == 8){
                if(shield > 0){
                    shield--;
                }
                else{
                    body.pop_back();
                    length--;
                }
                gate_count++;
                gate_total_count++;
            }  
        }
    }
    bool die(){ // 길이가 3이하면 게임 끝나게 동작
        if(length < 3) return true;
        return false;
    }

    void draw() {
        // Snake 그리기
        init_pair(2,COLOR_WHITE,COLOR_WHITE);
        init_pair(3,COLOR_RED,COLOR_RED);
        int count = 0;
        for (const auto& segment : body) {
            if(count == 0){
                attron(COLOR_PAIR(3));
                mvprintw(segment.y, segment.x, "0");
                attroff(COLOR_PAIR(3));
                count = 1;
            }
            else{
                attron(COLOR_PAIR(2));
                mvprintw(segment.y, segment.x, "0");
                attroff(COLOR_PAIR(2));
            }
        }
    }

private:
    std::vector<Position> body;
    int width, height;
    int length;
    int max_length;
    char direction;
    int gate_count;
    int gate_total_count;
    int growth_item_count;
    int growwth_total_count;
    int poison_item_count;
    int posion_total_count;
    int stage;
    int shield;
};