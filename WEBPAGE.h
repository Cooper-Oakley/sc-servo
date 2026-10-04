const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>

<head>
    <title>Servo Driver with ESP32</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="icon" href="data:,">
    <style>
        html {
        font-family: Arial;
        display: inline-block;
        background: #000000;
        color: #efefef;
        text-align: center;
    }

    h2 {
        font-size: 3.0rem;
    }

    p {
        font-size: 1.0rem;
    }

    body {
        max-width: 600px;
        margin: 0px auto;
        padding-bottom: 25px;
    }

    button {
        display: inline-block;
        margin: 5px;
        padding: 10px 10px;
        border: 0;
        line-height: 21px;
        cursor: pointer;
        color: #fff;
        background: #4247b7;
        border-radius: 5px;
        font-size: 21px;
        outline: 0;
        width: 100px

        -webkit-touch-callout: none;
        -webkit-user-select: none;
        -khtml-user-select: none;
        -moz-user-select: none;
        -ms-user-select: none;

        user-select: none;
    }

    button:hover {
        background: #ff494d
    }

    button:active {
        background: #f21c21
    }

    .control-panel {
        margin: 16px 10px;
        padding: 12px;
        border: 1px solid #555;
        border-radius: 8px;
        text-align: left;
    }

    .control-panel h4 {
        margin: 4px 0 10px;
    }

    .slider-row {
        display: flex;
        align-items: center;
        gap: 10px;
    }

    input[type="range"] {
        flex: 1;
        min-width: 0;
    }

    .servo-control[hidden] {
        display: none;
    }

    .servo-control {
        margin-top: 12px;
        padding-top: 8px;
        border-top: 1px solid #555;
    }

    .servo-control p {
        margin: 6px 0;
    }

    </style>
</head>

