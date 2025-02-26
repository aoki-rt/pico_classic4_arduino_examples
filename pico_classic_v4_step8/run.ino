// Copyright 2025 RT Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


#include "TMC5240.h"
#include "parameter.h"
#include "run.h"

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
void controlInterrupt(void) {
  g_run.interrupt();
}

void RUN::interrupt(void) {  //割り込み内からコール
  double spd_r, spd_l;

  speed += accel;

  if (speed > max_speed) {
    speed = max_speed;
  }
  if (speed < min_speed) {
    speed = min_speed;
  }

  if (con_wall.enable == true) {
    con_wall.p_error = con_wall.error;
    if ((g_sensor.sen_r.is_control == true) && (g_sensor.sen_l.is_control == true)) {
      con_wall.error = g_sensor.sen_r.error - g_sensor.sen_l.error;
    } else {
      con_wall.error = 2.0 * (g_sensor.sen_r.error - g_sensor.sen_l.error);
    }
    con_wall.diff = con_wall.error - con_wall.p_error;
    con_wall.sum += con_wall.error;
    if (con_wall.sum > con_wall.sum_max) {
      con_wall.sum = con_wall.sum_max;
    } else if (con_wall.sum < (-con_wall.sum_max)) {
      con_wall.sum = -con_wall.sum_max;
    }
    con_wall.control = 0.001 * speed * con_wall.kp * con_wall.error;
    speed_target_r = speed + con_wall.control;
    speed_target_l = speed - con_wall.control;
  } else {
    speed_target_r = speed;
    speed_target_l = speed;
  }
  if (speed_target_r < MIN_SPEED) speed_target_r = MIN_SPEED;
  if (speed_target_l < MIN_SPEED) speed_target_l = MIN_SPEED;
}


void RUN::dirSet(t_CW_CCW dir_left, t_CW_CCW dir_right) {
  g_tmc5240.write(TMC5240_RAMPMODE, dir_left, dir_right);
}

void RUN::counterClear(void) {
  g_tmc5240.write(TMC5240_XACTUAL, 0, 0);
}

void RUN::speedSet(double l_speed, double r_speed) {
  g_tmc5240.write(TMC5240_VMAX, (unsigned int)(l_speed / (pulse * 0.787)), (unsigned int)(r_speed / (pulse * 0.787)));
}

void RUN::stay(float l_speed) {
  controlInterruptStop();
  max_speed = min_speed_accelerate = min_speed_decelerate = speed = l_speed;
  accel = 0.0;
  counterClear();
  speedSet(l_speed, l_speed);
  controlInterruptStart();
}

void RUN::stepGet(void) {
  step_lr = g_tmc5240.readXactual();
  step_lr_len = (int)((float)step_lr / 2.0 * pulse);
}

void RUN::stop(void) {
  g_tmc5240.write(TMC5240_VMAX, 0, 0);
  delay(300);
}


void RUN::straight(int len, int init_speed, int max_sp, int finish_speed) {
  int obj_step;

  controlInterruptStop();
  max_speed = max_sp;
  accel = SEARCH_ACCEL;

  if (init_speed < MIN_SPEED) {
    speed = MIN_SPEED;
  } else {
    speed = init_speed;
  }
  if (finish_speed < MIN_SPEED) {
    finish_speed = MIN_SPEED;
  }
  if (init_speed < finish_speed) {
    min_speed = MIN_SPEED;
  } else {
    min_speed = finish_speed;
  }

  con_wall.enable = true;
  counterClear();
  speedSet(init_speed,init_speed);
  dirSet(MOT_FORWARD, MOT_FORWARD);
  obj_step = (int)((float)len * 2.0 / TMC5240_PULSE);
  controlInterruptStart();

  while (1) {
    stepGet();
    speedSet(speed_target_l, speed_target_r);
    if ((int)(len - step_lr_len) < (int)(((speed * speed) - (MIN_SPEED * MIN_SPEED)) / (2.0 * 1000.0 * accel))) {
      break;
    }
  }

  accel = -1.0 * SEARCH_ACCEL;
  min_speed = MIN_SPEED;

  while (1) {
    stepGet();
    speedSet(speed_target_l, speed_target_r);
    if (step_lr > obj_step) {
      break;
    }
  }

  if (finish_speed == SEARCH_SPEED) {
    stay(finish_speed);
   } else {
    stop();
  }
}

