// Copyright 2024 RT Corporation
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


#ifndef SRC_RUN_H_
#define SRC_RUN_H_

#define microstep 16
#define TMC5240_PULSE (tire_diameter * PI / (200.0 * microstep))
//#define TMC5240_VELOCITY (TMC5240_PULSE*0.739)//12400000/2/2^23=0.739 -50c
#define TMC5240_VELOCITY (TMC5240_PULSE * 0.787)  //13200000/2/2^23=0.787 +50c

#define TREAD_CIRCUIT (tread_width * PI / 4)


typedef struct
{
  double control;
  double error;
  double p_error;
  double diff;
  double sum;
  double sum_max;
  double kp;
  double fkp;
  double kd;
  double ki;
  bool enable;
} t_control;

typedef enum {
  e_counter_clear,
  e_counter_notClear
} t_count_flag;

typedef enum {
  MOT_FORWARD = 1,  //TMC5240の方向に合わせた数字
  MOT_BACK = 2
} t_CW_CCW;

class RUN {
private:

public:
  volatile double accel;
  volatile double speed;
  volatile double speed_target_r;
  volatile double speed_target_l;  
  volatile double max_speed;
  volatile double min_speed;
  t_control con_wall;  

  float tire_diameter;
  float tread_width;

  RUN();
  virtual ~RUN();
  void interrupt(void);
  void speedSet(double l_speed, double r_speed);
  void dirSpeedSet(t_CW_CCW dir_left, t_CW_CCW dir_right, float l_init_speed, t_count_flag count_reset);
  void stepGet(void);
  void stop(void);
  void straight(int len, int init_speed, int max_sp, int finish_speed);  
  void accelerate(int len, int finish_speed);
  void oneStep(int len, int init_speed);
  void decelerate(int len, float init_speed);
  void rotate(t_direction dir, int times);  

private:
  int step_lr_len,step_lr;
};


extern RUN g_run;

#endif /* SRC_RUN_H_ */
