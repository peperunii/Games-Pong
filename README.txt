This is a small console game "Pong" written on C.
Compiled succesfully using Windows 7 / Visual Studio 2013

About this game:
 - You can play as P1 (player 1) using arrows.
 - Your opponent can be a artificial inteligence - several levels of difficulty
 - Your opponent can also be a second player. His controls are 'w' and 's' keys
 - after each Hit the speed can increase. This and many other options can be found in config file


Controls:
 - arrows. 

Config file:
 - should be in the same folder as project with name "config.cfg"
 - it contains:
arraySizeX					30  //
arraySizeY					16  // game window dimensions - smaller is better
StartingBallDir 	 			4
ShowScore		 	 		1   // 0 - 9 Ball can start in each direction or 0 for random
PauseAfterGOAL	 	 			1   // pause the game after a goal for either players
PauseInMSafterGoal	 			300 // If the above option is selected the game will pause for this time in MiliSeconds
Vreditel		 	 		1   // This is an Enemy.. his place is in the middle of the screen.He is moving :)
DoubleClickSpeed	 			1   // Enable a small cheat -> move double for one arrow press
IncreaseSpeed_afterHit 				1   // (0,1)Enable or Disable : Increase speed after each Hit of the ball
IncreaseSpeed_with				5   // Increase speed after each Hit of the ball if the above option is set to '1'
Vreditel_slowSpeed				1   // if Vreditel option is selected -> choose his speed