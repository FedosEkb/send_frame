#ifndef TEST_PROJ
#define TEST_PROJ



#include <signal.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "eth_raw_client.h"
#include "./Module/MsEthModule.h"

#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <fstream>

#include "mqueue_warp.h"

#include "assert.h"

#define FUNCTION_MEMBER_POINTER_TEST_DEVISE
// #define SOME_STRUCT_TEST

#ifdef FUNCTION_MEMBER_POINTER_TEST_DEVISE


#include "method_pointer_test/net_sim.h"
#include "method_pointer_test/dev_son.h"
#include "method_pointer_test/dev_son_t.h"

#endif /* FUNCTION_MEMBER_POINTER_TEST_DEVISE */


// #define ETH_TEST       0

#endif /* TEST_PROJ */