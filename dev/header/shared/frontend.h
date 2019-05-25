#ifndef FRONTEND_H_INCLUDED
#define FRONTEND_H_INCLUDED

/**
    FRONTEND_H
    Header Untuk Olah Tampilan
*/

/*##################################################################################################################*/
//LOW LEVEL FRONTEND
void printfColor(int ForgC,int BackC,const char *val, ...){
	va_list arg_list;
	int my_arg;
	va_start(arg_list, &val);
    int charlen=strlen(val);
    HANDLE h = GetStdHandle ( STD_OUTPUT_HANDLE );
    WORD wOldColorAttrs;
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
    GetConsoleScreenBufferInfo(h, &csbiInfo);
    wOldColorAttrs = csbiInfo.wAttributes;
    WORD wColor = ((BackC & 0x0F) << 4) + (ForgC & 0x0F);
    SetConsoleTextAttribute(h, wColor);
	for (my_arg = 0; my_arg<charlen; my_arg++){
        if(val[my_arg]=='%'){
            int r=0;
            char value[16];
            while(my_arg<charlen){
                value[r]=val[my_arg];
                value[r+1]=0;
                if(val[my_arg]=='d'||val[my_arg]=='c'||val[my_arg]=='s'){
                    printf(value, va_arg(arg_list, int));
                    break;
                }
                else if(val[my_arg]=='f'){
                    printf(value, va_arg(arg_list, double));
                    break;
                }
                my_arg++;r++;
            }
        }
        else if(val[my_arg]!='%')printf("%c",val[my_arg]);
	}
	va_end(arg_list);
	SetConsoleTextAttribute (h,wOldColorAttrs);
}

void printfPosition(int X, int Y, const char *val,...){
    if(X==CENTER_OF_SCREEN){
        CONSOLE_SCREEN_BUFFER_INFO csbI;
        GetConsoleScreenBufferInfo(Handle.stdOutput,&csbI);
        X=(csbI.dwMaximumWindowSize.X-strlen(val))/2;
    }
    cursor(X,Y);
    va_list arg_list;
	int my_arg;
	va_start(arg_list, &val);
    int charlen=strlen(val);
	for (my_arg = 0; my_arg<charlen; my_arg++){
        if(val[my_arg]=='%'){
            int r=0;
            char value[16];
            while(my_arg<charlen){
                value[r]=val[my_arg];
                value[r+1]=0;
                if(val[my_arg]=='d'||val[my_arg]=='c'||val[my_arg]=='s'){
                    printf(value, va_arg(arg_list, int));
                    break;
                }
                else if(val[my_arg]=='f'){
                    printf(value, va_arg(arg_list, double));
                    break;
                }
                my_arg++;r++;
            }
        }
        else if(val[my_arg]!='%')printf("%c",val[my_arg]);
	}
	va_end(arg_list);
}

void printx(const int ForgC, const int BackC, const int X, const int Y, const char *val){
    HANDLE h = GetStdHandle ( STD_OUTPUT_HANDLE );
    WORD wOldColorAttrs;
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
    COORD pos;
    pos.X=X;
    pos.Y=Y;
    GetConsoleScreenBufferInfo(h, &csbiInfo);
    wOldColorAttrs = csbiInfo.wAttributes;
    WORD wColor = ((BackC & 0x0F) << 4) + (ForgC & 0x0F);
    int charlen=strlen(val);
    for(int a=0;a<charlen;a++){
        SetConsoleCursorPosition(h, pos);
        SetConsoleTextAttribute(h, wColor);
        printf("%c",val[a]);
        pos.X++;
        SetConsoleTextAttribute ( h, wOldColorAttrs);
    }
}

#define CONTINUE_POS_Y 0xFFFF
void printfWrap(const int posX, int posY, const int MarginLeft,const int MarginRight, const char *val){
    CONSOLE_SCREEN_BUFFER_INFO csbI;
    GetConsoleScreenBufferInfo(Handle.stdOutput,&csbI);
    if(posY>=CONTINUE_POS_Y){
        posY=csbI.dwCursorPosition.Y+(posY-CONTINUE_POS_Y)+1;
    }
    int X=posX+MarginLeft;
    for(int a=0;a<strlen(val);a++){
        cursor(X,posY);
        printf("%c",val[a]);
        X++;
        if(X>=csbI.dwMaximumWindowSize.X-(MarginRight)){
            int temp=0;
            while(1){
                if(val[a]==' ')break;
                a--;
                temp++;
            }
            cursor(X-temp,posY);
            for(int b=0;b<temp;b++)printf(" ");
            X=posX+MarginLeft;
            posY++;

        }
    }
}

void printPianoIcon(const int posX, int posY){
    FILE * fp=fopen("bin\\pw.ic","r");
    cursor(posX,posY);
    while(1){
        char x=fgetc(fp);
        if(x==EOF)break;
        if(x=='#')printfColor(0,settingVar.color[COLORSET_GLOBAL_FG]," ");
        else if(x=='\n'){
            posY++;
            cursor(posX,posY);
        }
        else printfColor(0,settingVar.color[COLORSET_GLOBAL_BG]," ");
    }
    fclose(fp);
}

void boxLine(const int posX, const int posY, const int sizeX, const int sizeY){
    cursor(posX,posY);
    printf("%c",218);
    for(int a=posX;a<posX+sizeX;a++ )printf("%c",196);
    cursor(posX+sizeX,posY);
    printf("%c",191);
    for(int a=posY+1;a<posY+sizeY+1;a++){
        cursor(posX,a);
        printf("%c",179);
        cursor(posX+sizeX,a);
        printf("%c",179);
    }
    cursor(posX,posY+sizeY+1);
    printf("%c",192);
    for(int a=posX;a<posX+sizeX;a++ )printf("%c",196);
    cursor(posX+sizeX,posY+sizeY+1);
    printf("%c",217);
}

void boxLineErase(const int posX, const int posY, const int sizeX, const int sizeY){
    cursor(posX,posY);
    printf(" ");
    for(int a=posX;a<posX+sizeX;a++ )printf(" ");
    cursor(posX+sizeX,posY);
    printf(" ");
    for(int a=posY+1;a<posY+sizeY+1;a++){
        cursor(posX,a);
        printf(" ");
        cursor(posX+sizeX,a);
        printf(" ");
    }
    cursor(posX,posY+sizeY+1);
    printf(" ");
    for(int a=posX;a<posX+sizeX;a++ )printf(" ");
    cursor(posX+sizeX,posY+sizeY+1);
    printf(" ");
}

#define ERASE_BY_BACKGROUND_COLOR 0xFFFF
#define MAXIMUM_X_SCREEN_BUFFER 0xFFF0
#define MAXIMUM_Y_SCREEN_BUFFER 0xFFF1
void boxFill(const int posX, const int posY, int sizeX, int sizeY, const int colorBg){
    CONSOLE_SCREEN_BUFFER_INFO csbI;
    GetConsoleScreenBufferInfo(Handle.stdOutput,&csbI);
    if(sizeX==MAXIMUM_X_SCREEN_BUFFER){
        sizeX=csbI.dwMaximumWindowSize.X;
    }
    if(sizeY==MAXIMUM_Y_SCREEN_BUFFER){
        sizeY=csbI.dwMaximumWindowSize.Y-3;
    }
    for(int a=0;a<sizeY;a++){
        cursor(posX,posY+a);
        for(int a=0; a<sizeX;a++){
            if(colorBg==ERASE_BY_BACKGROUND_COLOR) printf(" ");
            else printfColor(NULL,colorBg," ");
        }
    }
}

void printfDots(const int posX, const int posY, int maxChar, char *val){
    cursor(posX,posY);
    if(maxChar==MAXIMUM_X_SCREEN_BUFFER){
        CONSOLE_SCREEN_BUFFER_INFO csbI;
        GetConsoleScreenBufferInfo(Handle.stdOutput,&csbI);
        maxChar=csbI.dwMaximumWindowSize.X-1;
    }
    int size=((maxChar-3-posX)>=strlen(val))?strlen(val):(maxChar-3-posX);
    for(int a=0;a<size;a++)printf("%c",val[a]);
    ((maxChar-3-posX)>=strlen(val))?0:printf("...");
}

void downloadBar(int state,COORD pos,float val,float maxVal){
    int x=pos.X,y=pos.Y;
    switch(state){
    case INIT_DBAR:
        SetConsoleCursorPosition(Handle.stdOutput,pos);
        printf("%c",ASCII_2LT_CORNER);
        for(int a=0;a<30;a++)printf("%c",ASCII_2HOR);
        printf("%c",ASCII_2TT);
        for(int a=0;a<4;a++)printf("%c",ASCII_2HOR);
        printf("%c\n",ASCII_2RT_CORNER);
        pos.X=x;pos.Y++;
        SetConsoleCursorPosition(Handle.stdOutput,pos);
        printf("%c                              %c0%%  %c",ASCII_2VER,ASCII_2VER,ASCII_2VER);
        pos.X=x;pos.Y++;
        SetConsoleCursorPosition(Handle.stdOutput,pos);
        printf("%c",ASCII_2TL);
        for(int a=0;a<21;a++)printf("%c",ASCII_2HOR);
        printf("%c",ASCII_2TT);
        for(int a=0;a<8;a++)printf("%c",ASCII_2HOR);
        printf("%c",ASCII_2TB);
        for(int a=0;a<4;a++)printf("%c",ASCII_2HOR);
        printf("%c",ASCII_2TR);
        pos.X=x;pos.Y++;
        SetConsoleCursorPosition(Handle.stdOutput,pos);
        printf("%c                     %c             %c",ASCII_2VER,ASCII_2VER,ASCII_2VER);
        pos.X=x;pos.Y++;
        SetConsoleCursorPosition(Handle.stdOutput,pos);
        printf("%c",ASCII_2LB_CORNER);
        for(int a=0;a<21;a++)printf("%c",ASCII_2HOR);
        printf("%c",ASCII_2TB);
        for(int a=0;a<13;a++)printf("%c",ASCII_2HOR);
        printf("%c",ASCII_2RB_CORNER);
        break;
    case LOAD_DBAR:;
        if(val>=maxVal)val=maxVal;
        float percentSize=(val/maxVal)*30;
        pos.X=pos.X+1;
        pos.Y=pos.Y+1;
        SetConsoleCursorPosition(Handle.stdOutput,pos);
        for(int a=0;a<percentSize;a++)printf("%c",ASCII_LOAD_BAR);
        pos.X=x+32;
        SetConsoleCursorPosition(Handle.stdOutput,pos);
        float n=(val/maxVal)*100;
        printf("%.0f%%",n);
        break;
    case DESTROY_DBAR:
        break;
    default:
        return NULL;
    }
    pos.X=0;
    pos.Y=y+4;
    SetConsoleCursorPosition(Handle.stdOutput,pos);
}

