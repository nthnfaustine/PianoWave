#ifndef ERRORHANDLING_H_INCLUDED
#define ERRORHANDLING_H_INCLUDED

/**
    ERRORHANDLING_H
    Header untuk memberikan notifikasi dan log error baik kepada pengguna ataupun developer
*/

int error(int state){
    switch(state){
    case INSTRUMENT_DOESNT_EXIST:
        printf("INSTRUMENT DOESNT EXIST, CLICK ANY KEY TO GO TO MAIN MENU");
        _getch();
        //MENU!!!
        break;
    case NO_CONFIG_FILE:
        printf("ERROR CONFIG FILE NOT FOUND, CLICK ANY KEY TO GO TO MAIN MENU");
        _getch();
        //MENU!!!
        break;
    case ERROR_NO_DIRECTORY:;
        printfPosition(CENTER_OF_SCREEN,1,"ERROR NO DIRECTORY, CLICK ANY KEY TO TERMINATE THE PROGRAM");
        _getch();
        exit(0x0003);
    }
}

#endif // ERRORHANDLING_H_INCLUDED
