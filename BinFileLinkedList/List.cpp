#include <string>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <sstream>
#include "List.h"

using namespace std;

void List::newFile()
{
   fs.open(fileName, ios::out | ios::binary);

   fs.write( (char*)&listSize, sizeof(int) );
   fs.write( (char*)&firstNode, sizeof(int) );
   fs.write( (char*)&lastNode, sizeof(int) );

   fs.close();
}

bool List::isEmpty()
{
   bool flag;

   fs.seekg(0, ios::end);
   int fileSize= (int)fs.tellg();

   flag= fileSize <= HEADER_SIZE ? true : false;

   return flag;
}

bool List::openFile()
{
   fs.open(fileName, ios::in | ios::out | ios::binary);

   int holder= listSize;

   if ( fs.fail() ) {
      newFile();
      openFile();
   }

   if ( isEmpty() ) 
      serializeHeader();

   else {
      fs.seekg(SIZE);
      fs.read((char*)&listSize, sizeof(int));
      fs.seekg(FIRST);
      fs.read((char*)&firstNode, sizeof(int));
      fs.seekg(LAST);
      fs.read((char*)&lastNode, sizeof(int));

      listSize= holder;


      if (listSize > 1) {
         Node* temp= seekNode(firstNode);
         while (temp->next != -1) {

            fs.seekg(temp->index);
            serializeNode(*temp);

            temp= seekNode(temp->next);
         }
      }
   }
   return true;
}

void List::fileStatus()
{
   if( fs.is_open()){
      fs.close();
     
   }
   openFile();
}

void List::clearFile()
{
   fs.close();
   fs.open(fileName, ios::out | ios::trunc);

   firstNode= lastNode= -1;
   listSize=0;
   serializeHeader();
}

void List::closeFile()
{
   fs.close();
}

string List::displayAsc()
{
   fileStatus();
   fs.seekg(HEADER_SIZE);
   ostringstream buffer;

   if (!listSize) {
      buffer << "Empty list.\n";
      return buffer.str();
   }
   Node* temp= seekNode(firstNode);

   while (temp->next != -1) {
      fs.seekg(temp->index);

      fs.read((char*)&temp->index, sizeof(int));
      fs.read((char*)&temp->value, sizeof(int));
      fs.read((char*)&temp->next, sizeof(int));
      fs.read((char*)&temp->prev, sizeof(int));
      fs.read((char*)&temp->state, sizeof(bool));

      buffer << "At index number: " << temp->index << " the value is: " << temp->value << endl;
      temp= seekNode(temp->next);
   }
   temp= seekNode(lastNode);
   buffer << "At index number: " << temp->index << " the value is: " << temp->value << endl;

   return buffer.str();
} 

string List::displayDesc()
{
   fs.seekg(HEADER_SIZE);
   ostringstream buffer;

   if (!listSize) {
      buffer << "Empty list.\n";
      return buffer.str();
   }

   Node* temp= seekNode(lastNode);

   while (temp->prev != -1) {
      fs.seekg(temp->index);

      fs.read((char*)&temp->index, sizeof(int));
      fs.read((char*)&temp->value, sizeof(int));
      fs.read((char*)&temp->next, sizeof(int));
      fs.read((char*)&temp->prev, sizeof(int));
      fs.read((char*)&temp->state, sizeof(bool));

      buffer << "At index number: " << temp->index << " the value is: " << temp->value << endl;
      temp= seekNode(temp->prev);
   }
   temp= seekNode(firstNode);
   buffer << "At index number: " << temp->index << " the value is: " << temp->value << endl;

   return buffer.str();
}

void List::concatenateList(List& list)
{
   fileStatus();
   list.fileStatus();

   Node* temp= list.seekNode(list.firstNode);

   while (temp->next != -1) {
      appendNode(temp->value);
      temp= list.seekNode(temp->next);
   }
   temp= list.seekNode(list.lastNode);
   appendNode(temp->value);
   list.clearFile();
} 

void List::appendNode(int value)
{
   fileStatus();
   fs.seekg(0, ios::end);

   Node* new_node= new Node(value);
   new_node->index= (int)fs.tellg();
   listSize++;

   if (lastNode != -1) {
      Node* temp= seekNode(lastNode);

      temp->next= new_node->index;
      new_node->prev= temp->index;
      lastNode= new_node->index;

      serializeNode(*temp);
   }

   else 
      firstNode= lastNode= new_node->index;

   serializeNode(*new_node);
   serializeSize();
   serializeHeader();
}

void List::prependNode(int value)
{
   fileStatus();
   fs.seekg(0, ios::end);

   Node* new_node= new Node(value);
   new_node->index= (int)fs.tellg();
   listSize++;

   if (firstNode != -1) {
      Node* temp= seekNode(firstNode);

      temp->prev= new_node->index;
      new_node->next= temp->index;
      firstNode= new_node->index;

      serializeNode(*temp);
   }

   else
      firstNode= lastNode= new_node->index;

   serializeNode(*new_node);
   serializeSize();
   serializeHeader();
}

