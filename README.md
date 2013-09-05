pa
==

Project Anna is a Software to tune up you Party.


---
What dos pa?

pa is a Project to mix up your Party with some Light stuff. You connect a Projector to your PC, and controll with your Smartphone some moving ligths.
The Project contains three parts.

The first one is the Server sonetimes caled Beamer(German word for Projector). This is the Part witch you schuold move to your Projector. It Displays the ligth points

The second Part is the Gui. There you controll the colors, size and Styles of your ligths from the Projector. You config the stuff with a style xml file. You are also able to record some things save them, and add it to the Style to play back the stuff when ever you want.

The third Part is the client. It's an Android app. The screen position of your fingers send to the Gui. The Gui adds the style Inforamtion, and send als things to the server. The server will Display the stuff.

Maby i put some Viedeos on Youtube the next time.


---
How to get it Work?

The whole stuff is written under Linux. I never Tested it on Windows, but it should work.

First get Qt form http://qt-project.org/downloads/ (on Linux use your package sources)
You will need the libarys. For Building i recommend the Qt-Creator.

Load the .pro files into Qt Vreator. In the Editor overview, click the rigth mousbutton on the Projects, and run both, the pa-server and the pa-gui. The Qt-Crator build the stuff for you and run it.

Next you need the Android SDK from http://developer.android.com/sdk/index.html
Load the pa-client-java and deploy to your Android device.

Connect your Device to your Wlan, and connect the Program to the PC where the pa-gui is running.
Next connect the pa-gui to the pa-server ("Datei->Connect to Beamer" i love crossover from German and English :-) i will fix it in a Later Version).
Now load on of the sample Styles(style.xml), you found in the pa-gui folder. Maybe you have to edit the path of the sample recording(rec_1.xml), also in this folder.

Thats it, now it should work.

Quit the server/bamer with alt+F4


---
Questions?

Pleas contact me per mail: andreas.gocht@gmail.com