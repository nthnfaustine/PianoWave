#include "dev/header/launcher_prototyping.h"

/**
    MAIN_LAUNCHER_C
    File program C yang merupakan sintaks main utama pada program launcher (untuk PianoWave.exe)
*/
int main()
{
    init_console(INIT_LAUNCHER_CONSOLE);
    setscreenbuffersize(96,16);
    if(CheckFile("bin\\pw_main.exe")==RETURN_FILE_EXIST) goto UPDATE;
    clrscreen();
    readme();
    char cwd[128];
    getcwd(cwd, sizeof(cwd));
    printfPosition(CENTER_OF_SCREEN,3,"Install Directory :");
    printfPosition(CENTER_OF_SCREEN,4,cwd);
    if(_getch()==27)exit(0);
    boxFill(0,2,MAXIMUM_X_SCREEN_BUFFER,12,ERASE_BY_BACKGROUND_COLOR);
    boolean offinst=FALSE;
    while(1){
        int x=metadataDownloader();
        if(x==ERROR_INET_CONNECTION_TIMEOUT||x==ERROR_INET_NO_INTERNET){
            boxFill(0,2,MAXIMUM_X_SCREEN_BUFFER,11,ERASE_BY_BACKGROUND_COLOR);
            printfPosition(CENTER_OF_SCREEN,7,"ERROR CANNOT CONNECT TO THE INTERNET");
            printfPosition(CENTER_OF_SCREEN,13,"Escape To Abort | Space/Enter to do Offline Installation | Any Key to Retry");
            int y=_getch();
            if(y==ENTER||y==SPACE){
                offinst=TRUE;
                break;
            }else if(y==ESCAPE){
                exit(0);
            }
            boxFill(0,2,MAXIMUM_X_SCREEN_BUFFER,14,ERASE_BY_BACKGROUND_COLOR);
        }else if(x==DOWNLOAD_METADATA_SUCCESS) break;
    }
    if(offinst==TRUE){
        char dir[128]="PW_InstallerPackage.zip";
        while(1){
            boxFill(0,2,MAXIMUM_X_SCREEN_BUFFER,14,ERASE_BY_BACKGROUND_COLOR);
            if(CheckFile(dir)==RETURN_FILE_EXIST) break;
            printfPosition(2,3,"\"%s\" file does not exist!. Please input directory manually!",dir);
            printfPosition(2,5,"DIR : ");
            cursorvisibility(SHOW_CURSOR);
            int loop=0;
            while(1){
                int r=_getch();
                if(r==ENTER)break;
                else if(r==BACKSPACE&&loop>0){
                    printf("\b \b");
                    dir[loop]='\0';
                    loop--;
                }else if(r==ESCAPE) exit(0);
                else if(r!=BACKSPACE){
                    printf("%c",r);
                    dir[loop]=r;
                    loop++;
                    dir[loop]='\0';
                }
            }
            cursorvisibility(HIDE_CURSOR);
        }
        int arg=0;
        cursor(0,3);
        zip_extract(dir,".",on_extract_entry,&arg);
        boxFill(0,2,MAXIMUM_X_SCREEN_BUFFER,14,ERASE_BY_BACKGROUND_COLOR);
    }
    getSetting();
UPDATE:;
    init_console(INIT_LAUNCHER_CONSOLE);
    setscreenbuffersize(96,30);
    clrscreen();
    printHeader(24);
    printPianoIcon(25,0);
    const int numRead = 99;
    char file[numRead];
    if(readAutoState()==FALSE) goto LAUNCH_APP;
    readversion();
    FILE *fp;
    HINTERNET hOpen, hURL;
    unsigned long readl;
    printfPosition(CENTER_OF_SCREEN,27,"Check For Updates Current Version %s",version);
    hOpen = InternetOpen("Default_User_Agent", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0 );
    if(!hOpen)goto LAUNCH_APP;
    DWORD rec_timeout = 200 * 1000;					// override the 20 second timeout
    InternetSetOption(hOpen, INTERNET_OPTION_RECEIVE_TIMEOUT, &rec_timeout, sizeof(rec_timeout));
    hURL = InternetOpenUrl( hOpen,"https://drive.google.com/uc?export=download&id=1jExbguOGUg-TtFzHpsD3ZNuusvmeQJaK",NULL, 0, INTERNET_FLAG_KEEP_CONNECTION, 0 );
    if(!hURL)goto LAUNCH_APP;
    fp = fopen("bin\\temp.txt", "wb");
    DWORD dwRead;
    while ( InternetReadFile(hURL, file, numRead - 1 , &readl) && readl != 0 )
    {
        fwrite(file, sizeof(char), readl, fp);
        file[readl] = '\0';
    }
    free(file);
    fclose(fp);
    InternetCloseHandle(hOpen);
    InternetCloseHandle(hURL);
    fp=fopen("bin\\temp.txt","r");
    int count=0;boolean read=FALSE, verread=FALSE;
    if(!fp)goto LAUNCH_APP;
    while(1){
        int x=fgetc(fp);
        if(x==EOF)goto LAUNCH_APP;
        if(read==TRUE){
            if(version[count]!=x)verread=TRUE;
            else if(x=='\n')goto LAUNCH_APP;
            version[count]=x;
            count++;
            if(count==4){
                if(verread==TRUE) break;
                else if(verread==FALSE) goto LAUNCH_APP;
            }
        }
        if(x==':')read=TRUE;
    }
    fclose(fp);
    printfPosition(CENTER_OF_SCREEN,27,"Downloading new updates : PianoWave %s",version);
    downloadFile("https://drive.google.com/uc?export=download&id=1SbgXmen2Ap7slnj87HDF8UeyU8Ngbno2","temp.zip",0,FALSE);
    clsline(0,27);
    printfPosition(CENTER_OF_SCREEN,27,"Installing : PianoWave %s",version);
    zip_extract("temp.zip",".",0,0);
    createSetting("bin\\data\\setting\\set.ver",version);
LAUNCH_APP:;
    remove("bin\\temp.zip");
    remove("temp.txt");
    remove("bin\\metadata.txt");
    remove("temp.zip");
    clsline(0,27);
    printfPosition(CENTER_OF_SCREEN,27,"Launching Apps");
    Sleep(1000);
    system("bin\\pw_main.exe");
    exit(0);
    return 0;
}