void RUN::accelerate(int len, int finish_speed) {
  int obj_step;

  controlInterruptStop();
  accel = SEARCH_ACCEL;
  speed = min_speed = MIN_SPEED;
  max_speed = finish_speed;
  con_wall.enable = true;
  dirSpeedSet(MOT_FORWARD, MOT_FORWARD, MIN_SPEED, e_counter_clear);
  obj_step = (int)((float)len * 2.0 / TMC5240_PULSE);
  controlInterruptStart();

  while (1) {
    stepGet();
    speedSet(speed_target_l, speed_target_r);
    if (step_lr > obj_step) {
      break;
    }
  }
  controlInterruptStop();
  max_speed = min_speed = speed = finish_speed;
  accel = 0.0;
  dirSpeedSet(MOT_FORWARD, MOT_FORWARD, finish_speed, e_counter_clear);
  controlInterruptStart();
}
void RUN::oneStep(int len, int init_speed) {
  int obj_step;

  controlInterruptStop();
  accel = 0.0;
  max_speed = init_speed;
  speed = min_speed = init_speed;
  con_wall.enable = true;
  dirSpeedSet(MOT_FORWARD, MOT_FORWARD, speed, e_counter_notClear);
  obj_step = (int)((float)len * 2.0 / TMC5240_PULSE);
  controlInterruptStart();

  while (1) {
    stepGet();
    speedSet(speed_target_l, speed_target_r);
    if (step_lr > obj_step) {
      break;
    }
  }
  controlInterruptStop();
  max_speed = min_speed = speed = init_speed;
  accel = 0.0;
  controlInterruptStart();
}

void RUN::decelerate(int len, float init_speed) {
  int obj_step;

  controlInterruptStop();
  accel = SEARCH_ACCEL;
  max_speed = init_speed;
  speed = min_speed = init_speed;
  con_wall.enable = true;
  dirSpeedSet(MOT_FORWARD, MOT_FORWARD, speed, e_counter_notClear);
  obj_step = (int)((float)len * 2.0 / TMC5240_PULSE);
  controlInterruptStart();

  while (1) {
    stepGet();
    speedSet(speed_target_l, speed_target_r);
    if ((int)(len - step_lr_len) < (int)(((speed * speed) - (MIN_SPEED * MIN_SPEED)) / (2.0 * 1000.0 * accel))) {
      break;
    }
  }

  accel = -1*SEARCH_ACCEL;
  min_speed = MIN_SPEED;

  while (1) {
    stepGet();
    speedSet(speed, speed);
    if (step_lr > obj_step) {
      break;
    }
  }

  stop();
}


void RUN::rotate(t_local_direction dir, int times) {
  int obj_step;

  controlInterruptStop();
  accel = TURN_ACCEL;
  max_speed = SEARCH_SPEED;
  speed = min_speed = MIN_SPEED;
  con_wall.enable = false;
  obj_step = (int)(TREAD_CIRCUIT * (float)times * 2.0 / TMC5240_PULSE);
  switch (dir) {
    case right:
      dirSpeedSet(MOT_FORWARD, MOT_BACK, min_speed, e_counter_clear);
      break;
    case left:
      dirSpeedSet(MOT_BACK, MOT_FORWARD, min_speed, e_counter_clear);
      break;
    default:
      dirSpeedSet(MOT_FORWARD, MOT_FORWARD, min_speed, e_counter_clear);
      break;
  }
  controlInterruptStart();

  while (1) {
    stepGet();
    speedSet(speed_target_l, speed_target_r);
    if ((int)((TREAD_CIRCUIT * times) - step_lr_len) < (int)(((speed * speed) - (MIN_SPEED * MIN_SPEED)) / (2.0 * 1000.0 * accel))) {
      break;
    }
  }  

  accel = -1.0 * TURN_ACCEL;

  while (1) {
    stepGet();
    speedSet(speed_target_l, speed_target_r);
    if (step_lr > obj_step) {
      break;
    }
  } 
  stop();
  delay(100);
}
