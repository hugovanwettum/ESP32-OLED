#include <Arduino.h>
#include <WiFi.h>

#include "WiFiConfig.h" // not synched to remote, only stored locally as it contains my wifi network SSID and password

extern uint16_t sharedVariable_radius;

// Set web server port number to 80
WiFiServer server(80);

extern SemaphoreHandle_t radius_variable_semaphore;

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
                        client.println("    <title>Ball buttons!</title>");
                        client.println("</head>");
                        client.println("<body>");
                        client.println("    <h1>Ball buttons!</h1>");
                        client.println("    <button onclick=\"sendButton1Press()\">Increase ball radius</button>");
                        client.println("    <button onclick=\"sendButton2Press()\">Decrease ball radius</button>");
                        client.println("    <script>");
                        client.println("        function sendButton1Press() {");
                        client.println("            var xhttp = new XMLHttpRequest();");
                        client.println("            xhttp.open(\"GET\", \"/button1\", true);");
                        client.println("            xhttp.send();");
                        client.println("        }");
                        client.println("");
                        client.println("        function sendButton2Press() {");
                        client.println("            var xhttp = new XMLHttpRequest();");
                        client.println("            xhttp.open(\"GET\", \"/button2\", true);");
                        client.println("            xhttp.send();");
                        client.println("        }");
                        client.println("    </script>");
                        client.println("</body>");
                        client.println("</html>");

                        // Close the connection
                        client.stop();
                        break; // Exit the loop after serving the response
                    }
                    else if (request.indexOf("GET /button1") != -1)
                    {
                        //TODO: move to a separate function
                        // Handle Button 1 press
                        Serial.println("Button 1");
                        // Send a response indicating Button 1 was pressed
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-Type: text/plain");
                        client.println();
                        // Acquire the semaphore
                        if (xSemaphoreTake(radius_variable_semaphore, portMAX_DELAY) == pdTRUE)
                        {
                            // Modify the shared variable
                            if (sharedVariable_radius < 31)
                                sharedVariable_radius++;
                            // Release the semaphore
                            xSemaphoreGive(radius_variable_semaphore);
                        }
                        // Close the connection
                        client.stop();
                        break;
                    }
                    else if (request.indexOf("GET /button2") != -1)
                    {
                        //TODO: move to a separate function
                        // Handle Button 2 press
                        Serial.println("Button 2");
                        // Send a response indicating Button 2 was pressed
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-Type: text/plain");
                        client.println();
                        // Acquire the semaphore
                        if (xSemaphoreTake(radius_variable_semaphore, portMAX_DELAY) == pdTRUE)
                        {
                            // Modify the shared variable
                            if (sharedVariable_radius > 1)
                                sharedVariable_radius--;
                            // Release the semaphore
                            xSemaphoreGive(radius_variable_semaphore);
                        }
                        // Close the connection
                        client.stop();
                        break;
                    }
                }
            }
        }
    }
}
