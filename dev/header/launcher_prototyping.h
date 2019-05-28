#ifndef LAUNCHER_PROTOTYPING_H_INCLUDED
#define LAUNCHER_PROTOTYPING_H_INCLUDED

/**
    MAIN_PROTOTYPING_H
    Header Utama untuk menjelaskan Prototype dari setiap fungsi yang digunakan dalam program main_launcher.c pada program launcher
*/

/*
    #define INIT_LAUNCHER_CONSOLE 1
*/

#include "shared/define.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <wininet.h>
#include <conio.h>
#include <time.h>
#include "shared/zip.h"

typedef struct {
    HANDLE stdInput;
    HANDLE stdOutput;
    HANDLE stdError;
}ALL_HANDLE;
ALL_HANDLE Handle;

char version[16];

void get_handle(){
    Handle.stdInput=GetStdHandle(STD_INPUT_HANDLE);
    Handle.stdOutput=GetStdHandle(STD_OUTPUT_HANDLE);
    Handle.stdError=GetStdHandle(STD_ERROR_HANDLE);
}

#define HIDE_CURSOR 1
#define SHOW_CURSOR 2
void cursorvisibility(int state){
    CONSOLE_CURSOR_INFO info;
    info.dwSize=100;
    info.bVisible=(state==HIDE_CURSOR)?FALSE:TRUE;
    SetConsoleCursorInfo(Handle.stdOutput,&info);
}

void setsystemcolor(int fg, int bg){
    char mycolor[9]="color ab";
    char chfg, chbg;
    if(fg>=0&&fg<10)chfg=fg+'0';
    else if(fg>=10&&fg<16)chfg=(fg-10)+'A';
    if(bg>=0&&bg<10)chbg=bg+'0';
    else if(bg>=10&&bg<16)chbg=(bg-10)+'A';
    mycolor[6]=chbg;
    mycolor[7]=chfg;
    system(mycolor);
}

void windowresizelock(){
    HWND consoleWindow = GetConsoleWindow();
    SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
}

#define FULLSCREEN 1
#define WINDOWED 2
void windowMode(int state){
    SetConsoleDisplayMode(Handle.stdOutput,(state==FULLSCREEN)?TRUE:FALSE,0);
}

void clrscreen(){system("cls");}

void setscreenbuffersize(int x,int y){
    char mode[128]="mode con cols=";//100 lines=32");
    char modelines[8]=" lines=";
    char numx[4],numy[4];
    sprintf(numx, "%d", x);
    sprintf(numy, "%d", y);
    strcat(&mode,numx);
    strcat(&mode,modelines);
    strcat(&mode,numy);
    system(mode);
}

void ScreenRes(int x,int y) {
    HWND console = GetConsoleWindow();
    RECT ConsoleRect;
    GetWindowRect(console, &ConsoleRect);
    MoveWindow(console, ConsoleRect.left, ConsoleRect.top, x, y, TRUE);
}

#define CENTER_OF_SCREEN 0xFFFF
#define CURSOR_FROM_CENTER 0xFFF0
#define CONTINUE_Y 0xFFF1
#define CONTINUE_X 0xFFF2
void cursor(int x,int y){
    CONSOLE_SCREEN_BUFFER_INFO psBI;
    GetConsoleScreenBufferInfo(Handle.stdOutput,&psBI);
    if(x==CENTER_OF_SCREEN) x=(psBI.dwMaximumWindowSize.X)/2;
    if(y==CENTER_OF_SCREEN) y=(psBI.dwMaximumWindowSize.Y)/2;
    if(x>=CURSOR_FROM_CENTER-100&&x<=CURSOR_FROM_CENTER+100) x=((psBI.dwMaximumWindowSize.X)/2)+(x-CURSOR_FROM_CENTER);
    if(y>=CURSOR_FROM_CENTER-100&&y<=CURSOR_FROM_CENTER+100) y=((psBI.dwMaximumWindowSize.X)/2)+(y-CURSOR_FROM_CENTER);
    if(x==CONTINUE_X) x=psBI.dwCursorPosition.X;
    if(y==CONTINUE_Y) y=psBI.dwCursorPosition.Y;
    COORD pos;
    pos.X=x; pos.Y=y;
    SetConsoleCursorPosition(Handle.stdOutput,pos);
}

#define RETURN_FILE_EXIST 1
#define RETURN_FILE_DOESNT_EXIXT -1
int CheckFile(const char *dir){
    int foreturn;
    FILE *fp;
    fp=fopen(dir,"r");
    if(fp)foreturn=1;
    else foreturn=-1;
    fclose(fp);
    return foreturn;
}

