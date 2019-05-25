#ifndef INCLUDE_H_INCLUDED
#define INCLUDE_H_INCLUDED

/**
    INCLUDE_H
    Header Untuk memanggil seluruh header lain meliputi header statik dan dinamik yang dibutuhkan
*/
//Pendefinisian
#include "define.h"

//library & header dinamik
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

/**
    Library windows.h memerlukan pemanggilan opsi lainnya pada linker dengan mencantumkan opsi
        * -lwinmm
*/

#include <wininet.h>

/**
    Library wininet.h memerlukan pemanggilan opsi lainnya pada linker dengan mencantumkan opsi
        * -lwininet
*/

#include <SFML/Audio.h>

/**
    Library SFML diperoleh dari https://www.sfml-dev.org/ dengan menggunakan binding untuk bahasa C
    yaitu pada CSFML. pada Opsi Linker diperlukan pendefinisian pemanggilan library yaitu pada :
        * csfml-audio
*/

#include <conio.h>
#include <time.h>


//library & header statik
#include "zip.h"

/**
    Library zip yang terintegrasi dengan MINIZ diperoleh dari https://github.com/richgel999/miniz
*/

#include "globalvariable.h"
#include "backend.h"
#include "frontend.h"
#include "errorhandling.h"

#endif // INCLUDE_H_INCLUDED
