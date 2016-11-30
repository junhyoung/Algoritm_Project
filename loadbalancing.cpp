#include "dbconnect.h"
#include "bevereages.h"
#include <iostream>
#include <Windows.h>
#include <iomanip>//setw

using namespace std;

void menu();
void insert();
void run();
void print_db(int fields);
void clear_db(char query[]);
void exit();
void default();
int loading(int loadingTimeStart, int loadingTimeEnd, int plus); // 로드 밸런싱 알고리즘 보여주기 전 로딩하는 장면 출력 함수 (recursive 하게 구현)

int main() {
   db_con();
   menu();
   return 0;
}
void menu() {
   system("cls");
   cout << "-- MENU --" << endl;
   cout << "1. 음료주문하기" << endl << "2. 로드 밸런싱 알고리즘 실행결과 보기" << endl << "3. 주문확인" << endl << "4. 주문 초기화" << endl << "5. 프로그램 종료" << endl;
   cout << "메뉴에 해당하는 숫자를 입력하세요." << endl;
   char menu;
   cin >> menu;
   switch (menu) {
   case '1': insert(); break;
   case '2': run(); break;
   case '3': print_db(0); break;
   case '4': char query[255]; clear_db(query); break;
   case '5': exit(); break;
   default: default();
   }
}
void insert() {
   char name[20];
   char query[255];
   int sel = 0, count = 0, time = 0, ordertime = 0, fields = 0;
   int no = 1;
   count_Instance(&no);
   system("cls");
   int cafeKindNum = 10; // <- 카페에서 파는 음료 종류 갯수 (변할수있음)
   int spaceSize = 50; // 규칙적으로 출력 공백 적용하기 위해
   char *kindOfDrink[10] = { "espresso", "americano", "cafelatte", "cappuccino", "cafemocha", "caramelmacchiato", "hotchoco", "greentealatte", "sweetpotatolatte", "herbtea" };
   int kindOfDrinkTime[10] = { 3,3,4,5,6,6,7,7,7,10 };
   cout << "---------------------------------------------- 주문표----------------------------------------------" << endl;
   cout << " ☞  음료명 ";
   for (int i = 0; i < spaceSize - 15; i++)
      cout << " ";
   cout << "나오는데 걸리는시간              ☆" << endl;
   for (int i = 0; i < cafeKindNum; i++) {
      spaceSize = 50;
      cout << setw(2) << i + 1 << ". ";
      cout << kindOfDrink[i];
      int len = strlen(kindOfDrink[i]);
      for (int j = 0; j < len; j++)
         spaceSize--;
      for (int j = 0; j < spaceSize; j++)
         cout << " ";
      cout << kindOfDrinkTime[i];
      spaceSize = 50;
      if (kindOfDrinkTime[i] / 1000 >= 1)
         spaceSize -= 10;
      else if (kindOfDrinkTime[i] / 100 >= 1)
         spaceSize -= 9;
      else if (kindOfDrinkTime[i] / 10 >= 1)
         spaceSize -= 8;
      else
         spaceSize -= 7;
      for (int j = 0; j < spaceSize; j++)
         cout << " ";
      if (i % 2 == 0)
         cout << "★ ";
      else
         cout << "☆ ";
      cout << endl;
   }
   cout << "---------------------------------------------------------------------------------------------------" << endl;

   insert_db(name, query, &no, count, time, ordertime);
   menu();
}

