#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <esp_websocket_client.h>

const char *ssid = "DESKTOP-H29541I 1370";
const char *password = "787Pt7!7";

const int httpsPort = 443;
const char* host = "localhost";

static void get_string(char *line, size_t size);

// const char* fingerprint = "08 42 54 11 21 09 9E 94 65 8E B9 C4 07 58 7A 94 C4 B3 08 26";

const char* ca_cert = "-----BEGIN CERTIFICATE-----\n" \
"MIIC7DCCAdSgAwIBAgIQHRGLhTmRz4RHgURs8RzpjzANBgkqhkiG9w0BAQsFADAU\n" \
"MRIwEAYDVQQDEwlsb2NhbGhvc3QwHhcNMjIxMjAzMDkwMDA0WhcNMjcxMjAzMDAw\n" \
"MDAwWjAUMRIwEAYDVQQDEwlsb2NhbGhvc3QwggEiMA0GCSqGSIb3DQEBAQUAA4IB\n" \
"DwAwggEKAoIBAQCpAK3b2EemOrgWsviXPGkFbeDF2uQ15O795WQgeuod91VpNDf/\n" \
"D2hM76EAkCJ99qO2u4Mfs9rOTArN7N25dAiMRKGaRyJq6WYvdMv7wXv1j8eI5NKw\n" \
"LND29xTIBTbfh7yGJaeoeN2sNaZotCMoLWCvIbebRNCIQxqtfqc9/rbTt/P6ADIG\n" \
"z41NA/eRuvXzyShjFR9jC3eGhKB69U29VDQm2H0TIcTJt7YIlDb1w3RNMQPg/2MQ\n" \
"AHKjTgCeqe25z7OYMz9r+/U5956INXBMfVZ66d6IKM7ocOIisSxxLp1r9Z9Wektx\n" \
"wemteGLPD9/th345y5/EK6T59kFBnNiYex5ZAgMBAAGjOjA4MAsGA1UdDwQEAwIE\n" \
"sDATBgNVHSUEDDAKBggrBgEFBQcDATAUBgNVHREEDTALgglsb2NhbGhvc3QwDQYJ\n" \
"KoZIhvcNAQELBQADggEBAEmkEB/gJ0C4xcuGimIpn9CDB9CZ1ZKVmAQsEWszxauA\n" \
"pLOd90ZZ5A4PcLRPNPfVt1izk1RbPc1oWmozAKAmOD4rym2bLbGOheFfe7RN9HPR\n" \
"cJMGm47ccZs0otZgo68nz6YkroILjrvDw4ANnMzxZJtInPqxZddR7jdVYaJr4Owh\n" \
"/fC2DiJaM/COhiunJpHneZuHHFS/9CWpAIHsbp8H1jt0ShVKdsL3YI5siiZda6W4\n" \
"Yg6V/Vb1mUDis6iPGIMqVGhLH3ileLo1RICOGzkAjQyXAH9fB4pwBWRDk+Ok2HNO\n" \
"mS6I+mzrmAHMsDJ/wKh9tDIhaRLebl660M13pzB5x+w=\n"
"-----END CERTIFICATE-----\n";


static void websocket_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    //esp_websocket_event_data_t *data = (esp_websocket_event_data_t *)event_data;
    switch (event_id) {
    case WEBSOCKET_EVENT_CONNECTED:
        Serial.println("WEBSOCKET_EVENT_CONNECTED");
        break;
    case WEBSOCKET_EVENT_DISCONNECTED:
        Serial.println("WEBSOCKET_EVENT_DISCONNECTED");
        break;
    case WEBSOCKET_EVENT_DATA:
        Serial.println("WEBSOCKET_EVENT_DATA");
        break;
    case WEBSOCKET_EVENT_ERROR:
        Serial.println("WEBSOCKET_EVENT_ERROR");
        break;
    }
}
esp_websocket_client_handle_t client;

