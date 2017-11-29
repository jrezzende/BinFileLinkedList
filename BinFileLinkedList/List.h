#pragma once
#ifndef INCLUDED_LIST_H
#define INCLUDED_LIST_H

#define NODE_SIZE 17
#define HEADER_SIZE 12
#define SIZE 0
#define FIRST 4
#define LAST 8
#define VALUE 4
#define NEXT 8
#define PREV 12
#define STATE 16

#include <string>
#include <fstream>
#include <iostream>

using namespace std;

class List
{
private:
   struct Node
   {
   public:
      Node() { index= 0; value= 0; state= true; next= prev= -1; }
      Node(int v) { index= 0; value= v; state= true; next= prev= -1; }

      int index;
      int value;
      bool state;
      int next;
      int prev;
   };

   int firstNode;
   int lastNode;
   int listSize;
   fstream fs;
   const char* fileName;

public:
   List(const char* fName) { 
      fileName= fName; 
      firstNode= lastNode= -1; 
      listSize= 0;
      openFile();
   }
   ~List() { remove(fileName); }

   /////////////////

   void newFile();// ok
   bool openFile();// ok
   bool isEmpty();// ok
   void fileStatus();// ok
   void clearFile();// ok

   int getListSize() { return listSize; } // ok
   int getFirstNode() { return firstNode; }// ok
   int getLastNode() { return lastNode; }// ok
   string getFileName() { return fileName; }// ok
   
   string displayAsc();// ok
   string displayDesc();// ok

   void concatenateList(List&);// ok
   void sortList();// ok

   void appendNode(int value);// ok
   void prependNode(int value);// ok
   void addInPos(int value, int pos);// ok
   Node* seekNode(int index);// ok 
   void setNodeValue(int pos, int value); // ok

   void removeAllNodes();// ok
   void removeNode(int pos); // ok
   int purge(); // ?

   void serializeSize();// ok
   void serializeHeader();// ok
   void serializeNode(Node&);// ok
   void serializeSetValue(Node&);// ok
};

#endif // !INCLUDED_LIST_H
