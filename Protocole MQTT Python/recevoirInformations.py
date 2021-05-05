import paho.mqtt.client as mqttClient
import time

def on_message(client, userdata, message):
    print("Message recived"+str(message.payload.decode("utf-8")))
    print("Topic"+str(message.topic))

### Code on_message pour recevoir une image
"""
def on_message(client, userdata, msg):
    print ("Topic : ", msg.topic)
    f = open("output.jpg", "wb")  #there is a output.jpg which is different
    f.write(msg.payload)
    f.close()

"""


connected=False
Messagereceived=False

brokerAdress = "35.180.248.89"
port = 1883
user = "0x17"
password = "WP06qS"

isEnd=1

client = mqttClient.Client("Client1")
client.username_pw_set(user, password=password)

client.connect(brokerAdress, port=port)
client.loop_start()
client.subscribe("0x17/distanceDemandee")
client.on_message = on_message


time.sleep(5)





while connected != True:
    time.sleep(0.2)
while Messagereceived != True:
    time.sleep(0.2)


##client.publish("0x17", "essai python OK")
##print("Message published")
