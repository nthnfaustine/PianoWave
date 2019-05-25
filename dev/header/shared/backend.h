#ifndef BACKEND_H_INCLUDED
#define BACKEND_H_INCLUDED

/**
    BACKEND_H
    Header untuk proses Algoritma
*/

/*##################################################################################################################*/
//LOW LEVEL BACKEND
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

void setRes(const int pX,const int pY){
    DEVMODE devmode;
    devmode.dmPelsWidth=pX;
    devmode.dmPelsHeight=pY;
    devmode.dmFields= DM_PELSWIDTH | DM_PELSHEIGHT;
    devmode.dmSize=sizeof(DEVMODE);
    ChangeDisplaySettings(&devmode,0);
    return 0;
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

void getRegName(char *name){
    FILE *welcome;
    welcome=fopen("bin\\data\\setting\\register.nme","r");
    int loop=0;
    while(1){
        int x=fgetc(welcome);
        if(x==EOF)break;
        else{
            name[loop]=x;
            loop++;
            name[loop]='\0';
        }
    }
    fclose(welcome);
}

void writeRegName(char *name){
    FILE *welcome;
    welcome=fopen("bin\\data\\setting\\register.nme","w");
    fprintf(welcome,name);
    fclose(welcome);
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

#define NOTIF_CANCLE 0
#define NOTIF_ERROR 1
#define NOTIF_FAILED 2
#define NOTIF_INTRO 3
#define NOTIF_MAXIMAZE 4
#define NOTIF_MINIMAZE 5
#define NOTIF_OPEN 6
#define NOTIF_OUTRO 7
#define NOTIF_SAVED 8
#define NOTIF_TICK 9
#define NOTIF_UPDATE 10
#define NOTIF_ENTER 11
#define NOTIF_DESTROY_ALL 0xFFFF
void playNotif(int state){
    if(state==NOTIF_DESTROY_ALL){
        for(int a=0;a<=10;a++) sfMusic_destroy(notif.notif[a]);
        return 0;
    }
    if(notif.load==TRUE){
        FILE *fp;
        char dir2[32]={0};
        char dir3[13][32]={ "\\Cancle.ogg",   //0
                            "\\Error.ogg",    //1
                            "\\Failed.ogg",   //2
                            "\\Intro.ogg",    //3
                            "\\Maximaze.ogg", //4
                            "\\Minimaze.ogg", //5
                            "\\Open.ogg",     //6
                            "\\Outro.ogg",    //7
                            "\\Saved.ogg",    //8
                            "\\Tick.ogg",     //9
                            "\\Update.ogg",   //10
                            "\\Enter.ogg"};   //11
        notif.load=FALSE;
        fp=fopen("bin\\data\\sample\\Notification\\config.con","r");
        if(fp==NULL)error(NO_CONFIG_FILE);
        int count=0, charcount=0;
        while(1){
            int r=fgetc(fp);
            if(r==EOF) break;
            else if(r=='\n') count++;
            if(count==notif.Preset&&r!='\n'){
                dir2[charcount]=r;
                charcount++;
            }
        }
        dir2[charcount]='\0';
        for(int a=0;a<=11;a++){
            char dir1[128]="bin\\data\\sample\\Notification\\";
            strcat(&dir1,dir2);
            strcat(&dir1,dir3[a]);
            notif.notif[a]=sfMusic_createFromFile(dir1);
        }
    }
    if(notif.load==FALSE&&notif.state==TRUE){
        sfMusic_stop(notif.notif[state]);
        sfMusic_setVolume(notif.notif[state],notif.Vol);
        sfMusic_play(notif.notif[state]);
    }
    return 0;
}

/*##################################################################################################################*/
//THREAD
DWORD WINAPI PnoPlayer(void) { //PIANO PLAYER THREAD
    while(1){
        for(int a=0;a<INPUT_LEN;a++){
            if(pno.state[NOTE_ATTACK][a]==1){
                sfMusic_stop(pno.myMusic[a]);
                sfMusic_setVolume(pno.myMusic[a],pno.globalVol);
                sfMusic_play(pno.myMusic[a]);
                pno.state[NOTE_ATTACK][a]=2;
                pno.noteVol[a]=pno.globalVol;
            }
            if(pno.state[NOTE_ATTACK][a]==2&&pno.state[NOTE_ADDRESS][a]==0){
                pno.state[NOTE_ATTACK][a]=0;
            }
            if(pno.state[NOTE_ATTACK][a]==0&&pno.noteVol[a]>0){
                pno.noteVol[a]-=(pno.globalVol*((pno.sustain==TRUE)?(pno.sustainFactor/100):1));
                if(pno.noteVol[a]<0)pno.noteVol[a]=0;
                sfMusic_setVolume(pno.myMusic[a],pno.noteVol[a]);
            }
        }
        Sleep(10); //Buffer
    }
    return 0;
}

DWORD WINAPI MetronomePlayer(void){ //METRONOME PLAYER THREAD
    met.state=TRUE;
    char name[64]={0};
    char dir[128]="bin\\data\\sample\\Metronome\\";
    char tick[8]="\\0.ogg";
    FILE *fp;
    fp=fopen("bin\\data\\sample\\Metronome\\config.con","r");
    if(fp==NULL) error(NO_CONFIG_FILE);
    int count=0, a=0;
    while(1){
        int r=fgetc(fp);
        if(r==EOF)break;
        else if(r=='\n') count++;
        else if(count==met.Preset){
            name[a]=r;
            a++;
        }
    }
    strcat(&dir,name);
    strcat(&dir,tick);
    sfMusic * myTick[2];
    myTick[0]=sfMusic_createFromFile(dir);
    int len=strlen(dir);
    dir[len-5]='1';
    myTick[1]=sfMusic_createFromFile(dir);
    double msPerBeatLoop=0;
    double tempClock=clock();
    met.BeatCount=0;
    while(met.state==TRUE){
        clock_t asynTime=clock()-tempClock;
        double msPerBeat=(60/met.Bpm)*1000;
        if(asynTime>=msPerBeatLoop){
            msPerBeatLoop+=msPerBeat;
            if(met.BeatCount==0){
                sfMusic_stop(myTick[0]);
                sfMusic_setVolume(myTick[0],met.Vol);
                sfMusic_play(myTick[0]);
            }else if (met.BeatCount>0){
                sfMusic_stop(myTick[1]);
                sfMusic_setVolume(myTick[1],met.Vol);
                sfMusic_play(myTick[1]);
            }
            met.BeatCount++;
            if(met.BeatCount==met.Beat) met.BeatCount=0;
        }
    }
    met.BeatCount=0;
    sfMusic_destroy(myTick[0]);
    sfMusic_destroy(myTick[1]);
    return 0;
}

DWORD WINAPI WindowPianoPlayer(void);

/*##################################################################################################################*/
//HIGH LEVEL BACKEND
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
        settingVar.color[COLORSET_GLOBAL_BG]=WHITE;
        settingVar.color[COLORSET_GLOBAL_FG]=BLACK;
        break;
    case INIT_MAIN_CONSOLE:;
        setRes(1366,768);
        SetConsoleTitle("PianoWave Player");
        windowMode(settingVar.ScreenMode);
        windowresizelock();
        setscreenbuffersize(170,48);
        cursorvisibility(HIDE_CURSOR);
        setsystemcolor(settingVar.color[COLORSET_GLOBAL_FG],settingVar.color[COLORSET_GLOBAL_BG]);
        clrscreen();
        break;
    default:
        return 0;
        break;
    }
}

