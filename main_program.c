#include "dev/header/main_prototyping.h"

/*

PianoWave
Copyright (c) 2019
by Alfian Badrul Isnan
and Nathaniel Faustine

*/

/**
    MAIN_PROGRAM_C
    File program C yang merupakan sintaks main utama pada program utama (untuk bin/pw.exe)
*/

int main()
{
    setting(LOAD_SETTING);
    init_console(INIT_MAIN_CONSOLE);
    if(settingVar.IntroMode==TRUE) WindowWelcome();
    printChangelog(INTRO_CHANGELOG,0);
    while(1){
        int x=WindowMainMenu();
        switch(x){
        case WINDOW_EXIT_RETURN : if(WindowExitPrompt()==TRUE) goto TERMINATE;
            break;
        case WINDOW_ABOUT_RETURN :
            while(1){
                int y=WindowAbout();
                if(y==2) break;
                else if(y==0) {
                    int index=0;
                    while(1){
                        index=printChangelog(SHOW_CHANGELOG,index);
                        if(index==RETURN_CHANGELOG_EXIT) break;
                    }
                    playNotif(NOTIF_ENTER);
                }
                else if(y==1) WindowUpdate();
            }
            break;
        case WINDOW_SETTING_RETURN :
            WindowSetting();
            break;
        case WINDOW_PIANOPLAYER_RETURN :
            ThreadWindowPianoPlayer();
            break;
        case WINDOW_DLC_RETURN :
            WindowDLC();
            break;
        }
    }
TERMINATE:
    if(settingVar.IntroMode==TRUE) WindowGoodbye();
    playNotif(NOTIF_DESTROY_ALL);
    return 0;
}
