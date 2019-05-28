#ifndef GLOBALVARIABLE_H_INCLUDED
#define GLOBALVARIABLE_H_INCLUDED

/**
    GLOBAL_VARIABLE_H
    Header Untuk menyimpan Variable global yang digunakan disetiap header
*/

char version    [8] = "3.40";
char build      [8] = "Beta";

typedef struct {
    HANDLE stdInput;
    HANDLE stdOutput;
    HANDLE stdError;
}ALL_HANDLE;
ALL_HANDLE Handle;

typedef struct{
    int color[128];
    int button[128];
    int menuTemp;
    int clockoffset;
    int colorPreset;
    int colorPresetMax;
    int inetTimeOut;
    boolean IntroMode;
    boolean ScreenMode;
}SETTING;
SETTING settingVar;

typedef struct{
    int inputaddr;
    int inputstate[INPUT_LEN];
    int last_octv;
    int index;
    clock_t time;
    clock_t timeInit;
    boolean openState;
    boolean recState;
    boolean fin;
    boolean record;
    boolean read;
    boolean play;
    HANDLE Thread;
}RECORDER;
RECORDER rec;

typedef struct{
    int state[2][INPUT_LEN];
    int count;
    int addr;
    int octv;
    int Preset;
    int maxPreset;
    float noteVol[INPUT_LEN];
    float globalVol;
    float sustainFactor;
    float volTemp;
    boolean sustain;
    boolean enable;
    boolean tempAct[INPUT_LEN];
    boolean mute;
    char dir[64];
    char name[2][INPUT_LEN+2];
    char namein[128];
    char lay[INPUT_LEN][1024];
    sfMusic * myMusic[INPUT_LEN];
    HANDLE hThread;
    HANDLE fThread;
}PIANO;
PIANO pno;

typedef struct{
    int Preset;
    int maxPreset;
    int Beat;
    int BeatCount;
    boolean state;
    float Vol;
    double Bpm;
    HANDLE mThread;
}METRONOME;
METRONOME met;

typedef struct{
    int Preset;
    int maxPreset;
    float Vol;
    boolean state;
    boolean load;
    sfMusic * notif[12];
}NOTIFICATION;
NOTIFICATION notif;

typedef struct{
    int Octv;
    int Preset;
    float Voltemp;
    boolean sust;
    boolean met;
    boolean mute;
    boolean metTemp;
    boolean hints;
    double Bpm;
}TEMP;
TEMP temp;

char link_at_thread [256];
char dir_at_thread  [128];
boolean download_state;
float filesize_at_thread;
char notes_pos [INPUT_LEN]="we0tyu0op0000asdfghjkl;/";

#endif // GLOBALVARIABLE_H_INCLUDED
