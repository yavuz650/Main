# Snake-Game

The good-old snake game developed for Linux. Frankly,the main goal here was to practice coding. 

**What are the current features?**

- Basic snake movement.
- Collision detection.
- Food generation and growth.

**How can I try it?**

THIS PROGRAM DEPENDS ON NCURSES LIBRARY. Make sure it's installed in your machine. That is, if you want to compile the code before playing. You can also obtain the executable itself from another source if you can't/won't install ncurses.

Compile the code in your Linux terminal using the following command.(Assuming you have g++ compiler installed)

```g++ -main.cpp Engine.cpp -o snake -lncurses -pthread```

Then run by executing the command below.

```./snake```

**RULES**

- Don't collide with the walls.
- Don't collide with the snake itself.

**TO-DO LIST**

This game has a lot to improve. Here's a list of what I'll be adding in the future.

- Difficulty options.(Easy,Medium,Hard)
- Score Table. So you can see how much of a progress you've made.
- Different food types. Foods with greater growth bonus and limited time.
- Speed boost. User will be able to boost the snake's movement speed by holding a specific button.
- Option to restart the game once it ends.
- Improve snake's responsivity.

Apart from in-game improvements, I'll try to improve the code's overall readability and ,possibly, its efficiency.