void run() {
   system("cls");
   cout << "분배되는 과정을 실시간으로 보여드릴까요?\nN을 누르시면 주문목록양에 비례한 로딩 시간을 거친 후 한번에 모두 출력됩니다. (Y/N)" << endl;
   char realTime;
   cin >> realTime;
   bool selecting;
   if (realTime == 'Y' || realTime == 'y')
      selecting = true;
   else
      selecting = false;

   /*******************Loading**************************/
   if (!selecting) {
      int count = 0;
      MYSQL_RES * cnt = select_db(); // id, name, time, orderTime
      while (row = mysql_fetch_row(cnt))
         count++;
      int loadingTime = 300;
      loadingTime += (count / 10) * 100; // 10개당 1초증가
      cout << loadingTime;
      // fields의 갯수만큼 loadingTime이 비례함
      // fields 수 대신 숫자를 넣으면 고정 로딩시간 가능 (100, 1000, 100) 넣으면 1000ms 걸림
      loading(100, loadingTime, 100); // (a,b,c) 라 했을 때 loadingTime = (b / a)ms, a = c 여야 함
   }
   /*******************Loading**************************/

   int numOfBar = 3;
   baristar *bar = new baristar[numOfBar]; // 각 바리스타 객체

   for (int i = 0; i < numOfBar; i++) // 바리스타의 능률을 정함(1이라는 일을 하기위해 걸리는 시간)
      bar[i].setEff(i + 1);
   MYSQL_RES * res = select_db(); // id, name, time, orderTime
   int fields = mysql_num_fields(res);
   while (row = mysql_fetch_row(res))
   {
      if (selecting)
         Sleep(200);
      beverage *coff = new beverage;
      for (int i = 0; i < fields; i++)
         coff->setItems(atoi(row[0]), row[1], atoi(row[2]), atoi(row[3])); // DB에서 하나씩 받아와서 저장할 객체

      int min = 9999;// 제일짧은 시간
      int minWorkBar = 0;

      for (int i = 0; i < numOfBar; i++) { // 제일 걸리는 시간이 짧은 바리스타 찾기
         int barNeedTime = coff->getTime() * (bar[i].getEff());
         int barAccumTime = bar[i].getEndOfWork();
         int nowTime = barAccumTime - coff->getOrderTime();
         if (nowTime < 0) nowTime = 0;
         minWorkBar = (min <= (nowTime + barNeedTime) ? minWorkBar : i);
         min = (min <= (nowTime + barNeedTime)) ? min : (nowTime + barNeedTime);
      }
      int accum = bar[minWorkBar].getEndOfWork() + coff->getTime() * (bar[minWorkBar].getEff());
      coff->setStartTime(bar[minWorkBar].getEndOfWork());
      bar[minWorkBar].setEndOfWork(accum);
      coff->setEndTime(accum);
      bar[minWorkBar].MkBever(coff);
      if (selecting) {
         system("cls");
         for (int i = 0; i < 138; i++)
            cout << "-";
         cout << endl;
         for (int i = 0; i < numOfBar; i++) {
            cout << "|";
            for (int j = 0; j < 64; j++)
               cout << " ";
            cout << (char)(i + 'A') << " Baristar (능률 : 1/" << bar[i].getEff() << ")";
            for (int j = 0; j < 49; j++)
               cout << " ";
            cout << "|" << endl;
            bar[i].printBever();
         }
      }
   }
   if (!selecting) {
      system("cls");
      for (int i = 0; i < 138; i++)
         cout << "-";
      cout << endl;
      for (int i = 0; i < numOfBar; i++) {
         cout << "|";
         for (int j = 0; j < 64; j++)
            cout << " ";
         cout << (char)(i + 'A') << " Baristar (능률 : 1/" << bar[i].getEff() << ")";
         for (int j = 0; j < 49; j++)
            cout << " ";
         cout << "|" << endl;
         bar[i].printBever();
      }
   }
   mysql_free_result(res);

   cout << "다시 --MENU--로 돌아가시겠습니까? (Y/N)" << endl;
   char backPage;
   cin >> backPage;
   if (backPage == 'Y' || backPage == 'y') {
      cout << "잠시 후 --MENU--로 돌아가겠습니다." << endl;
      Sleep(700);
      menu();
   }
   else {
      cout << "로드 밸런싱 과정을 다시 보여드리겠습니다." << endl;
      Sleep(700);
      run();
   }
   menu();
}

void exit() {
   system("cls");
   cout << "정말로 프로그램을 종료하시겠습니까? (Y/N)" << endl;
   char finish;
   cin >> finish;
   if (finish == 'Y' || finish == 'y') {
      cout << "프로그램을 종료합니다." << endl;
      exit(1);
   }
   else {
      cout << "다시 메뉴를 보여드립니다." << endl;
      Sleep(700);
      system("cls");
      menu();
   }
}

