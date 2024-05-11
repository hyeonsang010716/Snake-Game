class Snake;
void Color(int);
void gate(Snake& , int);
void item(Snake&, int );
bool box(Snake&,int&, int&, int);
void run(Snake&, int,bool&);
void wait_run();
void win(Snake&);
void lose(Snake&);
int time_count = 0; //시간초
int min = 0;