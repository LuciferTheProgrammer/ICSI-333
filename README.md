# ICSI333 - System Fundamentals  
Main applications are contained in the Projects folder.

## Words Without Friends - Web Version in C
This project is a web-based version of a word puzzle game called **Words Without Friends**, written in C. The program runs a simple web server that allows the game to be played through a browser instead of only through the terminal.
The game loads words from a dictionary file, selects a master word, and challenges the user to guess smaller words that can be formed from the letters of that master word. Correct guesses are marked as found, while missing words are displayed as blank underscores.

## Features
- Built in the C programming language
- Uses a custom socket-based web server
- Runs locally on port 8000
- Handles browser requests using HTTP
- Supports multiple client requests using POSIX threads
- Loads dictionary words from 2of12.txt
- Uses linked lists to store dictionary words and playable game words
- Displays the game using dynamically generated HTML
- Validates user guesses against the list of possible words

## Project Files
- `2of12.txt` - dictionary file used by the game
- `WordsWithoutFriends1_Rheinard_Zadanowsky.c` - early version of the terminal word game
- `WordsWithoutFriends2_Rheinard_Zadanowsky.c` - expanded version using dictionary loading and linked lists
- `WordsWithoutFriends3_Rheinard_Zadanowsky.c` - more complete terminal version of the game
- `WebServer_Rheinard_Zadanowsky.c` - standalone C web server implementation
- `WordsWithoutFriends4_Rheinard_Zadanowsky.c` - web-based version of the game

## How It Works
The program loads a word list from 2of12.txt and stores the words in a linked list. It then selects a master word and finds smaller words that can be created from the letters of that word. The web version creates an HTML page where the user can enter guesses in a form. Each guess is sent back to the C server through a browser request, checked against the game word list, and then the updated game board is displayed.

## Technologies Used
- C
- POSIX sockets
- POSIX threads
- HTML generation in C
- File I/O
- Linked lists
- String parsing
- Dictionary-based word validation

## Purpose
This project was created as a class assignment to practice systems-level programming concepts in C, including sockets, threading, memory management, file processing, linked lists, and basic web server behavior.
