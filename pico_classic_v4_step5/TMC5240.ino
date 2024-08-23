/*
 * TMC5240.cpp
 *
 *  Created on: 2024/01/05
 *      Author: maaok
 */

//RA6T3から移植
//C++ -> Cに変更

#include "TMC5240.h"

portMUX_TYPE timerMux2 = portMUX_INITIALIZER_UNLOCKED;

unsigned int TMC5240ReadXACTUAL(void) {
  unsigned char spi_write_read_data[10];

  unsigned int data_r, data_l;

  spi_write_read_data[0] = TMC5240_XACTUAL | TMC5240_READ;
  spi_write_read_data[1] = 0xff;
  spi_write_read_data[2] = 0xff;
  spi_write_read_data[3] = 0xff;
  spi_write_read_data[4] = 0xff;

  SPI.beginTransaction(SPISettings(5000000, MSBFIRST, SPI_MODE3));
  digitalWrite(SPI_CS, LOW);
  SPI.transfer(spi_write_read_data, 5);
  digitalWrite(SPI_CS, HIGH);
  SPI.endTransaction();

  spi_write_read_data[0] = TMC5240_XACTUAL | TMC5240_READ;
  spi_write_read_data[1] = 0xff;
  spi_write_read_data[2] = 0xff;
  spi_write_read_data[3] = 0xff;
  spi_write_read_data[4] = 0xff;

  SPI.beginTransaction(SPISettings(5000000, MSBFIRST, SPI_MODE3));
  digitalWrite(SPI_CS, LOW);
  SPI.transfer(spi_write_read_data, 5);
  digitalWrite(SPI_CS, HIGH);
  SPI.endTransaction();

  data_l = ((unsigned int)spi_write_read_data[1] << 24) | ((unsigned int)spi_write_read_data[2] << 16) | ((unsigned int)spi_write_read_data[3] << 8) | ((unsigned int)spi_write_read_data[4]);

  for (int i = 0; i < 0xff; i++) {
    asm("nop \n");
  }

  spi_write_read_data[0] = TMC5240_XACTUAL | TMC5240_READ;
  spi_write_read_data[1] = 0xff;
  spi_write_read_data[2] = 0xff;
  spi_write_read_data[3] = 0xff;
  spi_write_read_data[4] = 0xff;

  SPI.beginTransaction(SPISettings(5000000, MSBFIRST, SPI_MODE3));
  digitalWrite(SPI_CS2, LOW);
  SPI.transfer(spi_write_read_data, 5);
  digitalWrite(SPI_CS2, HIGH);
  SPI.endTransaction();

  spi_write_read_data[0] = TMC5240_XACTUAL | TMC5240_READ;
  spi_write_read_data[1] = 0xff;
  spi_write_read_data[2] = 0xff;
  spi_write_read_data[3] = 0xff;
  spi_write_read_data[4] = 0xff;

  SPI.beginTransaction(SPISettings(5000000, MSBFIRST, SPI_MODE3));
  digitalWrite(SPI_CS2, LOW);
  SPI.transfer(spi_write_read_data, 5);
  digitalWrite(SPI_CS2, HIGH);
  SPI.endTransaction();

  data_r = ((unsigned int)spi_write_read_data[1] << 24) | ((unsigned int)spi_write_read_data[2] << 16) | ((unsigned int)spi_write_read_data[3] << 8) | ((unsigned int)spi_write_read_data[4]);

  return (unsigned int)(abs((int)data_r) + abs((int)data_l));
}


unsigned int TMC5240Read(unsigned char add) {
  //読み込みは距離しか使わないのでこのread関数はサンプル

  unsigned char spi_write_read_data[10];

  unsigned int data_r, data_l;

  spi_write_read_data[0] = add | TMC5240_READ;
  spi_write_read_data[1] = 0xff;
  spi_write_read_data[2] = 0xff;
  spi_write_read_data[3] = 0xff;
  spi_write_read_data[4] = 0xff;

  SPI.beginTransaction(SPISettings(5000000, MSBFIRST, SPI_MODE3));
  digitalWrite(SPI_CS, LOW);
  SPI.transfer(spi_write_read_data, 5);
  digitalWrite(SPI_CS, HIGH);
  SPI.endTransaction();

  spi_write_read_data[0] = add | TMC5240_READ;
  spi_write_read_data[1] = 0xff;
  spi_write_read_data[2] = 0xff;
  spi_write_read_data[3] = 0xff;
  spi_write_read_data[4] = 0xff;

  SPI.beginTransaction(SPISettings(5000000, MSBFIRST, SPI_MODE3));
  digitalWrite(SPI_CS, LOW);
  SPI.transfer(spi_write_read_data, 5);
  digitalWrite(SPI_CS, HIGH);
  SPI.endTransaction();

  return ((unsigned int)spi_write_read_data[1] << 24) | ((unsigned int)spi_write_read_data[2] << 16) | ((unsigned int)spi_write_read_data[3] << 8) | ((unsigned int)spi_write_read_data[4]);
}



