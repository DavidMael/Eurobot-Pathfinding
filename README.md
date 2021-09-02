# Eurobot-Pathfinding
Pathfinding Program initially written for the Imperial College Robotics Society's entry to the cancelled 2021 Eurobot competition

In the [Eurobot 2021 rules](https://www.eurobot.org/wp-content/uploads/2021/04/E2021_Rules_EN.pdf "Eurobot English rules"), robots can score points by visiting different points on the game board and performing an action specific to each location. This either means activating a device such as a lighthouse or picking up cups that can be placed in other locations for points. Points are also awarded for ending the match with the robot in a specific location which is determined at the start.

The pathfinding algorithm implemented by the GameSituation.solver() method finds the path that scores the most points from the three methods mentioned above. It uses a dynamic algorithm to consider all viable and plausible paths and return the one that scores the most points. In the case of a tie, the shortest path is taken. 

The solver function takes information on the robot, such as average speed and time to complete various actions, and information on the game, such as the between objects or group of objects or the remaining match time. The former was partially computed from an existing prototype of the robot, while the latter was meant to be computed from coordinates returned by a computer vision module, which was mostly finished before the end of the project. When all available nodes are in game and the entire match time of 100 seconds is available, the pathfinder takes 1 to 2 seconds to return a solution. This is acceptable as it is the same rate at which the computer vision module processes frames.

![game board with numbered locations](https://github.com/DavidMael/Eurobot-Pathfinding/blob/main/board.png "game board with numbered locations")