void default() {
   cout << "MENU에 있는 알맞은 값을 제대로 입력하세요." << endl;
   Sleep(1000);
   system("cls");
   menu();
}

int loading(int loadingTimeStart, int loadingTimeEnd, int plus) {
   if (loadingTimeStart == loadingTimeEnd) {
      system("cls");
      cout << "잠시 후 로드 밸런싱 알고리즘을 이용하여 바리스타에게 일이 분배되는 과정을 보여드리겠습니다." << endl << endl;
      cout << "잠시만 기다려 주세요. 로딩중입니다. 주문목록양에 비례한 로딩 시간이 걸립니다." << endl;
      for (int i = 0; i < loadingTimeEnd; i += plus)
         cout << "★";
      cout << endl << "COMPLETE!!!" << endl;
      Sleep(700);
      system("cls");
      return 0;
   }
   system("cls");
   cout << "잠시 후 로드 밸런싱 알고리즘을 이용하여 바리스타에게 일이 분배되는 과정을 보여드리겠습니다." << endl << endl;
   cout << "잠시만 기다려 주세요. 로딩중입니다. 주문목록양에 비례한 로딩 시간이 걸립니다." << endl;
   for (int i = 0; i < loadingTimeStart; i += plus)
      cout << "★";
   for (int i = loadingTimeStart; i < loadingTimeEnd; i += plus)
      cout << "☆";
   Sleep(100);
   return loading(loadingTimeStart + plus, loadingTimeEnd, plus);
}

void clear_db(char query[]) {
   system("cls");
   cout << "SQL문 \'";
   sprintf(query, "delete from cafe;"); // table 명 바뀌면 바꿔줘야함
   mysql_query(&mysql, query);
   printf("%s", query);
   cout << "\'를 실행합니다." << endl;
   Sleep(500);
   cout << endl << "모든 주문정보가 삭제되었습니다." << endl;
   cout << "+------+----------+------+------------+" << endl;
   cout << "| ID   | NAME     | TIME | ORDER_TIME |" << endl;
   cout << "+------+----------+------+------------+" << endl;
   cout << "|      |          |      |            |" << endl;
   cout << "+------+----------+------+------------+" << endl;
   Sleep(500);
   cout << "잠시 후 --MENU--로 돌아가겠습니다." << endl;
   Sleep(1500);
   menu();
}

void print_db(int fields)
{
   system("cls");
   mysql_query(&mysql, "select * from cafe");
   res = mysql_store_result(&mysql);
   fields = mysql_num_fields(res);
   cout << "주문 받은 목록을 보여드리겠습니다." << endl;
   Sleep(300);
   cout << "------------------------주문목록----------------------------" << endl;
   cout << "ID    |        NAME             |걸리는시간|  주문시간" << endl;
   cout << "------------------------------------------------------------" << endl;
   while (row = mysql_fetch_row(res))
   {
      for (int i = 0; i < fields; i++) {
         if (i == 0)
            cout << " ";
         int spaceSize; // 규칙적으로 출력 공백 적용하기 위해
         int rowLen = strlen(row[i]);
         printf("%s", row[i]);
         if (i == 0) {
            spaceSize = 10;
            spaceSize -= rowLen;
         }
         else if (i == 1) {
            spaceSize = 25;
            spaceSize -= rowLen;
         }
         else if (i == 2) {
            spaceSize = 10;
            spaceSize -= rowLen;
         }
         else
            break;
         for (int j = 0; j < spaceSize - 5; j++)
            cout << " ";
         cout << "|";
         for (int j = spaceSize - 5; j < spaceSize; j++)
            cout << " ";
      }
      printf("\n");
   }
   mysql_free_result(res);
   cout << "------------------------------------------------------------" << endl;
   Sleep(500);
   cout << "다시 --MENU--로 돌아가시겠습니까? (Y/N)" << endl;
   char backPage;
   cin >> backPage;
   if (backPage == 'Y' || backPage == 'y') {
      cout << "잠시 후 --MENU--로 돌아가겠습니다." << endl;
      Sleep(700);
      menu();
   }
   else {
      cout << "주문목록을 다시 보여드리겠습니다." << endl;
      Sleep(700);
      print_db(0);
   }
   menu();
}