#define LOAD_SETTING 0
#define SAVE_SETTING 1
int setting(int state){
    //CUSTOM SYNTAX
    notif.load=TRUE;
    FILE * fp;
    switch(state){
    case LOAD_SETTING:;
        if(CheckFile("bin\\data\\setting\\set.con")==RETURN_FILE_EXIST) goto READ_SETTING;
        settingVar.color[COLORSET_GLOBAL_BG]=WHITE;
        settingVar.color[COLORSET_GLOBAL_FG]=BLACK;
        settingVar.color[COLORSET_BUTTON_ACTIVE]=BLACK;
        settingVar.color[COLORSET_BUTTON_DEACTIVE]=GREY;
        settingVar.color[COLORSET_TEXT_ACTIVE]=WHITE;
        settingVar.color[COLORSET_TEXT_DEACTIVE]=LGREY;
        settingVar.color[COLORSET_PNO_NATURAL_ACTIVE]=LGREY;
        settingVar.color[COLORSET_PNO_NATURAL_DEACTIVE]=GREY;
        settingVar.color[COLORSET_PNO_SHARP_ACTIVE]=GREY;
        settingVar.color[COLORSET_PNO_SHARP_DEACTIVE]=BLACK;
        settingVar.color[COLORSET_STATIC_BUTTON]=LGREY;
        settingVar.ScreenMode=FULLSCREEN;
        settingVar.colorPreset=0;
        settingVar.IntroMode=TRUE;
        settingVar.inetTimeOut=10;
        met.Preset=0;
        met.Bpm=120;
        met.Beat=4;
        met.Vol=50;
        met.state=TRUE;
        notif.Vol=50;
        notif.Preset=0;
        notif.state=TRUE;
        pno.count=0;
        pno.globalVol=50;
        pno.octv=3;
        pno.sustainFactor=3; //RANGE 1-100
        pno.sustain=TRUE;
        pno.Preset=0;
        pno.mute=FALSE;
        settingVar.clockoffset=7; //INDONESIA UTC+7
    case SAVE_SETTING:;
        fp=fopen("bin\\data\\setting\\set.con","w");
        for(int a=0;a<=10;a++) fputc(settingVar.color[a],fp);
        fputc(pno.count,fp);
        fputc(pno.globalVol,fp);
        fputc(pno.octv,fp);
        fputc(pno.sustainFactor,fp);
        fputc(pno.sustain,fp);
        fputc(pno.Preset,fp);
        fputc(pno.mute,fp);
        fputc(met.Preset,fp);
        fputc(met.Bpm,fp);
        fputc(met.Beat,fp);
        fputc(met.Vol,fp);
        fputc(met.state,fp);
        fputc(notif.Preset,fp);
        fputc(notif.state,fp);
        fputc(notif.Vol,fp);
        fputc(settingVar.clockoffset,fp);
        fputc(settingVar.ScreenMode,fp);
        fputc(settingVar.colorPreset,fp);
        fputc(settingVar.IntroMode,fp);
        fputc(settingVar.inetTimeOut,fp);
        fclose(fp);
        break;
    default: return 0;
    }
READ_SETTING:;
    fp=fopen("bin\\data\\setting\\set.con","r");
    for(int a=0;a<=10;a++) settingVar.color[a]=fgetc(fp);
    pno.count=fgetc(fp);
    pno.globalVol=fgetc(fp);
    pno.octv=fgetc(fp);
    pno.sustainFactor=fgetc(fp);
    pno.sustain=fgetc(fp);
    pno.Preset=fgetc(fp);
    pno.mute=fgetc(fp);
    met.Preset=fgetc(fp);
    met.Bpm=fgetc(fp);
    met.Beat=fgetc(fp);
    met.Vol=fgetc(fp);
    met.state=fgetc(fp);
    notif.Preset=fgetc(fp);
    notif.state=fgetc(fp);
    notif.Vol=fgetc(fp);
    settingVar.clockoffset=fgetc(fp);
    settingVar.ScreenMode=fgetc(fp);
    settingVar.colorPreset=fgetc(fp);
    settingVar.IntroMode=fgetc(fp);
    settingVar.inetTimeOut=fgetc(fp);
    pno.mute=fgetc(fp);
    fclose(fp);
    return 0;
}

