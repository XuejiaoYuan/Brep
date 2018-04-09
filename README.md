# Brep
Brep data structure and euler operator. Based on C++.

# Usage
[input file name]

# File Format
#outterLoop   //outter loop's tag

0 0 0         //outter loop's vertex

0 5 0         //listed in clockwise

5 5 0

5 0 0

#innerLoop    //inner loop's tag 
4             //the vertex number of the first inner loop
1 1 0         //inner loop's vertexes are listed in counterclockwise
2 1 0
2 2 0
1 2 0
4
3 3 0
4 3 0
4 4 0
3 4 0
3
1 3 0
2 3 0
1 4 0
#direction    //direction's tag
0 0 1 2       //vector of direction and distance

# Output
This project will output a file named "testCube.brp". You can changed the file name in main.cpp.
