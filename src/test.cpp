/* cSpell:disable */
#include "test.h"


int main(int argc, char *argv[])
{

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
#endif
     return (0);
}
