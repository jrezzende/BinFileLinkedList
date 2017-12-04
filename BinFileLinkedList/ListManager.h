#pragma once
#ifndef INCLUDED_LISTMANAGER_H
#define INCLUDED_LISTMANAGER_H

#include <string>

class List;

class ListManager
{
private:
   List* firstList;
   List* secondList;
   List *currentList;
   bool areInitialized= false;

public:
   enum ListOption { List1, List2 };

   ~ListManager();
   ListManager() { firstList= nullptr; secondList= nullptr; currentList= firstList; }

   bool areCreated() { return areInitialized; }
   void createBothLists();
   void displayLists();
   bool concatenateLists(int pos);
   void deleteList();

   void setCurrentList(ListOption lo);

   List& getFirstList() { return *firstList; };
   List& getSecondList() { return *secondList; };
   List& getCurrentList() {return *currentList;};

};

#endif // INCLUDED_LISTMANAGER_H
