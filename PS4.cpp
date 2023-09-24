/**
 *  @file   PS4.cpp
 *  @brief  PS4コントローラ受信用ライブラリ
 *  @author 梅沢晄平
 *  @date   23/8/17
 */
#include "PS4.h"

#include <cstring>

PS4::PS4(PinName tx, PinName rx, int baud) :
    UnbufferedSerial(tx, rx, baud)
{
    timeout  = TIMEOUT_COUNT;
    bufindex = 0;
}

void PS4::StartReceive()
{
    attach(callback(this, &PS4::ReceiveBytes));
    timeoutTimer.attach(callback(this, &PS4::TimeoutLoop), 100ms);
}

void PS4::TimeoutLoop()
{
    if (timeout >= TIMEOUT_COUNT) {
        status = false;
    } else {
        status = true;
        timeout++;
    }
}

void PS4::ReceiveBytes()
{
    // original: buffer[bufindex] = this->getc(); // Receive 1byte
    this->read(buffer + bufindex, 1);
    this->timeout = 0;

    if ( (!strncmp((char*)(buffer + ((256 + bufindex - 1)%256) ), "\r\n", 2)) ) { // <CR><LF> bufindex = <LF>(='\n')
        CheckData();
    }
    bufindex++;
}

void PS4::CheckData()
{
    indexofR=0;
    for (uint16_t i_Rbn=0; i_Rbn<256; i_Rbn++) {
        indexofR = (256 + bufindex - i_Rbn) % 256;
        if ( !strncmp((char*)(buffer + indexofR) , "RBN", 3) ) { // check header  indexofR is address of 'R'BN
            for (int i_ps4=0; i_ps4<DATALEN; i_ps4++) {
                ps4.data[i_ps4] = buffer[(indexofR+9+i_ps4)%256];
            }
      
            return;
        }
    }
}

bool PS4::getButton(uint8_t n)
{
    switch(n){
        case UP :       return ps4.button.up;
        case DOWN :     return ps4.button.down;
        case LEFT :     return ps4.button.left;
        case RIGHT :    return ps4.button.right;
        case L1 :       return ps4.button.l1;
        case R1 :       return ps4.button.r1;
        case TRIANGLE : return ps4.button.triangle;
        case CROSS :    return ps4.button.cross;
        case SQUARE :   return ps4.button.square;
        case CIRCLE :   return ps4.button.circle;
        case L3 :       return ps4.button.l3;
        case R3 :       return ps4.button.r3;
        case PS :       return ps4.button.ps;
        case SHARE :    return ps4.button.share;
        case OPTIONS :  return ps4.button.options;
        case TOUCHPAD : return ps4.button.touchpad;
        default :       return 0;
    }
}

int16_t PS4::getStick(uint8_t n)
{
    if (n%2) return 128-ps4.stick[n];
    else return ps4.stick[n]-128;
}

int16_t PS4::getTrigger(uint8_t n)
{
    return ps4.trigger[n];
}
/*
int16_t PS4::getAngle(uint8_t n)
{
    return ps4.jyroangle[n];
}
*/

uint8_t PS4::getState()
{
    return ps4.state;
}

bool PS4::getStatus()
{
    return status;
}
