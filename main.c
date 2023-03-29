//
//  Project 2 - Weaver Project
//  Course: CS 211, Spring 2023, UIC
//  System: CLion on macOS
//  Author: Shane Somson
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Return true or false if given string is found in string array
bool isWordInArray(char **dictionary, char *testString, int size){
    for (int i = 0; i < size; ++i) {
        if (strcmp(dictionary[i], testString) == 0){
            return true;
        }
    }
    return false;
}

// Returns a random word out of a string array
char* getRandomWord(char **dictionary, int size){
    return dictionary[rand() % size];

}

int main() {
    srand(1);
    int numberOfLetters;
    printf("Weaver is a game where you try to find a way to get from the starting word to the ending word.\n");
    printf("You can change only one letter at a time, and each word along the way must be a valid word.\n");
    printf("Enjoy!\n\n");

    bool gameIsOver = false;
    bool getNewNumWord = true;
    bool getNewWords = true;
    bool isValidStart = false;
    bool isValidEnd = false;
    char startingWord[12];
    char endingWord[12];


    char **dictionary;
    int totalDictionaryWords = 0;

    // Until User chooses option 3
    while(!gameIsOver){
        // Make a new dictionary
        if (getNewNumWord){
            totalDictionaryWords = 0;
            printf("How many letters do you want to have in the words? ");
            scanf("%d", &numberOfLetters);

            char filename[] = "words.txt";
            FILE *filePtr  = fopen(filename, "r"); // "r" means we open the file for reading

            // Check that the file was able to be opened
            if(filePtr == NULL) {
                printf("Error: could not open %s for reading\n", filename);
                exit(-1);
            }

            // Read each word from file, and print them one by one
            char inputString[numberOfLetters];

            dictionary = (char**)malloc(totalDictionaryWords * sizeof(char*));

            // Reads file data and stores in dynamic array
            while(fscanf(filePtr, "%s", inputString) != EOF) {
                // if word in file is same length as specified number, word gets added to dictionary
                if (strlen(inputString) == numberOfLetters){

                    totalDictionaryWords++;
                    dictionary = (char**)realloc(dictionary,(totalDictionaryWords) * sizeof(char*));
                    dictionary[totalDictionaryWords-1] = (char*)malloc(numberOfLetters * sizeof(char));
                    strcpy(dictionary[totalDictionaryWords-1],inputString);

                }
            }

            printf("Number of %d-letter words found: %d.\n\n", numberOfLetters , totalDictionaryWords);

            // Close the file
            fclose(filePtr);
            getNewNumWord = false;

        }

        // Getting a new starting and ending word
        if (getNewWords){
            isValidStart = false;
            isValidEnd = false;

            // Until starting word and ending word are valid
            while(!isValidStart || !isValidEnd){
                isValidStart = false;
                isValidEnd = false;
                printf("Enter starting and ending words, or 'r' for either for a random word: ");
                scanf("%s", startingWord);
                scanf("%s", endingWord);

                // Random check start
                if (strcmp(startingWord, "r") == 0){
                    strcpy(startingWord,getRandomWord(dictionary, totalDictionaryWords));
                    isValidStart = true;
                }

                // Random check end
                if (strcmp(endingWord, "r") == 0){
                    strcpy(endingWord,getRandomWord(dictionary, totalDictionaryWords));
                    isValidEnd = true;
                }

                // if words are in dictionary and are the correct length then the words are valid
                if (strlen(startingWord) == numberOfLetters){
                    if (strlen(endingWord) == numberOfLetters){
                        if (isWordInArray(dictionary,startingWord,totalDictionaryWords)){
                            isValidStart = true;
                            if (isWordInArray(dictionary,endingWord,totalDictionaryWords)){
                                isValidEnd = true;
                            }else{
                                // Not in dictionary start
                                printf("Your word, '%s', is not a valid dictionary word. Try again.\n", endingWord);
                            }
                        }else{
                            // Not in dictionary end
                            printf("Your word, '%s', is not a valid dictionary word. Try again.\n", startingWord);
                        }
                    }else{
                        // Too short end
                        printf("Your word, '%s', is not a %d-letter word. Try again.\n", endingWord, numberOfLetters);
                    }
                }else{
                    // Too short start
                    printf("Your word, '%s', is not a %d-letter word. Try again.\n", startingWord, numberOfLetters);
                }
            }
            printf("Your starting word is: %s.\n", startingWord);
            printf("Your ending word is: %s.\n", endingWord);
            getNewWords = false;
        }

        bool isWordFound = false;
        char currentWord[numberOfLetters];
        strcpy(currentWord, startingWord);

        char userGuess[12];
        int differences = 0;
        int numMoves = 0;

        printf("On each move enter a word of the same length that is at most 1 character different and is also in the dictionary.\n");
        printf("You may also type in 'q' to quit guessing.\n");

        // Guessing part of game
        while(!isWordFound){
            differences = 0;

            printf("\n%d. Previous word is '%s'. Goal word is '%s'. Next word: ", numMoves+1 ,currentWord, endingWord);
            scanf("%s", userGuess);

            // if the user wants to force quit
            if (strcmp(userGuess, "q") == 0){
                break;
            }

            if (strlen(userGuess) == numberOfLetters){
                if (isWordInArray(dictionary, userGuess, totalDictionaryWords)){
                    // Iterates through string counting differences in userGuess and currentWord
                    for (int i = 0; i < numberOfLetters; i++) {
                        if (userGuess[i] != currentWord[i]){
                            differences++;
                        }
                    }

                    if (differences == 1){
                        strcpy(currentWord, userGuess);
                        numMoves++;
                        if (strcmp(currentWord, endingWord) == 0){
                            isWordFound = true;
                            printf("Congratulations! You changed '%s' into '%s' in %d moves.\n", startingWord, endingWord, numMoves);
                        }
                    }else{
                        printf("Your word, '%s', is not exactly 1 character different. Try again.\n", userGuess);
                    }
                }else{
                    printf("Your word, '%s', is not a valid dictionary word. Try again.\n", userGuess);
                }
            }else{
                printf("Your word, '%s', is not a %d-letter word. Try again.\n", userGuess, numberOfLetters);
            }
        }

            int userChoice;
            printf("\nEnter: \t1 to play again,\n");
            printf("\t2 to change the number of letters in the words and then play again, or\n");
            printf("\t3 to exit the program.\n");
            printf("Your choice --> ");

            scanf("%d", &userChoice);

            // Play again with same # of letters
            if (userChoice == 1){
                getNewWords = true;

            }

            // Play again with new # of letters
            if (userChoice == 2){
                getNewNumWord = true;
                getNewWords = true;
                for (int i = 0; i < totalDictionaryWords; ++i) {
                    free(dictionary[i]);
                }
                free(dictionary);
            }

            // Exit program
            if (userChoice == 3){
                gameIsOver = true;
            }

        }
    printf("\nThanks for playing!\n");
    printf("Exiting...\n");

    return 0;
}
