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

#ifndef MAP_MANAGER_H_
#define MAP_MANAGER_H_

#define MAZESIZE_X (16)  //迷路の大きさ(MAZESIZE_X * MAZESIZE_Y)迷路
#define MAZESIZE_Y (16)  //迷路の大きさ(MAZESIZE_Y * MAZESIZE_Y)迷路

#define _UNKNOWN 2  //壁があるかないか判らない状態の場合の値
#define NOWALL 0    //壁がないばあいの値
#define WALL 1      //壁がある場合の値
#define VWALL 3     //仮想壁の値(未使用)

typedef struct
{
  unsigned char west : 2;   //西の壁情報bit7-6
  unsigned char south : 2;  //南の壁情報 bit5-4
  unsigned char east : 2;   //東の壁情報 bit3-2
  unsigned char north : 2;  //北の壁情報 bit1-0
} t_wall;                   //壁情報を格納する構造体(ビットフィールド)

typedef enum { front, right, left, rear } t_direction;

typedef enum { north, east, south, west } t_direction_glob;

typedef struct
{
  unsigned char x;
  unsigned char y;
  t_direction_glob dir;
} t_position;

class MapManager
{
public:
  MapManager();
  virtual ~MapManager();

  t_position mypos;
  short goal_mx, goal_my;

  void axisUpdate(void);
  void nextDir(t_direction dir);
  t_direction nextDirGet(char x, char y, t_direction_glob * dir);
  t_direction nextDir2Get(short x, short y, t_direction_glob * dir);
  void positionInit(void);
  void wallDataSet(unsigned char x, unsigned char y, t_direction_glob dir, char data);
  char wallDataGet(unsigned char x, unsigned char y, t_direction_glob dir);
  void wallSet(bool IS_SEN_FR, bool IS_SEN_R, bool IS_SEN_L);


  void setMyPosDir(t_direction_glob dir);
  short getMyPosX(void);
  short getMyPosY(void);
  char getGoalX(void);
  char getGoalY(void);
  void setGoalX(short data);
  void setGoalY(short data);


private:
  unsigned short steps_map[MAZESIZE_X][MAZESIZE_Y];  //歩数マップ
  t_wall wall[MAZESIZE_X][MAZESIZE_Y];               //壁の情報を格納する構造体配列

  void searchMapMake(int x, int y);
  void map2Make(int x, int y);
  int priorityGet(unsigned char x, unsigned char y, t_direction_glob dir);
};

extern MapManager g_map;

#endif  // MAP_MANAGER_H_