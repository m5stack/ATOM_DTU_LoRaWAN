/*
    Description: 
    Use ATOM DTU LoRaWAN to connect to the Gateway by ABP mode, and implement subscription and publishing messages.
    Before use, please configure the device information, receiving window, frequency band mask and other information according to the actual connected network.
    Please install library before compiling:  
    FastLED: https://github.com/FastLED/FastLED
    M5Atom: https://github.com/m5stack/M5Atom
*/

#include "ATOM_DTU_LoRaWAN.h"
#include "M5Atom.h"

ATOM_DTU_LoRaWAN LoRaWAN;
String response;

typedef enum {
    kError = 0,
    kConnecting,
    kConnected, 
    kSending
} DTUState_t;

DTUState_t State = kConnecting;

void TaskLED(void *pvParameters) {
    while(1) {
        switch (State)
        {
        case kError:
            M5.dis.drawpix(0, 0x00ff00);
            break;
        case kConnecting:
            M5.dis.drawpix(0, 0xff0000);
            break;
        case kConnected:
            M5.dis.drawpix(0, 0x0000ff);
            break;
        case kSending:
            M5.dis.drawpix(0, 0xff000ff);
            break;
        }
        for(int i=100;i>0;i--) {
            M5.dis.setBrightness(i);
            FastLED.show();
            vTaskDelay( 10 / portTICK_RATE_MS );
        }
        for(int i=0;i<100;i++) {
            M5.dis.setBrightness(i);
            FastLED.show();
            vTaskDelay( 10 / portTICK_RATE_MS );
        }
    }
}

void setup()
{
    M5.begin(true, true, true);
    //InIt
    LoRaWAN.Init();
    //Reset Module
    Serial.print("Module Rerest.....");
    LoRaWAN.writeCMD("AT+ILOGLVL=5\r\n");
    LoRaWAN.writeCMD("AT+CSAVE\r\n");
    LoRaWAN.writeCMD("AT+IREBOOT=0\r\n");
    delay(5000);

    //Create LED Task
    xTaskCreatePinnedToCore(
        TaskLED
        ,  "TaskLED"   // A name just for humans
        ,  4096  // This stack size can be checked & adjusted by reading the Stack Highwater
        ,  NULL
        ,  1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
        ,  NULL 
        ,  0);


    LoRaWAN.configABP(
        "00dee04b",//Device ADDR
        "c72d71040684b489b7db59eaea209e07",//APP SKEY
        "81b8f25726d9b961b0e07cab6f91eb3c",//Nwk SKEY
        "2"//Upload Download Mode
    );




    response = LoRaWAN.waitMsg(1000);
    Serial.println(response);
    
    //Set Class Mode 
    LoRaWAN.setClass("2");

    LoRaWAN.writeCMD("AT+CWORKMODE=2\r\n");

    //LoRaWAN470
    LoRaWAN.setRxWindow("505300000");

    //LoRaWAN868
    // LoRaWAN.setRxWindow("869525000");

    //LoRaWAN915
    // LoRaWAN.setRxWindow("923300000");

    // LoRaWAN470 TX Freq
    // 486.3
    // 486.5
    // 486.7
    // 486.9
    // 487.1
    // 487.3
    // 487.5
    // 487.7
    //MARK 0000 0100 0000 0000 | 0x0400
    LoRaWAN.setFreqMask("0400");


    // LoRaWAN868 TX Freq
    // 868.1 - SF7BW125 to SF12BW125
    // 868.3 - SF7BW125 to SF12BW125 and SF7BW250
    // 868.5 - SF7BW125 to SF12BW125
    // 867.1 - SF7BW125 to SF12BW125
    // 867.3 - SF7BW125 to SF12BW125
    // 867.5 - SF7BW125 to SF12BW125
    // 867.7 - SF7BW125 to SF12BW125
    // 867.9 - SF7BW125 to SF12BW125
    // 868.8 - FSK
    // LoRaWAN.setFreqMask("0001");

    // LoRaWAN915 TX Freq
    // 902.3
    // 902.5
    // 902.7
    // 902.9
    // 903.1
    // 903.3
    // 903.5
    // 903.7
    //MARK 0000 0000 0000 0001 | 0x001
    // LoRaWAN.setFreqMask("0001");
}

void loop()
{

    //send data
    LoRaWAN.sendMsg(1,15,7,"4d35535441434b");
    while(1) {
        State = kSending;
        response = LoRaWAN.waitMsg(1000);
        Serial.println(response);
        if(response.indexOf("OK") != -1) {
            break;
        }else if(response.indexOf("ERR") != -1){
            State = kError;
            break;
        }
    }
    delay(3000);
    //receive data
    response = LoRaWAN.receiveMsg();
    Serial.print(response);
    delay(3000);
}
