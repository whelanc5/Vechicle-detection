# Vechicle-detection
put text documents in same folder as executable

Lots.txt -- holds list of lots, should be formatted as follows:
"Lot1 name"
"Lot1 maximum capacity"
"lot1 id"
"Lot2 name"
"Lot2 maximum capacity"
"lot2 id"
-----------------------------------------------------------------------------------------------------------------------------------------------------------
Example lots.txt:
Lot A
300
1
Lot B
200
2
------------------------------------------------------------------------------------------------------------------------------------------------------------
Each lot should have a txt document that matches its name. This text holds the information of the feeds that monitors the lot.
These txts should be formated as follows:
"Feed1 name"
"Feed1 rtsp address"
"direction or enterance(East/West)"
"Feed2 name"
"Feed2 rtsp address"
"direction or enterance(East/West)"
---------------------------------------------------------------------------------------------------------------------------------------------------------------
East Enterance
rtsp://tech:tech@72.82.144.138:556/cam/realmonitor?channel=8&subtype=0
East
South Enterance
rtsp://tech:tech@72.82.144.138:556/cam/realmonitor?channel=5&subtype=0
West
