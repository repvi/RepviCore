#include "repvicore.h"
#include "RPVC_MEMORYPOOL.h"
#include "SoftwareBus.h"
#include <cassert>
#include <cstring>

#include <iostream>
using namespace std;

static void failOnError(RPVC_Status_t status) 
{
    if (status != RPVC_OK) {
        assert(false && "Operation failed");
    }
}

int main() 
{
    //RPVC_Init();
    const RPVC_SoftwareBusConfig_t sbConfig = {};
    failOnError(RPVC_MEMORYPOOL_Init());
    failOnError(RPVC_SB_Init(&sbConfig));

    failOnError(RPVC_SB_Subscribe(0, 1));

    RPVC_SbMsgHandle_t messageHandle = nullptr;
    const char *messageData = "Test Message";
    failOnError(RPVC_SB_CreateMessage(1, (const uint8_t *)messageData, strlen(messageData) + 1, &messageHandle));
    if (messageHandle == nullptr) {
        assert(false && "Message handle is null");
    }

    failOnError(RPVC_SB_Publish(messageHandle));

    uint8_t receiveBuffer[256] = {0};
    failOnError(RPVC_SB_Receive(0, receiveBuffer, sizeof(receiveBuffer)));
    cout << "Received message: " << receiveBuffer << endl;
    
    failOnError(RPVC_SB_ReleaseMessage(messageHandle));



    failOnError(RPVC_SB_Deinit());

    cout << "All tests passed!" << endl;
    return 0;
}