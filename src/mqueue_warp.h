#ifndef MQUEUE_WARP
#define MQUEUE_WARP

#include <stdio.h> 
#include <sys/ipc.h> 
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define ProjectId 321
#define PathName  "/home/rsu-ti/WorkSpace/RSUTI_Net_emul/src/mqueue_warp.h"
/* any existing, accessible file would do */
#define MsgLen    sizeof(uint32_t)

class mqueue_warp
{
private:
  enum MASSAGE_TYPE : long  //!< Тип последнего принятого сообщения.
  {
    NOT_A_MASSAGE = -1,     //!< При последнем запросе ссобщение небыло полученно.
    DEVISE_ERROR_SET = 1,   //!< При последнем запросе было получено сообщение о неисправности прибора.
    DEVISE_ERROR_RESET = 2, //!< При последнем запросе было получено сообщение о востановлении работы прибора.
    NET_ERROR_SET = 3,      //!< При последнем запросе было получено сообщение о неисправности на сети.
    NET_ERROR_RESET = 4     //!< При последнем запросе было получено сообщение о востановлении работы на сети.
  };

  typedef struct queuedMessage //!< Структура для получения сообщений из очереди.
  {
    MASSAGE_TYPE type;      /* must be of type long */
    ssize_t devise_number; /* bytes in the message */
    queuedMessage(){
      type = NOT_A_MASSAGE;
      devise_number = 0;
    }
    
  } queuedMessage;

  int queue_id;                          //!< Поле для храниния ID очереди.
  queuedMessage last_received_massage;   //!< Поле для храниния последнего полученного сообщения из очереди.

  void report_and_exit(const char *msg);

  ssize_t take_msg_from_queue();
public:
  /// @brief Метод опрашивает очередь, на предмет полученных сообщений об установке неисправности прибора.
  /// @return -1 если в очереди нет сообщений. Номер прибора, если сообщение присутствует.
  ssize_t get_devise_error_set(); 
  /// @brief Метод опрашивает очередь, на предмет полученных сообщений о снятии неисправности прибора.
  /// @return -1 если в очереди нет сообщений. Номер прибора, если сообщение присутствует. 
  ssize_t get_devise_error_reset();
  /// @brief Метод опрашивает очередь, на предмет полученных сообщений об установке неисправности на сети.
  /// @return -1 если в очереди нет сообщений. Номер прибора перед которым произошёл разрыв, если сообщение присутствует.
  ssize_t get_net_error_set();
  /// @brief Метод опрашивает очередь, на предмет полученных сообщений о снятии неисправности на сети.
  /// @return -1 если в очереди нет сообщений. Номер прибора перед которым не было ни единого разрыва, если сообщение присутствует.
  ssize_t get_net_error_reset();

  mqueue_warp();
  ~mqueue_warp();
};


#endif /* MQUEUE_WARP */