#define GENERATE_OCTAVE 2
#define GENERATE_INSTRUMENT 1
#define DESTROY_INSTRUMENT 3
void GenerateInstrument(int state){
    switch(state){
    case GENERATE_INSTRUMENT:;
        FILE * fp;
        fp=fopen("bin\\data\\sample\\Instrument\\config.con","r");
        if(fp==NULL) error(NO_CONFIG_FILE);
        int count=0, readcount=0;
        while(1){
            int r=fgetc(fp);
            if(r==EOF) break;
            if(count==pno.Preset){
                if(r=='\n')break;
                if(readcount<3) pno.dir[readcount+27]=r;
                else if(readcount>=3){
                    if(r=='_') r=' ';
                    pno.namein[readcount-4]=r;
                    pno.namein[readcount-3]='\0';
                }
                readcount++;

            }
            else if(r=='\n')count++;
        }
        fclose(fp);
    case GENERATE_OCTAVE:;
        for(int a=0;a<INPUT_LEN;a++){
            if(pno.name[NOTE_NAME][a]!='0'){
                char mydir[64]={0};
                strcat(&mydir,pno.dir);
                mydir[NOTE_NAME_IN_DIR]=pno.name[NOTE_NAME][a];
                mydir[NOTE_OCTV_IN_DIR]=pno.name[NOTE_OCTV][a]+pno.octv;
                if(CheckFile(mydir)==RETURN_FILE_DOESNT_EXIXT) error(INSTRUMENT_DOESNT_EXIST);
                pno.myMusic[a]=sfMusic_createFromFile(mydir);
            }
        }
        break;
    case DESTROY_INSTRUMENT:
        for(int a=0;a<INPUT_LEN;a++) sfMusic_destroy(pno.myMusic[a]);
        break;
    }
}

