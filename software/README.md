# Content

The software consists of the following files:
- GPStrackerDIY.ino ( connects the ESP8266 to WIFI to serve the UI, drives the NEO GPS receiver, delivers the UI, updates track.gpx ) before using it, you need to configure it with your own SSID and password.
- data / index.html ( the UI entry point )
- data / leaflet.js 
- data / leaflet.js.map
- data / leaflet.css
- data / osmutils.js
- data / plot.js
- data / shimg.js
- data / gmutils.js
- data / gra_canvas.js
- data / GPX2GM.css
- data / GPX2GM.js
- data / GPX2GM_Defs.js
- data / Icons / Loading_icon.gif
- data / Icons / lupe_p.png
- data / images / layers-2x.png

The "data" files must be loaded using the ESP8266 Sketch data Uploader plugin.
The "gpstracker.ino" is loaded using the Arduino IDE.

# Operation
When the device is powered on ( with batteries or through USB ) then it deletes existing "track.gpx" and creates a fresh one.
Then every 10 seconds checks signal from the GPS module, and if longitude / latitude information is available then updates the track.gpx
Upon inquiry of the UI, the index.html "embeds" the "track.gpx" and renders it through the javascript UI.

All files in the data folder come from the https://www.j-berkemeier.de/GPXViewer/ GPXViewer of J&uuml;rgen Berkemeir, I've ve only reorganized
them in order to be delivered through the ESP8266 being installed all in one folder.
