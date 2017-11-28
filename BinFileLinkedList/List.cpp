#include "List.h"
#include <string>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <sstream>


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
  /* if (fs.fail()) {*/
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
   serializeHeader();
}

string List::displayAsc()
{
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

   int totalSize= listSize;

   while (totalSize != SIZE) {
      fs.seekg(temp->index);

      fs.read((char*)&temp->index, sizeof(int));
      fs.read((char*)&temp->value, sizeof(int));
      fs.read((char*)&temp->next, sizeof(int));
      fs.read((char*)&temp->prev, sizeof(int));
      fs.read((char*)&temp->state, sizeof(bool));

      buffer << "At index number: " << temp->index << " the value is: " << temp->value << endl;
      temp= seekNode(temp->prev);
      totalSize-= NODE_SIZE;
   }

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
   appendNode(list.lastNode);
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
   serializeHeader();
}

void List::addInPos(int value, int pos)
{
   fileStatus();

   Node* new_node= new Node(value);
   Node* prev= new Node();
   Node* next= new Node();;

   if (pos <= 0) {
      prependNode(value);
      return;
   }

   else if (pos >= listSize) {
      appendNode(value);
      return;
   }

   else
      prev= seekNode( pos - 1 );

   fs.seekg(pos);

   new_node->index= fs.tellg();
   prev->next= new_node->index;
   new_node->prev= prev->index;
   next->prev= new_node->index;
   new_node->next= next->index;

   serializeNode(*new_node);
   serializeNode(*prev);
   serializeNode(*next);
   serializeHeader();

   listSize++;
}

List::Node * List::seekNode(int index)
{
   fileStatus();
   fs.seekg(index);

   Node* new_node= new Node();

   fs.read((char*)&new_node->index, sizeof(int));
   fs.read((char*)&new_node->value, sizeof(int));
   fs.read((char*)&new_node->next, sizeof(int));
   fs.read((char*)&new_node->prev, sizeof(int));
   fs.read((char*)&new_node->state, sizeof(bool));

   return new_node;
}

void List::setNodeValue(Node& node, int value)
{
   node.value= value;
   serializeNode(node);
}

void List::sortList()
{
   Node* current= seekNode(firstNode);
   Node* temp;

   for (int i= 0; i < listSize; i++) {
      temp= seekNode(current->next);
      for (int j= 0; j < listSize; j++) {
         if (current->index > temp->index) {
            swap(current->index, temp->index);
            serializeNode(*current);
            serializeNode(*temp);
         }
         temp= seekNode(temp->next);
      }
      current= seekNode(current->next);
   }
}

void List::removeAllNodes()
{
   fileStatus();
   fs.seekg(HEADER_SIZE);

   Node* temp= seekNode(fs.tellg());
   
   for (int pos= 0; !fs.eof(); pos= temp->index ) {
      temp->state= false;
      temp= seekNode(pos + NODE_SIZE);
   }
}

void List::removeNode(int pos)
{
   fileStatus();
   Node* temp= seekNode(pos);

   if (!temp) {
      return;
   }

   temp->state= false;
   listSize--;

   serializeHeader();
}

int List::purge()
{
   int purgeCount= 0;
   List* temp= new List("temp.bin");
   fileStatus();
   temp->fileStatus();

   Node* tempNode= new Node();
   fs.seekg(HEADER_SIZE);
   tempNode= seekNode(HEADER_SIZE);

   while (!fs.eof())
   {
      int oldFileIndex= tempNode->index;

      if (tempNode->state) {
         temp->appendNode(tempNode->value);
      }
      else
         purgeCount++;

      tempNode= seekNode(oldFileIndex + NODE_SIZE);
   }
   
   temp->listSize= listSize;
   temp->firstNode= firstNode;
   temp->lastNode= lastNode;
   temp->serializeHeader();

   fs.close();
   temp->fs.close();

   remove(fileName);
   rename(temp->fileName, fileName);
   fileStatus();
   delete temp;


   return purgeCount;
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