int init_piano(){
    //FREE UP MEMMORY
    for(int a=0;a<(sizeof(pno.dir)/sizeof(char));a++) pno.dir[a]=0;
    for(int b=0;b<2;b++) for(int a=0;a<(sizeof(pno.name[b])/sizeof(char));a++) pno.name[b][a]=0;
    for(int a=0;a<INPUT_LEN;a++) for(int b=0;b<2;b++) pno.state[b][a]=0;
    for(int a=0;a<INPUT_LEN;a++) pno.noteVol[a]=0;
    //SETTINGUP DIRECTORY
    strcat(pno.dir,INSTRUMENT_DIR_DEFAULT);
    strcat(pno.name[NOTE_NAME],INSTRUMENT_NAME_DEFAULT);
    strcat(pno.name[NOTE_OCTV],INSTRUMENT_OCTV_DEFAULT);
    GenerateInstrument(GENERATE_INSTRUMENT);
    pno.maxPreset=countFileLines("bin\\data\\sample\\instrument\\config.con");
    //CREATING THREAD
    pno.hThread=CreateThread(NULL, 0, PnoPlayer, NULL, 0, NULL);
    if(met.state==TRUE) met.mThread=CreateThread(NULL, 0, MetronomePlayer, NULL, 0, NULL);
}

