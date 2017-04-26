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

# Part II. Traversing Treacherous Terrain for Retrieval
Within the testing environment, there are hazards that will make traversing the environment
challenging. There are boulders (i.e. weighted containers), craters (i.e. removed floor tiles), and
stalagmites (i.e. pieces of PVC pipe). All hazards should be visible to at least one of the rover's sensors.

Some of the hazards are more dangerous to the rover then others. For example, falling in a crater could
potentially cripple the rover, so points will be removed from the score of this project part if the rover
falls in.

In the event the rover does fall into a crater it will be placed at the point it fell in facing perpendicular to
the crater. The iRobot portion of the VORTEX has cliff sensors, which should be checked frequently.

Bumping into a boulder is acceptable in most cases and will not result in a loss of points. However,
excessively bumping into a boulder are running into a stalagmite could become dangerous as bumping
into the stalagmite may cause it to fall onto the rover.

The Rover needs to respond quickly to the bump and cliff sensors. Because of this, the VORTEX cannot
wait for a response from the Mission Control on how to respond. Therefore, your program should have
some preprogrammed response to the bump sensor. Not responding to a bump sensor being pressed
will result in loss of points. The lack of response will be indicated by the VORTEX continuing to drive
forward while still in contact with the object. Some latency is acceptable, but after about a second the
energy loss resulting from this error could be detrimental to the mission, so points will be taken off from
this project part for each incident.

WARNING: The response of the VORTEX to the sensors should be complete. For example, if the robot
bumps into a boulder, it should not back up into a crater.

Sufficient clearance will be provided to ensure the VORTEX can traverse around obstacles.
Successful completion of this project part is determined by traversing the hazardous environment and
identifying the retrieval zone. The retrieval zone is a clear area marked off by four pillars.

Each of these pillars has distinct features that the VORTEX should be able to recognize using its sensors.
