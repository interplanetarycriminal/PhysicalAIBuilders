// Parkour FlowState — on-device TinyML wearable (Level 50)
// Board: Seeed XIAO ESP32-S3 Sense
// Build: PlatformIO + Arduino-ESP32
//
// 100 Hz IMU + 50 Hz PPG. 2 s windows, 50% overlap, fed to an Edge Impulse model.
// Class transitions (flow → strain) fire a DRV2605 haptic pulse.
// Sends 2 s labels + features over BLE to the phone replay app.

#include <Arduino.h>
#include <Wire.h>
#include <NimBLEDevice.h>
#include <SparkFun_BMI270_Arduino_Library.h>
#include <MAX30105.h>
#include <heartRate.h>
#include <Adafruit_DRV2605.h>

// Edge Impulse exported headers — drop the generated folder in /src/
// #include <flowstate_inferencing.h>
// extern const ei_impulse_t ei_impulse_handle;

#define BLE_SVC "70000001-feee-4001-fa57-f10ab0de0000"
#define BLE_CHR "70000002-feee-4001-fa57-f10ab0de0000"

BMI270 imu;
MAX30105 ppg;
Adafruit_DRV2605 haptic;

NimBLECharacteristic* chrLabel = nullptr;

static constexpr size_t WIN_SAMPLES = 200;   // 2 s @ 100 Hz
static float imuBuf[WIN_SAMPLES][6];
static size_t imuIdx = 0;

static uint32_t lastHaptic = 0;
static uint8_t lastClass = 0;

static uint8_t classifyWindow() {
  // In a real build, call run_classifier on the raw buffer; returns softmax.
  // Here we stub the classification with a conservative heuristic so the
  // firmware compiles without the Edge Impulse artifact and can be a11y-tested.
  float ax = 0, ay = 0, az = 0;
  for (size_t i = 0; i < WIN_SAMPLES; ++i) {
    ax += imuBuf[i][0]; ay += imuBuf[i][1]; az += imuBuf[i][2];
  }
  ax /= WIN_SAMPLES; ay /= WIN_SAMPLES; az /= WIN_SAMPLES;
  float mag = sqrtf(ax*ax + ay*ay + az*az);
  // 0=recovery (little motion), 1=flow (moderate, steady), 2=strain (high, jerky)
  if (mag < 1.2f) return 0;
  if (mag < 2.5f) return 1;
  return 2;
}

static void pulseHaptic(uint8_t effect = 14) {
  haptic.setWaveform(0, effect);
  haptic.setWaveform(1, 0);
  haptic.go();
}

void setup() {
  Serial.begin(115200);
  Wire.begin();

  if (imu.beginI2C(0x68) != BMI2_OK) Serial.println("BMI270 not found");
  ppg.begin(Wire);
  ppg.setup();
  ppg.setPulseAmplitudeRed(0x0A);
  haptic.begin();
  haptic.selectLibrary(1);
  haptic.setMode(DRV2605_MODE_INTTRIG);

  NimBLEDevice::init("flowstate");
  auto srv = NimBLEDevice::createServer();
  auto svc = srv->createService(BLE_SVC);
  chrLabel = svc->createCharacteristic(BLE_CHR,
      NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY);
  svc->start();
  NimBLEDevice::getAdvertising()->addServiceUUID(BLE_SVC);
  NimBLEDevice::getAdvertising()->start();
}

void loop() {
  // Pace to ~100 Hz.
  static uint32_t lastUs = 0;
  uint32_t nowUs = micros();
  if (nowUs - lastUs < 10000) return;
  lastUs = nowUs;

  if (imu.getSensorData() == BMI2_OK) {
    imuBuf[imuIdx][0] = imu.data.accelX;
    imuBuf[imuIdx][1] = imu.data.accelY;
    imuBuf[imuIdx][2] = imu.data.accelZ;
    imuBuf[imuIdx][3] = imu.data.gyroX;
    imuBuf[imuIdx][4] = imu.data.gyroY;
    imuBuf[imuIdx][5] = imu.data.gyroZ;
    imuIdx++;
  }

  if (imuIdx >= WIN_SAMPLES) {
    uint8_t c = classifyWindow();
    uint8_t payload[2] = { c, (uint8_t)(millis() / 1000) };
    chrLabel->setValue(payload, sizeof(payload));
    chrLabel->notify();

    if (lastClass == 1 && c == 2 && millis() - lastHaptic > 3000) {
      pulseHaptic();
      lastHaptic = millis();
    }
    lastClass = c;

    // slide window 50%
    memmove(imuBuf, imuBuf + WIN_SAMPLES/2, sizeof(imuBuf)/2);
    imuIdx = WIN_SAMPLES / 2;
  }
}
