# Smart-Fan

Dorm thermostat built on the Seeed XIAO ESP32-C3. Captures proprietary IR remote codes from a DC tower fan using an Arduino Nano and TSOP38238 receiver, then replays raw pulse timing through an IR LED to autonomously control fan power based on DHT11 readings. 

This thermostat makes sure that the temperature in the dorm room is between 20-22 degrees celsius (adjustable). If the temperature is equal to 22 C or goes beyond the threshold, the fan turns off. Otherwise, the fan continues blowing at a speed of 4 (adjustable). 
