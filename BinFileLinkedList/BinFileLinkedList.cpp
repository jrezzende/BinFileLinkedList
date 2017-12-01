#include <iostream>
#include <sstream>
#include "List.h"

int main()
{
   List* list= new List("fileName.bin");

   list->appendNode(1);
   list->appendNode(2);
   list->appendNode(3);
   list->appendNode(4);
   list->appendNode(5);

   cout << list->getListSize() << endl;
   cout << list->displayAsc();
   

   list->removeAllNodes();
   cout << list->purge() << endl;
   cout << list->getListSize() << endl;
   cout << list->displayAsc();

   return 0;
}

