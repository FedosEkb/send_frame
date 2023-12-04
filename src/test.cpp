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


#ifdef SOME_STRUCT_TEST

enum class OPERATON_STATES_t : uint8_t // operation states
{
    VALID = 0b11110000,  //!< Положительный.
    INVALID = 0b00001111 //!< Отрицательный.
};

typedef struct __attribute__((packed)) selftest_packet // TODO вернутся к выравниванию!
{
    uint8_t Uk_0;                       //!< 8-разрядное слово с дискретом преобра-зования 0,353 А. Для ПрКИР поле зарезервиравано.
    uint8_t Uk_100;                     //!< 8-разрядное слово с дискретом преобра-зования 0,353 А. Для ПрКИР поле зарезервиравано.
    OPERATON_STATES_t operation_states; //!< Обобщенный результат функционирова-ния (положительный / отрицательный).
    uint16_t CRCF;                      //!< Фактическая контрольная сумма.
    uint16_t CRCROM;                    //!< Контрольная сумма ПЗУ.
    uint8_t Myaddr;                     //!< Значение адреса прибора, проводившего самопроверку.
    selftest_packet() : Uk_0(0), Uk_100(0), operation_states(OPERATON_STATES_t::INVALID), CRCF(0),
                        CRCROM(0), Myaddr(0) {}
}selftest_packet;

int some_func(selftest_packet & some_data){
}

struct selftest_frame
{
     struct ethhdr frame_header; //!< MAC + Type/Len field
     uint8_t ready;              //!< Статус готовности к работе. 0 - ПП не готов к работе. 1 - ПП готов к работе.
     selftest_packet diag;       //!< Диагностика от прибора
     uint8_t diag_padding[80 - sizeof(selftest_packet)];
     selftest_frame() : frame_header{.h_dest{12,0,0,0,0,0},.h_source{12,0,0,0,0,0},.h_proto = 40000}
     {

     }
} __attribute__((packet));

#endif /* SOME_STRUCT_TEST */


void some_test(uint16_t t){
     std::cout << t << std::endl;
}

int main(int argc, char *argv[])
{
some_test((int16_t)-1);

#ifdef SOME_STRUCT_TEST
     // assert(false && "Нарушен порядок внесения приборов на сеть.");
     std::cout << "------size_of_struct------" << std::endl;
     std::cout << sizeof(selftest_packet) << std::endl;
     std::cout << sizeof(struct ethhdr) << std::endl;
     std::cout << sizeof(struct selftest_frame) << std::endl;
     std::cout << "------self_packet_test------" << std::endl;
     struct selftest_frame test_packed;
     std::cout << "Uk_0: " << uint32_t(test_packed.diag.Uk_0) << std::endl; 
     std::cout << "Uk_100: " << uint32_t(test_packed.diag.Uk_100) << std::endl; 
     std::cout << "operation_states: " << uint32_t(test_packed.diag.operation_states) << std::endl; 
     std::cout << "CRCF: " << test_packed.diag.CRCF << std::endl; 
     std::cout << "CRCROM: " << test_packed.diag.CRCROM << std::endl; 
     std::cout << "Myaddr: " << uint32_t(test_packed.diag.Myaddr) << std::endl; 
     std::cout << "src_MAC: ";
     for (size_t i = 0; i < 6; i++)
     {
          std::cout <<uint32_t(test_packed.frame_header.h_source[i]) << ":";
     }
     std::cout << std::endl;
     std::cout << "dest_MAC: ";
     for (size_t i = 0; i < 6; i++)
     {
          std::cout <<uint32_t(test_packed.frame_header.h_dest[i]) << ":";
     }
     std::cout << std::endl; 
     std::cout << "type: " << uint32_t(test_packed.frame_header.h_proto) << std::endl; 
     std::cout << "cast test" << std::endl;

     struct selftest_frame new_struct;
     void *void_ptr = &new_struct;

     struct selftest_frame new_struct_after_cast = *reinterpret_cast<struct selftest_frame*>(void_ptr);
     
     for (size_t i = 0; i < 6; i++)
     {
          std::cout <<uint32_t(new_struct_after_cast.frame_header.h_source[i]) << ":";
     }

#endif /* SOME_STRUCT_TEST */

#ifdef FUNCTION_MEMBER_POINTER_TEST_DEVISE
     net_sim &devise_net = net_sim::get();

     dev_son ampermetr1(3.335);
     dev_son_t voltemter1(15.23, 27.34);
     dev_son_t voltemter2(132.23, 232.34);
     dev_son_t voltemter3(1.23, 2.34);
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

//#define BINARY_OUT_TEST
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
//#define ARREY_OF_PTR_ON_VOID 
#ifdef ARREY_OF_PTR_ON_VOID
     #define LEN 32
     
     int somedate[LEN];
     for (size_t i = 0; i < LEN; i++)
     {
          somedate[i] = i*i;
     }

     void *ptr_to_data[LEN];
     
     for (size_t i = 0; i < LEN; i++)
     {
          ptr_to_data[i] = somedate + i;
     }

     for (size_t i = 0; i < LEN; i++)
     {
          std::cout << *reinterpret_cast<int*>(ptr_to_data[i]) << " | ";
     }
#endif /* ARREY_OF_PTR_ON_VOID */

     return 0;
}
