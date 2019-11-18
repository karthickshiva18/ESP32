
#include <WiFi.h>
#include <WiFiClient.h>
#include <ESPmDNS.h>
#include <WebServer.h>

WebServer server(80);


const char* host = "esp32-webupdate";
//Router Login
const char *ssid = "karthick";
const char *password = "18KarthickShiva9591#$#$";
// Wifi Hotspot password set
const char *wifiApesp = "ESP32-Access-Point"; 
const char *key = "123456789";

bool is_authentified() {
  Serial.println("Enter is_authentified");
  if (server.hasHeader("Cookie")) {
    Serial.print("Found cookie: ");
    String cookie = server.header("Cookie");
    Serial.println(cookie);
    if (cookie.indexOf("ESPSESSIONID=1") != -1) {
      Serial.println("Authentification Successful");
      return true;
    }
  }
  Serial.println("Authentification Failed");
  return false;
}

//login page, also called for disconnect
void handleLogin() {
  String msg;
  if (server.hasHeader("Cookie")) {
    Serial.print("Found cookie: ");
    String cookie = server.header("Cookie");
    Serial.println(cookie);
  }
  if (server.hasArg("DISCONNECT")) {
    Serial.println("Disconnection");
    server.sendHeader("Location", "/login");
    server.sendHeader("Cache-Control", "no-cache");
    server.sendHeader("Set-Cookie", "ESPSESSIONID=0");
    server.send(301);
    return;
  }
  // UserName and Password Set code 1st User
  if (server.hasArg("USERNAME") && server.hasArg("PASSWORD")) {
    if (server.arg("USERNAME") == "admin" &&  server.arg("PASSWORD") == "admin" ) {
      server.sendHeader("Location", "/");
      server.sendHeader("Cache-Control", "no-cache");
      server.sendHeader("Set-Cookie", "ESPSESSIONID=1");
      server.send(301);
      Serial.println("Log in Successful");
      digitalWrite(12, HIGH);
      digitalWrite(13, LOW);
      digitalWrite(14, LOW);
      delay(100);
      return;
    }
    // UserName and Password Set code 2nd User
    else if (server.arg("USERNAME") == "karthick" &&  server.arg("PASSWORD") == "password" ) {
      server.sendHeader("Location", "/");
      server.sendHeader("Cache-Control", "no-cache");
      server.sendHeader("Set-Cookie", "ESPSESSIONID=1");
      server.send(301);
      Serial.println("Log in Successful");
      digitalWrite(12, LOW);
      digitalWrite(13, HIGH);
      digitalWrite(14, LOW);
      delay(100);
      return;
    }
    // UserName and Password Set code 3rd User
    else if (server.arg("USERNAME") == "shashi" &&  server.arg("PASSWORD") == "shashi" ) {
      server.sendHeader("Location", "/");
      server.sendHeader("Cache-Control", "no-cache");
      server.sendHeader("Set-Cookie", "ESPSESSIONID=1");
      server.send(301);
      Serial.println("Log in Successful");
      digitalWrite(12, LOW);
      digitalWrite(13, LOW);
      digitalWrite(14, HIGH);
      delay(100);
      return;
    }

    else {
      msg = "Wrong username/password! try again.";
      Serial.println("Log in Failed");
    }

  }
  String content = "<html><body><form action='/login' method='POST'>Login Page<br>";
  content += "User:<input type='text' name='USERNAME' placeholder='user name'><br>";
  content += "Password:<input type='password' name='PASSWORD' placeholder='password'><br>";
  content += "<input type='submit' name='SUBMIT' value='Submit'></form>" + msg + "<br>";
  //Public page without login for demo
  content += "You also can go <a href='/inline'>here</a></body></html>";
  server.send(200, "text/html", content);
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);
  digitalWrite(14, LOW);
}

//root page can be accessed only if authentification is ok
void handleRoot() {
  Serial.println("Enter handleRoot");
  String header;
  if (!is_authentified()) {
    server.sendHeader("Location", "/login");
    server.sendHeader("Cache-Control", "no-cache");
    server.send(301);
    return;
  }
  String content = "<html><body><H2>hello, you successfully connected to esp32!</H2><br>";
  // UserAgent Code to identify the Hardware login
  if (server.hasHeader("User-Agent")) {
    content += "the user agent used is : " + server.header("User-Agent") + "<br><br>";
  }
  content += "You can access this page until you <a href=\"/login?DISCONNECT=YES\">Logout</a></body></html>";

  server.send(200, "text/html", content);

}





void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);

}


void setup(void) {
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(14, OUTPUT);
  Serial.begin(115200);
  Serial.println();
  Serial.println("Booting Sketch...");
  //Access Point for ESP32
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() == WL_CONNECTED) {
    MDNS.begin(host);
    server.on("/", HTTP_GET, []() {
      server.sendHeader("Connection", "close");
      server.send(200, "text/plain", "hello from esp32!");
    });

    // Connect to Wi-Fi network with SSID and password
    Serial.print("Setting AP (Access Point)…");
    // Remove the password parameter, if you want the AP (Access Point) to be open
    WiFi.softAP(wifiApesp, key);

    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);

    server.begin();
    MDNS.addService("http", "tcp", 80);

    Serial.printf("Ready! Open http://%s.local in your browser\n", host);
  } else {
    Serial.println("WiFi Failed");
  }



  server.on("/", handleRoot);
  server.on("/login", handleLogin);
  server.on("/inline", []() {
    server.send(200, "text/plain", "this works without need of authentification");
  });

  server.onNotFound(handleNotFound);
  //here the list of headers to be recorded
  const char * headerkeys[] = {"User-Agent", "Cookie"} ;
  size_t headerkeyssize = sizeof(headerkeys) / sizeof(char*);
  //ask server to track these headers
  server.collectHeaders(headerkeys, headerkeyssize );
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  delay(1);
}
