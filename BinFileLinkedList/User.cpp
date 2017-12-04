#include <iostream>
#include "User.h"
#include "List.h"
#include "ListManager.h"


int User::uCreateBothLists(ListManager& lm)
{
   system("cls");

   lm.createBothLists();

   return 1;

   system("cls");
}

void User::uDisplayLists(ListManager& lm)
{
   system("cls");

   if (!lm.areCreated()) {
      std::cout << "No list available.\n";
      return;
   }

   lm.displayLists();
}

int User::uDeleteLists(ListManager& lm)
{
   system("cls");

   if (!lm.areCreated()) 
      return 0;

   lm.getFirstList().disableAllNodes();
   lm.getFirstList().purge();

   lm.getSecondList().disableAllNodes();
   lm.getSecondList().purge();

   return 1;
}

bool User::uConcatenateLists(ListManager& lm)
{
   system("cls");

   if (!lm.areCreated())
      return false;

   int pos; 

   std::cout << "Choose the list you want to attach at the end.\n";
   std::cin >> pos;

   return lm.concatenateLists(pos);
}

int User::uSetCurrentList(ListManager& lm)
{
   lm.displayLists();

   int num;

   std::cout << "///////////////////////////\n";
   std::cout << "Choose a list position to work on.\n";
   std::cin >> num;
   system("cls");

   if (num != 1 && num != 2)
      return 0;

   lm.setCurrentList((num == 1) ? ListManager::List1 : ListManager::List2);
   return 1;
}

void User::uPrependNode(ListManager& lm)
{
   int value;

   system("cls");
   std::cout << "Set an initial node data value: \n";
   std::cin >> value;

   lm.getCurrentList().prependNode(value);

   system("cls");
   std::cout << "Successful operation.\n";
}

void User::uAppendNode(ListManager& lm)
{
   int value;

   system("cls");
   std::cout << "Set an initial node data value: \n";
   std::cin >> value;

   lm.getCurrentList().appendNode(value);

   system("cls");
   
   std::cout << "Succesful operation.\n";
}

void User::uAddInPosition(ListManager& lm)
{
   int value;
   int pos;

   system("cls");

   lm.getCurrentList().displayAsc();
   std::cout << "///////////////////////////\n";
   
   std::cout << "Set an initial node data value.\n";
   std::cin >> value;

   std::cout << "Set the position you want to add the node.\n";
   std::cin >> pos;

   lm.getCurrentList().addInPos(value, pos);
   std::cout << "Succesful operation.\n";
}

void User::uDisplayAsc(ListManager& lm)
{
   system("cls");

   lm.getCurrentList().displayAsc();

   std::cout << std::endl;
}

void User::uDisplayDesc(ListManager& lm)
{
   system("cls");

   lm.getCurrentList().displayDesc();

   std::cout << std::endl;
}

void User::uSortList(ListManager& lm)
{
   system("cls");

   if (!lm.getCurrentList().getListSize())
      return;

   lm.getCurrentList().sortList();
   
   return;
}

void User::uDisableInPos(ListManager& lm)
{
   system("cls");

   if (!lm.getCurrentList().getListSize())
      return;
   
   int pos;

   lm.getCurrentList().displayAsc();
   std::cout << "Choose a position to disable a node from the list.\n";
   std::cin >> pos;

   lm.getCurrentList().disableNode(pos);
}

void User::uDisableAllNodes(ListManager& lm)
{
   system("cls");

   if (!lm.getCurrentList().getListSize())
      return;

   lm.getCurrentList().disableAllNodes();
   
   return;
}


void User::uPurge(ListManager& lm)
{
   system("cls");

   lm.getCurrentList().purge();

   return;
}
