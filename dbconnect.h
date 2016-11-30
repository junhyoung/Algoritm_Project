#ifndef __DBCONNECT_H__
#define __DBCONNECT_H__
#define _CRT_SECURE_NO_WARNINGS

#include <my_global.h>
#include <mysql.h>
#include <iostream>

#pragma comment(lib, "libmysql.lib")
#define DB_HOST "127.0.0.1"
#define DB_USER "root"
#define DB_PASS "password"
#define DB_NAME "mydb"

MYSQL mysql;
MYSQL_RES * res = NULL;
MYSQL_ROW row = 0;
void insert_db(char name[], char query[], int * no, int count, int time, int ordertime);
int selectLastOrderTime();

void db_con()
{

   mysql_init(&mysql); // mysql 객체를 초기화

   if (!mysql_real_connect(&mysql, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, (char *)NULL, 0))
   {
      printf("%s\n", mysql_error(&mysql));
      exit(1);
   }
   else
      printf("success\n");
}

void insert_db(char name[], char query[], int * no, int count, int time, int ordertime)
{
   int n = 1;
   int lastTime = selectLastOrderTime();
   char *kindOfDrink[10] = { "espresso", "americano", "cafelatte", "cappuccino", "cafemocha", "caramelmacchiato", "hotchoco", "greentealatte", "sweetpotatolatte", "herbtea" };
   int kindOfDrinkTime[10] = { 3,3,4,5,6,6,7,7,7,10 };
   printf("음료 번호? ");
   while (1) {
      scanf("%d", &n);
      if (n >= 1 && n <= 10)
         break;
      else
         printf("번호를 확인해 주세요\n");
   }
   name = kindOfDrink[n - 1];
   time = kindOfDrinkTime[n - 1];
   printf("%s\n", name);
   printf("잔수? ");
   scanf("%d", &count);

   printf("Last Order Time : %d\n", lastTime);
   printf("주문시간? ");
   while (1) {
      scanf("%d", &ordertime);
      if (ordertime < lastTime) {
         printf("마지막 주문시간 보다 작습니다. 다시 입력해주세요\n");
      }
      else
         break;
   }

   for (int i = 0; i < count; i++)
   {
      sprintf(query, "insert into cafe values ""(%d, '%s', %d, %d)", *no, name, time, ordertime);
      printf("%s\n", query);
      if (mysql_query(&mysql, query))
         printf("insert query error\n");
      (*no)++;
   }
}
int selectLastOrderTime() {
   int r;
   mysql_query(&mysql, "select * from cafe");
   res = mysql_store_result(&mysql);
   int fields = mysql_num_fields(res);
   bool check = false;
   while (row = mysql_fetch_row(res)) {
      r = atoi(row[3]);
      check = true;
   }
   if (check)
      return r;
   else
      return 0;
}
void count_Instance(int* no) { //DB의 인스턴스 갯수(마지막ID)를 받아오는 함수
   int fields;
   mysql_query(&mysql, "select * from cafe");
   res = mysql_store_result(&mysql);
   fields = mysql_num_fields(res);

   while (row = mysql_fetch_row(res))
   {
      (*no)++;
   }
   mysql_free_result(res);
   printf("%d", *no);
}

MYSQL_RES* select_db()
{
   mysql_query(&mysql, "select * from cafe");
   res = mysql_store_result(&mysql);

   return res;
}
#endif
