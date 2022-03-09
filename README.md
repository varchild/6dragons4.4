This is a README/Help file for the files in this reposity.

This is a version of 6dragons v4.4 which has been modified to compile and run on a 64-bit Linux operating system
installed on a Rapsberry Pi. 
These will compile and operate just fine on a Raspberry 3/4.

How to compile on Ubuntu:

After cloning the git reposity navigate into the /src/ directory and take "make clean".
This will compile the files and create the executable.
There will be warnings displayed during the compiling process.

After the compiling has completed, to execute the MUD program, 
type ./6dragons on the command line in the directory above the /src/ directory.
One 
6dragons uses the port 4100.

General tips for running the MUD:
nohup ./6dragons <selected port> &

Create your first account that you will use to administer the MUD. 
After character creation be sure to SAVE your character and then log out.

Navigate to the player directory, and then further into the letter of your character's name.
Modify your character's file and edit the level to 108 to be able to access 
full administrative and building functions of the MUD.
  
