# binary tree guessing game in C++

This is a guessing game, similar to [twenty questions](https://en.wikipedia.org/wiki/Twenty_questions), where the user thinks of an animal and the program asks a series of yes/no questions to figure it out. If the program guesses incorrectly, it prompts the user to come up with a question it could ask to differentiate the wrong guess from the answer; with each game, the program's knowledge increases.

Behind the scenes, the "animal knowledge" is represented as a full binary tree. Each node is either a question or a guess, and each question node points to one lower node for "yes" and another for "no". The leaves of the tree are guesses. The program starts by loading knowledge from a serialized database ([`animals.txt`](./animals.txt)), and when the program is over, it serializes the binary tree again and updates the file.

## output example

```
Loading knowledge database from "animals.txt".
Think of an animal, then press enter to continue.

Is your animal a mammal? [y/n] y
Does your animal ever have to program in C++? [y/n] n
Does your animal ever bark? [y/n] n
Does your animal live in the water? [y/n] n
Does your animal purr? [y/n] n
Does your animal produce milk? [y/n] n
Are you thinking of a sheep? [y/n] n
Darn! I couldn't think of your animal. What was it?
squirrel
Ohhh, tricky! What's a good yes-or-no question I could use to differentiate a squirrel from a sheep?
Does your animal live in trees?
And what is the answer to "Does your animal live in trees?" for a squirrel? [y/n] y
Do you want to play again? [y/n] n

Thanks for playing! Goodbye :)
```

## database snippet

```txt
Q Is your animal a mammal?
	Q Does your animal ever have to program in C++?
		G human
		Q Does your animal ever bark?
			Q Does your animal live in the water?
				G sea lion
				G dog
			G dolphin
	Q Is your animal a reptile?
		Q Does your animal have legs?
			G lizard
			G snake
		G bee
```

## notes

Compiled with `g++ -std=c++17 -include Romans_toolbox.hpp -include Romans_toolbox.cpp -o main main.cpp` on Linux.

## special features
	- [Custom header file](./Romans_toolbox.hpp) for common functions
	- Program is customizable: designer can modify it from guessing animals to movies, for example
	- Dynamically uses "a" or "an" based on the proceeding noun instead of writing a(n)
	- Some measures to prevent unusual responses to [y/n] questions from breaking things
	- Human-readable database format using indents
