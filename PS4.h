/**
 *  @file   PS4.h
 *  @brief  PS4コントローラ受信用ライブラリ
 *  @author 梅沢晄平
 *  @date   23/8/17
 */
#ifndef PS4_H
#define PS4_H

#include "mbed.h"

#define TIMEOUT_COUNT 5
#define DATALEN 10

#define LEFT_X 0
#define LEFT_Y 1
#define RIGHT_X 2
#define RIGHT_Y 3

#define L2 0
#define R2 1

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define L1 4
#define R1 5
#define TRIANGLE 6
#define CROSS 7
#define SQUARE 8
#define CIRCLE 9

#define L3 10
#define R3 11
#define PS 12
#define SHARE 13
#define OPTIONS 14
#define TOUCHPAD 15

#define PITCH 0
#define ROLL 1

union btn{
    struct {
        uint8_t up : 1;
        uint8_t down : 1;
        uint8_t left : 1;
        uint8_t right : 1;
        uint8_t l1 : 1;
        uint8_t r1 : 1;
        uint8_t triangle : 1;
        uint8_t cross : 1;
        uint8_t square : 1;
        uint8_t circle : 1;
        uint8_t l3 : 1;
        uint8_t r3 : 1;
        uint8_t ps : 1;
        uint8_t share : 1;
        uint8_t options : 1;
        uint8_t touchpad : 1;
    }__attribute__((packed));
    uint16_t all;
}__attribute__((packed));

union PS4data {
    struct {
        uint8_t stick[4];
        union btn button;
        uint8_t trigger[2];
        uint8_t jyroangle[2];
    }__attribute__((packed));
    uint8_t data[10];
}__attribute__((packed));

/**
 *  @class con 
 *  @brief class for FEP
 */
class PS4 : public mbed::UnbufferedSerial {
public :
    /** constructor
     *  @param tx   FEPと接続するSerialTX pin
     *  @param rx   FEPと接続するSerialRX pin
     *  @param baud 通信速度(デフォルト115200)
     */
    PS4(PinName tx, PinName rx, int baud=115200);

    /** Start receiving
     */
    void StartReceive();
    
    /** Check timeout
     *  @brief 0.1秒毎のループで受信のタイムアウトをチェック
     */
    void TimeoutLoop();

    /** Interrupt input
     */
    void ReceiveBytes();

    /** extract the message
     */
    void CheckData();

    /** Write the received message
     *  @param data 受信メッセージを格納する配列
     */
    uint8_t getData(uint8_t *data);
    
    bool getButton(uint8_t n); //! ボタンの値を返す。
    int16_t getStick(uint8_t n); //! スティックの値を返す。中心を0,範囲は-128 ~ 128とする。
    int16_t getTrigger(uint8_t n); //! トリガーの値を返す。
    int16_t getAngle(uint8_t n); //! ジャイロの値を返す。
    uint8_t getTimeout();
    bool getStatus();
    

private :
    
    Ticker timeoutTimer;
    uint8_t buffer[256];  //! 全受信データを格納するリングバッファ配列
    uint8_t bufindex;     //! buffer の添え字変数
    uint8_t timeout;      //! 0.1秒間通信が成功していないと1増える
    uint8_t indexofR;     //! RBNの先頭文字の位置
    bool status;          //! (TIMEOUT_COUNT * 0.1)秒間通信が成り立っていない場合、false
    union PS4data ps4;    //! コントローラのデータを格納する変数
};

#endif
