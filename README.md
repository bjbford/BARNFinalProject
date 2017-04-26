## CprE 288 Embedded Systems Final Project: Mars Rover (Spring 2017)
# BARNFinalProject

* Brian Bradford: bjbford@iastate.edu
* Andrew Thai: athai@iastate.edu
* Rajiv Bhoopala: rajiv@iastate.edu
* Nick Knuth: njknuth@iastate.edu

# Story
Briefing: VORTEX, the code name for your development platform, includes the iRobot Create, the Cerebot
II board, and attachments such as the servo, SONAR, IR sensor, and LCD.

Your team has been asked to develop an interplanetary rover using the VORTEX platform. Both NASA
and the European Space Agency have heard about your work with this platform and have assigned your
team the job of designing the rover. Delegating this task will allow these agencies to focus on developing
a launch vehicle. The primary task of the rover is retrieval; other international teams are busy working
on a package that will collect rock samples and perform analysis. The VORTEX platform will be landing in
Martian terrain that is unknown to you. The only knowledge of the terrain you will have is that which
the VORTEX transmits to you.

At this early phase of the project, the only objective for you to complete is for the VORTEX platform to
traverse the hazardous terrain for retrieval at a predetermined rendezvous site.

The terrain is hazardous, so the VORTEX must be capable of navigating the hazards of the terrain. The
retrieval zone will be marked, so you will be able to recognize it from the information the VORTEX sends
you. After the VORTEX enters the retrieval zone, it should send signals to mission control indicating the
rover is ready for retrieval.

# Part I. Communication with the VORTEX
Since the key task of this project is information collection, you will be required to implement some form
of user interface (UI) that can be used to retrieve information from the VORTEX and also to control the
VORTEX.

One example of a simple UI would be a terminal interface where mission control would send single
characters as commands to the robot. Such a terminal interface would be a text only interface. The
design of the user interface is up to you, however, it must provide for the following functionality:
Your interface should be able to show the current state of the following sensors:
* iRobot Bump Sensor (left, right)
* iRobot Cliff Sensors (left, front left, front right, right)
* iRobot Cliff Sensor Signals (left, front left, front right, right)

List of functions which you will need to be able to control remotely:
* Moving the robot
* Taking a sweep of distance measurements (should transmit to the user interface either raw data, or information about the objects detected).
