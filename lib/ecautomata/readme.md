
# ECAUTOMATA - 2D Cellular Framework

Small and extendable framework, that is meant to be used in order to speed up the development of cellular automata by offering
functionality that is meant to be useful and is meant to remove the boilerplate associated with creating 2D CAs.

While I update the readme I must say that the documentation is still under development, and as I build projects with this
I may add more functionality overtime.

The framework is only really meant to be used by me but, if someone wishes to use it they can go to the header files and view the
objects and functionality I have defined, alongside the documentation in the form of comments.


### Examples

Projects made with the tool.

##### Terrain Generator

https://github.com/user-attachments/assets/b8eb0ab3-6e84-4589-b992-ab7c6e0cb0b7

##### Christmas Tree

https://github.com/user-attachments/assets/697087be-6e9f-43e6-96e5-b4b9e32af571

### Compile

```
  g++ -c ./ecautomata.cpp ./util/seeder.cpp ./util/grid.cpp
  ar rvs ecautomata.a ./ecautomata.o ./seeder.o ./grid.o
```
