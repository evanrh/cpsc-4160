# Assignment 1
### Evan Hastings
### CPSC 6160 Spring 2021

## Usage
`make run`

This will compile the code into its respective directories and run the program.

## Description
For this assignment, I chose to use an image of a trombone on a plain two-tone background.
I did not really feel very comfortable doing anything fancy with the background, so that is why I chose a simple one.
As for the animation, I thought it would be fun to make the image move around randomly.
I decided it would be best if the image moved across the screen horizontally at a fixed rate, and I made its vertical movement random.
The vertical movement is determined from a uniform distribution of numbers from *[-10,10]*, which makes it move up and down nicely.
Aside from motion, there is an SDL_Rect that makes the colored area in the middle of the screen.
I went with a light blue color, because I like blue.
