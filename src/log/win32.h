
#ifndef RLF_WIN32_H
#define RLF_WIN32_H

#include  <iostream>

#ifdef _WIN32

#pragma warning( disable : 4291 ) // Warning   no matching operator delete found;
#pragma warning( disable : 4800 ) //
#pragma warning( disable : 4804 ) // Warning   unsafe use of type 'bool' in operation
#pragma warning( disable:  4996 4100) // _CRT_SECURE_NO_WARNINGS

#include  <io.h>
#include  <stdio.h>
#include  <stdlib.h>

const std::locale locDE( "German_germany" );
//#include <mutex>

#endif

#ifdef __linux__
//const std::locale locDE( "de_DE.utf8" );
#endif



#endif
// EOF

