#pragma once
#ifndef USER_H
#define USER_H

class ListManager;

class User
{
public: 
   void uCreateBothLists(ListManager&);
   void uDisplayLists(ListManager&);
   void uDeleteLists(ListManager&);
   bool uConcatenateLists(ListManager&);
   void uSetCurrentList(ListManager&);

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
