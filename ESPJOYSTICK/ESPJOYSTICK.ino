#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Wire.h> 

const char* ssid = "WirelessJoystick";
const char* password = "WirelessJoystick";

ESP8266WebServer server(80);

const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Switch Pro Controller</title>
    <style>
        body {
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
            margin: 0;
            background-color: #f0f0f0;
            -webkit-user-select: none;
            -moz-user-select: none;   
            -ms-user-select: none;  
            user-select: none;    
        }
        .gamepad {
            display: flex;
            gap: 1000px;
        }
        .left-column, .right-column {
            display: flex;
            flex-direction: column;
            align-items: center;
            gap: 10px;
        }
        .trigger, .bumper, .minus, .plus, .square, .home {
            width: 300px;
            height: 100px;
            background-color: #666;
            color: white;
            display: flex;
            justify-content: center;
            align-items: center;
            border-radius: 10px;
            font-size: 18px;
            cursor: pointer;
        }
        #minus { transform: translate(500%, -70%); }
        #square { transform: translate(500%, 200%); }
        #plus { transform: translate(-500%, -70%); }
        #home { transform: translate(-500%, 200%); }
        .minus, .plus, .square, .home {
            background-color: #888;
            height: 80px;
            width: 80px;
            font-size: 24px;
        }
        .square, .home { background-color: #777; }
        .joystick {
            width: 300px;
            height: 300px;
            background-color: #333;
            border-radius: 50%;
            position: relative;
            display: flex;
            justify-content: center;
            align-items: center;
            cursor: pointer;
        }
        #joystick2 { transform: translateX(-50%); }
        .stick {
            width: 100px;
            height: 100px;
            background-color: #666;
            border-radius: 50%;
            position: absolute;
        }
        .dpad {
            display: flex;
            flex-wrap: wrap;
            width: 300px;
            height: 300px;
            position: relative;
            transform: translateX(50%);
        }
        .dpad-button {
            width: 100px;
            height: 100px;
            background-color: #444;
            color: white;
            display: flex;
            justify-content: center;
            align-items: center;
            border-radius: 10px;
            font-size: 24px;
            position: absolute;
            cursor: pointer;
        }
        #dpad-up { top: 0; left: 50%; transform: translateX(-50%); }
        #dpad-right { top: 50%; right: 0; transform: translateY(-50%); }
        #dpad-down { bottom: 0; left: 50%; transform: translateX(-50%); }
        #dpad-left { top: 50%; left: 0; transform: translateY(-50%); }
        .buttons {
            display: flex;
            flex-wrap: wrap;
            width: 300px;
            height: 300px;
            position: relative;
        }
        .button {
            width: 100px;
            height: 100px;
            background-color: #444;
            color: white;
            display: flex;
            justify-content: center;
            align-items: center;
            border-radius: 10px;
            font-size: 24px;
            position: absolute;
            cursor: pointer;
        }
        #button-y { top: 0; left: 50%; transform: translateX(-50%); }
        #button-a { bottom: 0; left: 50%; transform: translateX(-50%); }
        #button-b { right: 0; top: 50%; transform: translateY(-50%); }
        #button-x { left: 0; top: 50%; transform: translateY(-50%); }
    </style>
</head>
<body>
    <div class="gamepad">
        <div class="left-column">
            <div class="trigger" id="trigger-left">L2</div>
            <div class="bumper" id="bumper-left">L</div>
            <div class="minus" id="minus">-</div>
            <div class="square" id="square">◻</div>
            <div class="joystick" id="joystick1">
                <div class="stick" id="stick1"></div>
            </div>
            <div class="dpad">
                <div class="dpad-button" id="dpad-up">↑</div>
                <div class="dpad-button" id="dpad-right">→</div>
                <div class="dpad-button" id="dpad-down">↓</div>
                <div class="dpad-button" id="dpad-left">←</div>
            </div>
        </div>
        <div class="right-column">
            <div class="trigger" id="trigger-right">R2</div>
            <div class="bumper" id="bumper-right">R</div>
            <div class="home" id="home">Home</div>
            <div class="plus" id="plus">+</div>
            <div class="buttons">
                <div class="button" id="button-y">Y</div>
                <div class="button" id="button-a">A</div>
                <div class="button" id="button-b">B</div>
                <div class="button" id="button-x">X</div>
            </div>
            <div class="joystick" id="joystick2">
                <div class="stick" id="stick2"></div>
            </div>
        </div>
    </div>
    <script>
        document.addEventListener('DOMContentLoaded', () => {
            const joystick1 = document.getElementById('joystick1');
            const stick1 = document.getElementById('stick1');
            const joystick2 = document.getElementById('joystick2');
            const stick2 = document.getElementById('stick2');

            let isDragging = false;
            let currentStick = null;

            function updateStickPosition(e, joystick, stick) {
              const rect = joystick.getBoundingClientRect();
              const joystickCenterX = rect.left + rect.width / 2;
              const joystickCenterY = rect.top + rect.height / 2;
              const maxDistance = rect.width / 2;

              const clientX = e.clientX || (e.touches && e.touches[0].clientX);
              const clientY = e.clientY || (e.touches && e.touches[0].clientY);

              const x = clientX - joystickCenterX;
              const y = clientY - joystickCenterY;

              const distance = Math.min(Math.sqrt(x * x + y * y), maxDistance);
              const angle = Math.atan2(y, x);

              const stickX = distance * Math.cos(angle);
              const stickY = distance * Math.sin(angle);

              stick.style.transform = translate(${stickX}px, ${stickY}px);

              const mappedX = Math.round((stickX / 150) * 1024);
              const mappedY = Math.round((-stickY / 150) * 1024);

              const formattedX = (mappedX >= 0 ? '+' : '-') + Math.abs(mappedX).toString().padStart(4, '0');
              const formattedY = (mappedY >= 0 ? '+' : '-') + Math.abs(mappedY).toString().padStart(4, '0');

              const joystickId = joystick.id === 'joystick1' ? 'LJ' : 'RJ';
              fetch(/joystick?id=${joystickId}&x=${formattedX}&y=${formattedY});
            }

            function resetStick(stick, joystickId) {
                stick.style.transform = translate(0, 0);
                fetch(/joystick?id=${joystickId}&x=+0000&y=+0000);
            }

            function onMove(e) {
                if (isDragging && currentStick) {
                    updateStickPosition(e, currentStick.joystick, currentStick.stick);
                }
            }

            function onStart(e) {
                if (e.target.classList.contains('stick')) {
                    isDragging = true;
                    currentStick = {
                        joystick: e.target.parentElement,
                        stick: e.target,
                        joystickId: e.target.parentElement.id === 'joystick1' ? 'LJ' : 'RJ'
                    };
                    updateStickPosition(e, currentStick.joystick, currentStick.stick);
                }
            }

            function onEnd() {
                if (isDragging && currentStick) {
                    resetStick(currentStick.stick, currentStick.joystickId);
                    currentStick = null;
                }
                isDragging = false;
            }

            document.addEventListener('mousemove', onMove);
            document.addEventListener('mousedown', onStart);
            document.addEventListener('mouseup', onEnd);

            document.addEventListener('touchmove', onMove, { passive: false });
            document.addEventListener('touchstart', onStart, { passive: false });
            document.addEventListener('touchend', onEnd);

            const buttons = document.querySelectorAll('.button, .trigger, .bumper, .dpad-button, #minus, #plus, #square, #home');
            
            const buttonMappings = {
                'trigger-left': 'LT',
                'trigger-right': 'RT',
                'bumper-left': 'LB',
                'bumper-right': 'RB',
                'minus': 'MB',
                'plus': 'PB',
                'square': 'SB',
                'home': 'HB',
                'dpad-up': 'DU',
                'dpad-down': 'DD',
                'dpad-left': 'DL',
                'dpad-right': 'DR',
                'button-a': 'AB',
                'button-b': 'BB',
                'button-x': 'XB',
                'button-y': 'YB'
            };

            buttons.forEach(button => {
                button.addEventListener('mousedown', () => {
                    const code = buttonMappings[button.id] || button.id;
                    fetch(/button?id=${code}&state=1);
                });
                button.addEventListener('touchstart', () => {
                    const code = buttonMappings[button.id] || button.id;
                    fetch(/button?id=${code}&state=1);
                });

                button.addEventListener('mouseup', () => {
                    const code = buttonMappings[button.id] || button.id;
                    fetch(/button?id=${code}&state=0);
                });
                button.addEventListener('touchend', () => {
                    const code = buttonMappings[button.id] || button.id;
                    fetch(/button?id=${code}&state=0);
                });
            });
        });
    </script>
</body>
</html>
)=====";

void setup() {
  Serial.begin(115200);

  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  server.on("/", HTTP_GET, handleRoot);
  server.on("/joystick", HTTP_GET, handleJoystick);
  server.on("/button", HTTP_GET, handleButton);

  server.begin();
  Serial.println("HTTP server started");

  Wire.begin();
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  server.send(200, "text/html", MAIN_page);
}

void handleJoystick() {
  String id = server.arg("id");
  String x = server.arg("x");
  String y = server.arg("y");
  
  Serial.print(id);
  Serial.print(" ");
  Serial.print(x);
  Serial.print(",");
  Serial.println(y);
  
  Wire.beginTransmission(9); 
  Wire.write(id.c_str());   
  Wire.write(x.c_str());     
  Wire.write(y.c_str());     
  Wire.endTransmission();   
  
  server.send(200, "text/plain", "OK");
}

void handleButton() {
  String id = server.arg("id");
  String state = server.arg("state");
  
  Serial.print(id);
  Serial.print(" ");
  Serial.println(state);
  
  Wire.beginTransmission(9); 
  Wire.write(id.c_str());
  Wire.write(state.c_str()); 
  Wire.endTransmission();  
  
  server.send(200, "text/plain", "OK");
}