void setup() {
  
  Serial.begin(115200);
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());
  const esp_websocket_client_config_t ws_cfg = {
    // .uri = "ws://localhost:44309/send",
    .uri = "ws://192.168.137.1:15911/send",
    .host = "localhost",
    .port=15911,
    .username="",
    .password="",
    .path="/send"
  };

  // ws_cfg.port = 44309;
  // ws_cfg.path = "/send";
  //ws_cfg.cert_pem = (const char *)ca_cert;
  Serial.print("Uri: ");
  Serial.println(ws_cfg.uri);
  Serial.print("Port: ");
  Serial.println(ws_cfg.port);
  Serial.print("Path: ");
  Serial.println(ws_cfg.path);
  client = esp_websocket_client_init(&ws_cfg);
  Serial.print("Port: ");
  Serial.println(ws_cfg.port);
  Serial.print("Path: ");
  Serial.println(ws_cfg.path);
  esp_websocket_register_events(client, WEBSOCKET_EVENT_ANY, websocket_event_handler, (void *)client);
  while (true)
  {
    esp_websocket_client_start(client);
    if (esp_websocket_client_is_connected(client))
    {
      break;
    }
    else
    {
      esp_websocket_client_stop(client);
      Serial.println("Client is not connected");
      char line[128];
      get_string(line, sizeof(line));
      esp_websocket_client_set_uri(client, line);
      //ws_cfg.uri = line;
      esp_websocket_client_start(client);
  
    }
    
  }
  
  
  //client.setCACert(ca_cert);
  
  // Serial.print("connecting to ");
  // Serial.println(host);
  // if (!client.connect(host, httpsPort)) {
  //   Serial.println("connection failed");
  //   return;
  // }

  // if (client.verify(fingerprint, host)) {
  //   Serial.println("certificate matches");
  // } else {
  //   Serial.println("certificate doesn't match");
  // }

}

static void get_string(char *line, size_t size)
{
    String input = Serial.readStringUntil('\n');
    line = (char*)input.c_str();
}

void loop() {
  char data[32];
  int len = sprintf(data, "hello %04d", 5);
  if (esp_websocket_client_is_connected(client)) {
  esp_websocket_client_send_text(client, data,len , portMAX_DELAY);
  vTaskDelay(3000);
  }
  // else{
  //   esp_websocket_client_stop(client);
  //   Serial.println("Client is not connected");
  //   char line[128];
  //   get_string(line, sizeof(line));
  //   esp_websocket_client_set_uri(client, line);
  //   //ws_cfg.uri = line;
  //   esp_websocket_client_start(client);
  // }
  //  int  conn;
  //  int chip_id = ESP.getEfuseMac();;
  //  Serial.printf("  Flash Chip id = %08X\t", chip_id);
  //  Serial.println();
  //  Serial.println();
  //  String body = "ChipId=" + String(chip_id) + "&SentBy=" + "your_name";
  //  int body_len = body.length();

  //  Serial.println(".....");
  //  Serial.println(); Serial.print("For sending parameters, connecting to "); Serial.println(host);
  //  conn = client.connect(host, httpsPort);

  //  if (conn == 1) {
  //     Serial.println(); Serial.print("Sending Parameters...");
  //     //Request
  //     client.println("POST /post HTTP/1.1");
  //     //Headers
  //     client.print("Host: "); client.println(host);
  //     client.println("Content-Type: application/x−www−form−urlencoded");
  //     client.print("Content-Length: "); client.println(body_len);
  //     client.println("Connection: Close");
  //     client.println();
  //     //Body
  //     client.println(body);
  //     client.println();

  //     //Wait for server response
  //     while (client.available() == 0);

  //     //Print Server Response
  //     while (client.available()) {
  //        char c = client.read();
  //        Serial.write(c);
  //     }
  //  } else {
  //     client.stop();
  //     Serial.println("Connection Failed");
  //  }
  //  delay(5000);



}

