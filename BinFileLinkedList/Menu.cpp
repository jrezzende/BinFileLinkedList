#include <iostream>
#include <iomanip>
#include <string>
#include "Menu.h"
#include "ListManager.h"
#include "List.h"
#include "User.h"

static void displayFirstMenu(ListManager&, User&);
static void displaySecondMenu(ListManager&, User&);

static int getFirstOption();
static int getSecondOption();

using namespace std;

void Menu::menu()
{
   User user;
   ListManager lm;

   displayFirstMenu(lm, user);
}

static void displayFirstMenu(ListManager& lm, User& user) 
{
   system("cls");

   int op;

   do 
   {
      op= getFirstOption();
      switch (op)
      {
      case 1: user.uCreateBothLists(lm); break;
      case 2: user.uDisplayLists(lm); break;
      case 3: user.uConcatenateLists(lm); break;
      case 4: user.uDeleteLists(lm); break;
      case 5:
      {
         system("cls");
         user.uSetCurrentList(lm);
         if (lm.areCreated()) {
            displaySecondMenu(lm, user);
            break;
         }
         else
            user.uDisplayLists(lm); break;
      }
      case 0: return;
      default: system("cls"); cout << "Invalid option.\n" << endl; break;
      }
   } while (op != NULL);
}

static void displaySecondMenu(ListManager& lm, User& user) 
{
   system("cls");

   int op;
   
   do 
   {
      op= getFirstOption();
      switch (op)
      {
      case 1: user.uPrependNode(lm); break;
      case 2: user.uAppendNode(lm); break;
      case 3: user.uAddInPosition(lm); break;
      case 4: user.uDisableInPos(lm); break;
      case 5: user.uDisableAllNodes(lm); break;
      case 6: user.uDisplayAsc(lm); break;
      case 7: user.uDisplayDesc(lm); break;
      case 8: user.uSortList(lm); break;
      case 9: user.uPurge(lm); break;
      case 10: system("cls"); op= NULL; break;
      case 0: exit(0);
      default: system("cls"); std::cout << "Invalid option.\n"; break;
      }
   } while (op != NULL);
}

int getFirstOption()
{
   int option;

   cout << "//////////////////////////////////////////////////////\n";
   cout << setw(27) << "Menu" << endl;
   cout << "//////////////////////////////////////////////////////\n";
   cout << "            " << "Option 1: Create both lists" << endl;
   cout << "            " << "Option 2: Display all lists" << endl;
   cout << "            " << "Option 3: Concatenate two lists" << endl;
   cout << "            " << "Option 4: Delete both lists" << endl;
   cout << "            " << "Option 5: Manage a list" << endl;
   cout << "            " << "Option 0: Exit" << endl;
   cout << "//////////////////////////////////////////////////////\n" << endl;
   
   cin >> option;

   return option;
}

int getSecondOption()
{
   int option;

   cout << "/////////////////////////////////////////////////////////////////\n";
   cout << setw(43) << "Menu->List Manager" << endl;
   cout << "/////////////////////////////////////////////////////////////////\n";
   cout << "            " << "Option 1: Prepend a node (attach to first position)" << endl;
   cout << "            " << "Option 2: Append a node (attach to last position)" << endl;
   cout << "            " << "Option 3: Add in a specific position" << endl;
   cout << "            " << "Option 4: Disable a node in a specific position" << endl;
   cout << "            " << "Option 5: Disable all nodes" << endl;
   cout << "            " << "Option 6: List all nodes" << endl;
   cout << "            " << "Option 7: List all nodes in reverse" << endl;
   cout << "            " << "Option 8: Sort list" << endl;
   cout << "            " << "Option 9: Purge List" << endl;
   cout << "            " << "Option 10: Back to previous menu" << endl;
   cout << "            " << "Option 0: Exit" << endl;
   cout << "/////////////////////////////////////////////////////////////////\n" << endl;
   cin >> option;

   return option;
}