int readInputBuffer(){
    DWORD cNumRead, fdwMode, i;
    INPUT_RECORD irInBuf[MAX_NUM_READ];
    ReadConsoleInput(Handle.stdInput,irInBuf,MAX_NUM_READ,&cNumRead); //READING INPUT
    int inputstate, inputaddr;
    for(int a=0;a<cNumRead;a++){
        inputaddr=irInBuf[cNumRead-1].Event.KeyEvent.wVirtualScanCode;
        inputstate=irInBuf[cNumRead-1].Event.KeyEvent.bKeyDown,irInBuf[i];
    }
    if(inputstate==1) temp.hints=FALSE;
    pno.addr=inputaddr-17;
    if(IS_KEY_NOTE){ //IS KEY NOTE PRESSED
        pno.state[NOTE_ADDRESS][pno.addr]=inputstate;
        if(pno.state[NOTE_ATTACK][pno.addr]==0&&inputstate==1){
            pno.state[NOTE_ATTACK][pno.addr]=1;
            pno.count++;
        }
    }
    //SPECIAL INPUT
    if(inputstate==1){
        if(inputaddr==1){ //IS ESCAPE PRESSED
            for(int a=0;a<INPUT_LEN;a++) sfMusic_destroy(pno.myMusic[a]);
            TerminateThread(pno.hThread,NULL);
            if(met.state==TRUE){
                met.state=FALSE;
                WaitForSingleObject(met.mThread,1000);
                TerminateThread(met.mThread,NULL);
                met.state=TRUE;
            }
            return 0;
        }else if(inputaddr==58){ //IS SUSTAIN BUTTON PRESSED
            pno.sustain=(pno.sustain==TRUE)?FALSE:TRUE;
        }else if(inputaddr>=2&&inputaddr<=8){ //IS PIANO OCTAVE NUMBER PRESSED
            pno.octv=inputaddr-1;
            GenerateInstrument(GENERATE_OCTAVE);
        }else if(inputaddr==54&&pno.octv<7){ //IS PIANO OCTAVE INCREASE PRESSED
            pno.octv++;
            GenerateInstrument(GENERATE_OCTAVE);
        }else if(inputaddr==42&&pno.octv>1){ //IS PIANO OCTAVE DECREASE PRESSED
            pno.octv--;
            GenerateInstrument(GENERATE_OCTAVE);
        }else if(inputaddr==11){ //IS MUTE BUTTON PRESSED
            if(pno.globalVol!=0){
                pno.volTemp=pno.globalVol;
                pno.globalVol=0;
                pno.mute=TRUE;
                if(met.state==TRUE){
                    met.state=FALSE;
                    temp.metTemp=TRUE;
                }
            }else if(pno.globalVol==0){
                pno.globalVol=pno.volTemp;
                pno.mute=FALSE;
                if(temp.metTemp==TRUE){
                    temp.metTemp=FALSE;
                    met.state=TRUE;
                    met.mThread=CreateThread(NULL, 0, MetronomePlayer, NULL, 0, NULL);
                }
            }
        }else if(inputaddr==12&&pno.globalVol>0) pno.globalVol--; //IS VOLUME DOWN PRESSED
        else if(inputaddr==13&&pno.globalVol<100) pno.globalVol++; //IS VOLUME UP PRESSED
        else if(inputaddr==57){ //IS METRONOME BUTTON PRESSED
            if(met.state==FALSE) met.mThread=CreateThread(NULL, 0, MetronomePlayer, NULL, 0, NULL);
            else if(met.state==TRUE) met.state=FALSE;
        }else if(inputaddr==72&&met.Bpm<998) met.Bpm++;  //IS METRONOME INCREASE PRESSED
        else if(inputaddr==80&&met.Bpm>1) met.Bpm--; //IS METRONOME DECREASE PRESSED
        else if(inputaddr==77){ //IS PRESET INCREASE PRESSED
            pno.Preset=(pno.Preset==pno.maxPreset)? 0:pno.Preset+1;
            GenerateInstrument(GENERATE_INSTRUMENT);
        }else if(inputaddr==75){  //IS PRESET DECREASE PRESSED
            pno.Preset=(pno.Preset==0)?pno.maxPreset:pno.Preset-1;
            GenerateInstrument(GENERATE_INSTRUMENT);
        }else if(inputaddr==F1_BUTTON){
            temp.hints=TRUE;
        };
    }
    return 1;
}