void createSetting(const char *dir,const char *val){
    FILE *fp;
    fp=fopen(dir,"w");
    fprintf(fp,"%s",val);
    fclose(fp);
    return 0;
}

int countFileLines(const char *dir){
    FILE *fp;
    fp=fopen(dir,"r");
    int count=0;
    if(fp==NULL) error(ERROR_NO_DIRECTORY);
    while(1){
        int r=fgetc(fp);
        if(r==EOF) break;
        else if(r=='\n') count++;
    }
    fclose(fp);
    return count;
}

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
        if(x=='#')printfColor(0,BLACK," ");
        else if(x=='\n'){
            posY++;
            cursor(posX,posY);
        }
        else printfColor(0,WHITE," ");
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

#define INIT_MAIN_CONSOLE 2
#define INIT_LAUNCHER_CONSOLE 1
void init_console(int state){
    get_handle();
    switch(state){
    case INIT_LAUNCHER_CONSOLE:;
        SetConsoleTitle("PianoWave Launcher");
        windowMode(WINDOWED);
        windowresizelock();
        setsystemcolor(BLACK,WHITE);
        cursorvisibility(HIDE_CURSOR);
        break;
    default:
        return 0;
        break;
    }
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

void readme(){
    printfPosition(CENTER_OF_SCREEN,1,"PianoWave Installer");
    printfWrap(0,3,2,2,"Thank You for downloading PianoWave Software by Alfian Badrul Isnan and Nathaniel Faustine. This software built for fulfill the Pengantar Teknik Komputer Subject Task at Undergraduate Program of Computer Engineering, Fakultas Teknik, Universitas Indonesia. This software is open-source based software, with some library from Simple and Fast Multimedia Library (SFML) with C Binding by Laurent Gomilla, Miniz Library by Rich Geldreich, and also based on Windows Console API.");
    printfWrap(0,CONTINUE_POS_Y+1,2,2,"Before Install, make sure you are connected to the Internet. This installer will download some files needed to install the PianoWave and some additional instrument files");
    printfPosition(CENTER_OF_SCREEN,13,"Press ANY KEY to Continue, ESCAPE to Abort...");
    boxLine(0,2,95,6);
    if(_getch()==27)exit(0);
    boxFill(0,2,MAXIMUM_X_SCREEN_BUFFER,11,ERASE_BY_BACKGROUND_COLOR);
    return 0;
}

//BELUM KELAR ERROR HANDLING NYA
#define ERROR_INET_NO_INTERNET 0xFFAA
#define ERROR_INET_CONNECTION_TIMEOUT 0xFFAB
int downloadFile(const char *url,const char *dir_filename,int sizeBita,boolean state_of_disp){
    FILE *fp;
    HINTERNET hOpen, hURL;
    const int numRead = 99;
    char file[numRead];
    unsigned long read;
    hOpen = InternetOpen("Default_User_Agent", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0 );
    if(!hOpen) return ERROR_INET_NO_INTERNET;
    DWORD rec_timeout = 200 * 1000;					// override the 10 second timeout
    InternetSetOption(hOpen, INTERNET_OPTION_RECEIVE_TIMEOUT, &rec_timeout, sizeof(rec_timeout));
    hURL = InternetOpenUrl( hOpen,url,NULL, 0, INTERNET_FLAG_KEEP_CONNECTION, 0 );
    if(!hURL) return ERROR_INET_CONNECTION_TIMEOUT;
    fp = fopen(dir_filename, "wb");
    DWORD dwRead;
    float filesize=0,speed=0,fmax;
    char *n;
    char *n2;
    float fmaxsize=sizeBita/100;
    if(fmaxsize>=10000){
        fmax=fmaxsize/10000;
        n2="MB";
    }else if(fmaxsize<10000){
        n2="KB";
    fmax=fmaxsize/10;
    }
    //printf("\nSIZE (byte) : %d | Converted : %0.2f %s",sizeBita,fmax,n2);
    CONSOLE_SCREEN_BUFFER_INFO sBi;
    //printf("\nSTART DOWNLOADING\n");
    GetConsoleScreenBufferInfo(Handle.stdOutput,&sBi);
    if(state_of_disp==TRUE) downloadBar(INIT_DBAR,sBi.dwCursorPosition,0,0);
    clock_t myTime,iniTime;
    myTime=clock();
    iniTime=clock();
    float tempTime=((float)myTime/CLOCKS_PER_SEC)+0.5;
    while ( InternetReadFile(hURL, file, numRead - 1 , &read) && read != 0 )
    {
        fwrite(file, sizeof(char), read, fp);
        file[read] = '\0';
        myTime=clock();filesize++;speed++;
        float fsize;
        if(filesize>=10000){
            fsize=filesize/10000;
            n="MB";
        }else if(filesize<10000){
            n="KB";
        fsize=filesize/10;
        }
        COORD pos;
        pos.Y=sBi.dwCursorPosition.Y+3;
        if(filesize<=fmaxsize) {

            pos.X=sBi.dwCursorPosition.X+1;
            SetConsoleCursorPosition(Handle.stdOutput,pos);
            if(state_of_disp==TRUE) printf("%0.2f%s of %0.2f%s  ",fsize,n,fmax,n2);
        }
        float second=(float)myTime/CLOCKS_PER_SEC;
        if(second>=tempTime){
            if(speed>=10000){
                speed=speed/10000;
                n="MBps";
            }else if(speed<10000){
                n="KBps";
            speed=speed/10;
            }
            pos.X=sBi.dwCursorPosition.X+23;
            SetConsoleCursorPosition(Handle.stdOutput,pos);
            if(state_of_disp==TRUE) printf("%0.2f%s  ",speed,n);
            tempTime=second+1;
            speed=0;
            if(state_of_disp==TRUE) downloadBar(LOAD_DBAR,sBi.dwCursorPosition,filesize,fmaxsize);
        }
    }
    myTime=clock()-iniTime;
    fclose(fp);
    free(file);
    InternetCloseHandle(hOpen);
    InternetCloseHandle(hURL);
    //printf("\nFINISH | Time Elapsed : %0.2f Sec\n",((float)(myTime) / CLOCKS_PER_SEC));
}

#define DOWNLOAD_METADATA_SUCCESS 0xFFFF
int metadataDownloader(){
    printfPosition(2,3,"Downloading metadata");
    printfDots(2,4,MAXIMUM_X_SCREEN_BUFFER,"URL: https://drive.google.com/uc?export=download&id=1D1tuyXfvEjuzMAInXSg68Zs6YKrQYSFa");
    cursor(CURSOR_FROM_CENTER-21,10);
    int x=downloadFile("https://drive.google.com/uc?export=download&id=1D1tuyXfvEjuzMAInXSg68Zs6YKrQYSFa","metadata.txt",10,TRUE);
    if(x==ERROR_INET_CONNECTION_TIMEOUT||x==ERROR_INET_NO_INTERNET) return x;
    FILE *fp;
    fp=fopen("metadata.txt","r");
    int mycount=0;
    int myfilesize=0;
    while(1){
        int x=fgetc(fp);
        if(x==EOF)exit(1);
        if(mycount==2){
            if(x=='~')break;
            myfilesize=(x-'0')+(myfilesize*10);
        }
        if(x==':')mycount++;
    }
    fclose(fp);
    if(CheckFile("bin\\pw.exe")==RETURN_FILE_EXIST) goto SKIPDOWNFILES;
    printfPosition(2,3,"Downloading Files Installer.zip");
    printfDots(2,4,MAXIMUM_X_SCREEN_BUFFER,"URL: https://drive.google.com/uc?export=download&id=1LMxgjqc-Q3PQillCUSgRmJSalsIGt1hR");
    cursor(CURSOR_FROM_CENTER-21,10);
    downloadFile("https://drive.google.com/uc?export=download&id=1LMxgjqc-Q3PQillCUSgRmJSalsIGt1hR","temp.zip",myfilesize,TRUE);
    boxFill(1,2,MAXIMUM_X_SCREEN_BUFFER,3,ERASE_BY_BACKGROUND_COLOR);
    printfPosition(2,3,"Unpacking Installer.zip\n");
    int arg;
    zip_extract("temp.zip",".",on_extract_entry,&arg);
    remove("temp.zip");
    boxFill(0,2,MAXIMUM_X_SCREEN_BUFFER,MAXIMUM_Y_SCREEN_BUFFER,ERASE_BY_BACKGROUND_COLOR);
SKIPDOWNFILES:;
    return DOWNLOAD_METADATA_SUCCESS;
}

void readversion(){
    FILE *fp;
    fp=fopen("bin\\data\\setting\\set.ver","r");
    free(version);
    fscanf(fp,"%s",&version);
    fclose(fp);
    return 0;
}

boolean readAutoState(){
    FILE *fp;
    fp=fopen("bin\\data\\setting\\autoUp.set","r");
    char num;
    fscanf(fp,"%c",&num);
    fclose(fp);
    if(num=='1')return TRUE;
    else{
        if(CheckFile("upcheck.uc")==RETURN_FILE_EXIST) {
                remove("upcheck.uc");
                return TRUE;
        }
        else return FALSE;
    }
}

void getSetting(){
    printfPosition(2,3,"Please Register Your Name : ");
    char name[64];
    cursorvisibility(SHOW_CURSOR);
    scanf("%[^\n]s", &name);
    cursorvisibility(HIDE_CURSOR);
    boxFill(0,2,MAXIMUM_X_SCREEN_BUFFER,MAXIMUM_Y_SCREEN_BUFFER,ERASE_BY_BACKGROUND_COLOR);
    printfPosition(2,3,"Do you want to set AutoUpdate? :  ");
    boolean autoupdate=TRUE;
    while(1){
        cursor(35,3);
        if(autoupdate==TRUE){
            printfColor(WHITE,BLACK," YES ");
            printf(" ");
            printfColor(BLACK,WHITE," N O ");
        }else if(autoupdate==FALSE){
            printfColor(BLACK,WHITE," YES ");
            printf(" ");
            printfColor(WHITE,BLACK," N O ");
        }
        int x=_getch();
        if(x==ENTER)break;
        else if(x==LEFT_ARROW||x==RIGHT_ARROW) autoupdate=(autoupdate==TRUE)?FALSE:TRUE;
    }
    char autoUp[2];
    if(autoupdate==TRUE)autoUp[0]='1';
    else if(autoupdate==FALSE)autoUp[0]='0';
    boxFill(0,2,MAXIMUM_X_SCREEN_BUFFER,MAXIMUM_Y_SCREEN_BUFFER,ERASE_BY_BACKGROUND_COLOR);
    printfPosition(2,3,"Do you want to make desktop icon? :  ");
    boolean desktopicon=TRUE;
    while(1){
        cursor(36,3);
        if(desktopicon==TRUE){
            printfColor(WHITE,BLACK," YES ");
            printf(" ");
            printfColor(BLACK,WHITE," N O ");
        }else if(desktopicon==FALSE){
            printfColor(BLACK,WHITE," YES ");
            printf(" ");
            printfColor(WHITE,BLACK," N O ");
        }
        int x=_getch();
        if(x==ENTER){
            if(desktopicon==TRUE) setShortcut();
            break;
        }
        else if(x==LEFT_ARROW||x==RIGHT_ARROW) desktopicon=(desktopicon==TRUE)?FALSE:TRUE;
    }
    createSetting("bin\\data\\setting\\register.nme",name);
    createSetting("bin\\data\\setting\\autoUp.set",autoUp);
    createSetting("bin\\data\\setting\\set.ver",version);
    return 0;
}

void printHeader(int y){
    CONSOLE_SCREEN_BUFFER_INFO csbI;
    GetConsoleScreenBufferInfo(Handle.stdOutput,&csbI);
    for(int a=0;a<csbI.dwMaximumWindowSize.X;a++) printfPosition(a,y,"%c",205);
    printfPosition(CENTER_OF_SCREEN,y,"   P   I   A   N   O         W   A   V   E   ");
    printfPosition(CENTER_OF_SCREEN,y+1,"All Right Reserved | Copyright(C) AN 2019");
}

int setShortcut() {
    printf("\n\n");
    char cwd[128];
    getcwd(cwd, sizeof(cwd));
    char echo1[128]="echo oLink.TargetPath = \"";
    char echo2[128]="\\PianoWave.exe\" >> CreateShortcut.vbs";
    char echo3[128]="echo oLink.WorkingDirectory = \"";
    char echo4[128]="\" >> CreateShortcut.vbs";
    strcat(&echo1,cwd);
    strcat(&echo1,echo2);
    strcat(&echo3,cwd);
    strcat(&echo3,echo4);
    system("@echo off");
    system("echo Set oWS = WScript.CreateObject(\"WScript.Shell\") > CreateShortcut.vbs");
    system("echo sLinkFile = \"%userprofile%\\Desktop\\PianoWave.lnk\" >> CreateShortcut.vbs");
    system("echo Set oLink = oWS.CreateShortcut(sLinkFile) >> CreateShortcut.vbs");
    system(echo1);
    system(echo3);
    system("echo oLink.Save >> CreateShortcut.vbs");
    system("cscript CreateShortcut.vbs");
    system("del CreateShortcut.vbs");
    return 0;
}

#include "shared/errorhandling.h"

#endif // LAUNCHER_PROTOTYPING_H_INCLUDED