void clsline(int x, int y){
    CONSOLE_SCREEN_BUFFER_INFO csbI;
    GetConsoleScreenBufferInfo(Handle.stdOutput,&csbI);
    int maxX=csbI.dwMaximumWindowSize.X-x;
    for(int a=x;a<maxX;a++){
        cursor(a,y);
        printf(" ");
    }
}

int on_extract_entry(const char *filename,void *arg){
        printf ("\r  Extracted: %s     ",filename);
        return 0;
}

void printClock(){
    SYSTEMTIME myTime;
    cursor(159,45);
    GetSystemTime(&myTime);
    int utc=myTime.wHour+settingVar.clockoffset;
    printf("%d:%d:%d   ",(utc>24)?utc-24:utc,myTime.wMinute,myTime.wSecond);
}

void footer(){
    int r=60;
    cursor(2,42);
    for(int a=0;a<r;a++) printf("%c",205);
    printf("   P   I   A   N   O         W   A   V   E   ");
    for(int a=0;a<r;a++) printf("%c",205);
    printfPosition(2,45,"Ver. %s %s",VERSION,BUILD);
    printClock();
}

void header(){
    char myName[128]="PianoWave ";
    char gap[8]=" Build ";
    char proj[64]=" | Project by ";
    char gap2[8]=" and ";
    strcat(&myName,VERSION);
    strcat(&myName,gap);
    strcat(&myName,BUILD);
    strcat(&myName,proj);
    strcat(&myName,BACKEND);
    strcat(&myName,gap2);
    strcat(&myName,FRONTEND);
    HANDLE h = GetStdHandle ( STD_OUTPUT_HANDLE );
    WORD wOldColorAttrs;
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
    GetConsoleScreenBufferInfo(h, &csbiInfo);
    wOldColorAttrs = csbiInfo.wAttributes;
    WORD wColor = ((settingVar.color[COLORSET_GLOBAL_BG] & 0x0F) << 4) + (settingVar.color[COLORSET_TEXT_DEACTIVE] & 0x0F);
    SetConsoleTextAttribute(h, wColor);
    printfPosition(CENTER_OF_SCREEN,44,myName);
    printfPosition(CENTER_OF_SCREEN,45,"Copyright (C) AN 2019 | All Right Reserved");
    SetConsoleTextAttribute(h, wOldColorAttrs);
}

void printIcon(int state,int posX, int posY, int colorfg,int colorbg){
    switch(state){
    case 0:
        printx(colorfg,colorbg,posX,posY,"|\\");
        printx(colorfg,colorbg,posX,posY+1,"| \\");
        printx(colorfg,colorbg,posX,posY+2,"| /");
        printx(colorfg,colorbg,posX,posY+3,"|/");
        break;
    case 1:
        printx(colorfg,colorbg,posX,posY-1," __");
        printx(colorfg,colorbg,posX,posY," || ");
        printx(colorfg,colorbg,posX,posY+1,"_||_");
        printx(colorfg,colorbg,posX,posY+2,"\\  /");
        printx(colorfg,colorbg,posX,posY+3," \\/");
        break;
    case 2:
        printx(colorfg,colorbg,posX,posY-1," __");
        printx(colorfg,colorbg,posX,posY,"/  \\");
        printx(colorfg,colorbg,posX,posY+1,"   /");
        printx(colorfg,colorbg,posX,posY+2,"  |");
        printx(colorfg,colorbg,posX,posY+3,"  0");
        break;
    case 3:
        printx(colorfg,colorbg,posX,posY,"\\\\//");
        printx(colorfg,colorbg,posX,posY+1,"-||-");
        printx(colorfg,colorbg,posX,posY+2,"-||-");
        printx(colorfg,colorbg,posX,posY+3,"//\\\\");
        break;
    case 4:
        printx(colorfg,colorbg,posX,posY,"\\  /");
        printx(colorfg,colorbg,posX,posY+1," \\/");
        printx(colorfg,colorbg,posX,posY+2," /\\");
        printx(colorfg,colorbg,posX,posY+3,"/  \\");
        break;
    }
}

void printHints(){
    printfPosition(6,15,"     F1     - HINTS TABLE");
    printfPosition(6,16,"  CapsLock  - SUSTAIN TOGLE");
    printfPosition(6,17,"  LeftShift - OCTAVE DECREASE");
    printfPosition(6,18," RightShift - OCTAVE INCREASE");
    printfPosition(6,19,"    SPACE   - METRONOME TOGGLE");
    printfPosition(6,20,"   ArrowUp  - TEMPO INCREASE");
    printfPosition(6,21,"  ArrowDown - TEMPO DECREASE");
    printfPosition(6,22,"  ArrowLeft - INSTRUMENT PREVIOUS");
    printfPosition(6,23," ArrowRight - INSTRUMENT NEXT");
    printfPosition(6,27,"     ESC    - RETURN");
    printfPosition(6,25,"      +     - VOLUME UP");
    printfPosition(6,26,"      -     - VOLUME DOWN");
    printfPosition(6,24,"      0     - VOLUME MUTE");
    printfPosition(6,30,"    ANY KEY TO DISMISS");
    for(int a=0;a<50;a++){
        if(temp.hints==FALSE)break;
        Sleep(100);
    }
    for(int a=15;a<=30;a++) for(int b=6;b<40;b++) printfPosition(b,a," ");
    temp.hints=FALSE;
    return;
}

/*##################################################################################################################*/
//HIGH LEVEL FRONTEND

#define INTRO_CHANGELOG 0
#define SHOW_CHANGELOG 1
#define RETURN_CHANGELOG_EXIT -1
int printChangelog(int State,int index){
    sfMusic * updatesuccess;
    system("cls");
    FILE *myInstr;
    if(State==INTRO_CHANGELOG) {
        myInstr=fopen("bin\\temp.txt","r");
        if(myInstr==NULL) return 0;
        playNotif(NOTIF_UPDATE);
        printfPosition(CENTER_OF_SCREEN,10,"PIANOWAVE IS UP TO DATE");
    }
    else if(State==SHOW_CHANGELOG) myInstr=fopen("bin\\changelog.txt","r");
    int mycount=0; boolean rchng=FALSE;
    int curs_pos=12; int val=0;
    boolean maxVal=FALSE;
    while(1){
        int x=fgetc(myInstr);
        if(x==EOF) break;
        else if((x=='\n'||x=='~')&&rchng==TRUE&&val==index){
            if(x=='~'){
                return -1;
                break;
            }
            curs_pos++;
            cursor(44,curs_pos);
            x=' ';
        }
        else if(x==':'&&rchng==FALSE)mycount++;
        if(rchng==TRUE){
            if(val==index){
                if(x=='_')x=' ';
                else if(x=='[')break;
                printf("%c",x);
            }else if(x=='['){
                val++;
            }
        }
        if(mycount==4)rchng=TRUE;
    }
    fclose(myInstr);
    boxLine(40,11,90,curs_pos-10);
    printfPosition(CENTER_OF_SCREEN,45,(State==INTRO_CHANGELOG)?"ANY KEY TO CONTINUE...":"L/R TO NAVIGATE | ANY KEY TO RETURN");
    boolean exitval=FALSE;
    int x=_getch();
    if(x==224){
        x=_getch();
        if(x==LEFT_ARROW) index--;
        else if(x==RIGHT_ARROW){
            if(maxVal==TRUE) exitval=TRUE;
            else if(maxVal==FALSE) index++;
        }
        else{
            exitval=TRUE;
        }
    }else {
        exitval=TRUE;
    }
    if(index<-1)index==0;
    if(State==INTRO_CHANGELOG) {
        rename("bin\\temp.txt","bin\\changelog.txt");
        remove("bin\\temp.txt");
    }
    clrscreen();
    if(exitval==FALSE&maxVal==FALSE) {
        playNotif(NOTIF_TICK);
        return index;
    }
    else if(exitval==TRUE) return -1;
}

int WindowWelcome(){
    header();
    FILE *welcome;
    welcome = fopen("bin\\data\\layout\\welcome.txt","r");
    if(welcome==NULL) error(ERROR_NO_DIRECTORY);
    int posX = (170/2)-(82/2);
    int posY = 18;
    cursor(posX,posY);
    playNotif(NOTIF_INTRO);
    while(1){
        int x;
        x=fgetc(welcome);
        if(x==EOF)break;
        else if(x=='\n'){
            posY++;
            cursor(posX,posY);
            Sleep(100);
        }
        else printf("%c", x);
        if(_kbhit()){
            _getch();
            return 0;
        }
    }
    fclose(welcome);
    welcome=fopen("bin\\data\\setting\\register.nme","r");
    char name[64];
    fscanf(welcome,"%[^\n]s",&name);
    printfPosition(CENTER_OF_SCREEN,27,name);
    for(int a=0;a<500;a++){
        Sleep(1);
        if(_kbhit()){
            _getch();
            goto SKIP;
            return 0;
        }
    }
    fclose(welcome);
SKIP:;
    clrscreen();
    return 0;

}

int WindowGoodbye(){
    header();
    FILE *welcome;
    welcome = fopen("bin\\data\\layout\\goodbye.txt","r");
    if(welcome==NULL) error(ERROR_NO_DIRECTORY);
    int posX = (170/2)-(76/2);
    int posY = 18;
    cursor(posX,posY);
    playNotif(NOTIF_OUTRO);
    while(1){
        int x;
        x=fgetc(welcome);
        if(x==EOF)break;
        else if(x=='\n'){
            posY++;
            cursor(posX,posY);
            Sleep(100);
        }
        else printf("%c", x);
        if(_kbhit()){
            _getch();
            return 0;
        }
    }
    fclose(welcome);
    char name[64];
    getRegName(&name);
    printfPosition(CENTER_OF_SCREEN,27,name);
    Sleep(3000);
    return 0;
}

