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


void flashInit(void) {
  String cmd_tmp;
  String file_tmp;
  file_tmp = "/parameters.txt";

  Serial.printf("\n\r parameter init ");
  Serial.println(file_tmp);
  cmd_tmp = "ref_sen_r " + String(REF_SEN_R_INIT) + '\n';
  writeFile(LittleFS, file_tmp, cmd_tmp);
  cmd_tmp = "ref_sen_l " + String(REF_SEN_L_INIT) + '\n';
  appendFile(LittleFS, file_tmp, cmd_tmp);
  cmd_tmp = "th_sen_r " + String(TH_SEN_R_INIT) + '\n';
  appendFile(LittleFS, file_tmp, cmd_tmp);
  cmd_tmp = "th_sen_l " + String(TH_SEN_L_INIT) + '\n';
  appendFile(LittleFS, file_tmp, cmd_tmp);
  cmd_tmp = "th_sen_fr " + String(TH_SEN_FR_INIT) + '\n';
  appendFile(LittleFS, file_tmp, cmd_tmp);
  cmd_tmp = "th_sen_fl " + String(TH_SEN_FL_INIT) + '\n';
  appendFile(LittleFS, file_tmp, cmd_tmp);
  cmd_tmp = "goal_x " + String(GOAL_X_INIT) + '\n';
  appendFile(LittleFS, file_tmp, cmd_tmp);
  cmd_tmp = "goal_y " + String(GOAL_Y_INIT) + '\n';
  appendFile(LittleFS, file_tmp, cmd_tmp);
  cmd_tmp = "tire_d " + String(TIRE_DIAMETER_INIT) + '\n';
  appendFile(LittleFS, file_tmp, cmd_tmp);
  cmd_tmp = "tread_w " + String(TREAD_WIDTH_INIT) + '\n';
  appendFile(LittleFS, file_tmp, cmd_tmp);
}

void flashBegin(void) {

  controlInterruptStop();
  sensorInterruptStop();
  delay(100);

  Serial.println("begin LittleFS");
  if (!LittleFS.begin(true)) {
    Serial.println("LittleFS Mount Failed");
    delay(100);
  }
  Serial.println("end LittleFS");

  if (switchGet() == SW_CM) {
    delay(10);
    flashInit();
  }

  controlInterruptStart();
  sensorInterruptStart();
}



void mapWrite(void) {
  String file_tmp;
  unsigned char data_temp;
  file_tmp = "/map.txt";

  controlInterruptStop();
  sensorInterruptStop();


  Serial.printf("\n\r map_data write ");
  Serial.println(file_tmp);

  File file = LittleFS.open(file_tmp, FILE_WRITE);
  if (!file) {
    Serial.println("- failed to open file for writing");
    return;
  }
  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 16; j++) {
      data_temp = g_map.wallDataGet(i, j, north) + (g_map.wallDataGet(i, j, east) << 2) + (g_map.wallDataGet(i, j, south) << 4) + (g_map.wallDataGet(i, j, west) << 6);
      if (file.write(data_temp)) {  //バイナリ書き込み
      } else {
        Serial.println("- write failed");
      }
    }
  }

  file.close();
  controlInterruptStart();
  sensorInterruptStart();
}

void mapCopy(void) {
  String file_tmp;
  unsigned char read_data;

  controlInterruptStop();
  sensorInterruptStop();


  File file = LittleFS.open("/map.txt", FILE_READ);
  if (!file || file.isDirectory()) {
    Serial.println("- failed to open file for reading");
    return;
  }
  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 16; j++) {
      if (file.available()) {
        read_data = file.read();
        g_map.wallDataSet(i, j, north, read_data & 0x03);
        g_map.wallDataSet(i, j, east, (read_data >> 2) & 0x03);
        g_map.wallDataSet(i, j, south, (read_data >> 4) & 0x03);
        g_map.wallDataSet(i, j, west, (read_data >> 6) & 0x03);
      } else {
        Serial.println("Read Error");
      }
    }
  }
  file.close();
  controlInterruptStart();
  sensorInterruptStart();
}


