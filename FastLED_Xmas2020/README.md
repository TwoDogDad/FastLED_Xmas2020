# FastLED_Xmas2020
WS2811 strips in windows to show Christmas themed colors.

https://youtu.be/gBm1sgy9fZg

This is my first github repository, so I'm still trying to figure it out, and in case anyone wants to make it better, be my guest. 
This is obviously free to use since the majority of the code is ColorPalette from the FastLED example list. 

My goal for this project was to put my WS2811 led strips in my windows on the front of my house and have a little light show. 
But as I was developing the idea I quickly ran in to a fun problem to solve. 
How do I sync all the windows? I know damn well my wife will not allow me to run wire to each one room and I don't want wires on the outside of my house. 

Here comes RF24 and the little nRF24L01 modules. 
I used an ESP to connect to the internet to get the time so I can set my schedule. 
I then connected the RF24 to the ESP to send a counter to each Arduino Nano controlling the strips. 
I sent a variable that counts because the ColorPalette used a for loop to cycle through the palettes. 
This would be the easiest way to implement a single source for what tells the Nano's to change to the next pattern. 
As for coding the Nano's it was pretty straight forward except for I couldn't get the second string to be a copy of the fist. 
So, shoutout to u/Marmillicious for helping me with some quick math! 

During installation I realized that I would needed to to shorten my strips, so I figured out that in the same code that Marc helped me with I cold subtract the number of nodes that I didn't need on from the NUM_LEDS. 

After a couple of iterations and revisions related to missing RF signal and trying to increase the range I finished and everything is working. 
Fun fact, to increase the range, lower the data rate, and don't forget to find a channel with low activity. 
The LEDs turn on when the Christmas lights turn on and then they run for two cycles at the top of every hour. 
Thanks for an awesome community and through documentation! 