void List::addInPos(int value, int pos)
{
   fileStatus();

   Node* new_node= new Node(value);
   Node* prev= new Node();
   Node* next= new Node();

   if (pos <= 0) {
      prependNode(value);
      return;
   }

   else if (pos >= listSize) {
      appendNode(value);
      return;
   }

   else {
      fs.seekg(0, ios::end);
      new_node->index= fs.tellg();

      next= seekNode(firstNode);
      for (int i= 0; i < pos; i++)
         next= seekNode(next->next);
   }

   prev= seekNode(next->prev);

   next->prev= new_node->index;
   new_node->next= next->index;
   prev->next= new_node->index;
   new_node->prev= prev->index;

   serializeNode(*new_node);
   serializeNode(*prev);
   serializeNode(*next);
   serializeHeader();

   listSize++;
}

List::Node * List::seekNode(int index)
{
   fs.seekg(index);

   Node* new_node= new Node();

   fs.read((char*)&new_node->index, sizeof(int));
   fs.read((char*)&new_node->value, sizeof(int));
   fs.read((char*)&new_node->next, sizeof(int));
   fs.read((char*)&new_node->prev, sizeof(int));
   fs.read((char*)&new_node->state, sizeof(bool));

   return new_node;
}

void List::setNodeValue(int pos, int value)
{
   Node* temp= new Node();
   pos = ((pos - 1) * NODE_SIZE) + HEADER_SIZE;
   temp= seekNode(pos);
   temp->value= value;
   serializeNode(*temp);
}

void List::sortList()
{
   Node* current= seekNode(firstNode);
   Node* temp;

   for (int i= 1; i < listSize; i++) {
      temp= seekNode(current->next);

      while (temp->next != -1) {
         if (current->value > temp->value) {
            swap(current->value, temp->value);
            serializeNode(*current);
            serializeNode(*temp);
         }

         temp= seekNode(temp->next);
      }

      if (current->value > temp->value) {
         swap(current->value, temp->value);
         serializeNode(*current);
         serializeNode(*temp);
      }

      current= seekNode(current->next);
   }
}

void List::disableAllNodes()
{
   fileStatus();
   Node* temp= seekNode(firstNode);

   for (int i = 1; i <= listSize; i++) {
      temp->state= false;
      serializeNode(*temp);
      temp= seekNode(temp->next);
   }
   listSize= 0;
   serializeSize();
   serializeHeader();
}

void List::disableNode(int pos)
{
   Node* temp= seekNode(firstNode);

   if (!temp)
      return;

   if (pos >= listSize) {
      temp= seekNode(lastNode);
      lastNode= temp->prev;
      Node* auxTemp= temp;
      temp= seekNode(lastNode);
      temp->next= -1;
      auxTemp->state= false;

      serializeNode(*auxTemp);
      serializeNode(*temp);
   }

   else if (pos <= 1) {
      temp= seekNode(firstNode);
      firstNode= temp->next;
      Node* auxTemp= temp;
      temp= seekNode(firstNode);
      temp->prev= -1;
      auxTemp->state= false;

      serializeNode(*auxTemp);
      serializeNode(*temp);
   }

   else {
      for (int i = 1; i < pos; i++) 
         temp= seekNode(temp->next);

         Node* tempNext= seekNode(temp->next);
         Node* tempPrev= seekNode(temp->prev);

         tempPrev->next= tempNext->index;
         tempNext->prev= tempPrev->index;
         temp->state= false;

         serializeNode(*temp);
         serializeNode(*tempNext);
         serializeNode(*tempPrev);
   }
   listSize--;
   serializeSize();
   serializeHeader();
}

void List::purge()
{
   List* temp= new List("temp.bin");
   fileStatus();
   temp->fileStatus();

   fs.seekg(HEADER_SIZE);
   Node* tempNode= seekNode(firstNode);

   while (tempNode->next != -1)
   {
      int oldFileIndex= tempNode->index;

      if (tempNode->state) {
         temp->appendNode(tempNode->value);
      }

      tempNode= seekNode(tempNode->next);
   }

   if (tempNode->state)
      temp->appendNode(tempNode->value);

   temp->serializeHeader();
   temp->serializeSize();

   fs.close();
   temp->fs.close();

   remove(fileName);
   rename(temp->fileName, fileName);
   delete temp;
   fileStatus();

}

void List::serializeSize()
{
   fs.seekp(SIZE);
   fs.write((char*)&listSize, sizeof(int));
}

void List::serializeHeader()
{
   fs.seekp(0);

   fs.write((char*)&listSize, sizeof(int));
   fs.write((char*)&firstNode, sizeof(int));
   fs.write((char*)&lastNode, sizeof(int));
   fs.flush();
}

void List::serializeNode(Node& node)
{
   fs.seekp(node.index);

   if (fs.is_open()) {
      fs.write((char*)&node.index, sizeof(int));
      fs.write((char*)&node.value, sizeof(int));
      fs.write((char*)&node.next, sizeof(int));
      fs.write((char*)&node.prev, sizeof(int));
      fs.write((char*)&node.state, sizeof(bool));
      fs.flush();
   }
}

void List::serializeSetValue(Node& node)
{
   fs.seekp(node.index + VALUE);
   fs.write((char*)&node.value, sizeof(int));
}