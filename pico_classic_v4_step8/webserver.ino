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


AsyncWebServer server(80);

const char* ssid = "PICO4";
const char* password = "12345678";

void WebServerSetup(void) {

  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.println(myIP);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    String html = "";
    html += "<!DOCTYPE html>";
    html += "<html lang=\"en\">";
    html += "<head>";
    html += "<meta charset=\"UTF-8\">";
    html += "<title>Pi:Co Classic4_WebServer</title>";
    html += "<style>";
    html += "  html {";
    html += "    font-family: Helvetica;";
    html += "    display: inline-block;";
    html += "    margin: 0px auto;";
    html += "    text-align: center;";
    html += "  }";
    html += "  h1 {";
    html += "    font-size: 3.0rem;";
    html += "    color: blue;";
    html += "  }";
    html += "  </style>";
    html += "</head>";
    html += "<body>";
    html += "<h1>Pi:Co Classic4_Parameter</h1>";
    html += "<form action=\"/get\">";

    html += "<p><h2>battery Value</h2></p>";
    html += String(g_sensor.battery_value)+"mV";
    html += "<br>";
    html += "<br>";

    html += "<p><h2>Sensor Value</h2></p>";
    html += "<table align=\"center\">";
    html += "<tr><th></th><th>Left</th><th>Right</th></tr>";
    html += "<tr><th>SIDE VALUE</th><td><input name=\"ref_left\" id=\"left_value\" type=\"text\" size=\"10\" value=";
    html += String(g_sensor.sen_l.value);
    html += "></td><td><input name=\"ref_right\" id=\"right_value\" type=\"text\" size=\"10\" value=";
    html += String(g_sensor.sen_r.value);
    html += "></td></tr>";
    html += "<tr><th>FRONT VALUE</th><td><input name=\"th_left\" id=\"left_front_value\" type=\"text\" size=\"10\" value=";
    html += String(g_sensor.sen_fl.value);
    html += "></td><td><input name=\"th_right\" id=\"right_front_value\" type=\"text\" size=\"10\" value=";
    html += String(g_sensor.sen_fr.value);
    html += "></td></tr>";
    html += "</table>";
    html += "<br>";
    html += "<br>";

    html += "<p><h2>Tire Paramter</h2></p>";
    html += "<table align=\"center\">";
    html += "<tr><th>TIRE_DIAMETER</th><td><input name=\"tire_dia\" type=\"text\" size=\"10\" value=";
    html += String(g_run.tire_diameter)+"mm";
    html += "></td></tr>";
    html += "<tr><th>TREAD_WIDTH</th><td><input name=\"tread_width\" type=\"text\" size=\"10\" value=";
    html += String(g_run.tread_width)+"mm";
    html += "></td></tr>";
    html += "</table>";
    html += "<br>";
    html += "<br>";

    html += "<p><h2>GOAL Paramter</h2></p>";
    html += "<table align=\"center\">";
    html += "<tr><th></th><th>X</th><th>Y</th></tr>";
    html += "<tr><th>AXIS</th><td><input name=\"goal_x\" type=\"text\" size=\"10\" value=";
    html += String(g_map.goal_mx);
    html += "></td><td><input name=\"goal_y\" type=\"text\" size=\"10\" value=";
    html += String(g_map.goal_my);
    html += "></td></tr>";
    html += "</table>";
    html += "<br>";
    html += "<br>";

    html += "<p><h2>Sensor Paramter</h2></p>";
    html += "<table align=\"center\">";
    html += "<tr><th></th><th>Left</th><th>Right</th></tr>";
    html += "<tr><th>SIDE REF</th><th><input name=\"ref_left\" type=\"text\" size=\"10\" value=";
    html += String(g_sensor.sen_l.ref);
    html += "></th><th><input name=\"ref_right\" type=\"text\" size=\"10\" value=";
    html += String(g_sensor.sen_r.ref);
    html += "></th></tr>";
    html += "<tr><th>SIDE Threshold</th><td><input name=\"th_left\" type=\"text\" size=\"10\" value=";
    html += String(g_sensor.sen_l.th_wall);
    html += "></td><td><input name=\"th_right\" type=\"text\" size=\"10\" value=";
    html += String(g_sensor.sen_r.th_wall);
    html += "></td></tr>";
    html += "<tr><th>FRONT Threshold</th><th><input name=\"th_fl\" type=\"text\"size=\"10\"  value=";
    html += String(g_sensor.sen_fl.th_wall);
    html += "></th><th><input name=\"th_fr\" type=\"text\" size=\"10\" value=";
    html += String(g_sensor.sen_fr.th_wall);
    html += "></th></tr>";
    html += "</table>";
    html += "<br>";
    html += "<br>";
    html += "<input type=\"submit\" value=\"Save\">";
    html += "</form><br>";
    html += "</body>";
    html += "</html>";
    request->send(200, "text/html", html);
  });

  server.on("/get", HTTP_GET, [](AsyncWebServerRequest* request) {
    String inputMessage;
    inputMessage = request->getParam("ref_left")->value();
    g_sensor.sen_l.ref = inputMessage.toInt();
    inputMessage = request->getParam("ref_right")->value();
    g_sensor.sen_r.ref = inputMessage.toInt();

    inputMessage = request->getParam("th_left")->value();
    g_sensor.sen_l.th_wall = inputMessage.toInt();
    inputMessage = request->getParam("th_right")->value();
    g_sensor.sen_r.th_wall = inputMessage.toInt();

    inputMessage = request->getParam("th_fl")->value();
    g_sensor.sen_fl.th_wall = inputMessage.toInt();
    inputMessage = request->getParam("th_fr")->value();
    g_sensor.sen_fr.th_wall = inputMessage.toInt();

    inputMessage = request->getParam("goal_x")->value();
    g_map.goal_mx = inputMessage.toInt();
    inputMessage = request->getParam("goal_y")->value();
    g_map.goal_my = inputMessage.toInt();

    request->redirect("/");
  });


  // ESP32_WebServer start
  server.begin();  //APモードを起動
  Serial.println("ESP32_WebServer start!");
}