void SaveColorSetting(const char *dir2,const int *color){
    FILE * fp;
    fp=fopen("bin\\data\\setting\\config.con","a");
    if(fp==NULL) error(NO_CONFIG_FILE);
    fprintf(fp,dir2);
    fputc('\n',fp);
    fclose(fp);
    char dir[128]="bin\\data\\setting\\";
    char dir3[8]=".thme";
    strcat(&dir,dir2);
    strcat(&dir,dir3);
    fp=fopen(dir,"w");
    for(int a=0;a<11;a++) fputc(color[a],fp);
    fclose(fp);
    return 0;
}
#define COLOR_DEFAULT 0
void GetColorSetting(const int index, int *color, char *name){
    FILE *fp;
    char dir[128]="bin\\data\\setting\\";
    char dir3[8]=".thme";
    fp=fopen("bin\\data\\setting\\config.con","r");
    if(fp==NULL) error(NO_CONFIG_FILE);
    int count=0; int val=0;
    while(1){
        int x=fgetc(fp);
        if(x==EOF) break;
        if(x=='\n') count++;
        else if(count==index){
            name[val]=x;
            val++;
        }
    }
    settingVar.colorPresetMax=count-1;
    fclose(fp);
    strcat(&dir,name);
    strcat(&dir,dir3);
    fp=fopen(dir,"r");
    if(fp==NULL) error(ERROR_NO_DIRECTORY);
    val=0;
    while(1){
        int x=fgetc(fp);
        if(x==EOF)break;
        else {
            color[val]=x;
            val++;
        }
    }
    fclose(fp);
    while(1)
    return 0;
}

int getConfigData(const char *dir ,const int index, char *name){
    char directory[128]="bin\\data\\sample\\";
    char dir2[64]="\\config.con";
    strcat(&directory,dir);
    strcat(&directory,dir2);
    FILE *fp;
    fp=fopen(directory,"r");
    if(fp==NULL) error(NO_CONFIG_FILE);
    int count=0,val=0;
    while(1){
        int x=fgetc(fp);
        if(x==EOF) break;
        else if(x=='\n') count++;
        if(count==index&&x!='\n'){
            name[val]=x;
            val++;
        }
    }
    name[val]='\0';
    fclose(fp);
    return count-1;
}

#define NO_INPUT_MANUAL 0xFFFF
int getManualInput(int posX,int posY, int min, int max, int digits, int firstInput){
    int val, valChar[digits+1];
    int loop=1;
    cursor(posX,posY);
    printf("%c",firstInput);
    valChar[0]=firstInput;
    cursor(posX+1,posY);
    cursorvisibility(SHOW_CURSOR);
    while(1){
        int x=_getch();
        if(x==ENTER||x==UP_ARROW||x==DOWN_ARROW){
            val=0;
            for(int a=0;a<loop;a++){
                val=(val*10)+valChar[a]-'0';
            }
            if(val>=min&&val<=max) break;
            else playNotif(NOTIF_ERROR);
        }else if(x>='0'&&x<='9'){
            if(loop>=digits) playNotif(NOTIF_ERROR);
            else{
                playNotif(NOTIF_TICK);
                valChar[loop]=x;
                printf("%c",valChar[loop]);
                loop++;
            }
        }else if(x==BACKSPACE){
             if(loop<=0) playNotif(NOTIF_ERROR);
             else{
                playNotif(NOTIF_TICK);
                valChar[loop]='\0';
                printf("\b \b");
                loop--;
             }
        }else if(x==ESCAPE) {
            cursor(posX,posY);
            for(int a=0;a<digits;a++) printf(" ");
            playNotif(NOTIF_CANCLE);
            cursorvisibility(HIDE_CURSOR);
            return NO_INPUT_MANUAL;
        }else if(x!=ENTER&&x!=UP_ARROW&&x!=DOWN_ARROW&&x!=BACKSPACE&&x!=ESCAPE&&(x>='0'&&x<='9')) playNotif(NOTIF_FAILED);
    }
    playNotif(NOTIF_SAVED);
    cursorvisibility(HIDE_CURSOR);
    return val;
}

void ThreadWindowPianoPlayer(){
    pno.enable=TRUE;
    pno.fThread=CreateThread(NULL, 0, WindowPianoPlayer, NULL, 0, NULL);
    init_piano();
    while(readInputBuffer()!=0);
    pno.enable=FALSE;
    WaitForSingleObject(pno.fThread,5000);
    setting(SAVE_SETTING);
    return 0;
}

#endif // BACKEND_H_INCLUDED
