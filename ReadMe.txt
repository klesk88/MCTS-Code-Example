========================================================================
   CONSOLE APPLICATION: CodeExample
========================================================================

@Author: Michele Ermacora

This is a C++ code sample build with Visual Studio 2012. It consist in a game in which a prisoner 
(represented by the character P in the game) need to escape using the exit (represented by
 the character E) whithout being captured by the guard (represented by the character G). 
The positions of the prisoner, guard, exit and obstacles are randomly placed at the start of the game. 
The logic of both the prisoner and the guard is implemented using Monte Carlo Tree Search (MCTS)
with UCT formula. 
Because is just a sample code for show my knowledge of C++, the MCTS of both entities will 
apply only basic logic. Instead, I apply different kind of object-oriented programming
concepts (such as inheritance) to give an idea to the reader of what I am able  to do with C++. 
I followed a light version of the Hungarian C++ style guide during the development of the code.
The code will follow these steps:
- The main will call the GameLoop method of the Game class in which is contained all the 
  information about the game, such as players and the level itself.
- The game loop then will call the update methods of the two entities every second
- The update method of each entity will call it's personal instance of the MCTS for find the
  next move to perform.
- Finally the game displayed on the console, will be updated. The process will continue
  until or the prisoner escape, or the guard catch him, or one of them surrenders.
  
Each Header file will contatin an explanation of what the class is supposed to do, as well
as the explanation of the methods, if needed.
