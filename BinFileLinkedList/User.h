#pragma once
#ifndef USER_H
#define USER_H

class ListManager;

class User
{
public: 
   int uCreateBothLists(ListManager&);
   void uDisplayLists(ListManager&);
   int uDeleteLists(ListManager&);
   bool uConcatenateLists(ListManager&);
   int uSetCurrentList(ListManager&);

   void uPrependNode(ListManager&);
   void uAppendNode(ListManager&);
   void uAddInPosition(ListManager&);
   void uDisplayAsc(ListManager&);
   void uDisplayDesc(ListManager&);
   void uSortList(ListManager&);
   void uDisableInPos(ListManager&);
   void uDisableAllNodes(ListManager&);
   void uPurge(ListManager&);

};

#endif // USER_H
