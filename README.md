bottomless-pitcher
==================

#### Project Details
Please see the FirstBuild website for all the details about this project @ https://firstbuild.com/JBerg/bottomless-chilled-water-pitcher/. 


#### Electronics
The project is based on the ATtiny85 chip. All of the schematics can be found on upverter @ https://upverter.com/Triode/f7179c67e7e12ec7/Bottomless-Pitcher-Controller-Board/

#### State Machine
Using the easy to use Arduino state machine @ http://playground.arduino.cc/Code/FiniteStateMachine



<pre>
	

                                                   level switch activated                  
                           +--------------------+                                          
                           |                    +---------------------------------+        
                           |                    |                                 |        
        prox switch activated                   |  prox switch de-activated       |        
                           |                    |                                 |        
           +-------------> |     filling        +---------------------------+     |        
           |               |                    |                           |     |        
           |               |                    |  global timeout exceeded  |     |        
           |               |                    |                           |     |        
           |               |                    +--------------------+      |     |        
           |               |                    |                    |      |     |        
           |               +--------------------+                    |      |     |        
           |                                                         |      |     |        
           |                                                         |      |     |        
           |                                                         v      v     v        
           |                                                                               
     +-----+---------------+                                  +---------------------+      
     |                     |                                  |                     |      
     |                     |                                  |                     |      
     |                     |                                  |                     |      
     |                     |                                  |                     |      
     |      standby        |                                  |     complete        |      
     |                     | &lt;------------------------------+ |                     |      
     |                     |        prox switch &             |                     |      
     |                     |        level switch deactivated  |                     |      
     |                     |                                  |                     |      
     |                     |                                  |                     |      
     +---------------------+                                  +---------------------+      
        
                                                                                   
                                                                                           
</pre>

#### Wiring Diagram

<pre>
                                         +----------------------------------------+                                     
         +------------+                  |                                        |                                     
         |            |                  |                                    J3  +-------------------+A/C IN - L1      
+--------+  Proximity +------------------+ J1 Pin 1                               |                                     
|        |  Reed SW   |                  |                                        |                                     
|        +------------+                  |                                    J7  +-------------------+A/C IN - Neutral 
|                                        |           Controller PCB               |                                     
|        +------------+                  |                                        |                                     
|        | Float      |                  |                                        |                                     
+--------+ Reed SW    +------------------+ J1 Pin 2                               |                                     
|        |            |                  |                                        |                    +---------------+
|        +------------+                  |                                    J4  +--------------------+               |
|                                        |                                        |                    |  Water Valve  |
+----------------------------------------+ J1 Pin 4                           J5  +--------------------+               |
                                         |                                        |                    |               |
                                         +----------------------------------------+                    +---------------+

</pre>

### Fuse settings for ATTINY85
low_fuses=0xe2

high_fuses=0xdf

extended_fuses=0xff

