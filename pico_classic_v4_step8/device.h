#ifndef DEVICE_H_
#define DEVICE_H_

#define SW_LM 1
#define SW_CM 2
#define SW_RM 4

//Pi:co4
#define LED0 13
#define LED1 14
#define LED2 47
#define LED3 48

#define BLED0 45
#define BLED1 21

#define SW_L 16
#define SW_C 15
#define SW_R 18

#define SLED_FR 9
#define SLED_FL 10
#define SLED_R 11
#define SLED_L 12

#define BUZZER 38

#define AD4 7
#define AD3 6
#define AD2 5
#define AD1 4
#define AD0 8

#define MOTOR_EN 17

#define SPI_CLK  39
#define SPI_MOSI 42
#define SPI_CS_L   40 //左モータ
#define SPI_CS_R   3 //右モータ
#define SPI_CS_J  46 //ジャイロ
#define SPI_MISO 41

#endif  // DEVICE_H_