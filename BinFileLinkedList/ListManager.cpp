#include <iostream>
#include "ListManager.h"
#include "List.h"


ListManager::~ListManager()
{
   if (firstList) delete firstList;
   if (secondList) delete secondList;

   firstList= secondList= nullptr;
}

void ListManager::createBothLists()
{
   if (firstList) {
      firstList->disableAllNodes();
      firstList->purge();
   }
   else
      firstList= new List("firstList.bin");

   if (secondList) {
      secondList->disableAllNodes();
      secondList->purge();
   }
   else
      secondList= new List("secondList.bin");

   areInitialized= true;
}

void ListManager::displayLists()
{
   int pos= 1;
   
   std::cout << "In position: " << pos++ << " the list name is: " << firstList->getFileName() << std::endl;
   std::cout << "In position: " << pos << " the list name is: " << secondList->getFileName() << std::endl;
   
   return;
}

bool ListManager::concatenateLists(int pos)
{
   if ((pos != 1) && (pos != 2))
      return false;

   if (pos == 1)
      secondList->concatenateList(*firstList);
   else if (pos == 2)
      firstList->concatenateList(*secondList);

   return true;
}

void ListManager::deleteList()
{
   currentList->disableAllNodes();
   currentList->purge();

   if (currentList == firstList)
      remove("firstList.bin");
   else if (currentList == secondList)
      remove("secondList.bin");

   return;
}

void ListManager::setCurrentList(ListOption lo)
{
   if (lo == List1) {
      currentList= firstList;
      return;
   }
   // else
   currentList= secondList;
}