void ParamWrite(void) {
  String cmd_tmp;
  String file_tmp;
  char *temp_char;

  //タイマーによる割り込みは禁止にする必要がある。
  controlInterruptStop();
  sensorInterruptStop();
  delay(10);

  int temp;
  file_tmp = "/parameters.txt";

  Serial.printf("\n\r parameter write ");
  Serial.println(file_tmp);
  cmd_tmp = "ref_sen_r " + String(g_sensor.sen_r.ref) + '\n';
  writeFile(LittleFS, file_tmp, cmd_tmp);
  cmd_tmp = "ref_sen_l " + String(g_sensor.sen_l.ref) + '\n';
  appendFile(LittleFS, file_tmp, cmd_tmp);
  cmd_tmp = "th_sen_r " + String(g_sensor.sen_r.th_wall) + '\n';
  appendFile(LittleFS, file_tmp, cmd_tmp);
  cmd_tmp = "th_sen_l " + String(g_sensor.sen_l.th_wall) + '\n';
  appendFile(LittleFS, file_tmp, cmd_tmp);
  cmd_tmp = "th_sen_fr " + String(g_sensor.sen_fr.th_wall) + '\n';
  appendFile(LittleFS, file_tmp, cmd_tmp);
  cmd_tmp = "th_sen_fl " + String(g_sensor.sen_fl.th_wall) + '\n';
  appendFile(LittleFS, file_tmp, cmd_tmp);
  cmd_tmp = "goal_x " + String((short)g_map.goal_mx) + '\n';
  appendFile(LittleFS, file_tmp, cmd_tmp);
  cmd_tmp = "goal_y " + String((short)g_map.goal_my) + '\n';
  appendFile(LittleFS, file_tmp, cmd_tmp);
  cmd_tmp = "tire_d " + String(g_run.tire_diameter) + '\n';
  appendFile(LittleFS, file_tmp, cmd_tmp);
  cmd_tmp = "tread_w " + String(g_run.tread_width) + '\n';
  appendFile(LittleFS, file_tmp, cmd_tmp);

  controlInterruptStart();
  sensorInterruptStart();
}




void paramRead(void) {
  String cmd_tmp;
  String file_tmp;
  int temp;

  String cmds[2] = { "\0" };  // 分割された文字列を格納する配列

  //タイマーによる割り込みは禁止にする必要がある。
  controlInterruptStop();
  sensorInterruptStop();

  delay(10);

  File file = LittleFS.open("/parameters.txt");
  if (!file || file.isDirectory()) {
    Serial.println("- failed to open file for reading");
    flashInit();
    //    return;
  }

  Serial.println("- read from file:");
  while (file.available()) {
    int index = split(file.readStringUntil('\n'), ' ', cmds);
    if (cmds[0].equals("ref_sen_r")) {
      g_sensor.sen_r.ref = cmds[1].toInt();
    } else if (cmds[0].equals("ref_sen_l")) {
      g_sensor.sen_l.ref = cmds[1].toInt();
    } else if (cmds[0].equals("th_sen_r")) {
      g_sensor.sen_r.th_wall = cmds[1].toInt();
      g_sensor.sen_r.th_control = g_sensor.sen_r.th_wall;
    } else if (cmds[0].equals("th_sen_l")) {
      g_sensor.sen_l.th_wall = cmds[1].toInt();
      g_sensor.sen_l.th_control = g_sensor.sen_l.th_wall;
    } else if (cmds[0].equals("th_sen_fr")) {
      g_sensor.sen_fr.th_wall = cmds[1].toInt();
    } else if (cmds[0].equals("th_sen_fl")) {
      g_sensor.sen_fl.th_wall = cmds[1].toInt();
    } else if (cmds[0].equals("goal_x")) {
      g_map.goal_mx = cmds[1].toInt();
    } else if (cmds[0].equals("goal_y")) {
      g_map.goal_my = cmds[1].toInt();
    } else if (cmds[0].equals("tire_d")) {
      g_run.tire_diameter = cmds[1].toFloat();
    } else if (cmds[0].equals("tread_w")) {
      g_run.tread_width = cmds[1].toFloat();
    } else {
      Serial.print("parameter cmds Error ");
      Serial.println(cmds[0]);
      delay(10);
      flashInit();
      break;
    }
    cmds[0] = { "\0" };
    cmds[1] = { "\0" };
  }
  file.close();
  controlInterruptStart();
  sensorInterruptStart();
}

int split(String data, char delimiter, String *dst) {
  int index = 0;
  int arraySize = (sizeof(data) / sizeof((data)[0]));
  int datalength = data.length();
  for (int i = 0; i < datalength; i++) {
    char tmp = data.charAt(i);
    if (tmp == delimiter) {
      index++;
      if (index > (arraySize - 1)) return -1;
    } else dst[index] += tmp;
  }
  return (index + 1);
}


void readFile(fs::FS &fs, String path) {
  Serial.printf("Reading file: ");
  Serial.println(path);

  File file = fs.open(path);
  if (!file || file.isDirectory()) {
    Serial.println("- failed to open file for reading");
    return;
  }

  Serial.println("- read from file:");
  while (file.available()) {
    Serial.write(file.read());
  }
  file.close();
}

void writeFile(fs::FS &fs, String path, String message) {
  Serial.printf("Writing file: ");
  Serial.println(path);
  //  Serial.println(message);

  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("- failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    //    Serial.println("- file written");
  } else {
    Serial.println("- frite failed");
  }
  file.close();
}

void appendFile(fs::FS &fs, String path, String message) {
  Serial.printf("Appending to file: ");
  Serial.println(path);
  //  Serial.println(message);

  File file = fs.open(path, FILE_APPEND);
  if (!file) {
    Serial.println("- failed to open file for appending");
    return;
  }
  if (file.print(message)) {
    //    Serial.println("- message appended");
  } else {
    Serial.println("- append failed");
  }
  file.close();
}