#define WINDOW_PIANOPLAYER_RETURN 0
#define WINDOW_DLC_RETURN 1
#define WINDOW_ABOUT_RETURN 2
#define WINDOW_SETTING_RETURN 3
#define WINDOW_EXIT_RETURN 4
int WindowMainMenu(){
    if(settingVar.menuTemp>3||settingVar.menuTemp<0) settingVar.menuTemp=0;
    int count=settingVar.menuTemp;
    printPianoIcon(62,4);
    footer();
    const int sizeX=14;
    const int gap=2;
    const int sizeY=7;
    const int navPosY=30;
    const int startPosX=30;
    const char myMenu[7][16]={"     PLAY     ",
                              "      DLC     ",
                              "     ABOUT    ",
                              "   SETTINGS   ",
                              "     EXIT     "};

    for(int a=0;a<5;a++){
        const int navPosX=startPosX+(sizeX+a)+((gap+sizeX)*a);
        if(a==settingVar.menuTemp){
            boxFill(navPosX,navPosY,sizeX,sizeY,settingVar.color[COLORSET_BUTTON_ACTIVE]);
            cursor(navPosX,navPosY+5);
            printfColor(settingVar.color[COLORSET_TEXT_ACTIVE],settingVar.color[COLORSET_BUTTON_ACTIVE],myMenu[a]);
            printIcon(a,navPosX+5,navPosY+1,settingVar.color[COLORSET_TEXT_ACTIVE],settingVar.color[COLORSET_BUTTON_ACTIVE]);
        }else{
            boxFill(navPosX,navPosY,sizeX,sizeY,settingVar.color[COLORSET_BUTTON_DEACTIVE]);
            cursor(navPosX,navPosY+5);
            printfColor(settingVar.color[COLORSET_TEXT_DEACTIVE],settingVar.color[COLORSET_BUTTON_DEACTIVE],myMenu[a]);
            printIcon(a,navPosX+5,navPosY+1,settingVar.color[COLORSET_TEXT_DEACTIVE],settingVar.color[COLORSET_BUTTON_DEACTIVE]);
        }
    }
    int tempcount=count;
    while(1){
        printClock();
        if(_kbhit()){
            int x=getch();
            if(x==RIGHT_ARROW){
                count++;
                playNotif(NOTIF_TICK);
            }
            else if(x==LEFT_ARROW){
                count--;
                playNotif(NOTIF_TICK);
            }
            else if(x==ENTER){
                if(count!=4&&count!=0) playNotif(NOTIF_ENTER);
                settingVar.menuTemp=count;
                clrscreen();
                return count;
            }
            else if(x==ESCAPE){
                settingVar.menuTemp=count;
                clrscreen();
                return 4;
            }
            if(count>4)count=0;
            else if(count<0)count=4;
            if(tempcount!=count){
                int navPosX=startPosX+(sizeX+tempcount)+((gap+sizeX)*tempcount);
                boxFill(navPosX,navPosY,sizeX,sizeY,settingVar.color[COLORSET_BUTTON_DEACTIVE]);
                cursor(navPosX,navPosY+5);
                printfColor(settingVar.color[COLORSET_TEXT_DEACTIVE],settingVar.color[COLORSET_BUTTON_DEACTIVE],myMenu[tempcount]);
                printIcon(tempcount,navPosX+5,navPosY+1,settingVar.color[COLORSET_TEXT_DEACTIVE],settingVar.color[COLORSET_BUTTON_DEACTIVE]);
                navPosX=startPosX+(sizeX+count)+((gap+sizeX)*count);
                boxFill(navPosX,navPosY,sizeX,sizeY,settingVar.color[COLORSET_BUTTON_ACTIVE]);
                cursor(navPosX,navPosY+5);
                printfColor(settingVar.color[COLORSET_TEXT_ACTIVE],settingVar.color[COLORSET_BUTTON_ACTIVE],myMenu[count]);
                printIcon(count,navPosX+5,navPosY+1,settingVar.color[COLORSET_TEXT_ACTIVE],settingVar.color[COLORSET_BUTTON_ACTIVE]);
                tempcount=count;
            }
        }
    }
}

int WindowExitPrompt(){
    printfPosition(CENTER_OF_SCREEN,20,"ARE YOU SURE WANT TO EXIT?");
    playNotif(NOTIF_ERROR);
    boolean selection=TRUE;
    boxFill(88,23,16,3,settingVar.color[COLORSET_BUTTON_ACTIVE]);
    boxFill(66,23,16,3,settingVar.color[COLORSET_BUTTON_DEACTIVE]);
    printx(settingVar.color[COLORSET_TEXT_ACTIVE],settingVar.color[COLORSET_BUTTON_ACTIVE],94,24,"EXIT");
    printx(settingVar.color[COLORSET_TEXT_DEACTIVE],settingVar.color[COLORSET_BUTTON_DEACTIVE],71,24,"CANCLE");
    while(1){
        int x=_getch();
        if(x==ENTER){
            if(selection==FALSE) playNotif(NOTIF_CANCLE);
            clrscreen();
            return selection;
        }else if(x==ESCAPE){
            playNotif(NOTIF_CANCLE);
            clrscreen();
            return 0;
        }else if(x==LEFT_ARROW||x==RIGHT_ARROW){
            playNotif(NOTIF_TICK);
            selection=(selection==TRUE)?FALSE:TRUE;
            if(selection==TRUE){
                boxFill(88,23,16,3,settingVar.color[COLORSET_BUTTON_ACTIVE]);
                boxFill(66,23,16,3,settingVar.color[COLORSET_BUTTON_DEACTIVE]);
                printx(settingVar.color[COLORSET_TEXT_ACTIVE],settingVar.color[COLORSET_BUTTON_ACTIVE],94,24,"EXIT");
                printx(settingVar.color[COLORSET_TEXT_DEACTIVE],settingVar.color[COLORSET_BUTTON_DEACTIVE],71,24,"CANCLE");
            }else if(selection==FALSE){
                boxFill(88,23,16,3,settingVar.color[COLORSET_BUTTON_DEACTIVE]);
                boxFill(66,23,16,3,settingVar.color[COLORSET_BUTTON_ACTIVE]);
                printx(settingVar.color[COLORSET_TEXT_DEACTIVE],settingVar.color[COLORSET_BUTTON_DEACTIVE],94,24,"EXIT");
                printx(settingVar.color[COLORSET_TEXT_ACTIVE],settingVar.color[COLORSET_BUTTON_ACTIVE],71,24,"CANCLE");
            }
        }
    }
}

int WindowAbout(){
    int tempcount=0;
    int x;
    system("cls");
    FILE *pw;
    FILE *KATA;
    KATA = fopen("bin\\data\\Layout\\katamutiara.txt","r");
    pw = fopen("bin\\data\\Layout\\pw.txt","r");
    int posX = 59;
    int posY = 26;
    while(1){
        int x;
        x=fgetc(pw);
        if(x==EOF)break;
        else if(x=='\n'){
            posY++;
            cursor(posX,posY);
        }
        else printf("%c", x);
    }
    posX-=12;
    posY-=2;
    while(1){
        int x;
        x=fgetc(KATA);
        if(x==EOF)break;
        else if(x=='\n'){
            posY++;
            cursor(posX,posY);
        }
        else printf("%c", x);
    }
    printPianoIcon(62,4);
    boxLine(45,33,75,6);
    const int sizeX=15;
    const int gap=2;
    const int sizeY=3;
    const int navPosY=42;
    const int startPosX=43;
    const int bgcolor=BLACK;
    const int fgcolor=WHITE;
    const char myMenu[8][15]={"  CHANGE LOGS",
                              "    UPDATES  ",
                              "     MENU    "};
    int mcount=0;
    for(int a=0;a<3;a++){
        const int navPosX=startPosX+(sizeX+a)+((gap+sizeX)*a);
        if(a==mcount){
            boxFill(navPosX,navPosY,sizeX,sizeY,settingVar.color[COLORSET_BUTTON_ACTIVE]);
            cursor(navPosX,navPosY+1);
            printfColor(settingVar.color[COLORSET_TEXT_ACTIVE],settingVar.color[COLORSET_BUTTON_ACTIVE],myMenu[a]);
        }
        else{
            boxFill(navPosX,navPosY,sizeX,sizeY,settingVar.color[COLORSET_BUTTON_DEACTIVE]);
            cursor(navPosX,navPosY+1);
            printfColor(settingVar.color[COLORSET_TEXT_DEACTIVE],settingVar.color[COLORSET_BUTTON_DEACTIVE], myMenu[a]);
        }
    }
    while(1){
        if(x==RIGHT_ARROW&&mcount<2){
            mcount++;
            playNotif(NOTIF_TICK);
        }
        else if(x==LEFT_ARROW&&mcount>0){
            mcount--;
            playNotif(NOTIF_TICK);
        }
        else if(x==ENTER){
            playNotif(NOTIF_ENTER);
            clrscreen();
            return mcount;
        }else if(x==ESCAPE){
            playNotif(NOTIF_CANCLE);
            clrscreen();
            return 2;
        }
        if(mcount!=tempcount){
            for(int a=0;a<3;a++){
                const int navPosX=startPosX+(sizeX+a)+((gap+sizeX)*a);
                if(a==tempcount){
                    boxFill(navPosX,navPosY,sizeX,sizeY,settingVar.color[COLORSET_BUTTON_DEACTIVE]);
                    cursor(navPosX,navPosY+1);
                    printfColor(settingVar.color[COLORSET_TEXT_DEACTIVE],settingVar.color[COLORSET_BUTTON_DEACTIVE],myMenu[a]);
                    tempcount=mcount;
                }
                if(a==mcount){
                    boxFill(navPosX,navPosY,sizeX,sizeY,settingVar.color[COLORSET_BUTTON_ACTIVE]);
                    cursor(navPosX,navPosY+1);
                    printfColor(settingVar.color[COLORSET_TEXT_ACTIVE],settingVar.color[COLORSET_BUTTON_ACTIVE],myMenu[a]);
                }
            }
        }
        x=_getch();
    }
}

int WindowUpdate(){
    printfPosition(CENTER_OF_SCREEN,40,"CHECKING FOR UPDATES | ANY KEY TO ABORT");
    char load[7][8]={"L      "," O     ","  A    ","   D   ","    I  ","     N ","      G"};
    int loadVal=0;
    int loadInv=1;
    while(1){
         printfPosition(CENTER_OF_SCREEN,42,load[loadVal]);
         Sleep(125);
         loadVal+=loadInv;
         if(loadVal==6)loadInv=-1;
         else if(loadVal==0)loadInv=1;
         if(_kbhit()){
            _getch();
            playNotif(NOTIF_CANCLE);
            return 0;
         }
    }
}

