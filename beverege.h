#ifndef __BEVEREAGES_H__
#define __BEVEREAGES_H__

#include<cstring>
#include<iostream>
#include<fstream>
#include<cstdlib>
using namespace std;
class beverage {
private:
   int startTime;
   int endTime;
   int id;
   int time;
   int orderTime;
   char name[20];
   beverage* next;
public:
   beverage() {
      time = -1;
      next = NULL;
      orderTime = -1;
   }
   beverage(int id, char* Name, int time, int orderTime) {
      this->id = id;
      strcpy(name, Name);
      next = NULL;
      this->time = time;
      this->orderTime = orderTime;
   }
   void setStartTime(int tmp) {
      startTime = tmp;
   }
   int getStartTime() {
      return startTime;
   }
   void setEndTime(int tmp) {
      endTime = tmp;
   }
   int getEndTime() {
      return endTime;
   }
   void setItems(int id, char* Name, int time, int orderTime) {
      this->id = id;
      strcpy(name, Name);
      next = NULL;
      this->time = time;
      this->orderTime = orderTime;
   }
   int getOrderTime() {
      return orderTime;
   }
   int getId() {
      return id;
   }
   char* getName() {
      return name;
   }
   void setNext(beverage* tmp) {
      next = tmp;
   }
   beverage* getNext() {
      return next;
   }
   void setTime(int t) {
      time = t;
   }
   int getTime() {
      return time;
   }
};

class baristar {
private:
   int efficiency; // 1이라는 일을 하기위해 걸리는 시간 
   int endOfWork;  // 끝나기까지 걸리는 시간
   beverage* head;
public:
   baristar() {
      efficiency = 1;
      endOfWork = 0;
      head = NULL;
   }
   int getEff() {
      return efficiency;
   }
   int getEndOfWork() {
      return endOfWork;
   }
   void setEff(int eff) {
      efficiency = eff;
   }
   void setEndOfWork(int end) {
      endOfWork = end;
   }
   void MkBever(beverage *tmp) {
      if (head == NULL) 
         head = tmp;
      else {
         beverage *t = head;
         while (t->getNext() != NULL)
            t = t->getNext();
         t->setNext(tmp);
      }
   }
   void printBever() {
      beverage *tmp = head;
     int spaceSize;
     for (int i = 0; i < 138; i++)
        cout << "-";
     cout << endl;
     while (tmp != NULL) {
        spaceSize = 7;
        cout << "ID : " << tmp->getId();
        if (tmp->getId() / 10 < 1) // ID 한자리
           spaceSize -= 1;
        else if (tmp->getId() / 10 < 10) // ID 두자리
           spaceSize -= 2;
        else if (tmp->getId() / 10 < 100) // ID 세자리
           spaceSize -= 3;
        else // ID 네자리 이상
           spaceSize -= 4;
        for (int i = 0; i < spaceSize-2; i++) 
           cout << " ";
        cout << "|";
        for (int i = spaceSize-2; i < spaceSize; i++)
           cout << " ";

        cout << "NAME : " << tmp->getName();
        spaceSize = 20;
        spaceSize -= strlen(tmp->getName());
        for (int i = 0; i < spaceSize-2; i++)
           cout << " ";
        cout << "|";
        for (int i = spaceSize - 2; i < spaceSize; i++)
           cout << " ";

        cout << " Need TIME : " << tmp->getTime();
        spaceSize = 7;
        if (tmp->getTime() / 10 < 1) // Need Time 한자리
           spaceSize -= 1;
        else if (tmp->getTime() / 10 < 10) // Need Time 두자리
           spaceSize -= 2;
        else if (tmp->getTime() / 10 < 100) // Need Time 세자리
           spaceSize -= 3;
        else // Need Time 네자리 이상
           spaceSize -= 4;
        for (int i = 0; i < spaceSize-2; i++)
           cout << " ";
        cout << "|";
        for (int i = spaceSize - 2; i < spaceSize; i++)
           cout << " ";

        cout << " Order Time : " << tmp->getOrderTime();
        spaceSize = 7;
        if (tmp->getOrderTime() / 10 < 1) // Order Time 한자리
           spaceSize -= 1;
        else if (tmp->getOrderTime() / 10 < 10) // Order Time 두자리
           spaceSize -= 2;
        else if (tmp->getOrderTime() / 10 < 100) // Order Time 세자리
           spaceSize -= 3;
        else // Order Time 네자리 이상
           spaceSize -= 4;
        for (int i = 0; i < spaceSize - 2; i++)
           cout << " ";
        cout << "|";
        for (int i = spaceSize - 2; i < spaceSize; i++)
           cout << " ";

        cout << " Making Start Time : " << tmp->getStartTime();
        spaceSize = 7;
        if (tmp->getStartTime() / 10 < 1) // Start Time 한자리
           spaceSize -= 1;
        else if (tmp->getStartTime() / 10 < 10) // Start Time 두자리
           spaceSize -= 2;
        else if (tmp->getStartTime() / 10 < 100) // Start Time 세자리
           spaceSize -= 3;
        else // Start Time 네자리 이상
           spaceSize -= 4;
        for (int i = 0; i < spaceSize - 2; i++)
           cout << " ";
        cout << "|";
        for (int i = spaceSize - 2; i < spaceSize; i++)
           cout << " ";

        cout << " Making End Time : " << tmp->getEndTime();
        spaceSize = 5;
        if (tmp->getEndTime() / 10 < 1) // End Time 한자리
           spaceSize -= 1;
        else if (tmp->getEndTime() / 10 < 10) // End Time 두자리
           spaceSize -= 2;
        else if (tmp->getEndTime() / 10 < 100) // End Time 세자리
           spaceSize -= 3;
        else // End Time 네자리 이상
           spaceSize -= 4;
        for (int i = 0; i < spaceSize; i++)
           cout << " ";
        cout << "|";
        cout << endl;
        tmp = tmp->getNext();
     }
     for (int i = 0; i < 138; i++)
        cout << "-";
     cout << endl;
   }
};
#endif
