# dab-iot

## Pinout:
* Sound sensor: A0
* UV sensor: A1
* Light sensor: A2

* Acceleration sensor: I2C (I)
* Barometer: I2C (II)

* Humidity/Temperature sensor: D2
* Dust sensor: D3
* Hall sensor: D4
* LED Bar: D5
* Touch sensor: D7

## Instructions

1. LinkIt zusammenstecken (siehe PinOut in der Readme unseres GitHub-Repos)
2. Repo pullen und den hall-sensor Branch bauen und hochladen (wichtig: Bitte baut /einheit/all/all.ino  und nicht den Studenten-Ordner)
3. auf Batterie-Betrieb umstellen und losfahren (bearbeitet)

Der Hall-Sensor Code im Loop ist im Moment auskommentiert. Wer einen zur Hand hat, steckt den an Pin D4 an und wedelt alle 5 Minuten mit nem Magneten vorbei. Ansonsten könnt ihr das einfach ignorieren. Der Rest funktioniert auch ohne.