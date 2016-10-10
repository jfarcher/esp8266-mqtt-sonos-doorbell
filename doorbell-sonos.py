#!/usr/bin/env python

import sys
import time
import socket
from threading import Thread        
from soco import SoCo
import paho.mqtt.client as mqtt

# Add Sonos device IP address here - scanning will be added at a later date
device = '192.168.2.48'
queueable_item = "https://raw.githubusercontent.com/jfarcher/esp8266-mqtt-sonos-doorbell/master/doorbell-1.mp3"





def play(device, queueable_item):
    sonos = SoCo(device)
    track = sonos.get_current_track_info()
    initial_playlist_position = int(track["playlist_position"]) - 1
    
    initial_track_position = (track["position"])
    initial_state = sonos.get_current_transport_info().get("current_transport_state")
    initial_volume = sonos.volume
    sonos.volume=3
    sonos.play_uri(uri=queueable_item, meta='')
    time.sleep( 2 )
    sonos.volume=initial_volume    
    sonos.play_from_queue(initial_playlist_position)
    sonos.seek(initial_track_position)
    if initial_state != 'PLAYING':
                sonos.pause()

def on_connect(mosq,obj,rc):
    mosq.subscribe(topic, 0)
def on_message(mosq,obj,msg):
   message= msg.payload
   intopic=msg.topic
   play_thread = Thread(target=play, args=(device,queueable_item))
   play_thread.start()


#add MQTT broker IP here
broker= '192.168.1.10'
port= '1883'
topic = 'house/doorbell/#'
mqttc = mqtt.Client()
mqttc.connect (broker, port, 60)

mqttc.on_connect = on_connect
mqttc.on_message = on_message


mqttc.loop_forever()

