#include<conio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<stdio.h>
#include<stdbool.h>
#include<windows.h>

enum{
    DIRECTION_NORTH,
    DIRECTION_WEST,
    DIRECTION_SOUTH,
    DIRECTION_EAST,
    DIRECTION_MAX};

int directions[][2]={
         //DIRECTION_NORTH
    {0,-1},//DIRECTION_WEST
    {-1,0},//DIRECTION_SOUTH
    {0,1},//DIRECTION_EAST
    {1,0}};


#define MAP_WIDTH 9
#define MAP_HEIGHT 9

#define SCREEN_WIDTH 9


enum{
    CELL_TYPE_NONE,
    CELL_TYPE_HARD_BLOCK,
    CELL_TYPE_SOFT_BLOCK,
    CELL_TYPE_EXPLPSEION,
    CELL_TYPE_EXIT,
    CELL_TYPE_AREA,
    CELL_TYPE_BAKU,
    CELL_TYPE_MAX};


int cells[MAP_HEIGHT][MAP_WIDTH];

char cellAA[][2+1]={
        "Å@",//CELL_TYPE_NONE,
        "Å°",//CELL_TYPE_HARD_BLOCK,
        "Å†",//CELL_TYPE_SOFT_BLOCK,
        "Å¶",//CELL_TYPE_EXPLPSEION
        "Å†",
        "îö",
        "Å¢"
};

typedef struct {
    int x,y;
    int direction;
    bool isDead;
}MONSTER;

#define MONSTER_MAX 3
MONSTER monsters[MONSTER_MAX];

typedef struct {
    int x,y;
    int count;/* data */
}BOMB;

#define BOMB_MAX 3
#define BOMB_COUNT_MAX 9

BOMB bombs [BOMB_MAX];


int exitX, exitY;

int getMonster(int _x, int _y){
    for(int i=0; i<MONSTER_MAX;i++)
        if((!monsters[i].isDead)&&(monsters[i].x==_x)&&(monsters[i].y==_y))
            return i;
    return -1;
}


void setFreePosition(int *_pX, int*_pY){
    while (1){
        int x=rand()%MAP_WIDTH;
        int y=rand()%MAP_HEIGHT;
        switch(cells[y][x]){
        case CELL_TYPE_HARD_BLOCK:
        case CELL_TYPE_SOFT_BLOCK:
        case CELL_TYPE_AREA:
        case CELL_TYPE_BAKU:
            break;
        default:
            {
                int monster = getMonster(x,y);
                if (monster<0){
                    *_pX=x;
                    *_pY=y;
                    return;
                }
            }
            break;
        }
    }
}

void setRandomSoftBlockPosition(int *_pX,int *_pY){
    while(1){
        int x=rand()%MAP_WIDTH;
        int y=rand()%MAP_HEIGHT;
        if(cells[y][x]==CELL_TYPE_SOFT_BLOCK){
            *_pX=x;
            *_pY=y; 
            printf("x:%d y:%d\n",x,y);
            _getch();        
            return;
        }
    }
}

int getFreeBomb(){
    for(int i=0;i<BOMB_MAX;i++)
        if(bombs[i].count<=0)
            return i;
    return -1;
}

int getBomb(int _x,int _y){
    for (int i=0;i<BOMB_MAX;i++)
        if((bombs[i].count>0)&&(bombs[i].x==_x)&&(bombs[i].y==_y))
            return i;
    return -1;
}

bool checkCanMove(int _x,int _y){
    switch (cells[_y][_x]){
    case CELL_TYPE_HARD_BLOCK:
    case CELL_TYPE_SOFT_BLOCK:
        return false;
    default:
        if((getMonster(_x,_y)<0)
        && getBomb(_x,_y)<0)
            return true;
    }
    return false;
}

void display(){
    system("cls");
        int left = monsters[0].x-SCREEN_WIDTH/2;
        if(left<0)
            left = 0;
        if(left>MAP_WIDTH-SCREEN_WIDTH)
            left=MAP_WIDTH-SCREEN_WIDTH;
        printf("???e????@Ver.1.01\n");
        printf("?@?`???????????`\n");
       
        for(int y=0; y<MAP_HEIGHT; y++){
            for(int x=left; x<left+SCREEN_WIDTH; x++){
                int monster= getMonster(x,y);
                int bomb=getBomb(x,y);
                if (monster>0)
                    printf("ìG");
                else if(monster==0)
                    printf("îö");    
                else if(bomb>=0){
                    char aa[]="ÇO";
                    aa[1]+=bombs[bomb].count;
                    
                    printf(aa);
                }                
                else   
                    printf(cellAA[cells[y][x]]);      
            }
            printf("\n");
            
        }
        printf("    ??W?@ \n");
        printf("A        D  \n");
        printf("???@??S?@??  |  BOM!(Space)  |\n");
        
}




void gameOver(){    
    display(); 
    bombs[0].count=0;
    bombs[1].count=0;
    bombs[2].count=0;
    printf("GAME OVER\a");
    printf("CONTINUE(Space)\a");
    
    
    _getch();
    monsters[0].x=
    monsters[0].y=1;
    
    for(int i=1;i<MONSTER_MAX;i++){
        monsters[i].isDead=true;}

    for(int y=1;y<MAP_HEIGHT-1;y++)
        for(int x=1;x<MAP_WIDTH-1; x++){
            if(cells[y][x]==CELL_TYPE_SOFT_BLOCK)
                cells[y][x] = CELL_TYPE_NONE;
            else if((x%2==0)&&(y%2==0))
                cells[y][x] = CELL_TYPE_HARD_BLOCK;
            else if(rand()%2)
                cells[y][x]=CELL_TYPE_SOFT_BLOCK;
            
        }
        
    cells[1][1]=CELL_TYPE_BAKU;
    cells[2][1]=
    cells[1][2]=CELL_TYPE_AREA;

    cells[3][1]=
    cells[1][3]=CELL_TYPE_SOFT_BLOCK;



    for(int i=1; i<MONSTER_MAX-1;i++)
        setFreePosition(&monsters[i].x, &monsters[i].y);

    for(int i=1;i<MONSTER_MAX;i++){
        monsters[i].isDead=false;
    }
    

    
}

