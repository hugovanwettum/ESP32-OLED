#include <Arduino.h>
#include <WiFi.h>

#include "settings.h"
#include "WiFiConfig.h" // not synched to remote, only stored locally as it contains my wifi network SSID and password

extern uint16_t sharedVariable_paddle_position;

// Set web server port number to 80
WiFiServer server(80);

extern SemaphoreHandle_t paddle_position_variable_semaphore;

void serverTask(void *parameter)
{
    // Connect to Wi-Fi
    WiFi.begin(ssid, password); // Stored in WiFiConfig.h
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi!");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    // Start server
    server.begin();

    for (;;)
    {
        // Check for client connections
        WiFiClient client = server.available();
        if (client)
        {
            Serial.println("Client connected!");
            // Wait for data from the client
            while (client.connected())
            {
                if (client.available())
                {
                    // Read the first line of the request
                    String request = client.readStringUntil('\r');
                    // If the request is for the root path ("/"), serve the webpage
                    if (request.indexOf("GET / ") != -1)
                    {
                        // Send HTTP headers
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-Type: text/html");
                        client.println();

                        // Send the webpage
                        client.println("<!DOCTYPE html>");
                        client.println("<html lang=\"en\">");
                        client.println("<head>");
                        client.println("    <meta charset=\"UTF-8\">");
                        client.println("    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">");
                        client.println("    <title>Pong Paddle Position Control</title>");
                        client.println("    <style>");
                        client.println("        body {");
                        client.println("            display: flex;");
                        client.println("            flex-direction: column;");
                        client.println("            align-items: center;");
                        client.println("            justify-content: flex-end;");
                        client.println("            height: 100vh;");
                        client.println("            margin: 0;");
                        client.println("        }");
                        client.println("");
                        client.println("        h1, p, .slidecontainer {");
                        client.println("            text-align: center;");
                        client.println("            width: 80%;");
                        client.println("            margin-bottom: 20px;");
                        client.println("        }");
                        client.println("");
                        client.println("        .slidecontainer {");
                        client.println("            margin-bottom: 5%;");
                        client.println("        }");
                        client.println("");
                        client.println("        .slider {");
                        client.println("            -webkit-appearance: none;");
                        client.println("            width: 100%;");
                        client.println("            height: 25px;");
                        client.println("            background: #d3d3d3;");
                        client.println("            outline: none;");
                        client.println("            opacity: 0.7;");
                        client.println("            -webkit-transition: .2s;");
                        client.println("            transition: opacity .2s;");
                        client.println("        }");
                        client.println("");
                        client.println("        .slider:hover {");
                        client.println("            opacity: 1;");
                        client.println("        }");
                        client.println("");
                        client.println("        .slider::-webkit-slider-thumb {");
                        client.println("            -webkit-appearance: none;");
                        client.println("            appearance: none;");
                        client.println("            width: 20%; /* Set thumb width as 5% of the screen width */");
                        client.println("            height: 25px;");
                        client.println("            background: #04AA6D;");
                        client.println("            cursor: pointer;");
                        client.println("        }");
                        client.println("");
                        client.println("        .slider::-moz-range-thumb {");
                        client.println("            width: 20%; /* Set thumb width as 5% of the screen width */");
                        client.println("            height: 25px;");
                        client.println("            background: #04AA6D;");
                        client.println("            cursor: pointer;");
                        client.println("        }");
                        client.println("    </style>");
                        client.println("</head>");
                        client.println("<body>");
                        client.println("    <h1>Pong Paddle Position Control</h1>");
                        client.println("    <p>Drag the slider to set the position of the paddle.</p>");
                        client.println("    <div class=\"slidecontainer\">");
                        client.println("        <input type=\"range\" min=\"0\" max=\"6\" value=\"3\" class=\"slider\" id=\"paddleSlider\">");
                        client.println("        <p>Paddle Position: <span id=\"paddleValue\">3</span></p>");
                        client.println("    </div>");
                        client.println("    <script>");
                        client.println("        var slider = document.getElementById(\"paddleSlider\");");
                        client.println("        var output = document.getElementById(\"paddleValue\");");
                        client.println("        output.innerHTML = slider.value;");
                        client.println("");
                        client.println("        slider.oninput = function() {");
                        client.println("            output.innerHTML = this.value;");
                        client.println("            // Send slider value to ESP32");
                        client.println("            sendSliderValue(this.value);");
                        client.println("        }");
                        client.println("");
                        client.println("        function sendSliderValue(value) {");
                        client.println("            var xhttp = new XMLHttpRequest();");
                        client.println("            xhttp.open(\"GET\", \"/paddle?position=\" + value, true);");
                        client.println("            xhttp.send();");
                        client.println("        }");
                        client.println("    </script>");
                        client.println("</body>");
                        client.println("</html>");

                        // Close the connection
                        client.stop();
                        break; // Exit the loop after serving the response
                    }
                    else if (request.indexOf("GET /paddle") != -1)
                    {
                        // Handle paddle position update
                        String positionString = request.substring(request.indexOf("position=") + 9);
                        int paddlePosition = positionString.toInt();
                        // Update the shared variable
                        if (xSemaphoreTake(paddle_position_variable_semaphore, portMAX_DELAY) == pdTRUE)
                        {
                            sharedVariable_paddle_position = paddlePosition * 8;
                            xSemaphoreGive(paddle_position_variable_semaphore);
                        }
                        // Send a response indicating the paddle position was updated
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-Type: text/plain");
                        client.println();
                        client.println("Paddle position updated.");
                        // Close the connection
                        client.stop();
                        break;
                    }
                }
            }
        }
    }
}