int WindowDLC(){
    printfPosition(CENTER_OF_SCREEN,40,"RETRIVE METADATA | ANY KEY TO ABORT");
    char load[7][8]={"L      "," O     ","  A    ","   D   ","    I  ","     N ","      G"};
    int loadVal=0;
    int loadInv=1;
    while(1){
         printfPosition(CENTER_OF_SCREEN,42,load[loadVal]);
         Sleep(125);
         loadVal+=loadInv;
         if(loadVal==6)loadInv=-1;
         else if(loadVal==0)loadInv=1;
         if(_kbhit()){
            _getch();
            playNotif(NOTIF_CANCLE);
            clrscreen();
            return 0;
         }
    }
}
#define PRINT_SUBMENU 0
#define ENTER_SUBMENU 1
#define SUBMENU_DISPLAY 0
#define SUBMENU_SOUND 1
#define SUBMENU_ADVANCE 2
#define SUBMENU_APPLY 0
#define SUBMENU_DEFAULT 1
#define SUBMENU_CANCLE 2
#define SUBMENU_SAVE 3
#define SUBMENU_EXIT 3
int WindowSubMenu(int state,int submenu,int submenuTemp){
    int cgbg=settingVar.color[COLORSET_GLOBAL_BG];
    int loop[2]={submenuTemp,submenu};
    if(state==PRINT_SUBMENU){
        for(int x=0;x<2;x++){
            if(loop[x]==SUBMENU_DISPLAY){
                int cgfg=(submenu==SUBMENU_DISPLAY)?settingVar.color[COLORSET_GLOBAL_FG]:cgbg;
                printx(cgbg,cgbg,62,32,"You must restart to change the effect");
                printx(cgfg,cgbg,10,8,    "COLOR THEME                : ");
                printx(cgfg,cgbg,10,10,   "GLOBAL BACKGROUND COLOR    : "); //0
                printx(cgfg,cgbg,10,12,   "GLOBAL FONT COLOR          : "); //1
                printx(cgfg,cgbg,10,14,   "BUTTON ACTIVE COLOR        : "); //2
                printx(cgfg,cgbg,10,16,   "BUTTON DEACTIVE COLOR      : "); //3
                printx(cgfg,cgbg,10,18,   "TEXT ACTIVE COLOR          : "); //4
                printx(cgfg,cgbg,10,20,   "TEXT DEACTIVE COLOR        : "); //5
                printx(cgfg,cgbg,10,22,   "PIANO NATURAL PRESSED      : "); //6
                printx(cgfg,cgbg,10,24,   "PIANO NATURAL RELEASED     : "); //7
                printx(cgfg,cgbg,10,26,   "PIANO SHARP PRESSED        : "); //8
                printx(cgfg,cgbg,10,28,   "PIANO SHARP RELEASED       : "); //9
                printx(cgfg,cgbg,10,30,   "STATIC BUTTON COLOR        : "); //10
                printx(cgfg,cgbg,10,32,   "SCREEN MODE                : ");
                printx(cgfg,cgbg,10,34,   "INTRO/OUTRO WINDOW DISPLAY : ");
                int center=50;
                char navbar[4][64]={"      APPLY      ",
                                    "     DEFAULT     ",
                                    "      CANCLE     ",
                                    "       SAVE      "};
                char name[32]={0};
                GetColorSetting(settingVar.colorPreset,&settingVar.color,&name);
                for(int a=0;a<4;a++) printx((submenu==SUBMENU_DISPLAY)?settingVar.color[COLORSET_TEXT_DEACTIVE]:cgbg,(submenu==SUBMENU_DISPLAY)?settingVar.color[COLORSET_BUTTON_DEACTIVE]:cgbg,(a*18)+center,41,navbar[a]);
                for(int a=0;a<=11;a++) printx(cgbg,(submenu==SUBMENU_DISPLAY)?settingVar.color[a]:cgbg,42,(a*2)+10,"                 ");
                printx(cgfg,cgbg,42,32,(settingVar.ScreenMode==FULLSCREEN)?"    FULLSCREEN   ":"     WINDOWED    ");
                printx(cgfg,cgbg,42,34,(settingVar.IntroMode==TRUE)?       "      ENABLE     ":"      DISABLE    ");
                printx(cgbg,cgbg,42,8,"                 ");
                printx(cgfg,cgbg,42,8,strupr(name));
            }
            else if(loop[x]==SUBMENU_SOUND){
                int cgfg=(submenu==SUBMENU_SOUND)?settingVar.color[COLORSET_GLOBAL_FG]:cgbg;
                char notifName[64], metName[64];
                met.maxPreset=getConfigData("Metronome",met.Preset,&metName);
                notif.maxPreset=getConfigData("Notification",notif.Preset,&notifName);
                printx(cgfg,cgbg,42,8,"                 ");
                printx(cgfg,cgbg,42,14,"                 ");
                printx(cgfg,cgbg,10,8,    "NOTIFICATION SOUND         : "); printfColor(cgfg,cgbg,"   %s",strupr(notifName));
                printx(cgfg,cgbg,10,10,   "NOTIFICATION VOLUME        : "); printfColor(cgfg,cgbg,"   %3d Percent",(int)notif.Vol);
                printx(cgfg,cgbg,10,12,   "NOTIFICATION STATE         : "); printx(cgfg,cgbg,42,12,(notif.state==TRUE)?"      ENABLE     ":"     DISABLE     ");
                printx(cgfg,cgbg,10,14,   "METRONOME SOUND            : "); printfColor(cgfg,cgbg,"   %s",strupr(metName));
                printx(cgfg,cgbg,10,16,   "METRONOME VOLUME           : "); printfColor(cgfg,cgbg,"   %3d Percent",(int)met.Vol);
                printx(cgfg,cgbg,10,18,   "METRONOME BEAT             : "); printfColor(cgfg,cgbg,"   %3d Beat(s)/bar",met.Beat);
                printx(cgfg,cgbg,10,20,   "METRONOME BPM              : "); printfColor(cgfg,cgbg,"   %3d Beat(s)/min",(int)met.Bpm);
                printx(cgfg,cgbg,10,22,   "SUSTAIN FACTOR             : "); printfColor(cgfg,cgbg,"   %3d Percent",(int)pno.sustainFactor);
                int center=50;
                char navbar[3][64]={"      APPLY      ",
                                    "     DEFAULT     ",
                                    "      CANCLE     "};
                for(int a=0;a<3;a++) printx((submenu==SUBMENU_SOUND)?settingVar.color[COLORSET_TEXT_DEACTIVE]:cgbg,(submenu==SUBMENU_SOUND)?settingVar.color[COLORSET_BUTTON_DEACTIVE]:cgbg,(a*18)+center,41,navbar[a]);
            }
            else if(loop[x]==SUBMENU_ADVANCE){
                int cgfg=(submenu==SUBMENU_ADVANCE)?settingVar.color[COLORSET_GLOBAL_FG]:cgbg;
                char regname[64];
                getRegName(&regname);
                printx(cgfg,cgbg,10,8,    "CLOCK LOCALIZATION         : "); printfColor(cgfg,cgbg,"   UTC%s%d ",(settingVar.clockoffset<0)?" \b":"+",settingVar.clockoffset);
                printx(cgfg,cgbg,10,10,   "INTERNET TIMEOUT           : "); printfColor(cgfg,cgbg,"   %3d second",settingVar.inetTimeOut);
                printx(cgfg,cgbg,10,12,   "USER NAME                  : "); printfColor(cgfg,cgbg,"   %s",regname);
                int center=50;
                char navbar[3][64]={"      APPLY      ",
                                    "     DEFAULT     ",
                                    "      CANCLE     "};
                for(int a=0;a<3;a++) printx((submenu==SUBMENU_ADVANCE)?settingVar.color[COLORSET_TEXT_DEACTIVE]:cgbg,(submenu==SUBMENU_ADVANCE)?settingVar.color[COLORSET_BUTTON_DEACTIVE]:cgbg,(a*18)+center,41,navbar[a]);
            }
            else if(loop[x]==SUBMENU_EXIT){
                int cgfg=(submenu==SUBMENU_EXIT)?settingVar.color[COLORSET_GLOBAL_FG]:cgbg;
                printx(cgfg,cgbg,10,8,    "PRESS ENTER TO PROCEED...");
            }
        }
    }
    else if(state==ENTER_SUBMENU){
        int colorTemp[11]={0};
        int screenmodeTemp=settingVar.ScreenMode;
        int colorPresetTemp=settingVar.colorPreset;
        boolean intromodetemp=settingVar.IntroMode;
        for(int a=0;a<=10;a++) colorTemp[a]=settingVar.color[a];
        printx(cgbg,cgbg,19,4," ");
        printx(cgbg,cgbg,79,4," ");
        playNotif(NOTIF_OPEN);
        if(submenu==SUBMENU_DISPLAY){
            int count=0;
            int countTemp=-1;
            int navVal=0;
            int navValTemp=0;
            int center=50;
            boolean custom=FALSE;
            char navbar[4][64]={"      APPLY      ",
                                "     DEFAULT     ",
                                "      CANCLE     ",
                                "       SAVE      "};
            while(1){
                if(count!=countTemp){
                    if(countTemp!=-1){
                        boxLineErase(40,(countTemp*2)+7,20,1);
                        playNotif(NOTIF_TICK);
                    }
                    if(count!=14) boxLine(40,(count*2)+7,20,1);
                    countTemp=count;
                }
                int x=getch();
                if(x==UP_ARROW){
                    count--;
                    if(count<0) count=14;
                }
                else if(x==DOWN_ARROW){
                    count++;
                    if(count>14) count=0;
                }
                else if(x==ESCAPE){
                    boxLineErase(40,(countTemp*2)+7,20,1);
                    playNotif(NOTIF_CANCLE);
                    WindowSubMenu(PRINT_SUBMENU,SUBMENU_DISPLAY,SUBMENU_DISPLAY);
                    printx(cgbg,cgbg,47,41," ");
                    printx(cgbg,cgbg,123,41," ");
                    return 0;
                }
                else if(x==LEFT_ARROW&&(count>=1&&count<=11)){
                    colorTemp[count-1]--;
                    if(colorTemp[count-1]<0)colorTemp[count-1]=0xF;
                    printx(cgbg,colorTemp[count-1],42,((count-1)*2)+10,"                 ");
                    colorPresetTemp=1;
                }else if(x==RIGHT_ARROW&&(count>=1&&count<=11)){
                    colorTemp[count-1]++;
                    if(colorTemp[count-1]>15)colorTemp[count-1]=0;
                    printx(cgbg,colorTemp[count-1],42,((count-1)*2)+10,"                 ");
                    colorPresetTemp=1;
                }else if((x==LEFT_ARROW||x==RIGHT_ARROW)&&count==12){
                    screenmodeTemp=(screenmodeTemp==FULLSCREEN)?WINDOWED:FULLSCREEN;
                    printfPosition(42,32,(screenmodeTemp==FULLSCREEN)?"    FULLSCREEN   ":"     WINDOWED    ");
                    if(screenmodeTemp==WINDOWED) printx(settingVar.color[COLORSET_GLOBAL_FG],cgbg,62,32,"You must restart to change the effect");
                }else if(x==RIGHT_ARROW&&count==0){
                    if(colorPresetTemp<settingVar.colorPresetMax) colorPresetTemp++;
                    else if(colorPresetTemp>=settingVar.colorPresetMax) colorPresetTemp=0;
                    char name[32]={0};
                    GetColorSetting(colorPresetTemp,&colorTemp,name);
                    printx(cgbg,cgbg,42,8,"                 ");
                    printx(settingVar.color[COLORSET_GLOBAL_FG],cgbg,42,8,strupr(name));
                    for(int a=0;a<=10;a++) printx(cgbg,colorTemp[a],42,(a*2)+10,"                 ");
                }else if(x==LEFT_ARROW&&count==0){
                    if(colorPresetTemp>0) colorPresetTemp--;
                    else if(colorPresetTemp==0) colorPresetTemp=settingVar.colorPresetMax;
                    char name[32]={0};
                    GetColorSetting(colorPresetTemp,&colorTemp,name);
                    printx(cgbg,cgbg,42,8,"                 ");
                    printx(settingVar.color[COLORSET_GLOBAL_FG],cgbg,42,8,strupr(name));
                    for(int a=0;a<=10;a++) printx(cgbg,colorTemp[a],42,(a*2)+10,"                 ");
                }
                if(colorPresetTemp==1){
                    printx(cgbg,cgbg,42,8,"                 ");
                    printx(settingVar.color[COLORSET_GLOBAL_FG],cgbg,42,8,"CUSTOM");
                    custom=TRUE;
                }else if(colorPresetTemp!=1){
                    custom=FALSE;
                }
                if(count==14){
                    if(x==RIGHT_ARROW){
                        navVal++;
                        if(navVal>3)navVal=0;
                    }else if(x==LEFT_ARROW){
                        navVal--;
                        if(navVal<0)navVal=3;
                    }
                    printfPosition(47,41,"%c",174);
                    printfPosition(123,41,"%c",175);
                    printx(settingVar.color[COLORSET_TEXT_ACTIVE],settingVar.color[COLORSET_BUTTON_ACTIVE],(navVal*18)+center,41,navbar[navVal]);
                    if(navVal!=navValTemp){
                        playNotif(NOTIF_TICK);
                        printx(settingVar.color[COLORSET_TEXT_DEACTIVE],settingVar.color[COLORSET_BUTTON_DEACTIVE],(navValTemp*18)+center,41,navbar[navValTemp]);
                        navValTemp=navVal;
                    }
                }else if(countTemp==14&&count!=14){
                    printx(cgbg,cgbg,47,41," ");
                    printx(cgbg,cgbg,123,41," ");
                    printx(settingVar.color[COLORSET_TEXT_DEACTIVE],settingVar.color[COLORSET_BUTTON_DEACTIVE],(navValTemp*18)+center,41,navbar[navValTemp]);
                    navVal=0;
                    navValTemp=0;
                }else if(count==13){
                    if(x==RIGHT_ARROW||x==LEFT_ARROW){
                        intromodetemp=(intromodetemp==FALSE)?TRUE:FALSE;
                        printx(settingVar.color[COLORSET_GLOBAL_FG],cgbg,42,34,(intromodetemp==TRUE)?       "      ENABLE     ":"      DISABLE    ");
                    }
                }
                if((x==ENTER&&count==14)||(x==ENTER&&count>=0&&count<14)){
                    if(navVal==SUBMENU_CANCLE&&count==14){
                        printx(cgbg,cgbg,47,41," ");
                        printx(cgbg,cgbg,123,41," ");
                        playNotif(NOTIF_CANCLE);
                        WindowSubMenu(PRINT_SUBMENU,SUBMENU_DISPLAY,SUBMENU_DISPLAY);
                        return 0;
                    }else if((navVal==SUBMENU_APPLY&&count==14)||(x==ENTER&&count>=0&&count<14)){
                        playNotif(NOTIF_UPDATE);
                        for(int b=0;b<11;b++) settingVar.color[b]=colorTemp[b];
                        settingVar.ScreenMode=screenmodeTemp;
                        settingVar.colorPreset=colorPresetTemp;
                        settingVar.IntroMode=intromodetemp;
                        if(custom==TRUE){
                            FILE * fp;
                            fp=fopen("bin\\data\\setting\\Custom.thme","w");
                            for(int a=0;a<11;a++) fputc(colorTemp[a],fp);
                            fclose(fp);
                            settingVar.colorPreset=1;
                            colorPresetTemp=1;
                        }
                        setting(SAVE_SETTING);
                        init_console(INIT_MAIN_CONSOLE);
                        cgbg=settingVar.color[COLORSET_GLOBAL_BG];
                        WindowSubMenu(PRINT_SUBMENU,SUBMENU_DISPLAY,SUBMENU_DISPLAY);
                        printx(cgbg,cgbg,47,41," ");
                        printx(cgbg,cgbg,123,41," ");
                        WinSettingHeader(0);
                        if(count==14) return 0;
                        else boxLine(40,(count*2)+7,20,1);
                    }else if(navVal==SUBMENU_DEFAULT&&count==14){
                        playNotif(NOTIF_OPEN);
                        char name[64];
                        GetColorSetting(COLOR_DEFAULT,&colorTemp,name);
                        printx(cgbg,cgbg,42,8,"                 ");
                        printx(settingVar.color[COLORSET_GLOBAL_FG],cgbg,42,8,strupr(name));
                        colorPresetTemp=0;
                        screenmodeTemp=FULLSCREEN;
                        cgbg=settingVar.color[COLORSET_GLOBAL_BG];
                        for(int a=0;a<=10;a++) printx(cgbg,colorTemp[a],42,(a*2)+10,"                 ");
                        printx(settingVar.color[COLORSET_GLOBAL_FG],cgbg,42,32,(screenmodeTemp==FULLSCREEN)?"    FULLSCREEN   ":"     WINDOWED    ");
                        printx(cgbg,cgbg,62,32,"You must restart to change the effect");
                    }else if(navVal==SUBMENU_SAVE){
                        playNotif(NOTIF_ENTER);
                        boolean cancled=FALSE;
                        char dir[32];
                        boxLine(75,37,20,1);
                        printfPosition(75,36,"THEME NAME :");
                        cursor(77,38);
                        int val=0;
                        cursorvisibility(SHOW_CURSOR);
                        while(1){
                            char x=_getch();
                            if(x==ENTER) break;
                            if(x==ESCAPE){
                                cancled=TRUE;
                                goto CANCLE;
                            }
                            else if(x==BACKSPACE&&val>0){
                                playNotif(NOTIF_TICK);
                                printf("\b \b");
                                dir[val]=0;
                                val--;
                            }
                            else if(x!=BACKSPACE&&val<17){
                                playNotif(NOTIF_TICK);
                                printf("%c",x);
                                dir[val]=x;
                                val++;
                            }else if(val==17||val==0) playNotif(NOTIF_ERROR);
                            cursor(77+val,38);
                        }
                        playNotif(NOTIF_SAVED);
                        cursorvisibility(HIDE_CURSOR);
                        boxFill(75,36,21,4,settingVar.color[COLORSET_GLOBAL_BG]);
                        SaveColorSetting(dir,colorTemp);
                        settingVar.colorPreset=settingVar.colorPresetMax+1;
                        colorPresetTemp=settingVar.colorPreset;
                        WindowSubMenu(PRINT_SUBMENU,SUBMENU_DISPLAY,SUBMENU_DISPLAY);
                        custom=FALSE;
                        CANCLE:;
                        if(cancled==TRUE){
                            playNotif(NOTIF_CANCLE);
                            cursorvisibility(HIDE_CURSOR);
                            boxFill(75,36,21,4,settingVar.color[COLORSET_GLOBAL_BG]);
                        }
                    }
                }
            }
        }
        else if(submenu==SUBMENU_SOUND){
            int center=50;
            int temp_notifPreset=notif.Preset;
            float temp_notifVol=notif.Vol;
            boolean temp_notifState=notif.state;
            int temp_metPreset=met.Preset;
            float temp_metVol=met.Vol;
            int temp_metBear=met.Beat;
            double temp_metBpm=met.Bpm;
            float temp_susFact=pno.sustainFactor;
            char navbar[3][64]={"      APPLY      ",
                                "     DEFAULT     ",
                                "      CANCLE     "};
            for(int a=0;a<3;a++) printx((submenu==SUBMENU_SOUND)?settingVar.color[COLORSET_TEXT_DEACTIVE]:cgbg,(submenu==SUBMENU_SOUND)?settingVar.color[COLORSET_BUTTON_DEACTIVE]:cgbg,(a*18)+center,41,navbar[a]);
            int count=0;
            int countTemp=-1;
            int navVal=0;
            int navValTemp=0;
            int notifVal=notif.Preset;
            int notifValMax=notif.maxPreset;
            int metVal=met.Preset;
            int metValMax=met.maxPreset;
            int notifvolVal=(int)notif.Vol;
            int metVol=(int)met.Vol;
            int beat=met.Beat;
            int Bpm=(int)met.Bpm;
            int sust=(int)pno.sustainFactor;
            boolean stateTemp=notif.state;
            while(1){
                if(count!=countTemp){
                    if(countTemp!=-1){
                        boxLineErase(40,(countTemp*2)+7,20,1);
                        playNotif(NOTIF_TICK);
                    }
                    if(count!=8) boxLine(40,(count*2)+7,20,1);
                    countTemp=count;
                }
                int x=getch();
                if(x==UP_ARROW){
                    count--;
                    if(count<0) count=8;
                }
                else if(x==DOWN_ARROW){
                    count++;
                    if(count>8) count=0;
                }
                else if(x==ESCAPE){
                    boxLineErase(40,(countTemp*2)+7,20,1);
                    playNotif(NOTIF_CANCLE);
                    if(count==8){
                        printx(settingVar.color[COLORSET_TEXT_DEACTIVE],settingVar.color[COLORSET_BUTTON_DEACTIVE],(navValTemp*18)+center,41,navbar[navValTemp]);
                        printx(cgbg,cgbg,47,41," ");
                        printx(cgbg,cgbg,105,41," ");
                    }
                    WindowSubMenu(PRINT_SUBMENU,SUBMENU_SOUND,SUBMENU_SOUND);
                    return 0;
                }
                else if(x==ENTER){
                    if(count==8&&navVal==SUBMENU_DEFAULT){ //SET DEFAULT
                        metVal=0;
                        Bpm=120;
                        beat=4;
                        metVol=50;
                        notifvolVal=50;
                        notifVal=0;
                        stateTemp=TRUE;
                        sust=3; //RANGE 1-100
                        int cgfg=settingVar.color[COLORSET_GLOBAL_FG];
                        char notifName[64], metName[64];
                        met.maxPreset=getConfigData("Metronome",metVal,&metName);
                        notif.maxPreset=getConfigData("Notification",notifVal,&notifName);
                        printx(cgfg,cgbg,42,8,"                 ");
                        printx(cgfg,cgbg,42,14,"                 ");
                        printfPosition(42,8,strupr(notifName));
                        printfPosition(42,10,"%3d",notifvolVal);
                        printfPosition(42,12,"      ENABLE     ");
                        printfPosition(42,14,"%s",strupr(metName));
                        printfPosition(42,16,"%3d",metVol);
                        printfPosition(42,18,"%3d",beat);
                        printfPosition(42,20,"%3d",Bpm);
                        printfPosition(42,22,"%3d",sust);
                        playNotif(NOTIF_OPEN);
                    }else if(count==8&&navVal==SUBMENU_CANCLE){ //CANCLE
                        playNotif(NOTIF_CANCLE);
                    }else {
                        notif.Preset=notifVal;
                        met.Preset=metVal;
                        notif.Vol=(float)notifvolVal;
                        met.Vol=(float)metVol;
                        met.Beat=beat;
                        met.Bpm=(double)Bpm;
                        pno.sustainFactor=(float)sust;
                        notif.state=stateTemp;
                        setting(SAVE_SETTING);
                        playNotif(NOTIF_UPDATE);
                    }
                    if(count==8&&navVal!=1){
                        printx(settingVar.color[COLORSET_TEXT_DEACTIVE],settingVar.color[COLORSET_BUTTON_DEACTIVE],(navValTemp*18)+center,41,navbar[navValTemp]);
                        printx(cgbg,cgbg,47,41," ");
                        printx(cgbg,cgbg,105,41," ");
                        WindowSubMenu(PRINT_SUBMENU,SUBMENU_SOUND,SUBMENU_SOUND);
                        return 0;
                    }
                }
                if(count==0){
                    char notifName[64];
                    if(x==RIGHT_ARROW){
                        if(notifVal<notifValMax) notifVal++;
                        else if(notifVal>=notifValMax) notifVal=0;
                        notifValMax=getConfigData("Notification",notifVal,&notifName);
                        printfPosition(42,(count*2)+8,"                  ");
                        printfPosition(42,(count*2)+8,strupr(notifName));
                    }else if(x==LEFT_ARROW){
                        if(notifVal>0) notifVal--;
                        else if(notifVal==0) notifVal=notifValMax;
                        notifValMax=getConfigData("Notification",notifVal,&notifName);
                        printfPosition(42,(count*2)+8,"                  ");
                        printfPosition(42,(count*2)+8,strupr(notifName));
                    }
                }else if(count==3){
                    char metName[64];
                    if(x==RIGHT_ARROW){
                        if(metVal<metValMax) metVal++;
                        else if(metVal>=metValMax) metVal=0;
                        metValMax=getConfigData("Metronome",metVal,&metName);
                        printfPosition(42,(count*2)+8,"                  ");
                        printfPosition(42,(count*2)+8,strupr(metName));
                    }else if(x==LEFT_ARROW){
                        if(metVal>0) metVal--;
                        else if(metVal==0) metVal=metValMax;
                        metValMax=getConfigData("Metronome",metVal,&metName);
                        printfPosition(42,(count*2)+8,"                  ");
                        printfPosition(42,(count*2)+8,strupr(metName));
                    }
                }
                else if(count==1){ //NOTIF VOL
                    if(x==RIGHT_ARROW){
                        if(notifvolVal<100) notifvolVal++;
                        else if(notifvolVal>=100) notifvolVal=0;
                        printfPosition(42,(count*2)+8,"%3d",notifvolVal);
                    }else if(x==LEFT_ARROW){
                        if(notifvolVal>0) notifvolVal--;
                        else if(notifvolVal==0) notifvolVal=100;
                        printfPosition(42,(count*2)+8,"%3d",notifvolVal);
                    }else if(x>='0'&&x<='9'){
                        printfPosition(42,(count*2)+8,"   ");
                        int input=getManualInput(42,(count*2)+8,0,100,3,x);
                        if(input!=NO_INPUT_MANUAL){
                            notifvolVal=input;
                            printfPosition(42,(count*2)+8,"%3d",notifvolVal);
                        }else printfPosition(42,(count*2)+8,"%3d",notifvolVal);
                    }
                }
                else if(count==4){ //MET VOL
                    if(x==RIGHT_ARROW){
                        if(metVol<100) metVol++;
                        else if(metVol>=100) metVol=0;
                        printfPosition(42,(count*2)+8,"%3d",metVol);
                    }else if(x==LEFT_ARROW){
                        if(metVol>0) metVol--;
                        else if(metVol==0) metVol=100;
                        printfPosition(42,(count*2)+8,"%3d",metVol);
                    }else if(x>='0'&&x<='9'){
                        printfPosition(42,(count*2)+8,"   ");
                        int input=getManualInput(42,(count*2)+8,0,100,3,x);
                        if(input!=NO_INPUT_MANUAL){
                            metVol=input;
                            printfPosition(42,(count*2)+8,"%3d",metVol);
                        }else printfPosition(42,(count*2)+8,"%3d",metVol);
                    }
                }
                else if(count==5){ //BEAT
                    if(x==RIGHT_ARROW){
                        if(beat<10) beat++;
                        else if(beat>=10) beat=1;
                        printfPosition(42,(count*2)+8,"%3d",beat);
                    }else if(x==LEFT_ARROW){
                        if(beat>1) beat--;
                        else if(beat==1) beat=10;
                        printfPosition(42,(count*2)+8,"%3d",beat);
                    }else if(x>='0'&&x<='9'){
                        printfPosition(42,(count*2)+8,"   ");
                        int input=getManualInput(42,(count*2)+8,1,10,2,x);
                        if(input!=NO_INPUT_MANUAL){
                            beat=input;
                            printfPosition(42,(count*2)+8,"%3d",beat);
                        }else printfPosition(42,(count*2)+8,"%3d",beat);
                    }
                }
                else if(count==6){ //BPM
                    if(x==RIGHT_ARROW){
                        if(Bpm<600) Bpm++;
                        else if(Bpm>=600) Bpm=1;
                        printfPosition(42,(count*2)+8,"%3d",Bpm);
                    }else if(x==LEFT_ARROW){
                        if(Bpm>1) Bpm--;
                        else if(Bpm==1) Bpm=600;
                        printfPosition(42,(count*2)+8,"%3d",Bpm);
                    }else if(x>='0'&&x<='9'){
                        printfPosition(42,(count*2)+8,"   ");
                        int input=getManualInput(42,(count*2)+8,1,600,3,x);
                        if(input!=NO_INPUT_MANUAL){
                            Bpm=input;
                            printfPosition(42,(count*2)+8,"%3d",Bpm);
                        }else printfPosition(42,(count*2)+8,"%3d",Bpm);
                    }
                }
                else if(count==7){ //SUSTAIN
                    if(x==RIGHT_ARROW){
                        if(sust<100) sust++;
                        else if(sust>=100) sust=1;
                        printfPosition(42,(count*2)+8,"%3d",sust);
                    }else if(x==LEFT_ARROW){
                        if(sust>1) sust--;
                        else if(sust==1) sust=100;
                        printfPosition(42,(count*2)+8,"%3d",sust);
                    }else if(x>='0'&&x<='9'){
                        printfPosition(42,(count*2)+8,"   ");
                        int input=getManualInput(42,(count*2)+8,1,100,3,x);
                        if(input!=NO_INPUT_MANUAL){
                            sust=input;
                            printfPosition(42,(count*2)+8,"%3d",sust);
                        }else printfPosition(42,(count*2)+8,"%3d",sust);
                    }
                }
                else if(count==8){
                    if(x==RIGHT_ARROW){
                        navVal++;
                        if(navVal>2)navVal=0;
                    }else if(x==LEFT_ARROW){
                        navVal--;
                        if(navVal<0)navVal=2;
                    }
                    printfPosition(47,41,"%c",174);
                    printfPosition(105,41,"%c",175);
                    printx(settingVar.color[COLORSET_TEXT_ACTIVE],settingVar.color[COLORSET_BUTTON_ACTIVE],(navVal*18)+center,41,navbar[navVal]);
                    if(navVal!=navValTemp){
                        playNotif(NOTIF_TICK);
                        printx(settingVar.color[COLORSET_TEXT_DEACTIVE],settingVar.color[COLORSET_BUTTON_DEACTIVE],(navValTemp*18)+center,41,navbar[navValTemp]);
                        navValTemp=navVal;
                    }
                }
                else if(count==2){
                    if(x==LEFT_ARROW||x==RIGHT_ARROW){
                        stateTemp=(stateTemp==TRUE)?FALSE:TRUE;
                        printx(settingVar.color[COLORSET_GLOBAL_FG],cgbg,42,(count*2)+8,(stateTemp==TRUE)?"      ENABLE     ":"     DISABLE     ");
                    }
                }
                if(countTemp==8&&count!=8){
                    printx(cgbg,cgbg,47,41," ");
                    printx(cgbg,cgbg,105,41," ");
                    printx(settingVar.color[COLORSET_TEXT_DEACTIVE],settingVar.color[COLORSET_BUTTON_DEACTIVE],(navValTemp*18)+center,41,navbar[navValTemp]);
                    navVal=0;
                    navValTemp=0;
                }
            }
        }
        else if(submenu==SUBMENU_ADVANCE){
            int center=50;
            int timeout=settingVar.inetTimeOut;
            int utc=settingVar.clockoffset;
            char navbar[3][64]={"      APPLY      ",
                                "     DEFAULT     ",
                                "      CANCLE     "};
            for(int a=0;a<3;a++) printx((submenu==SUBMENU_ADVANCE)?settingVar.color[COLORSET_TEXT_DEACTIVE]:cgbg,(submenu==SUBMENU_ADVANCE)?settingVar.color[COLORSET_BUTTON_DEACTIVE]:cgbg,(a*18)+center,41,navbar[a]);
            int count=0;
            int countTemp=-1;
            int navVal=0;
            int navValTemp=0;
            char myName[64];
            getRegName(&myName);
            while(1){
                if(count!=countTemp){
                    if(countTemp==2&&count!=2) boxLineErase(40,(countTemp*2)+7,40,1);
                    if(countTemp!=-1){
                        if(countTemp!=2) boxLineErase(40,(countTemp*2)+7,20,1);
                        playNotif(NOTIF_TICK);
                    }
                    if(count!=2&&count!=3) boxLine(40,(count*2)+7,20,1);
                    else if(count==2){
                        boxLine(40,(count*2)+7,40,1);
                    }
                    countTemp=count;
                }
                int x=getch();
                if(x==224) x=_getch();
                if(x==UP_ARROW){
                    count--;
                    if(count<0) count=3;
                }
                else if(x==DOWN_ARROW){
                    count++;
                    if(count>3) count=0;
                }
                else if(x==ESCAPE||(count==3&&navVal==SUBMENU_CANCLE&&x==ENTER)){
                    if (countTemp==2) boxLineErase(40,(countTemp*2)+7,40,1);
                    else if(countTemp!=2) boxLineErase(40,(countTemp*2)+7,20,1);
                    playNotif(NOTIF_CANCLE);
                    if(count==3){
                        printx(settingVar.color[COLORSET_TEXT_DEACTIVE],settingVar.color[COLORSET_BUTTON_DEACTIVE],(navValTemp*18)+center,41,navbar[navValTemp]);
                        printx(cgbg,cgbg,47,41," ");
                        printx(cgbg,cgbg,105,41," ");
                    }
                    WindowSubMenu(PRINT_SUBMENU,SUBMENU_ADVANCE,SUBMENU_ADVANCE);
                    return 0;
                }
                if(count==3&&x!=ENTER){
                    if(x==RIGHT_ARROW){
                        navVal++;
                        if(navVal>2)navVal=0;
                    }else if(x==LEFT_ARROW){
                        navVal--;
                        if(navVal<0)navVal=2;
                    }
                    printfPosition(47,41,"%c",174);
                    printfPosition(105,41,"%c",175);
                    printx(settingVar.color[COLORSET_TEXT_ACTIVE],settingVar.color[COLORSET_BUTTON_ACTIVE],(navVal*18)+center,41,navbar[navVal]);
                    if(navVal!=navValTemp){
                        playNotif(NOTIF_TICK);
                        printx(settingVar.color[COLORSET_TEXT_DEACTIVE],settingVar.color[COLORSET_BUTTON_DEACTIVE],(navValTemp*18)+center,41,navbar[navValTemp]);
                        navValTemp=navVal;
                    }
                }else if(count==3&&x==ENTER){
                    if(navVal==SUBMENU_APPLY&&x==ENTER){
                        playNotif(NOTIF_UPDATE);
                        printx(settingVar.color[COLORSET_TEXT_DEACTIVE],settingVar.color[COLORSET_BUTTON_DEACTIVE],(navValTemp*18)+center,41,navbar[navValTemp]);
                        printx(cgbg,cgbg,47,41," ");
                        printx(cgbg,cgbg,105,41," ");
                        settingVar.inetTimeOut=timeout;
                        settingVar.clockoffset=utc;
                        writeRegName(myName);
                        setting(SAVE_SETTING);
                        for(int a=0;a<32;a++) printf(" ");
                        WindowSubMenu(PRINT_SUBMENU,SUBMENU_ADVANCE,SUBMENU_ADVANCE);
                        return 0;
                    }else if(navVal==SUBMENU_DEFAULT&&x==ENTER){
                        timeout=10;
                        utc=7;
                        for(int a=0;a<64;a++)myName[a]=0;
                        getRegName(&myName);
                        cursor(42,12);
                        for(int a=0;a<32;a++) printf(" ");
                        printfPosition(42,10,"%3d",timeout);
                        printfPosition(45,8,"%s%d ",(utc<0)?" \b":"+",utc);
                        printfPosition(42,12,myName);
                        playNotif(NOTIF_OPEN);

                    }
                }
                else if(countTemp==3&&count!=3){
                    printx(cgbg,cgbg,47,41," ");
                    printx(cgbg,cgbg,105,41," ");
                    printx(settingVar.color[COLORSET_TEXT_DEACTIVE],settingVar.color[COLORSET_BUTTON_DEACTIVE],(navValTemp*18)+center,41,navbar[navValTemp]);
                    navVal=0;
                    navValTemp=0;
                }else if((count==2&&x==ENTER)||(count==2&&x>='?'&&x<='O')||(count==2&&x>='Q'&&x<='z')){
                    cursor(42,(count*2)+8);
                    for(int a=0;a<32;a++) printf(" ");
                    int loop=(x==ENTER)?0:1;
                    char myNameTemp[64]={0};
                    if(x!=ENTER) myNameTemp[0]=x;
                    cursor(42,(count*2)+8);
                    if(x!=ENTER)printf("%c",x);
                    cursorvisibility(SHOW_CURSOR);
                    while(1){
                        int x=_getch();
                        if(x==ENTER){
                            playNotif(NOTIF_SAVED);
                            break;
                        }else if(x==ESCAPE){
                            playNotif(NOTIF_CANCLE);
                            goto CANCLED;
                        }else if(x==BACKSPACE&&loop>0){
                            playNotif(NOTIF_TICK);
                            printf("\b \b");
                            myNameTemp[loop]='\0';
                            loop--;
                        }else if(x!=BACKSPACE&&loop<32){
                            playNotif(NOTIF_TICK);
                            printf("%c",x);
                            myNameTemp[loop]=x;
                            loop++;
                            myNameTemp[loop]='\0';
                        }else if(loop==0||loop==32) playNotif(NOTIF_ERROR);
                    }
                    for(int a=0;a<64;a++)myName[a]=0;
                    strcat(&myName,myNameTemp);
                    CANCLED:;
                    cursorvisibility(HIDE_CURSOR);
                    cursor(42,(count*2)+8);
                    for(int a=0;a<32;a++) printf(" ");
                    printfPosition(42,(count*2)+8,myName);
                }
                else if(count==0){
                    if(x==LEFT_ARROW){
                        utc--;
                        if(utc<-12)utc=12;
                        printfPosition(45,(count*2)+8,"%s%d ",(utc<0)?" \b":"+",utc);
                    }else if(x==RIGHT_ARROW){
                        utc++;
                        if(utc>12)utc=-12;
                        printfPosition(45,(count*2)+8,"%s%d ",(utc<0)?" \b":"+",utc);
                    }
                }else if(count==1){
                    if(x>='0'&&x<='9'){
                        printfPosition(42,(count*2)+8,"   ");
                        int input=getManualInput(42,(count*2)+8,1,600,3,x);
                        if(input!=NO_INPUT_MANUAL){
                            timeout=input;
                            printfPosition(42,(count*2)+8,"%3d",timeout);
                        }else printfPosition(42,(count*2)+8,"%3d",timeout);
                    }else if(x==LEFT_ARROW){
                        timeout--;
                        if(timeout<1)timeout=600;
                        printfPosition(42,(count*2)+8,"%3d",timeout);
                    }else if(x==RIGHT_ARROW){
                        timeout++;
                        if(timeout>600)timeout=1;
                        printfPosition(42,(count*2)+8,"%3d",timeout);
                    }
                }
            }
        }
    }
}

