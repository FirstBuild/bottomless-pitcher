bottomless-pitcher
==================

#### Project Details
Please see the FirstBuild website for all the details about this project @ https://firstbuild.com/JBerg/bottomless-chilled-water-pitcher/. 


#### State Machine
Using the easy to use Arduino state machine @ http://playground.arduino.cc/Code/FiniteStateMachine

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
     |                     | <------------------------------+ |                     |      
     |                     |        prox switch &             |                     |      
     |                     |        level switch deactivated  |                     |      
     |                     |                                  |                     |      
     |                     |                                  |                     |      
     +---------------------+                                  +---------------------+      
                                                                                           
                                                                                           


#### Electronics
The project is based on the ATtiny85 chip. All of the schematics can be found on upverter @ https://upverter.com/Triode/f7179c67e7e12ec7/Bottomless-Pitcher-Controller-Board/




