/*
 * RUN.cpp
 *
 *  Created on: Apr 7, 2024
 *      Author: aokimasatake
 */

#include "RunMouse.h"
#include "TMC5240.h"

RUN g_run;

RUN::RUN() {
  // TODO Auto-generated constructor stub
  speed = 0.0;
  accel = 0.0;
}

RUN::~RUN() {
  // TODO Auto-generated destructor stub
}

//割り込み
void RunInterruptControl(void) {
  g_run.Interrupt();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RUN::Interrupt(void) {  //割り込み内からコール

  speed += accel;

  if (speed > max_speed) {
    speed = max_speed;
  }
  if (speed < min_speed) {
    speed = min_speed;
  }

}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RUN::accelerate(int len, int finish_speed) {
  int obj_step;

  max_speed = finish_speed;
  accel = 1.5;
  TMC5240Write(TMC5240_XACTUAL, 0, 0);  //初期化 
  speed = min_speed = MIN_SPEED;

  TMC5240Write(TMC5240_VMAX, (unsigned int)(speed / TMC5240_VELOCITY), (unsigned int)(speed / TMC5240_VELOCITY));
  TMC5240Write(TMC5240_VSTART, (unsigned int)(MIN_SPEED / TMC5240_VELOCITY), (unsigned int)(MIN_SPEED / TMC5240_VELOCITY));
  TMC5240Write(TMC5240_RAMPMODE, 1, 1);  //velocity mode(positive)
  obj_step = (int)((float)len * 2.0 / PULSE);


  while (1) {
    step_lr = TMC5240ReadXACTUAL();
    TMC5240Write(TMC5240_VMAX, (unsigned int)(speed / TMC5240_VELOCITY), (unsigned int)(speed / TMC5240_VELOCITY));
    if(step_lr < obj_step){
      break;
    }
  }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RUN::oneStep(int len, int init_speed) {
  int obj_step;
  max_speed = init_speed;
  accel = 0.0;
  TMC5240Write(TMC5240_XACTUAL, 0, 0);  //初期化 
  speed = min_speed = init_speed;
  TMC5240Write(TMC5240_VMAX, (unsigned int)(speed / TMC5240_VELOCITY), (unsigned int)(speed / TMC5240_VELOCITY));
  TMC5240Write(TMC5240_VSTART, (unsigned int)(MIN_SPEED / TMC5240_VELOCITY), (unsigned int)(MIN_SPEED / TMC5240_VELOCITY));
  TMC5240Write(TMC5240_RAMPMODE, 1, 1);  //velocity mode(positive)
  obj_step = (int)((float)len * 2.0 / PULSE);

  while (1) {
    step_lr = TMC5240ReadXACTUAL();
    TMC5240Write(TMC5240_VMAX, (unsigned int)(speed / TMC5240_VELOCITY), (unsigned int)(speed / TMC5240_VELOCITY));
    if(step_lr < obj_step){
      break;
    }
  }
 }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RUN::decelerate(int len, float init_speed) {
  int obj_step;
  max_speed = init_speed;
  accel = 0.0;
  TMC5240Write(TMC5240_XACTUAL, 0, 0);  //初期化 
  speed = min_speed = init_speed;
  TMC5240Write(TMC5240_VMAX, (unsigned int)(speed / TMC5240_VELOCITY), (unsigned int)(speed / TMC5240_VELOCITY));
  TMC5240Write(TMC5240_VSTART, (unsigned int)(MIN_SPEED / TMC5240_VELOCITY), (unsigned int)(MIN_SPEED / TMC5240_VELOCITY));
  TMC5240Write(TMC5240_RAMPMODE, 1, 1);  //velocity mode(positive)
  obj_step = (int)((float)len * 2.0 / PULSE);

  while (1) {
    step_lr = TMC5240ReadXACTUAL();
    if ((len - step_lr*PULSE) < (((speed * speed) - (MIN_SPEED * MIN_SPEED)) / (2.0 * 1000.0 * accel))) {
      break;
    }
  }

  accel = -1.5;

  while (1) {
    step_lr = TMC5240ReadXACTUAL();
    TMC5240Write(TMC5240_VMAX, (unsigned int)(speed / TMC5240_VELOCITY), (unsigned int)(speed / TMC5240_VELOCITY));
    if(step_lr < obj_step){
      break;
    }
  }

  TMC5240Write(TMC5240_VMAX, 0, 0);

}