void WinSettingHeader(int submenu){
    char settingMenu[8][32]={"   DISPLAY   ",
                            "    SOUND    ",
                            "   ADVANCE   ",
                            "    BACK     "};
    printfPosition(10,4,"SETTING");
    cursor(10,5);
    for(int a=0;a<150;a++) printf("%c",196);
    for(int a=0;a<4;a++) printx(settingVar.color[COLORSET_TEXT_DEACTIVE],settingVar.color[COLORSET_BUTTON_DEACTIVE],(a*14)+22,4,settingMenu[a]);
    printx(settingVar.color[COLORSET_TEXT_ACTIVE],settingVar.color[COLORSET_BUTTON_ACTIVE],(submenu*14)+22,4,settingMenu[submenu]);
    cursor(10,42);
    for(int a=0;a<150;a++) printf("%c",196);
}

int WindowSetting(){
    char settingMenu[8][32]={"   DISPLAY   ",
                            "    SOUND    ",
                            "   ADVANCE   ",
                            "    BACK     "};
    printfPosition(19,4,"%c",174);
    printfPosition(79,4,"%c",175);
    int submenu=0;
    int submenuTemp=-1;
    WinSettingHeader(submenu);
    while(1){
        if(submenu!=submenuTemp){
            if(submenuTemp!=-1){
                playNotif(NOTIF_TICK);
                printx(settingVar.color[COLORSET_TEXT_DEACTIVE],settingVar.color[COLORSET_BUTTON_DEACTIVE],(submenuTemp*14)+22,4,settingMenu[submenuTemp]);

            }
            printx(settingVar.color[COLORSET_TEXT_ACTIVE],settingVar.color[COLORSET_BUTTON_ACTIVE],(submenu*14)+22,4,settingMenu[submenu]);
            WindowSubMenu(PRINT_SUBMENU,submenu,submenuTemp);
            submenuTemp=submenu;
        }
        int x=_getch();
        if(x==RIGHT_ARROW){
            submenu++;
            if(submenu>3)submenu=0;
        }else if(x==LEFT_ARROW){
            submenu--;
            if(submenu<0)submenu=3;
        }else if(x==DOWN_ARROW||x==ENTER){
            if(submenu==3){
                clrscreen();
                playNotif(NOTIF_ENTER);
                return 0;
            }
            WindowSubMenu(ENTER_SUBMENU,submenu,submenuTemp);
            printfPosition(19,4,"%c",174);
            printfPosition(79,4,"%c",175);
        }else if(x==ESCAPE){
                clrscreen();
                playNotif(NOTIF_CANCLE);
                return 0;
        }
    }
    clrscreen();
}

