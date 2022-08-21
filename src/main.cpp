
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_NeoPixel.h>

#define PIN 5

double t = 0.0;

Adafruit_NeoPixel pixels(30, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 500

String sliderValue = "0";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
const int switch_pin = 14;

void setup(){
    // Serial port for debugging purposes
    Serial.begin(115200);
    pinMode(switch_pin, INPUT_PULLUP);

    // Connect to Wi-Fi
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
        Serial.println(WIFI_SSID);
    }

    // Print ESP Local IP Address
    Serial.println(WiFi.localIP());

    // Route for root / web page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "application/json", String("{ \"switch\": \"" + String(!digitalRead(switch_pin)) + "\" }").c_str());
    });

    // Start server
    server.begin();
    pixels.begin();
}

void test() {
    for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
        for(int i=0; i<pixels.numPixels(); i++) {
            int pixelHue = firstPixelHue + (i * 65536L / pixels.numPixels());
            pixels.setPixelColor(i, pixels.gamma32(pixels.ColorHSV(pixelHue)));
        }
        pixels.show();
        delay(5);
    }
}

void green() {
    pixels.clear();

    double sine = sin(t);
    double brightness = 0.01 + 0.6 * (0.5*sine + 0.5);
    uint8_t g = brightness * 255;

    uint32_t color = pixels.Color(0, g, 0);
    pixels.fill(color, 0, 30);

    pixels.show();
}


void red() {
    pixels.clear();

    double sine = sin(t);
    double brightness = 0.00 + 0.1*(0.5*sine + 0.5);
    uint8_t r = brightness * 255;

    uint32_t color = pixels.Color(r, 0, 0);
    pixels.fill(color, 0, 30);

    pixels.show();
}

void loop() {
    if(!digitalRead(switch_pin)) {
        green();
    } else {
        red();
    }

    t += 0.001;
}