// определение устройств (в конкретном случае моих часов) на esp32 по BLE макадресу устройства. При превышении опреленной силы сигнала - зажигаем встроенный светодиод.
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

String mac_ = "f5:87:c9:34:d2:d7";
int scanTime = 1; //In seconds
String Bluetooth_device_mac;
int Bluetooth_device_rssi;
BLEScan* pBLEScan;

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      //Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
    }
};

void setup() {
  Serial.begin(115200);
  Serial.println("Scanning...");
  pinMode(2, OUTPUT);
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);  // less or equal setInterval value
}

void loop() {
  // put your main code here, to run repeatedly:
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  //Serial.print("Devices found: ");
  //Serial.println(foundDevices.getCount());
  int CountDevice = foundDevices.getCount();
  for (int i = 0; i < CountDevice; i++){
    BLEAdvertisedDevice d = foundDevices.getDevice(i);
    Bluetooth_device_mac = d.getAddress().toString().c_str();
    if (Bluetooth_device_mac == mac_) {
      Bluetooth_device_rssi = d.getRSSI();
      Serial.print("RSSI :");
      Serial.println(Bluetooth_device_rssi);
      if (Bluetooth_device_rssi > -75){
         digitalWrite(2, HIGH);   
        }
       else {
         digitalWrite(2, LOW);  
       }
    }
  }
  //Serial.println("Scan done!");
  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
  delay(2000);
}
