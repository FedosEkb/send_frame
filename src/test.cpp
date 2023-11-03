/* cSpell:disable */
#include "test.h"
// #define FUNCTION_MEMBER_POINTER_TEST

#ifdef FUNCTION_MEMBER_POINTER_TEST
class A
{
public:
     int Afunc() { return 2; };
     virtual int over_func() { return 10; };
};
class B
{
public:
     int Bfunc() { return 3; };
};
// класс С использует одиночное наследование
class C : public A
{
public:
     int Cfunc() { return 4; };
     int over_func() { return 12; };
};
// класс D использует множественное наследование
class D : public A, public B
{
public:
     int Dfunc() { return 5; };
     int over_func() { return 13; };
};
#endif /* FUNCTION_MEMBER_POINTER_TEST */

int main(int argc, char *argv[])
{
#ifdef FUNCTION_MEMBER_POINTER_TEST_DEVISE
net_sim& devise_net = net_sim::get();

dev_son ampermetr1(3.335);
dev_son_t voltemter1(15.23,27.34);
dev_son_t voltemter2(132.23,232.34);
dev_son_t voltemter3(1.23,2.34);
dev_son ampermetr2(323.35);

devise_net.print_all_devise_value();

#endif /* FUNCTION_MEMBER_POINTER_TEST_DEVISE */

#ifdef FUNCTION_MEMBER_POINTER_TEST
     typedef int (A::*test_pnt)();
     A objA;
     B objB;
     C objC;
     D objD;
     A *ptr_to_A = &objA;
     B *ptr_to_B = &objB;
     C *ptr_to_C = &objC;
     D *ptr_to_D = &objD;
     test_pnt test_func = &A::over_func;

     std::cout << sizeof(test_pnt) << " " << (ptr_to_A->*test_func)() << std::endl;

#endif /* FUNCTION_MEMBER_POINTER_TEST */
#ifdef MESAGE_QUEUE_TEST
     mqueue_warp test_queue;
     ssize_t dev_number;
     while (true)
     {
          sleep(5);
          if ((dev_number = test_queue.get_devise_error_set()) == -1)
          {
               std::cout << "Сообщений о несправности приборов не поступало." << std::endl;
          }
          else
          {
               std::cout << "Прибор " << dev_number << " не исправен." << std::endl;
          }
          if ((dev_number = test_queue.get_devise_error_reset()) == -1)
          {
               std::cout << "Сообщений о исправности приборов не поступало." << std::endl;
          }
          else
          {
               std::cout << "Прибор " << dev_number << " исправен." << std::endl;
          }
          if ((dev_number = test_queue.get_net_error_set()) == -1)
          {
               std::cout << "Сообщений о разнывах на сети не поступало." << std::endl;
          }
          else
          {
               std::cout << "Разрыв на сети перед " << dev_number << " прибором." << std::endl;
          }
          if ((dev_number = test_queue.get_net_error_reset()) == -1)
          {
               std::cout << "Сообщений о востановлении сети не поступало." << std::endl;
          }
          else
          {
               std::cout << "Разрыв на сети перед " << dev_number << " прибором устранен." << std::endl;
          }
          std::cout << "---------------------------------" << std::endl;
     }
#endif /* MESAGE_QUEUE_TEST */
     // dev_number = test_queue.get_devise_error_reset()

#ifdef BINARY_OUT_TEST
     const char *FName = "/home/rsu-ti/test"; // Путь к файлу

     int x = 0x12345678; // Переменные для записи
     double y = 5.988;

     /*РАБОТА С БИНАРНЫМ ФАЙЛОМ*/
     std::ofstream out(FName, std::ios::binary); // Ставим режим "бинарный файл"
     out.write((char *)&x, sizeof(x));           // Записываем в файл значение "x"
     out.write((char *)&y, sizeof(y));           // Записываем в файл значение "y"
     out.close();                                // Сохраняем файл
     /*КОНЕЦ РАБОТЫ С БИНАРНЫМ ФАЙЛОМ*/
#endif /* BINARY_OUT_TEST */
     return (0);
}
