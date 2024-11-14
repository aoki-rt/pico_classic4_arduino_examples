#ifndef DEVICE_H_
#define DEVICE_H_


#define SW_LM 1
#define SW_CM 2
#define SW_RM 4


#define ESP32S3MINI

#ifdef ESP32S3MINI

#define LED0 35//21
#define LED1 48//47
#define LED2 47//48
#define LED3 21//35

#define BLED0 33
#define BLED1 34

#define SW_R 9
#define SW_C 6
#define SW_L 7

#define SLED_FR 15
#define SLED_FL 16
#define SLED_R 17
#define SLED_L 18

#define BUZZER 36

#define AD4 5
#define AD3 3
#define AD2 4
#define AD1 1
#define AD0 2

#define MOTOR_EN 8

#define SPI_CLK  37
#define SPI_MOSI 40
#define SPI_CS   38 //左モータ
#define SPI_CS2  10 //右モータ
#define SPI_CS3  13 //ジャイロ
#define SPI_MISO 39

#else

#define LED0 13
#define LED1 14
#define LED2 47
#define LED3 48

#define BLED0 45
#define BLED1 21

#define SW_R 18
#define SW_C 15
#define SW_L 16

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
#define SPI_CS   40 //左モータ
#define SPI_CS2   3 //右モータ
#define SPI_CS3  46 //ジャイロ
#define SPI_MISO 41
#endif


#endif  // DEVICE_H_