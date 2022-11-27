# Universal Turing Machine Simulator

![image](https://user-images.githubusercontent.com/63679072/200189750-8f22a947-2ba4-4d17-943e-45dd7a13821d.png)


## What is it?

This is a simple C project that was born as an exercise for my CS class. It replicates the behaviour of a [Universal Turing Machine](https://en.wikipedia.org/wiki/Universal_Turing_machine)


## How to use

To run the program, you can clone the repo on your local directory by using:
```
git clone https://github.com/tiz314/TuringMachine
```
Then, you can compile the program by using:
```
gcc ./main.c -o main
```
Finally, you can run the binary file. 

### Loading tape and instructions
The program accepts the initial tape content and the instructions in local files, already created in the folder `input`. There is an example program that tells you if a received word is palindrome or not by writing the result on the tape. 
You need to follow the following syntax rules for the instructions:
- To represent an empty slot on the tape, regardless of the rule in the instruction, you need to use `-`
- To give the command to exit the program, use the letter E

By now, the implementation doesn't accept status called by names, but only represented by a single character (numbers and letters)

## Credits

Thanks to [Mighty Gibbon](https://github.com/alberto-mg) for the first version of the file management and Staring Linus :| for the great idea!
