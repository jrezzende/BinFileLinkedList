#include "List.h"
#include <string>
#include <fstream>

void List::newFile()
{
   fs.open(fileName, ios::out | ios::binary);

   //header total size
   fs.write( (char*)&listSize, sizeof(int) );
   fs.write( (char*)&firstNode, sizeof(int) );
   fs.write( (char*)&lastNode, sizeof(int) );

   fs.close();
}

bool List::isEmpty()
{
   return false;
}

bool List::openFile()
{
   fs.open(fileName, ios::in | ios::out | ios::binary);

   if (!fs.good()) {
      newFile();
      openFile();
   }

   if (isEmpty()) {

   }

   return false;
}

void List::fileStatus()
{
}

void List::clearFile()
{
}

string List::displayAsc()
{
   return string();
}

string List::displayDesc()
{
   return string();
}

void List::concatenateList(List &)
{
}

void List::appendNode(int value)
{
}

void List::prependNode(int value)
{
}

void List::addInPos(int value, int pos)
{
}

List::Node * List::seekNode(int index)
{
   return nullptr;
}

void List::setNodeValue(Node*, int value)
{
}

void List::sortList()
{
}

void List::removeAllNodes()
{
}

void List::removeNode(Node&)
{
}

int List::purge()
{
   return 0;
}

void List::serializeSize()
{
   fs.seekp(SIZE);
   fs.write((char*)&listSize, sizeof(int));
}

void List::serializeHeader()
{
}

void List::serializeNode(Node&)
{
}

void List::serializeSetValue(Node&)
{
}
