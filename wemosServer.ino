#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char *ssid = "yourNetworkName";
const char *password = "yourpassword";

String myCSS;

String bootstrapCSS;
String bootstrapJS;

String body;
String ligar_btn;
String lig_deslig_btn;
String campoLuminosidade;
int REFRESH_SECONDS = 5;
//String refreshCode = "<meta http-equiv=\"refresh\" content=\""+String(REFRESH_SECONDS)+"\">";

//String myScript;
//String endScript;
//String atualizaFunction;

int ledPin = SCK;
int testPin = 12;
String ledState = "Desligada";
//int ldrPin = A0;
//int ldrValue;

/* STANDBY MODE
  // convert to microseconds
  ESP.deepSleep(sleepSeconds * 1000000);
*/

WiFiServer server(80);

/*int getLDRValue(){
  return (analogRead(ldrPin)*100)/1024;
}*/

void setup()
{
    Serial.begin(9600);
    delay(10);

    pinMode(ledPin, OUTPUT);
    //pinMode(ldrPin, INPUT);
    digitalWrite(ledPin, HIGH);

    // Connect to WiFi network
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");

    // Start the server
    server.begin();
    Serial.println("Server started");

    // Print the IP address
    Serial.print("Use this URL : ");
    Serial.print("http://");
    Serial.print(WiFi.localIP());
    Serial.println("/");

    //Preparing the CSS
    myCSS.concat("<style>");
    myCSS.concat("#liga_led_btn, #desliga_led_btn {");
    myCSS.concat("height: 200px;");
    myCSS.concat("font-size: 40px;");
    myCSS.concat("}");
    myCSS.concat("#li_led_txt, #li_ldr_txt {");
    myCSS.concat("font-size: 25px;");
    myCSS.concat("}");
    myCSS.concat("</style>");

    //Preparing bootstrap CSS
    bootstrapCSS.concat("<link rel=\"stylesheet\" href=\"https://stackpath.bootstrapcdn.com/bootstrap/4.1.0/css/bootstrap.min.css\" integrity=\"sha384-9gVQ4dYFwwWSjIDZnLEWnxCjeSWFphJiwGPXr1jddIhOegiu1FwO5qRGvFXOdJZ4\" crossorigin=\"anonymous\">");

    //Preparing bootstrap scripts
    bootstrapJS.concat("<script src=\"https://code.jquery.com/jquery-3.3.1.slim.min.js\" integrity=\"sha384-q8i/X+965DzO0rT7abK41JStQIAqVgRVzpbzo5smXKp4YfRvH+8abtTE1Pi6jizo\" crossorigin=\"anonymous\"></script>");
    bootstrapJS.concat("<script src=\"https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.14.0/umd/popper.min.js\" integrity=\"sha384-cs/chFZiN24E4KMATLdqdvsezGxaGsi4hLGOzlXwp5UZB1LY//20VyM2taTB4QvJ\" crossorigin=\"anonymous\"></script>");
    bootstrapJS.concat("<script src=\"https://stackpath.bootstrapcdn.com/bootstrap/4.1.0/js/bootstrap.min.js\" integrity=\"sha384-uefMccjFJAIv6A+rW+L4AHf99KvxDjWSu1z9VI8SKNVmz4sk7buKt/6v9KI65qnm\" crossorigin=\"anonymous\"></script>");

    //ldrValue = getLDRValue();

    //Preparing the html body
    /*body.concat("<li id=\"li_ldr_txt\" class=\"list-group-item list-group-item-info\">Luminosidade: " + String(ldrValue) + "% </li>");
  body.concat("<br>");
  body.concat("<li id=\"li_led_txt\" class=\"list-group-item list-group-item-success\">Lâmpada: "+ ledState + "</li>");*/

    body.concat("<br>");
    //Botão de ligar led
    lig_deslig_btn.concat("<form method=\"post\" action=\"/LED=ON\">");
    lig_deslig_btn.concat("<button id=\"liga_led_btn\" type=\"submit\" class=\"btn btn-success btn-lg btn-block\">Ligar Lâmpada</button>");
    lig_deslig_btn.concat("</form>");
    body.concat(lig_deslig_btn);

    //Preparing the script
    /*myScript = "<script type=\"text/javascript\">";
  myScript.concat("setInterval(function() { atualiza() }, 2000);");
  atualizaFunction = "function atualiza(){";
  atualizaFunction.concat("document.getElementById(\"li_ldr_txt\").innerHTML = \"Luminosidade: "+ String(ldrValue) +"\";");
  atualizaFunction.concat("}");
  endScript = "</script>";*/
}

void loop()
{
    // Check if a client has connected
    WiFiClient client = server.available();
    if (!client)
    {
        return;
    }

    // Wait until the client sends some data
    Serial.println("new client");
    while (!client.available())
    {
        delay(1);
    }

    // Read the first line of the request
    String request = client.readStringUntil('\r');
    Serial.println(request);
    client.flush();

    // Match the request

    int value = HIGH;
    //ldrValue = getLDRValue();

    if (request.indexOf("/LED=ON") != -1)
    {
        digitalWrite(ledPin, LOW);
        digitalWrite(testPin, HIGH);
        value = LOW;
        ledState = "Ligada";

        //Botão de desligar led
        lig_deslig_btn = "<form method=\"post\" action=\"/LED=OFF\">";
        lig_deslig_btn.concat("<button id=\"desliga_led_btn\" hre type=\"submit\" class=\"btn btn-danger btn-lg btn-block\">Desligar Lâmpada</button>");
        lig_deslig_btn.concat("</form>");
    }
    if (request.indexOf("/LED=OFF") != -1)
    {
        digitalWrite(ledPin, HIGH);
        value = HIGH;
        ledState = "Desligada";

        //Botão de ligar led
        lig_deslig_btn = "<form method=\"post\" action=\"/LED=ON\">";
        lig_deslig_btn.concat("<button id=\"liga_led_btn\" type=\"submit\" class=\"btn btn-success btn-lg btn-block\">Ligar Lâmpada</button>");
        lig_deslig_btn.concat("</form>");
    }

    //Uptating body
    //body = "<li id=\"li_ldr_txt\" class=\"list-group-item list-group-item-info\">Luminosidade: " + String(ldrValue) + "% </li>";
    body = "<br>";
    body.concat("<li id=\"li_led_txt\" class=\"list-group-item list-group-item-success\">Lâmpada: " + ledState + "</li>");
    body.concat("<br>");
    body.concat("<br><br>");
    body.concat(lig_deslig_btn);

    //Updating atualiza function
    /*atualizaFunction = "function atualiza(){";
  atualizaFunction.concat("document.getElementById(\"li_ldr_txt\").innerHTML = \"Luminosidade: "+ String(analogRead(ldrPin)) +"%\";");
  atualizaFunction.concat("}");*/

    // Return the response
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println(""); //  do not forget this one

    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println("<head>");
    client.println("<meta charset=\"UTF-8\">");
    //client.println(refreshCode);
    /*client.println(myScript);
  client.println(atualizaFunction);
  client.println(endScript);*/
    client.println(myCSS);
    client.println(bootstrapCSS);
    client.println("</head>");
    client.println("<body>");

    client.println("<br><br>");
    client.println("");
    client.println(body);
    client.println(bootstrapJS);
    client.println("</body>");
    client.println("</html>");

    delay(1);
    Serial.println("Client disconnected");
    Serial.println("");
}