<body>
    <h3>SERVO DRIVER with ESP32</h3>
    <p>
    <span id="IDValue">Click this button to start searching servos.</span>
    <p>
    <label align="center"><button class="button" onclick="toggleCheckbox(9, 0, 0, 0);">Start Searching</button></label>
    <p>
    <span id="STSValue">Single servo infomation.</span>
    <p>
    <div class="control-panel">
        <button class="button" id="wakeUpButton" type="button" onclick="wakeUpServos();">Wake up</button>
        <p id="wakeUpMessage" role="status" aria-live="polite"></p>
    </div>
    <div class="control-panel">
        <h4>Global movement speed</h4>
        <div class="slider-row">
            <input id="speedSlider" type="range" min="0" max="1500" value="100" disabled>
            <span id="speedValue">100</span>
        </div>
        <p id="controlsMessage">Waiting for servo status...</p>
        <div id="servoControls">
            <div class="servo-control" id="servoControl0" hidden>
                <h4 id="servoTitle0">Servo 1</h4>
                <p id="servoPosition0">Current position: --</p>
                <p id="servoMode0">Waiting for servo state...</p>
                <div class="slider-row"><input id="servoSlider0" type="range" min="0" max="1022" value="0" disabled></div>
            </div>
            <div class="servo-control" id="servoControl1" hidden>
                <h4 id="servoTitle1">Servo 2</h4>
                <p id="servoPosition1">Current position: --</p>
                <p id="servoMode1">Waiting for servo state...</p>
                <div class="slider-row"><input id="servoSlider1" type="range" min="0" max="1022" value="0" disabled></div>
            </div>
            <div class="servo-control" id="servoControl2" hidden>
                <h4 id="servoTitle2">Servo 3</h4>
                <p id="servoPosition2">Current position: --</p>
                <p id="servoMode2">Waiting for servo state...</p>
                <div class="slider-row"><input id="servoSlider2" type="range" min="0" max="1022" value="0" disabled></div>
            </div>
            <div class="servo-control" id="servoControl3" hidden>
                <h4 id="servoTitle3">Servo 4</h4>
                <p id="servoPosition3">Current position: --</p>
                <p id="servoMode3">Waiting for servo state...</p>
                <div class="slider-row"><input id="servoSlider3" type="range" min="0" max="1022" value="0" disabled></div>
            </div>
            <div class="servo-control" id="servoControl4" hidden>
                <h4 id="servoTitle4">Servo 5</h4>
                <p id="servoPosition4">Current position: --</p>
                <p id="servoMode4">Waiting for servo state...</p>
                <div class="slider-row"><input id="servoSlider4" type="range" min="0" max="1022" value="0" disabled></div>
            </div>
        </div>
    </div>
    <p>
        <label align="center"><button class="button" onclick="toggleCheckbox(0, 1, 0, 0);">ID Select+</button></label>
        <label align="center"><button class="button" onclick="toggleCheckbox(0, -1, 0, 0);">ID Select-</button></label>
    <p>
        <label align="center"><button class="button" onclick="toggleCheckbox(1, 1, 0, 0);">Middle</button></label>
        <label align="center"><button class="button" onclick="toggleCheckbox(1, 2, 0, 0);">Stop</button></label>
        <label align="center"><button class="button" onclick="toggleCheckbox(1, 3, 0, 0);">Release</button></label>
        <label align="center"><button class="button" onclick="toggleCheckbox(1, 4, 0, 0);">Torque</button></label>
    <p>
        <label align="center"><button class="button" onmousedown="toggleCheckbox(1, 5, 0, 0);" ontouchstart="toggleCheckbox(1, 5, 0, 0);" onmouseup="toggleCheckbox(1, 2, 0, 0);" ontouchend="toggleCheckbox(1, 2, 0, 0);">Position+</button></label>
        <label align="center"><button class="button" onmousedown="toggleCheckbox(1, 6, 0, 0);" ontouchstart="toggleCheckbox(1, 6, 0, 0);" onmouseup="toggleCheckbox(1, 2, 0, 0);" ontouchend="toggleCheckbox(1, 2, 0, 0);">Position-</button></label>
    <p>
        <label align="center"><button class="button" onclick="toggleCheckbox(1, 7, 0, 0);">Speed+</button></label>
        <label align="center"><button class="button" onclick="toggleCheckbox(1, 8, 0, 0);">Speed-</button></label>
    <p>
        <label align="center"><button class="button" onclick="toggleCheckbox(1, 9, 0, 0);">ID to Set+</button></label>
        <label align="center"><button class="button" onclick="toggleCheckbox(1, 10, 0, 0);">ID to Set-</button></label>
    <p>
        <label align="center"><button class="button" onclick="setMiddle();">Set Middle Position</button></label>
        <label align="center"><button class="button" onclick="setNewID();">Set New ID</button></label>
    <p>
        <label align="center"><button class="button" onclick="setServoMode();">Set Servo Mode</button></label>
        <label align="center"><button class="button" onclick="setStepperMode();">Set Motor Mode</button></label>
    <p>
        <label align="center"><button class="button" id="serialForwarding" onclick="serialForwarding();">Start Serial Forwarding</button></label>
    <p>
        <label align="center"><button class="button" onclick="setRole(0);">Normal</button></label>
        <label align="center"><button class="button" onclick="setRole(1);">Leader</button></label>
        <label align="center"><button class="button" onclick="setRole(2);">Follower</button></label>
    <p>
        <label align="center"><button class="button" onclick="toggleCheckbox(1, 20, 0, 0);">RainbowON</button></label>
        <label align="center"><button class="button" onclick="toggleCheckbox(1, 21, 0, 0);">RainbowOFF</button></label>
    <script>
        serialForwardStatus = false;
        var servoCount = 5;
        var servoSliders = [];
        var servoTimers = [];
        var servoInteracting = [];
        var speedInteracting = false;
        var wakeUpRequestPending = false;

        function reportControlMessage(message) {
            document.getElementById("controlsMessage").textContent = message;
        }

        function sendControlRequest(url) {
            var xhr = new XMLHttpRequest();
            xhr.onreadystatechange = function() {
                if (this.readyState == 4) {
                    if (this.status == 200) {
                        reportControlMessage("Controls updated.");
                    } else {
                        reportControlMessage("Control request failed: " + this.responseText);
                    }
                }
            };
            xhr.onerror = function() {
                reportControlMessage("Unable to reach the servo controller.");
            };
            xhr.open("GET", url, true);
            xhr.send();
        }

        function wakeUpServos() {
            if (wakeUpRequestPending) {
                return;
            }
            wakeUpRequestPending = true;
            var button = document.getElementById("wakeUpButton");
            var message = document.getElementById("wakeUpMessage");
            button.disabled = true;
            message.textContent = "Sending wake-up positions to servo IDs 1-6...";

            var xhr = new XMLHttpRequest();
            xhr.onreadystatechange = function() {
                if (this.readyState != 4) {
                    return;
                }
                wakeUpRequestPending = false;
                button.disabled = false;
                var result;
                try {
                    result = JSON.parse(this.responseText);
                } catch (error) {
                    message.textContent = "Wake up failed: invalid response from controller.";
                    return;
                }

                if (this.status != 200) {
                    message.textContent = "Wake up failed: " +
                        (result.error || result.skipped || "controller returned HTTP " + this.status);
                    return;
                }
                message.textContent = "Wake-up commands sent to IDs " +
                    (result.moved.length ? result.moved.join(", ") : "none") +
                    (result.skipped ? ". Skipped IDs: " + result.skipped : ".");
            };
            xhr.onerror = function() {
                wakeUpRequestPending = false;
                button.disabled = false;
                message.textContent = "Wake up failed: unable to reach the servo controller.";
            };
            xhr.open("POST", "wakeUp", true);
            xhr.send();
        }

        function toggleCheckbox(inputT, inputI, inputA, inputB) {
            var xhr = new XMLHttpRequest();
            xhr.open("GET", "cmd?inputT="+inputT+"&inputI="+inputI+"&inputA="+inputA+"&inputB="+inputB, true);
            xhr.send();
        }

        function ctrlMode() {
            xhr.open("GET", "ctrl", true);
            xhr.send();
        }

        setInterval(function() {
          getData();
        }, 300);

        setInterval(function() {
          getServoID();
        }, 1500);

        setInterval(function() {
          getControls();
        }, 700);

        function getControls() {
            var xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function() {
                if (this.readyState == 4) {
                    if (this.status != 200) {
                        reportControlMessage("Unable to read servo status.");
                        return;
                    }
                    try {
                        updateControls(JSON.parse(this.responseText));
                    } catch (error) {
                        reportControlMessage("Invalid servo status received.");
                    }
                }
            };
            xhttp.onerror = function() {
                reportControlMessage("Unable to reach the servo controller.");
            };
            xhttp.open("GET", "readControls", true);
            xhttp.send();
        }

        function updateControls(data) {
            var speedSlider = document.getElementById("speedSlider");
            var speedValue = document.getElementById("speedValue");
            speedSlider.max = data.maxSpeed;
            speedSlider.disabled = false;
            if (!speedInteracting) {
                speedSlider.value = data.speed;
            }
            speedValue.textContent = speedSlider.value;

            var servos = data.servos || [];
            for (var i = 0; i < servoCount; i++) {
                var card = document.getElementById("servoControl" + i);
                var slider = servoSliders[i];
                if (i >= servos.length) {
                    card.hidden = true;
                    continue;
                }

                var servo = servos[i];
                card.hidden = false;
                slider.dataset.servoId = servo.id;
                document.getElementById("servoTitle" + i).textContent =
                    "Servo " + (i + 1) + " (ID: " + servo.id + ")";
                document.getElementById("servoPosition" + i).textContent =
                    servo.ready ? "Current position: " + servo.position : "Current position: unavailable";
                slider.max = data.maxPosition;
                if (!servoInteracting[i]) {
                    slider.value = servo.position;
                }

                if (!servo.ready) {
                    slider.disabled = true;
                    document.getElementById("servoMode" + i).textContent = "Reading servo state...";
                } else if (servo.mode == 0) {
                    slider.disabled = false;
                    document.getElementById("servoMode" + i).textContent = "Servo mode";
                } else if (servo.mode == 3) {
                    slider.disabled = true;
                    document.getElementById("servoMode" + i).textContent =
                        "Motor mode - position slider disabled";
                } else {
                    slider.disabled = true;
                    document.getElementById("servoMode" + i).textContent =
                        "Unsupported mode - position slider disabled";
                }
            }
        }

        function sendServoPosition(index, released) {
            var slider = servoSliders[index];
            sendControlRequest("setServo?id=" + encodeURIComponent(slider.dataset.servoId) +
                "&position=" + encodeURIComponent(slider.value));
            if (released) {
                servoInteracting[index] = false;
            }
        }

        function initPositionSlider(index) {
            var slider = document.getElementById("servoSlider" + index);
            servoSliders[index] = slider;
            servoTimers[index] = null;
            servoInteracting[index] = false;
            slider.addEventListener("input", function() {
                servoInteracting[index] = true;
                clearTimeout(servoTimers[index]);
                servoTimers[index] = setTimeout(function() {
                    sendServoPosition(index, false);
                }, 120);
            });
            slider.addEventListener("change", function() {
                clearTimeout(servoTimers[index]);
                sendServoPosition(index, true);
            });
        }

        for (var sliderIndex = 0; sliderIndex < servoCount; sliderIndex++) {
            initPositionSlider(sliderIndex);
        }

        document.getElementById("speedSlider").addEventListener("input", function() {
            speedInteracting = true;
            document.getElementById("speedValue").textContent = this.value;
        });
        document.getElementById("speedSlider").addEventListener("change", function() {
            sendControlRequest("setSpeed?value=" + encodeURIComponent(this.value));
            speedInteracting = false;
        });

        function getData() {
            var xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function() {
                if (this.readyState == 4 && this.status == 200) {
                  document.getElementById("STSValue").innerHTML =
                  this.responseText;
                }
            };
            xhttp.open("GET", "readSTS", true);
            xhttp.send();
        }

        function getServoID() {
            var xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function() {
                if (this.readyState == 4 && this.status == 200) {
                  document.getElementById("IDValue").innerHTML =
                  this.responseText;
                }
            };
            xhttp.open("GET", "readID", true);
            xhttp.send();
        }

        function setRole(modeNum){
            if(modeNum == 0){
                var r=confirm("Set the role as Normal. Dev won't send or receive data via ESP-NOW.");
                if(r==true){
                    toggleCheckbox(1, 17, 0, 0);
                }
            }
            if(modeNum == 1){
                var r=confirm("Set the role as Leader. Dev will send data via ESP-NOW.");
                if(r==true){
                    toggleCheckbox(1, 18, 0, 0);
                }
            }
            if(modeNum == 2){
                var r=confirm("Set the role as Follower. Dev will receive data via ESP-NOW.");
                if(r==true){
                    toggleCheckbox(1, 19, 0, 0);
                }
            }
        }

        function setMiddle(){
            var r=confirm("The middle position of the active servo will be set.");
            if(r==true){
                toggleCheckbox(1, 11, 0, 0);
            }
        }

        function setServoMode(){
            var r=confirm("The active servo will be set as servoMode.");
            if(r==true){
                toggleCheckbox(1, 12, 0, 0);
            }
        }

        function setStepperMode(){
            var r=confirm("The active servo will be set as motorMode.");
            if(r==true){
                toggleCheckbox(1, 13, 0, 0);
            }
        }

        function setNewID(){
            var r=confirm("A new ID of the active servo will be set.");
            if(r==true){
                toggleCheckbox(1, 16, 0, 0);
            }
        }

        function serialForwarding(){
            if(!serialForwardStatus){
                var r=confirm("Do you want to start serial forwarding?");
                if(r){
                    toggleCheckbox(1, 14, 0, 0);
                    serialForwardStatus = true;
                    document.getElementById("serialForwarding").innerHTML = "Stop Serial Forwarding";
                }
            }
            else{
                var r=confirm("Do you want to stop serial forwarding?");
                if(r){
                    toggleCheckbox(1, 15, 0, 0);
                    serialForwardStatus = false;
                    document.getElementById("serialForwarding").innerHTML = "Start Serial Forwarding";
                }
            }
        }

    </script>
</body>
</html>
)rawliteral";