#define INIT_PIANO 1
#define UPDATE_PIANO 2
#define BLACK_NOTE_OR x=='w'||x=='e'||x=='t'||x=='y'||x=='u'||x=='o'||x=='p'
#define WHITE_NOTE_OR x=='a'||x=='s'||x=='d'||x=='f'||x=='g'||x=='h'||x=='j'||x=='k'||x=='l'||x==';'||x=='/'
void PianoDisp(const int state,const int locX,const int locY){
    COORD pos;
    pos.X=locX; pos.Y=locY;
    int loop=0;
    FILE * fp;
    if(state==INIT_PIANO){
        fp=fopen("bin\\data\\layout\\pno.pin","r");
        if(fp==NULL) error(ERROR_NO_DIRECTORY);
        int tempPos[2][INPUT_LEN];
        for(int a=0;a<INPUT_LEN;a++){
            tempPos[0][a]=0;
            tempPos[1][a]=1;
            for(int b=0;b<1024;b++) pno.lay[a][b]=0;
        }
        while(1){
            char x=fgetc(fp);
            loop++;
            SetConsoleCursorPosition(Handle.stdOutput,pos);
            if(x==EOF) break;
            else if(BLACK_NOTE_OR||WHITE_NOTE_OR){
                if(BLACK_NOTE_OR) printfColor(0,settingVar.color[COLORSET_PNO_SHARP_DEACTIVE]," ");
                else if(WHITE_NOTE_OR) printfColor(0,settingVar.color[COLORSET_PNO_NATURAL_DEACTIVE]," ");
                for(int a=0;a<INPUT_LEN;a++) if(x==notes_pos[a]){
                    pno.lay[a][tempPos[0][a]]=pos.X;
                    pno.lay[a][tempPos[1][a]]=pos.Y;
                    tempPos[0][a]+=2;
                    tempPos[1][a]+=2;
                }
            }
            else if(x==' ') printf("%c",' ');
            else if(x=='\n'){
                pos.X=locX-1;
                pos.Y++;
            }
            pos.X++;

        }
        fclose(fp);
        for(int a=0;a<INPUT_LEN;a++) pno.tempAct[a]=FALSE;
    }else if(state==UPDATE_PIANO){
        for(int b=0;b<INPUT_LEN;b++){
            if(pno.state[0][b]==1&&pno.tempAct[b]==FALSE){
                for(int a=0;a<1024;a+=2){
                    if(pno.lay[b][a]==0)break;
                    pos.X=pno.lay[b][a];
                    pos.Y=pno.lay[b][a+1];
                    SetConsoleCursorPosition(Handle.stdOutput,pos);
                    if(b<10)printfColor(0,settingVar.color[COLORSET_PNO_SHARP_ACTIVE]," ");
                    else if(b>10)printfColor(0,settingVar.color[COLORSET_PNO_NATURAL_ACTIVE]," ");
                }
                pno.tempAct[b]=TRUE;
            }
            else if(pno.state[0][b]==0&&pno.tempAct[b]==TRUE){
                for(int a=0;a<1024;a+=2){
                    if(pno.lay[b][a]==0)break;
                    pos.X=pno.lay[b][a];
                    pos.Y=pno.lay[b][a+1];
                    SetConsoleCursorPosition(Handle.stdOutput,pos);
                    if(b<10)printfColor(0,settingVar.color[COLORSET_PNO_SHARP_DEACTIVE]," ");
                    else if(b>10)printfColor(0,settingVar.color[COLORSET_PNO_NATURAL_DEACTIVE]," ");
                }
                pno.tempAct[b]=FALSE;
            }
        }
    }
}