void TMC5240Write(unsigned char add, unsigned int data_l, unsigned int data_r) {

  unsigned char spi_write_data[10];

  portENTER_CRITICAL_ISR(&timerMux2);
  spi_write_data[0] = add | TMC5240_WRITE;
  spi_write_data[1] = (unsigned char)((data_l >> 24) & 0x00ff);
  spi_write_data[2] = (unsigned char)((data_l >> 16) & 0x00ff);
  spi_write_data[3] = (unsigned char)((data_l >> 8) & 0x00ff);
  spi_write_data[4] = (unsigned char)data_l;

  SPI.beginTransaction(SPISettings(5000000, MSBFIRST, SPI_MODE3));
  digitalWrite(SPI_CS, LOW);
  SPI.transfer(spi_write_data, 5);
  digitalWrite(SPI_CS, HIGH);
  SPI.endTransaction();

  spi_write_data[0] = add | TMC5240_WRITE;
  spi_write_data[1] = (unsigned char)((data_r >> 24) & 0x00ff);
  spi_write_data[2] = (unsigned char)((data_r >> 16) & 0x00ff);
  spi_write_data[3] = (unsigned char)((data_r >> 8) & 0x00ff);
  spi_write_data[4] = (unsigned char)data_r;

  SPI.beginTransaction(SPISettings(5000000, MSBFIRST, SPI_MODE3));
  digitalWrite(SPI_CS2, LOW);
  SPI.transfer(spi_write_data, 5);
  digitalWrite(SPI_CS2, HIGH);
  SPI.endTransaction();
  portEXIT_CRITICAL_ISR(&timerMux2);
}

void TMC5240Stop(void) {
  TMC5240Write(TMC5240_VMAX, 0, 0);
}

void TMC5240Init(void) {
  SPI.begin(SPI_CLK, SPI_MISO, SPI_MOSI);
//  pinMode(SPI.pinSS(), OUTPUT);//SSを設定しないので不要
    pinMode(SPI_CS, OUTPUT);//left
    pinMode(SPI_CS2, OUTPUT);//right
    digitalWrite(SPI_CS, HIGH);
    digitalWrite(SPI_CS2, HIGH);

//  TMC5240Write(TMC5240_DRV_CONF, 0x00000001, 0x00000001);  //Current Range 2A   1:2A 0:1A
  TMC5240Write(TMC5240_DRV_CONF,   0x00000000, 0x00000000); //Current Range 1A   1:2A 0:1A

  TMC5240Write(TMC5240_IHOLD_IRUN, 0x01041F03, 0x01041F03); //IHOLDDELAY=4 IRUN=32/32 IHOLD=3/32 レンジmax 1A or 2A
//  TMC5240Write(TMC5240_IHOLD_IRUN, 0x01041803, 0x01041803);  //IHOLDDELAY=4 IRUN=24/32 IHOLD=3/32 2A設定時1.5A?

//  TMC5240Write(TMC5240_CHOPCONG,   0x00000001, 0x00000001); //MRES=0 1/256step TOFFTime=1 1以上でないとモータが動作しない
//  TMC5240Write(TMC5240_CHOPCONG,   0x03000001, 0x03000001); //MRES=0 1/32step TOFFTime=1 1以上でないとモータが動作しない
  TMC5240Write(TMC5240_CHOPCONG, 0x04000001, 0x04000001);  //MRES=0 1/16step TOFFTime=1 1以上でないとモータが動作しない
//  TMC5240Write(TMC5240_CHOPCONG,   0x06000001, 0x06000001); //MRES=0 1/4step TOFFTime=1 1以上でないとモータが動作しない
//  TMC5240Write(TMC5240_CHOPCONG,   0x07000001, 0x07000001); //MRES=0 1/2step TOFFTime=1 1以上でないとモータが動作しない

  TMC5240Write(TMC5240_RAMPMODE, 0x00000001, 0x00000001);  //velocity mode(positive)
  TMC5240Write(TMC5240_GCONF, 0x00000000, 0x00000010);     //右のみシャフトインバース
  TMC5240Write(TMC5240_AMAX, 0x3ffff, 0x3ffff);            //加速度max 17..0 18bit
  TMC5240Write(TMC5240_XACTUAL, 0, 0);                     //初期化
  TMC5240Write(TMC5240_VSTART, 0, 0);
  TMC5240Write(TMC5240_VMAX, 0, 0);
}

