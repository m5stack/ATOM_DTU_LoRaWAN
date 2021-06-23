#include "ATOM_DTU_LoRaWAN.h"


void ATOM_DTU_LoRaWAN::Init(HardwareSerial *serial, uint8_t RX, uint8_t TX) {
    _serial = serial;
    _serial->begin(115200, SERIAL_8N1, RX, TX);
    _serial->flush();
}

String ATOM_DTU_LoRaWAN::waitMsg(int time) {
    String restr;
    delay(time);
    while(1){
        if(Serial2.available()) {
            String str = Serial2.readString();
                restr += str;
        }else{
            break;
        }
    }
    return restr;
}


void ATOM_DTU_LoRaWAN::writeCMD(String command) {
    _serial->print(command);
    delay(100);
}

void ATOM_DTU_LoRaWAN::sendMsg(uint8_t confirm, uint8_t nbtrials, size_t length, String data) {
    String cmd = "AT+DTRX="+String(confirm)+','+String(nbtrials)+','+String(length)+','+data+"\r\n";
    writeCMD(cmd);
}

String ATOM_DTU_LoRaWAN::receiveMsg() {
    writeCMD("AT+DRX?\r\n");
    String data = waitMsg(500);
    return data;
}


void ATOM_DTU_LoRaWAN::configOTTA(String join_mode, String device_eui, String app_eui, String app_key, String ul_dl_mode) {
    writeCMD("AT+CJOINMODE="+join_mode+"\r\n");
    writeCMD("AT+CDEVEUI="+device_eui+"\r\n");
    writeCMD("AT+CAPPEUI="+app_eui+"\r\n");
    writeCMD("AT+CAPPKEY="+app_key+"\r\n");
    writeCMD("AT+CULDLMODE="+ul_dl_mode+"\r\n");
}


void ATOM_DTU_LoRaWAN::setClass(String mode) {
    writeCMD("AT+CCLASS="+mode+"\r\n");
}

void ATOM_DTU_LoRaWAN::setRxWindow(String freq) {
    writeCMD("AT+CRXP=0,0,"+freq+"\r\n");
}

void ATOM_DTU_LoRaWAN::setFreqMask(String mask) {
    writeCMD("AT+CFREQBANDMASK="+mask+"\r\n");
}

void ATOM_DTU_LoRaWAN::startJoin() {
    writeCMD("AT+CJOIN=1,0,10,8\r\n");
}