DWORD WINAPI WindowPianoPlayer(void){
    clrscreen();
    playNotif(NOTIF_MAXIMAZE);
    int piano_posX=41;
    int piano_posY=13;
    const int sizeX=7;
    const int gap=-1;
    const int sizeY=3;
    const int navPosY=piano_posY-4;
    const int startPosX=piano_posX-7;
    const char myMenu[7][8]= {"   1   ",
                              "   2   ",
                              "   3   ",
                              "   4   ",
                              "   5   ",
                              "   6   ",
                              "   7   "};
    footer();
    for(int a=0;a<7;a++){
        const int navPosX=startPosX+(sizeX+a)+((gap+sizeX)*a);
        if(a!=(pno.octv-1)) boxFill(navPosX,navPosY,sizeX,sizeY,settingVar.color[COLORSET_BUTTON_DEACTIVE]);
        else if(a==(pno.octv-1))boxFill(navPosX,navPosY,sizeX,sizeY,settingVar.color[COLORSET_BUTTON_ACTIVE]);
        cursor(navPosX,navPosY+1);
        if(a!=(pno.octv-1)) printfColor(settingVar.color[COLORSET_TEXT_DEACTIVE],settingVar.color[COLORSET_BUTTON_DEACTIVE],myMenu[a]);
        else if(a==(pno.octv-1)) printfColor(settingVar.color[COLORSET_TEXT_ACTIVE],settingVar.color[COLORSET_BUTTON_ACTIVE],myMenu[a]);
    }
    boxFill(111,9,17,3,(pno.sustain==TRUE)?settingVar.color[COLORSET_BUTTON_ACTIVE]:settingVar.color[COLORSET_BUTTON_DEACTIVE]);
    cursor(114,10);
    if(pno.sustain==TRUE) printfColor(settingVar.color[COLORSET_TEXT_ACTIVE],settingVar.color[COLORSET_BUTTON_ACTIVE],"SUSTAIN: ON");
    else if(pno.sustain==FALSE) printfColor(settingVar.color[COLORSET_TEXT_DEACTIVE],settingVar.color[COLORSET_BUTTON_DEACTIVE],"SUSTAIN:OFF");
    boxLine(132,13,2,17);
    printfPosition(133,12,"+");
    printfPosition(133,32,"-");
    int mybar=(pno.globalVol/100)*16;
    for(int a=0;a<=mybar;a++) printfPosition(133,30-a,"%c",177);
    boxFill(89,33,39,3,settingVar.color[COLORSET_STATIC_BUTTON]);
    int len=strlen(pno.namein);
    int boxlen=(39-len)/2;
    cursor(boxlen+89,34);
    printfColor(settingVar.color[COLORSET_GLOBAL_FG],settingVar.color[COLORSET_STATIC_BUTTON],"%s",pno.namein);
    boxFill(80,2,11,4,(met.state==FALSE)?settingVar.color[COLORSET_BUTTON_DEACTIVE]:settingVar.color[COLORSET_BUTTON_ACTIVE]);
    cursor(84,4); printfColor((met.state==FALSE)?settingVar.color[COLORSET_TEXT_DEACTIVE]:settingVar.color[COLORSET_TEXT_ACTIVE],(met.state==FALSE)?settingVar.color[COLORSET_BUTTON_DEACTIVE]:settingVar.color[COLORSET_BUTTON_ACTIVE],"BPM");
    cursor(84,3); printfColor((met.state==FALSE)?settingVar.color[COLORSET_TEXT_DEACTIVE]:settingVar.color[COLORSET_TEXT_ACTIVE],(met.state==FALSE)?settingVar.color[COLORSET_BUTTON_DEACTIVE]:settingVar.color[COLORSET_BUTTON_ACTIVE],"%3.0f",(float)met.Bpm);
    PianoDisp(INIT_PIANO,piano_posX,piano_posY);
    printfPosition(CENTER_OF_SCREEN,45,"F1 - HINTS");
    temp.sust=pno.sustain;
    temp.Octv=pno.octv;
    temp.Voltemp=pno.globalVol;
    temp.Bpm=met.Bpm;
    temp.Preset=pno.Preset;
    temp.met=met.state;
    temp.mute=pno.mute;
    while(pno.enable==TRUE){
        printClock();
        PianoDisp(UPDATE_PIANO,41,13);
        if(temp.sust!=pno.sustain){
            boxFill(111,9,17,3,(pno.sustain==TRUE)?settingVar.color[COLORSET_BUTTON_ACTIVE]:settingVar.color[COLORSET_BUTTON_DEACTIVE]);
            cursor(114,10);
            if(pno.sustain==TRUE){
                printfColor(settingVar.color[COLORSET_TEXT_ACTIVE],settingVar.color[COLORSET_BUTTON_ACTIVE],"SUSTAIN: ON");
                temp.sust=TRUE;
            }
            else if(pno.sustain==FALSE){
                printfColor(settingVar.color[COLORSET_TEXT_DEACTIVE],settingVar.color[COLORSET_BUTTON_DEACTIVE],"SUSTAIN:OFF");
                temp.sust=FALSE;
            }
        }
        if(temp.Octv!=pno.octv){
            for(int a=0;a<7;a++){
                const int navPosX=startPosX+(sizeX+a)+((gap+sizeX)*a);
                if(a==temp.Octv-1){
                    boxFill(navPosX,navPosY,sizeX,sizeY,settingVar.color[COLORSET_BUTTON_DEACTIVE]);
                    cursor(navPosX,navPosY+1);
                    printfColor(settingVar.color[COLORSET_TEXT_DEACTIVE],settingVar.color[COLORSET_BUTTON_DEACTIVE],myMenu[a]);
                }else if(a==pno.octv-1){
                    boxFill(navPosX,navPosY,sizeX,sizeY,settingVar.color[COLORSET_BUTTON_ACTIVE]);
                    cursor(navPosX,navPosY+1);
                    printfColor(settingVar.color[COLORSET_TEXT_DEACTIVE],settingVar.color[COLORSET_BUTTON_ACTIVE],myMenu[a]);
                }
            }
            temp.Octv=pno.octv;
        }
        if(pno.mute!=temp.mute){
            if(pno.mute==TRUE){
                for(int a=0;a<=16;a++) printfPosition(133,30-a," ");
            }else if(pno.mute==FALSE){
                int mybar=(pno.globalVol/100)*16;
                for(int a=0;a<=mybar;a++) printfPosition(133,30-a,"%c",177);
            }
        }
        else if(temp.Voltemp!=pno.globalVol){
            int bar=(pno.globalVol/100)*16;
            if(temp.Voltemp>pno.globalVol) printfPosition(133,29-bar," ");
            else if(temp.Voltemp<pno.globalVol) printfPosition(133,30-bar,"%c",177);
            else if(pno.globalVol==0)printfPosition(133,30," ");
            temp.Voltemp=pno.globalVol;
        }
        if(met.Bpm!=temp.Bpm){
            cursor(84,3); printfColor((met.state==FALSE)?settingVar.color[COLORSET_TEXT_DEACTIVE]:settingVar.color[COLORSET_TEXT_ACTIVE],(met.state==FALSE)?settingVar.color[COLORSET_BUTTON_DEACTIVE]:settingVar.color[COLORSET_BUTTON_ACTIVE],"%3.0f",(float)met.Bpm);
        }
        if(temp.Preset!=pno.Preset){
            boxFill(89,33,39,3,settingVar.color[COLORSET_STATIC_BUTTON]);
            int len=strlen(pno.namein);
            int boxlen=(39-len)/2;
            cursor(boxlen+89,34);
            printfColor(settingVar.color[COLORSET_GLOBAL_FG],settingVar.color[COLORSET_STATIC_BUTTON],"%s",pno.namein);
            temp.Preset=pno.Preset;
        }
        if(met.state!=temp.met){
            boxFill(80,2,11,4,(met.state==FALSE)?settingVar.color[COLORSET_BUTTON_DEACTIVE]:settingVar.color[COLORSET_BUTTON_ACTIVE]);
            cursor(84,4); printfColor((met.state==FALSE)?settingVar.color[COLORSET_TEXT_DEACTIVE]:settingVar.color[COLORSET_TEXT_ACTIVE],(met.state==FALSE)?settingVar.color[COLORSET_BUTTON_DEACTIVE]:settingVar.color[COLORSET_BUTTON_ACTIVE],"BPM");
            cursor(84,3); printfColor((met.state==FALSE)?settingVar.color[COLORSET_TEXT_DEACTIVE]:settingVar.color[COLORSET_TEXT_ACTIVE],(met.state==FALSE)?settingVar.color[COLORSET_BUTTON_DEACTIVE]:settingVar.color[COLORSET_BUTTON_ACTIVE],"%3.0f",(float)met.Bpm);
            temp.met=met.state;
        }
        if(temp.hints==TRUE) printHints();
    }
    clrscreen();
    playNotif(NOTIF_MINIMAZE);
    return 0;
}
#endif
