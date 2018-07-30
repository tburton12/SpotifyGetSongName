This program is part of project created to display name of current playing song on Spotify on Arduino Display.
It is consisted of three main parts. 
First one is this program. 
It gets window handler of Spotify, gets window description (that is name of currnet playing song) and save it to file.
According to a fact that Spotify has non-static window name, we are looking for a specific description among all of active windows to distinguish it.
We can predict it because it is always "Spotify" when music is paused. That's why we need to pause music for a moment to obtain window handler.
This program runs in a backgroud, that's why we always checks if mother exe (it is the second part of project) is working. If it's not, then we quit.

The second part is a program with GUI, that is written in C#. We use it to connect with Arduino. 
It reads songs name from file and sends it directly to Arduino via USB bus.
It opens the first program automatically.

The third one is Arduino part.

Author: Paweł Wrzesień
