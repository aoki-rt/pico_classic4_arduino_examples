// Copyright 2023 RT Corporation
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

void straight(int len, int init_speed, int max_speed, int finish_speed)
{
  int obj_step;

  controlInterruptStop();
  g_max_speed = max_speed;
  g_accel = SEARCH_ACCEL;

  if (init_speed < MIN_SPEED) {
    g_speed = MIN_SPEED;

  } else {
    g_speed = init_speed;
  }
  if (finish_speed < MIN_SPEED) {
    finish_speed = MIN_SPEED;
  }
  if (init_speed < finish_speed) {
    g_min_speed = MIN_SPEED;
  } else {
    g_min_speed = finish_speed;
  }


  g_con_wall.enable = true;
  obj_step = (int)((float)len * 2.0 / PULSE);

  controlInterruptStart();

  g_motor_move = 1;
  while ((len - (0) / 2.0 * PULSE) >
         (((g_speed * g_speed) - (finish_speed * finish_speed)) / (2.0 * 1000.0 * SEARCH_ACCEL))) {
    continue;
  }
  g_accel = -1.0 * SEARCH_ACCEL;

  while ((0) < obj_step) {
    continue;
  }

  if (finish_speed == SEARCH_SPEED) {
    controlInterruptStop();
    g_max_speed = g_min_speed = g_speed = finish_speed;
    g_accel = 0.0;

    controlInterruptStart();
  } else {
    g_motor_move = 0;
  }
}

void accelerate(int len, int finish_speed)
{
  int obj_step;

  controlInterruptStop();
  g_max_speed = finish_speed;
  g_accel = SEARCH_ACCEL;
  g_speed = g_min_speed = MIN_SPEED;

  g_con_wall.enable = true;
  obj_step = (int)((float)len * 2.0 / PULSE);

  controlInterruptStart();

  g_motor_move = 1;

  while ((0) < obj_step) {
    continue;
  }

  controlInterruptStop();
  g_max_speed = g_min_speed = g_speed = finish_speed;
  g_accel = 0.0;

  controlInterruptStart();
}

void oneStep(int len, int tar_speed)
{
  int obj_step;
  controlInterruptStop();
  g_speed = g_min_speed = g_max_speed = tar_speed;
  g_accel = 0.0;

  g_con_wall.enable = true;
  obj_step = (int)((float)len * 2.0 / PULSE);

  controlInterruptStart();

  while ((0) < obj_step) {
    continue;
  }
  controlInterruptStop();
  g_max_speed = g_min_speed = g_speed = tar_speed;
  g_accel = 0.0;

  controlInterruptStart();
}

void decelerate(int len, int tar_speed)
{
  int obj_step;
  controlInterruptStop();
  g_max_speed = tar_speed;
  g_accel = 0.0;
  g_speed = g_min_speed = tar_speed;


  g_con_wall.enable = true;
  obj_step = (int)((float)len * 2.0 / PULSE);

  controlInterruptStart();

  while ((len - (0) / 2.0 * PULSE) >
         (((g_speed * g_speed) - (MIN_SPEED * MIN_SPEED)) / (2.0 * 1000.0 * SEARCH_ACCEL))) {
    continue;
  }
  g_accel = -1.0 * SEARCH_ACCEL;
  g_min_speed = MIN_SPEED;

  while ((0) < obj_step) {
    continue;
  }

  g_motor_move = 0;

  delay(300);
}

void rotate(t_direction dir, int times)
{
  int obj_step;
  controlInterruptStop();
  g_max_speed = SEARCH_SPEED;
  g_accel = TURN_ACCEL;
  g_speed = g_min_speed = MIN_SPEED;

  g_con_wall.enable = false;
  obj_step = (int)(TREAD_WIDTH * PI / 4.0 * (float)times * 2.0 / PULSE);

  switch (dir) {
    case right:

      g_motor_move = 1;
      break;
    case left:

      g_motor_move = 1;
      break;
    default:
      g_motor_move = 0;
      break;
  }
  controlInterruptStart();

  while (((obj_step - (0)) / 2.0 * PULSE) >
         (((g_speed * g_speed) - (MIN_SPEED * MIN_SPEED)) / (2.0 * 1000.0 * TURN_ACCEL))) {
    continue;
  }
  g_accel = -1.0 * TURN_ACCEL;

  while ((0) < obj_step) {
    continue;
  }

  g_motor_move = 0;
  delay(300);
}