void explotion(int _x,int _y){
    cells[_y][_x]=CELL_TYPE_EXPLPSEION;
    for(int j=0; j<DIRECTION_MAX;j++){
        int x=_x;
        int y=_y;
        for(int i=0; i<2;i++){
            x+=directions[j][0];
            y+=directions[j][1];
            if(cells[y][x]==CELL_TYPE_HARD_BLOCK)
                break;
            else if(cells[y][x]==CELL_TYPE_SOFT_BLOCK){
                cells[y][x]=CELL_TYPE_EXPLPSEION;
                break;
            }
            else{
                int monster=getMonster(x,y);
                if(monster>0){
                    monsters[monster].isDead=true;
                }
                else if(monster==0){
                    cells[y][x]=CELL_TYPE_EXPLPSEION; 
                    gameOver();
                }

                int bomb=getBomb(x,y);
                if(bomb>=0){
                    bombs[bomb].count=0;
                    explotion(x,y);
                }

                cells[y][x]=CELL_TYPE_EXPLPSEION;                      

            }
        }
    }

}
 
void mySleep(int second) {
    time_t now = second * CLOCKS_PER_SEC + clock();
 
    while (now > clock()) ;
}




int main(){
    
    srand((unsigned int)time(NULL));
    for (int y=0;y<MAP_HEIGHT;y+=MAP_HEIGHT-1)
        for(int x=0;x<MAP_WIDTH; x++)
            cells[y][x]=CELL_TYPE_HARD_BLOCK;
    for(int y=0;y<MAP_HEIGHT;y++)
        for(int x=0; x<MAP_WIDTH;x+=MAP_WIDTH-1)
            cells[y][x]=CELL_TYPE_HARD_BLOCK;
    for(int y=1;y<MAP_HEIGHT-1;y++)
        for(int x=1;x<MAP_WIDTH-1; x++){
            if((x%2==0)&&(y%2==0))
                cells[y][x] = CELL_TYPE_HARD_BLOCK;
            else if(rand()%2)
                if(rand()%2)
                    cells[y][x]=CELL_TYPE_SOFT_BLOCK;
            
        }


    cells[1][1]=CELL_TYPE_BAKU;
    cells[2][1]=
    cells[1][2]=CELL_TYPE_AREA;

    cells[3][1]=
    cells[1][3]=CELL_TYPE_SOFT_BLOCK;
    

    setRandomSoftBlockPosition(&exitX,&exitY);
    printf("%c %c\n",exitX,exitY);

    monsters[0].x=
    monsters[0].y=1;
  
    for(int i=1; i<MONSTER_MAX;i++)
        setFreePosition(&monsters[i].x, &monsters[i].y);



    clock_t lastUpdateClock=clock();
    clock_t lastDrawClock=clock();

    int lope =0;
    
    while(1){
        display();
        int second = 1.8*100;
        Sleep(second);
        for(int y=1;y<MAP_HEIGHT-1;y++)
            for(int x=1;x<MAP_WIDTH-1;x++)
                if(cells[y][x]==CELL_TYPE_EXPLPSEION)
                    cells[y][x]=((x==exitX)&&(y==exitY))
                    ?CELL_TYPE_EXIT
                    :CELL_TYPE_NONE;
                    
                    


        int x=monsters[0].x;
        int y=monsters[0].y;       
        if(_kbhit()){
            
            switch(_getch()){
            case 'w':y--;break;
            case 's':y++;break;
            case 'a':x--;break;
            case 'd':x++;break;
            case ' ':
                {
                    int bomb=getFreeBomb();
                    if (bomb>=0){
                        bombs[bomb].x=monsters[0].x;
                        bombs[bomb].y=monsters[0].y;
                        bombs[bomb].count=BOMB_COUNT_MAX;
                    }
                }
                break;
            }
        }

        if(getMonster(x,y)>0)
            gameOver();


        if(checkCanMove(x,y)){
            monsters[0].x=x;
            monsters[0].y=y;

            if(cells[y][x]==CELL_TYPE_EXIT){
                int enemyCount=0;
                for (int i=1;i<MONSTER_MAX;i++)
                    if(!monsters[i].isDead){
                        enemyCount++;
                        break;
                    }
                if(enemyCount<=0){
                    
                    printf("CLEAR!\a");
                    _getch();
                    exit(0);
                }
            }
        }

        for (int i=0; i<BOMB_MAX;i++){
            if(bombs[i].count<=0)
                continue;

            bombs[i].count--;
            if(bombs[i].count<=0){
                explotion(bombs[i].x,bombs[i].y);
            }
        }
        
        
        for(int i=1;i<MONSTER_MAX;i++){
            if(monsters[i].isDead)
                continue;
            int x = monsters[i].x+directions[monsters[i].direction][0];
            int y = monsters[i].y+directions[monsters[i].direction][1];
            if(getMonster(x,y)==0){
                monsters[i].x=x;
                monsters[i].y=y;
                gameOver();
            }
            else if(checkCanMove(x,y)){               
                
                monsters[i].x=x;
                monsters[i].y=y;             
                                       
            }
            else
             monsters[i].direction=rand()%DIRECTION_MAX;
        }


    }
    
} 