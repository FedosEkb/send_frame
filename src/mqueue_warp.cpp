#include "mqueue_warp.h"

mqueue_warp::mqueue_warp()
{
    key_t key = ftok(PathName, ProjectId);
    if (key < 0)
        this->report_and_exit("couldn't get key...");

    queue_id = msgget(key, 0666 | IPC_CREAT);
    if (queue_id < 0)
        this->report_and_exit("couldn't get queue id...");
}

mqueue_warp::~mqueue_warp()
{
    if (msgctl(queue_id, IPC_RMID, NULL) < 0)
    { /* NULL = 'no flags' */
        this->report_and_exit("trouble removing queue...");
    }
}
ssize_t mqueue_warp::get_devise_error_set()
{
    last_received_massage.type = DEVISE_ERROR_SET;
    if (this->take_msg_from_queue() == -1)
    {
        return -1;
    }
    return last_received_massage.devise_number;
}

ssize_t mqueue_warp::get_devise_error_reset()
{
    last_received_massage.type = DEVISE_ERROR_RESET;
    if (this->take_msg_from_queue() == -1)
    {
        return -1;
    }
    return last_received_massage.devise_number;
}
ssize_t mqueue_warp::get_net_error_set()
{
    last_received_massage.type = NET_ERROR_SET;
    if (this->take_msg_from_queue() == -1)
    {
        return -1;
    }
    return last_received_massage.devise_number;
}
ssize_t mqueue_warp::get_net_error_reset()
{
    last_received_massage.type = NET_ERROR_RESET;
    if (this->take_msg_from_queue() == -1)
    {
        return -1;
    }
    return last_received_massage.devise_number;
}

ssize_t mqueue_warp::take_msg_from_queue()
{
    return msgrcv(queue_id, &last_received_massage, MsgLen, last_received_massage.type, MSG_NOERROR | IPC_NOWAIT);
}

void mqueue_warp::report_and_exit(const char *msg)
{
    perror(msg);
    exit(-1); /* EXIT_FAILURE */
}
