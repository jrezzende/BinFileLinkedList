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

/*(string List::displayAsc()
{
   fs.seekg(HEADER_SIZE);
   ostringstream buffer;

   if (!listSize) {
      buffer << "Empty list.\n";
      return buffer.str();
   }

   return string();
} 

string List::displayDesc()
{
   return string();
}

void List::concatenateList(List &)
{
} */ 


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

}

void List::removeAllNodes()
{
}

void List::removeNode(Node& node)
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
