# esp8266-mqtt-sonos-doorbell

Simple ESP8266 sketch (Arduino) to take a push button input and publish an MQTT message. delays have been added to create a debounce.

also a python script which listens on the MQTT topic for a message and upon receipt, sends a signal to a Sonos speaker to play the doorbell mp3 file.

Full installation and configuration detail to follow.
