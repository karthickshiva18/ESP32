#include <WiFi.h>
#include <WiFiClient.h>
#include <ESPmDNS.h>
#include <WebServer.h>
#include <ESPAsyncWebServer.h>
#include "FS.h"

WebServer server(80);


const char* host = "esp32-webupdate";
const char *wifiApesp = "ESP32-Access-Point";
const char *key = "123456789";

bool is_authentified() {
  Serial.println("Enter is_authentified");
  if (server.hasHeader("Cookie")) {
    String cookie = server.header("Cookie");
    Serial.println(cookie);
    if (cookie.indexOf("ESPSESSIONID=1") != -1) {
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
    String cookie = server.header("Cookie");
    Serial.println(cookie);
  }
  if (server.hasArg("Logout")) {
    Serial.println("Logout");
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

  char webpage[] PROGMEM = R"=====(
<html>
<head>
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <style>
       
        body {font-family: Arial, Helvetica, sans-serif;}
        form {border: 3px solid #f1f1f1;}
       
        input[type=text], input[type=password] {
          width: 100%;
          padding: 12px 20px;
          margin: 8px 0;
          display: inline-block;
          border: 1px solid #ccc;
          box-sizing: border-box;
        }
       
        button {
          background-color: #4CAF50;
          color: white;
          padding: 14px 20px;
          margin: 8px 0;
          border: none;
          cursor: pointer;
          width: 100%;
        }
       
        button:hover {
          opacity: 0.8;
        }
       
        .cancelbtn {
          width: auto;
          padding: 10px 18px;
          background-color: #f44336;
        }
       
        .imgcontainer {
          text-align: center;
          margin: 24px 0 12px 0;
        }
       
        img.avatar {
          width: 40%;
          border-radius: 50%;
        }
       
        .container {
          padding: 16px;
        }
       
        span.PASSWORD {
          float: right;
          padding-top: 16px;
        }
       
        /* Change styles for span and cancel button on extra small screens */
        @media screen and (max-width: 300px) {
          span.PASSWORD {
             display: block;
             float: none;
          }
          .cancelbtn {
             width: 100%;
          }
        }
        </style>
</head>
<body>

<div>
<h2>Login Form</h2>
<form action='/login' method='POST'>Welcome Userlogin page ESP32<br><br>
  <div class="container">
   <label for="uname"><b>UserName</b></label>
   <input type="text" placeholder="Enter Username" name="USERNAME" required>
    <label for="psw"><b>Password</b></label>
    <input type="password" placeholder="Enter Password" name="PASSWORD" required>
        <button type="submit" name='SUBMIT' value='Submit'>Login</button>
     <label>
      <input type="checkbox" checked="checked" name="remember"> Remember me
    </label>
  </div>

</form>
</div>

</body>
</html>

)=====";
 server.send(200, "text/html", webpage);

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

char webinner[] PROGMEM = R"=====(
<html>
<head>
<style>
ul {
  list-style-type: none;
  margin: 0;
  padding: 0;
  overflow: hidden;
  background-color: #333;
}

li {
  float: left;
  border-right:1px solid #bbb;
}

li:last-child {
  border-right: none;
}

li a {
  display: block;
  color: white;
  text-align: center;
  padding: 14px 16px;
  text-decoration: none;
}

li a:hover:not(.active) {
  background-color: #111;
}

.active {
  background-color: #4CAF50;
}
</style>
</head>
<body>

<div>
 <div>
 <ul>
  <li><a class="active" href="#home">Home</a></li>
  <li><a href="#news">News</a></li>
  <li><a href="#contact">Contact</a></li>
  <li style="float:right"><a href="#about">About</a></li>
</ul>
  <div>
<H2 style=\"color:blue;\">hello, you successfully log in to esp32!</H2>
</div>
<div>
<p>Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas.</p>
<h1>HTML Ipsum Presents</h1>

  <p><strong>Pellentesque habitant morbi tristique</strong> senectus et netus et malesuada fames ac turpis egestas.
    Vestibulum tortor quam, feugiat vitae, ultricies eget,
    tempor sit amet, ante. Donec eu libero sit amet quam egestas semper.
    <em>Aenean ultricies mi vitae est.</em> Mauris placerat eleifend leo.
     Quisque sit amet est et sapien ullamcorper pharetra. Vestibulum erat wisi,
     condimentum sed, <code>commodo vitae</code>, ornare sit amet, wisi. Aenean
      fermentum, elit eget tincidunt condimentum, eros ipsum rutrum orci, sagittis
      tempus lacus enim ac dui.
 
</div>
</body>
</html>

)=====";
   String content = "<html><body><a href=\"/login?Logout=YES\"><H2 style=\"color:red;font-style: italic;font-size: xx-large;\">Logout</H2></a></body></html>";

 server.send(200, "text/html", webinner + content);
 

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
  WiFi.mode(WIFI_AP_STA);
  if (WiFi.waitForConnectResult() == WL_CONNECTED) {
    MDNS.begin(host);
    server.on("/", HTTP_GET, []() {
      server.sendHeader("Connection", "close");
      server.send(200, "text/plain", "hello from esp32!");
    });

    Serial.print("Setting AP (Access Point)…");
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
