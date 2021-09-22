# Arduino Time Tracker

Der Code ist in zwei Ordner unterteilt:
- `arduino-tracker/` beinhält den Code der auf den Arduino geladen werden kann
- `receiver-script` ist das NodeJS Script mit dem die Daten gelesen und in eine CSV-Datei geschrieben werden

## Receiver-Script
### Systemvoraussetzungen
Damit das Script am Computer laufen kann muss [Node.js](https://nodejs.org/en/) installiert sein.

### Installation
Um die `index.js` Datei im `receiver-script` Ordner auszuführen müssen zuerst die `dependencies` mit einem dieser beiden Befehle installiert werden:
```
npm install
```
```
yarn install
```

Danach kann das Script mit dem Befehl 
```
node index.js
``` 
ausgeführt werden.

### Konfiguration
Im `receiver-script` können sowohl der CSV-Datei Pfad, als auch der Serial-Port angepasst werden.  
Dazu einfach in der `index.js` Datei in der Variable `csvPath` - `<yourFilePath>` und bei `serialPort` - `<yourArduinoSerialPort>` abändern, speichern und neu ausführen.