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
   int fileSize= fs.tellg();

   flag= fileSize <= HEADER_SIZE ? true : false;

   return flag;
}

bool List::openFile()
{
   fs.open(fileName, ios::in | ios::out | ios::binary);

   if ( !fs.good() ) {
      newFile();
      openFile();
   }

   if ( isEmpty() ) {
      listSize= 0;
      firstNode= lastNode= 1;
      serializeHeader();
   }

   else {
      fs.seekg(SIZE);
      fs.read((char*)&listSize, sizeof(int));
      fs.seekg(FIRST);
      fs.read((char*)&firstNode, sizeof(int));
      fs.seekg(LAST);
      fs.read((char*)&lastNode, sizeof(int));
   }

   return true;
}

void List::fileStatus()
{
   if (!fs.good()) {
      fs.close();
      openFile();
   }
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

   while (!fs.eof()) {
      fs.seekg(temp->index);

      fs.read((char*)&temp->index, sizeof(int));
      fs.read((char*)&temp->value, sizeof(int));
      fs.read((char*)&temp->next, sizeof(int));
      fs.read((char*)&temp->prev, sizeof(int));
      fs.read((char*)&temp->state, sizeof(bool));

      buffer << "At index number: " << temp->index << " the value is: " << temp->value << endl;
      temp= seekNode(temp->next);
   }

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
   new_node->index= fs.tellg();
   listSize++;

   if (lastNode != -1) {
      Node* temp= seekNode(lastNode);

      temp->next= new_node->index;
      new_node->prev= temp->index;
      lastNode= new_node->index;

      serializeNode(*new_node);
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
   new_node->index= fs.tellg();
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
   Node* prev;
   Node* next;

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
   int pos;

   for (; !fs.eof(); pos= temp->index ) {
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
   return 0;
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
}

void List::serializeNode(Node& node)
{
   fs.seekp(node.index);

   fs.write((char*)&node.index, sizeof(int));
   fs.write((char*)&node.value, sizeof(int));
   fs.write((char*)&node.next, sizeof(int));
   fs.write((char*)&node.prev, sizeof(int));
   fs.write((char*)&node.state, sizeof(bool));

   serializeSize();
}

void List::serializeSetValue(Node& node)
{
   fs.seekp(node.index + VALUE);
   fs.write((char*)&node.value, sizeof(int));
}
