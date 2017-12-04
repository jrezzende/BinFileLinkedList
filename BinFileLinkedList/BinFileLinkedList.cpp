#include <crtdbg.h>
#include <stdio.h>
#include "Menu.h"

void main()
{
   _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
   _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
   _CrtMemState s1;

   Menu::menu();

   _CrtMemCheckpoint(&s1);
   _CrtMemDumpStatistics(&s1);
   _CrtDumpMemoryLeaks();

}