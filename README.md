Motion Sensor to Slack Notification
-----------------------------------

Sends a message to Slack when motion is detected within a certain period.

1. Connect a PIR sensor, such as https://shop.rasp.io/products/pir-motion-sensor-2
to an ESP-8266 or sim (eg Wemos).

2. Create a slack webhook where notifications of motion detection are to be sent

3. Copy secrets.EXAMPLE.h to secrets.h and enter wifi details and slack webhook url
