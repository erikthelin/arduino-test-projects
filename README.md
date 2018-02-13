# arduino-test-projects
Contains small test projects.

## led_key_test
Tests how to write to a TM1638 led display.

## motiontest
Tests a HC-SR501 pir motionsensor. When motion is registered it will update a TM1638 led display with number of motions.

## wifi_dht_mqtt_sensor
Test a DHT sensor that transmits temperature and humidity to an MQTT server (e.g. home assistant). The sensor then "deep-sleeps" until it sends data again, making it suitable for battery usage.
