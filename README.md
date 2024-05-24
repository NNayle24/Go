# Go AI Project

## Overview

This project is an implementation of a Go game AI developed in C. The AI uses a combination of Monte Carlo Tree Search (MCTS) and heuristic evaluations to play Go effectively against human opponents or against itself. This project was designed and developed as part of our CS curriculum by a team of four students.

## Team Members
Lex0os - CPP display & interactions  
BigDataCorrompu - Data structure  
Nnayle24 - Monte Carlo heuristics & A.I development  
Feucho - Gameplay behaviour & Project integration  

## Features  
Player VS Computer Go game using a clickable C++ interface, running on C code.  

## Installation  
Clone repository and run makefile to compile, then start output program.  

## Usage  
Player always goes first, click the intersection you'd like to take to trigger the beginning of the game.  
Pass buton requests game termination, if the opposite player doesnt agree to end, the game goes on.  
The resign button surrenders the game and ends it.  
For now, the program has to be re-started to start a new game.

## Customize  
See const.h to modify board size or amount of games searched, amout of thread can be fine-tuned carefully  
