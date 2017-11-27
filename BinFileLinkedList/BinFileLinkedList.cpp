#include <iostream>
#include <sstream>
#include "List.h"

int main()
{
   List* list= new List("fileName.bin");

   list->appendNode(1);
   list->appendNode(2);
   list->appendNode(3);

   cout << list->displayAsc();

   return 0;
}

