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

    .speed-slider-row {
        display: flex;
        align-items: center;
        gap: 10px;
    }

    .speed-slider-row input[type="range"] {
        flex: 1;
        min-width: 0;
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
        <button class="button" id="wakeUpButton" type="button" onclick="sendPresetPositions('wakeUp', 'Wake up');">Wake up</button>
        <p id="wakeUpMessage" role="status" aria-live="polite"></p>
        <button class="button" id="sleepButton" type="button" onclick="sendPresetPositions('sleep', 'Sleep');">sleep</button>
        <p id="sleepMessage" role="status" aria-live="polite"></p>
    </div>
    <div class="control-panel">
        <h4>Global movement speed</h4>
        <div class="speed-slider-row">
            <input id="speedSlider" type="range" min="0" max="1500" value="100" disabled>
            <span id="speedValue">100</span>
        </div>
        <p id="speedMessage" role="status" aria-live="polite">Reading movement speed...</p>
    </div>
    <div class="control-panel">
        <h4>Servo ID 1 position</h4>
        <div class="speed-slider-row">
            <input id="servo1Slider" type="range" min="0" max="1022" value="0" disabled>
            <span id="servo1Value">--</span>
        </div>
        <p id="servo1GateMessage" role="status" aria-live="polite">Reading servo positions...</p>
        <p id="servo1Message" role="status" aria-live="polite"></p>
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
        var presetRequestPending = false;
        var speedInteracting = false;
        var speedRequestPending = false;
        var servo1Interacting = false;
        var servo1RequestPending = false;

        setInterval(function() {
            getSpeed();
        }, 1000);

        setInterval(function() {
            getServo1Control();
        }, 700);

        function getServo1Control() {
            var xhr = new XMLHttpRequest();
            xhr.onreadystatechange = function() {
                if (this.readyState != 4) {
                    return;
                }
                var slider = document.getElementById("servo1Slider");
                var gateMessage = document.getElementById("servo1GateMessage");
                if (this.status != 200) {
                    slider.disabled = true;
                    gateMessage.textContent =
                        "Unable to read servo positions (HTTP " + this.status + ").";
                    return;
                }
                try {
                    var state = JSON.parse(this.responseText);
                    slider.max = state.maxPosition;
                    if (!servo1Interacting && !servo1RequestPending && state.servo1Ready) {
                        slider.value = state.servo1Position;
                        document.getElementById("servo1Value").textContent = state.servo1Position;
                    }
                    if (!state.servo1Ready || !state.servo2Ready) {
                        slider.disabled = true;
                        gateMessage.textContent = "Position control disabled: servo feedback unavailable.";
                    } else if (state.servo1Mode != 0) {
                        slider.disabled = true;
                        gateMessage.textContent = "Position control disabled: servo 1 is not in servo mode.";
                    } else if (state.servo2Position <= 70) {
                        slider.disabled = true;
                        gateMessage.textContent = "Position control requires servo 2 position above 70 (current: " +
                            state.servo2Position + ").";
                    } else {
                        slider.disabled = servo1RequestPending;
                        gateMessage.textContent = "Servo 2 position: " + state.servo2Position +
                            " - movement enabled.";
                    }
                } catch (error) {
                    slider.disabled = true;
                    gateMessage.textContent = "Invalid servo position response from controller.";
                }
            };
            xhr.onerror = function() {
                document.getElementById("servo1Slider").disabled = true;
                document.getElementById("servo1GateMessage").textContent =
                    "Unable to reach the servo controller.";
            };
            xhr.open("GET", "readServo1Control", true);
            xhr.send();
        }

        function setServo1Position() {
            if (servo1RequestPending) {
                return;
            }
            var slider = document.getElementById("servo1Slider");
            var message = document.getElementById("servo1Message");
            servo1RequestPending = true;
            slider.disabled = true;
            message.textContent = "Checking servo 2 and moving servo 1...";

            var xhr = new XMLHttpRequest();
            xhr.onreadystatechange = function() {
                if (this.readyState != 4) {
                    return;
                }
                servo1RequestPending = false;
                servo1Interacting = false;
                message.textContent = this.status == 200
                    ? "Servo 1 position command sent."
                    : "Servo 1 movement rejected: " + this.responseText;
                getServo1Control();
            };
            xhr.onerror = function() {
                servo1RequestPending = false;
                servo1Interacting = false;
                message.textContent = "Unable to reach the servo controller.";
                getServo1Control();
            };
            xhr.open("POST", "setServo1Position", true);
            xhr.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
            xhr.send("position=" + encodeURIComponent(slider.value));
        }

        document.getElementById("servo1Slider").addEventListener("input", function() {
            servo1Interacting = true;
            document.getElementById("servo1Value").textContent = this.value;
        });
        document.getElementById("servo1Slider").addEventListener("change", setServo1Position);

        function getSpeed() {
            var xhr = new XMLHttpRequest();
            xhr.onreadystatechange = function() {
                if (this.readyState != 4) {
                    return;
                }
                if (this.status != 200) {
                    document.getElementById("speedMessage").textContent =
                        "Unable to read movement speed (HTTP " + this.status + ").";
                    return;
                }
                try {
                    var speed = JSON.parse(this.responseText);
                    var slider = document.getElementById("speedSlider");
                    slider.max = speed.maxSpeed;
                    slider.disabled = speedRequestPending;
                    if (!speedInteracting && !speedRequestPending) {
                        slider.value = speed.speed;
                        document.getElementById("speedValue").textContent = speed.speed;
                    }
                    document.getElementById("speedMessage").textContent =
                        "Speed range: 0-" + speed.maxSpeed;
                } catch (error) {
                    document.getElementById("speedMessage").textContent =
                        "Invalid movement speed response from controller.";
                }
            };
            xhr.onerror = function() {
                document.getElementById("speedMessage").textContent =
                    "Unable to reach the servo controller.";
            };
            xhr.open("GET", "readSpeed", true);
            xhr.send();
        }

        function setSpeed() {
            if (speedRequestPending) {
                return;
            }
            var slider = document.getElementById("speedSlider");
            var message = document.getElementById("speedMessage");
            speedRequestPending = true;
            slider.disabled = true;
            message.textContent = "Updating movement speed...";

            var xhr = new XMLHttpRequest();
            xhr.onreadystatechange = function() {
                if (this.readyState != 4) {
                    return;
                }
                speedRequestPending = false;
                speedInteracting = false;
                slider.disabled = false;
                if (this.status != 200) {
                    message.textContent = "Unable to update movement speed: " + this.responseText;
                    getSpeed();
                    return;
                }
                slider.value = this.responseText;
                document.getElementById("speedValue").textContent = this.responseText;
                message.textContent = "Movement speed updated.";
            };
            xhr.onerror = function() {
                speedRequestPending = false;
                speedInteracting = false;
                slider.disabled = false;
                message.textContent = "Unable to reach the servo controller.";
            };
            xhr.open("POST", "setSpeed?value=" + encodeURIComponent(slider.value), true);
            xhr.send();
        }

        document.getElementById("speedSlider").addEventListener("input", function() {
            speedInteracting = true;
            document.getElementById("speedValue").textContent = this.value;
        });
        document.getElementById("speedSlider").addEventListener("change", setSpeed);

        function sendPresetPositions(endpoint, label) {
            if (presetRequestPending) {
                return;
            }
            presetRequestPending = true;
            var wakeUpButton = document.getElementById("wakeUpButton");
            var sleepButton = document.getElementById("sleepButton");
            var message = document.getElementById(endpoint + "Message");
            wakeUpButton.disabled = true;
            sleepButton.disabled = true;
            message.textContent = "Sending " + label.toLowerCase() + " positions to servo IDs 1-6...";

            var xhr = new XMLHttpRequest();
            xhr.onreadystatechange = function() {
                if (this.readyState != 4) {
                    return;
                }
                presetRequestPending = false;
                wakeUpButton.disabled = false;
                sleepButton.disabled = false;
                var result;
                try {
                    result = JSON.parse(this.responseText);
                } catch (error) {
                    message.textContent = label + " failed: invalid response from controller.";
                    return;
                }

                if (this.status != 200) {
                    message.textContent = label + " failed: " +
                        (result.error || result.skipped || "controller returned HTTP " + this.status);
                    return;
                }
                message.textContent = label + " commands sent to IDs " +
                    (result.moved.length ? result.moved.join(", ") : "none") +
                    (result.skipped ? ". Skipped IDs: " + result.skipped : ".");
            };
            xhr.onerror = function() {
                presetRequestPending = false;
                wakeUpButton.disabled = false;
                sleepButton.disabled = false;
                message.textContent = label + " failed: unable to reach the servo controller.";
            };
            xhr.open("POST", endpoint, true);
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