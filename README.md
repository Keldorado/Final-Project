#Ableton Face Tracker

This is my Advanced Coding final project. 

##My Background 

I am passionate about music and have a background in software development. In an effort to fuse these areas of interest, I am motivated in empowering musicians through software developments.

##The Concept
The goal of designing this program is to empower musicians to think outside of traditional instrumentation and to provide musicians with the ability to control music through an alternative medium; freeing them from the limitations of traditional instrumentation and providing them the ability to manipulate sound waves in a hands-free manner.

This program allows multiple users to interact with Live with their faces. 

##How it Works

Xcode sends OSC messages to OSCulator and then the OSCulator converts those OSC messages into MIDI messages so that Live may understand them 

##How Do I Use it

#####The Things You Will Need

	1)Xcode 
    2)OSCulator
    3)Ableton Live

Once you have these items downloaded we are good to go. Open the Xcode file and look towards the top of the .h file. It should say PORT 3333. Good. Now open OSCulator. To the left of the green button on OSCulator, replace what is in the "OSC Input Port" with the number 3333. This is how we are going to get Xcode to talk to the OSCulator.

Once you have done that, then run the Xcode program. 7 rows of information should appear on the OSCulator. Once you have done that, change the Event Types to MIDI CC, all the values to 0, and give each row their their own respective channel number. See the image below for visual. 

  
![Screen Shot 2015-06-01 at 12.34.53 AM.png](/Users/austinkelley/Documents/Developer/of_v0.8.4_osx_release/apps/myApps/FaceOscTracker/Assets/Screen Shot 2015-06-01 at 12.34.53 AM.png)

Then, to access the input pane, either keycombo command-f or click the "secret arrow" near the top right of the OSCulator. See image below for visual.


![Screen Shot 2015-06-01 at 1.04.25 AM.png](/Users/austinkelley/Documents/Developer/of_v0.8.4_osx_release/apps/myApps/FaceOscTracker/Assets/Screen Shot 2015-06-01 at 1.04.25 AM.png)

Then change the In.Max to 1000 and the Out. mac to 10. These values will depend on what you want to manipulate in Live but for my project these values seemed to work well. Now uncheck all of the blue boxes in OSCulator. (These boxes tell OSCulator to send messages to our designed port. Why we unchecked them will become clear in a moment)

Then start Ableton live and go to the settings pane. And these are the settings in Ableton live for communicating with Osculator and Ableton live. 

![687474703a2f2f7777772e626a656c6d2e636f6d2f67697453747566662f6162656c746f6e4d69646953657475702e6a7067.jpeg](/Users/austinkelley/Documents/Developer/of_v0.8.4_osx_release/apps/myApps/FaceOscTracker/Assets/687474703a2f2f7777772e626a656c6d2e636f6d2f67697453747566662f6162656c746f6e4d69646953657475702e6a7067.jpeg)


Then load an instrument. I used the Off World Strings but you can choose any plug in you want. Double click on your instrument to load it into the Live session. 

After this you have go into Midi mapping mode on Ableton. This allows you to map the midi signals coming from Osculator to what you want to happen in Ableton live. To go into Midi mapping mode, click the MIDI button in the top right corner of Live. Now click the part of Live you wish to map. Then go back into OSCulator and check the box next to /face/area0/. Now you should see something like this in the Midi Mappings pane.


![Screen Shot 2015-06-01 at 1.28.16 AM.png](/Users/austinkelley/Documents/Developer/of_v0.8.4_osx_release/apps/myApps/FaceOscTracker/Assets/Screen Shot 2015-06-01 at 1.28.16 AM.png)

Then unclick the MIDI button to store your mapping in Live. Now, while Xcode is running, you can control Live with your face. 

Repeat these same steps to other parts of Live to control them with **your** face. If you want other people to help you manipulate Live with **their** faces, repeat these steps to /face/area1/ and /face/area2/ and so on. Once you have them mapped to differnt faces within Live, make sure you have all the blue boxes checked so OSCulator knows that the messages from Xcode need to be sent to Live. 



