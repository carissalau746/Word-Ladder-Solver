/*-------------------------------------------
Program 4: Shortest Word Ladder
	Given user choice of word size and two specific
    words, find the shortest "word ladder" (words connected by
    one letter differences) to connect both words.
Course: CS 211, Spring 2025, UIC
Author: Carissa Lau
------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

typedef struct WordNode_struct {
    char* myWord;
    struct WordNode_struct* next; 
} WordNode;

typedef struct LadderNode_struct {
    WordNode* topWord;
    struct LadderNode_struct* next; 
} LadderNode;

// Calculates number of words of wordSize length in given file.
// Parameter inputs:
//  filename - name of file
//  wordSize - length of the word
// returns total number of words counted 
// returns -1 if file could not be opened
int countWordsOfLength(char* filename, int wordSize) { 
    //Open file
    FILE* myFile = fopen(filename, "r");
    //Check if file was successfully opened
    if(myFile == NULL)
        return -1;
    char word[100];
    int count = 0;
    //Parse file and count number of words that have target size
    while(fscanf(myFile, "%s", word) == 1) {
        if(strlen(word) == wordSize)
            count++;
    }
    //Finished, close the file
    fclose(myFile);
    return count; 
}

// Builds word array of all words of length wordSize in file.
// Parameter inputs:
//  filename - name of file
//  words - word array 
//  numWords - number of words of length wordSize in file
//  wordSize - chosen word length
// returns true if successfully read words of wordSize into words array
// returns false if file could not be opened or did not read all correct words
bool buildWordArray(char* filename, char** words, int numWords, int wordSize) { 
    //Open file
    FILE* myFile = fopen(filename, "r");
    //Check if file was successfully opened
    if(myFile == NULL)
        return false;
    char word[100];
    int count = 0;
    //Parse file and add words of wordSize to words array
    while(fscanf(myFile, "%s", word) == 1) {
        if(strlen(word) == wordSize) {
            strcpy(words[count], word);
            //Go to next empty spot in words array
            count++;
        }
    }
    //Finished, close the file
    fclose(myFile);
    //Check if the number of words inserted into words array actually matches 
    //the number of words of wordSize in the file
    if(numWords != count)
        return false;
    //Successfully read all words of wordSize in file into the words array
    return true;
}

// Finds index of chosen word in word array using binary search.
// Parameter inputs:
//  words - word array to search in
//  aWord - target word to find
//  loInd - lower bound for finding the index
//  hiInd - upper bound for finding the index
// returns index of aWord if aWord was found
// returns -1 if aWord was not found
int findWord(char** words, char* aWord, int loInd, int hiInd) { 
    //While the upper bound is greater than lower bound
    while(hiInd >= loInd) {
        //Find the middle of the bounds
        int mid = (hiInd + loInd) / 2;
        //Change lower/upper bound depending on whether aWord is 
        //alphabetically greater/less than middle word
        if(strcmp(words[mid], aWord) < 0)
            loInd = mid + 1;
        else if(strcmp(words[mid], aWord) > 0)
            hiInd = mid - 1;
        else //Found aWord
            return mid;
    }
    //aWord not found, return -1
    return -1; 
}

//Frees all components of a word array.
//Parameter inputs:
//  words - word array
//  numWords - number of words in word array
void freeWords(char** words, int numWords) {
    //Free each word (char array) in word array
    for(int i = 0; i < numWords; i++) {
        free(words[i]);
    }
    //Free the word array itself
    free(words);
}

//Inserts a word at the front of a word ladder.
//Parameter inputs:
//  ladder - linked list of WordNodes connected through neighbors
//  newWord - new word to be inserted in front of word ladder
// Outputs: updates the ladder head node
void insertWordAtFront(WordNode** ladder, char* newWord) {
    //Instantiate a new node
    WordNode* newNode = (WordNode*)malloc(sizeof(WordNode));
    //Set data of the new node to the given word
    newNode->myWord = newWord;
    //Set the new node to point to the head of list
    newNode->next = *ladder;
    //Make the new node the head of the list
    (*ladder) = newNode;
}

//Calculates and returns the number of elements (height) in given ladder.
//Parameter inputs:
//  ladder - linked list of WordNodes connected through neighbors
// returns the height of ladder
int getLadderHeight(WordNode* ladder) {
    //Start the count at 0
    int count = 0;
    //For each node in the linked list, add 1 to count
    while(ladder != NULL) {
        count++;
        ladder = ladder->next;
    }
    //Return number of counted nodes
    return count;
}

//Creates and returns a copy of a ladder given another ladder.
//Parameter inputs:
//  ladder - linked list of WordNodes connected through neighbors
// returns the head of the new copy ladder
WordNode* copyLadder(WordNode* ladder) {
    //If the given ladder is empty
    if(ladder == NULL) //Return an empty ladder
        return NULL;
    //Create a new head
    WordNode* newHead = (WordNode*)malloc(sizeof(WordNode));
    //Set the data of the new head equal to data of other head
    newHead->myWord = ladder->myWord;
    //Set the new head's next to point to NULL
    newHead->next = NULL;

    WordNode* curr = newHead;
    //For each Node besides the head in other list
    while(ladder->next != NULL) {
        //Create a linked new Node 
        curr->next = (WordNode*)malloc(sizeof(WordNode));
        //with same data as other Node
        curr->next->myWord = ladder->next->myWord;
        //and set new Node to point to NULL
        curr->next->next = NULL;
        //Continue traversing
        curr = curr->next;
        ladder = ladder->next;
    }
    //Return head of copy ladder
    return newHead;
}

//Frees all components of a word ladder.
//Parameter inputs:
//  ladder - linked list of WordNodes connected through neighbors
void freeLadder(WordNode* ladder) {
    //If node does not exist, do not free
    if(ladder == NULL) 
        return;
    //Recursive call to free the next node
    freeLadder(ladder->next);
    //Free the current node
    free(ladder);
}

//Insert a node at the back of a Ladder list.
//Parameter inputs:
//  list - linked list of LadderNodes
//  newLadder - word ladder to be added to list
void insertLadderAtBack(LadderNode** list, WordNode* newLadder) {
    //Allocate memory for a new node
    LadderNode* newNode = (LadderNode*)malloc(sizeof(LadderNode));
    //Set values of the new node
    newNode->topWord = newLadder;
    newNode->next = NULL;

    //If the list is empty, set new Node as head and stop
    if(*list == NULL) {
        *list = newNode;
        return;
    } 
    LadderNode* curr = *list;
    //Find the end of the list
    while(curr->next != NULL) {
        curr = curr->next;
    }
    //Attach new node to end of the list
    curr->next = newNode;
    return;
}

//Remove a ladder from the front of a Ladder list.
//Parameter inputs:
//  list - linked list of LadderNodes
// returns the ladder stored in LadderNode at front of the ladder list
WordNode* popLadderFromFront(LadderNode** list) {
    //Store the Ladder contained within the LadderNode at front of list
    WordNode* wordNodeTemp = (*list)->topWord;
    //Free the LadderNode at front of list
    LadderNode* temp = *list;
    *list = (*list)->next;
    free(temp);
    //Return the stored ladder
    return wordNodeTemp;
}

//Frees all components of a Ladder list.
//Parameter inputs:
//  myList - linked list of LadderNodes
void freeLadderList(LadderNode* myList) {
    //If node does not exist, do not free
    if(myList == NULL)
        return;
    //Recursive call to free the next node
    freeLadderList(myList->next);
    //Free the ladder contained within the LadderNode
    freeLadder(myList->topWord);
    //Free the current node
    free(myList);
}

//Finds the shortest word ladder between two given words.
//Parameter inputs:
//  words - word array
//  usedWord - boolean array that represents if each word has been used
//  numWords - number of words in word array
//  wordSize - length of the two given words
//  startWord - starting word of ladder
//  finalWord - final word of ladder to be linked
// returns the shortest word Ladder
// returns NULL if word ladder not possible
WordNode* findShortestWordLadder(   char** words, 
                                    bool* usedWord, 
                                    int numWords, 
                                    int wordSize, 
                                    char* startWord, 
                                    char* finalWord ) {
    //Instantiate empty list and empty word ladder
    LadderNode* myList = NULL;
    WordNode* myLadder = NULL;
    //Prepend the starting word to the ladder
    insertWordAtFront(&myLadder, startWord);
    //Append the ladder to the end of the list
    insertLadderAtBack(&myList, myLadder);
    //while myList is not empty
    while(myList != NULL) {
        //pop laddernode off head and call it myLadder
        WordNode* myLadder = popLadderFromFront(&myList);
        //Future neighbor of word
        char neighbor[wordSize+1];
        //Copy the current word in dictionary to neighbor
        strcpy(neighbor, myLadder->myWord);

        //For each letter in current word
        for(int i = 0; i < wordSize; i++) {
            //Keep track of the original letter
            char original = neighbor[i];

            //For each letter in the alphabet
            for(char c = 'a'; c <= 'z'; c++) {
                //If current letter matches original, do not create a neighbor
                if(original == c)
                    continue;
                //Change 1 letter of the currWord to make it a neighbor
                neighbor[i] = c;
                //Get the neighbor word's index
                int neighborIndex = findWord(words, neighbor, 0, numWords-1);
                //If neighbor is in dictionary and has not been used
                if(neighborIndex != - 1 && !usedWord[neighborIndex]) {
                    //Note that it is now used
                    usedWord[neighborIndex] = true;
                    //If the neighbor matches the final word, found the shortest ladder!
                    if(strcmp(neighbor, finalWord) == 0) {
                        //Insert the word into the ladder
                        insertWordAtFront(&myLadder, finalWord);
                        //Free the list of ladders
                        freeLadderList(myList);
                        //Return the shortest word ladder
                        return myLadder;
                    } //Neighbor did not match the final word, but was a valid word
                    else { 
                        //Create a new copy of the ladder with neighbor attached
                        WordNode* anotherLadder = copyLadder(myLadder);
                        insertWordAtFront(&anotherLadder, words[neighborIndex]);
                        //Add it to the list of ladders
                        insertLadderAtBack(&myList, anotherLadder);
                    }
                }
            }
            //Modify the word back to its original
            neighbor[i] = original;
        }
        //Free current word ladder after use (unsuccessful ladder)
        freeLadder(myLadder);
    }
    //Shortest word ladder not possible, return NULL
    return NULL;
}

// interactive user-input to set a word; 
//  ensures the word is in the dictionary word array
void setWord(char** words, int numWords, int wordSize, char* aWord) {
    bool valid = false;
    printf("  Enter a %d-letter word: ", wordSize);
    int count = 0;
    while (!valid) {
        scanf("%s",aWord);
        count++;
        valid = (strlen(aWord) == wordSize);
        if (valid) {
            int wordInd = findWord(words, aWord, 0, numWords-1);
            if (wordInd < 0) {
                valid = false;
                printf("    Entered word %s is not in the dictionary.\n",aWord);
                printf("  Enter a %d-letter word: ", wordSize); 
            }
        } else {
            printf("    Entered word %s is not a valid %d-letter word.\n",aWord,wordSize);
            printf("  Enter a %d-letter word: ", wordSize);
        }
        if (!valid && count >= 5) { //too many tries, picking random word
            printf("\n");
            printf("  Picking a random word for you...\n");
            strcpy(aWord,words[rand()%numWords]);
            printf("  Your word is: %s\n",aWord);
            valid = true;
        }
    }
}

// helpful debugging function to print a single Ladder
void printLadder(WordNode* ladder) {
    WordNode* currNode = ladder;
    while (currNode != NULL) {
        printf("\t\t\t%s\n",currNode->myWord);
        currNode = currNode->next;
    }
}

// helpful debugging function to print the entire list of Ladders
void printList(LadderNode* list) {
    printf("\n");
    printf("Printing the full list of ladders:\n");
    LadderNode* currList = list;
    while (currList != NULL) {
        printf("  Printing a ladder:\n");
        printLadder(currList->topWord);
        currList = currList->next;
    }
    printf("\n");
}


//-----------------------------------------------------
// The primary application is fully provided in main(); 
//  no changes should be made to main(), except for 
//  testing-related purposes (such as command-line 
//  arguments for "TESTING MODE" to call a test case 
//  master function, or something similar)
//-----------------------------------------------------
int main() {
    srand((int)time(0));
    
    printf("\nWelcome to the CS 211 Word Ladder Generator!\n\n");

    // set word length using interactive user-input
    int wordSize;
    printf("Enter the word size for the ladder: ");
    scanf("%d",&wordSize);
    printf("\n");

    printf("This program will make the shortest possible\n"); 
    printf("word ladder between two %d-letter words.\n\n",wordSize);
    
    // interactive user-input sets the dictionary file;
    //  check that file exists; if not, user enters another file
    //  if file exists, count #words of desired length [wordSize]
    char dict[100];
    printf("Enter filename for dictionary: ");
    scanf("%s", dict);
    printf("\n");
    int numWords = countWordsOfLength(dict,wordSize);
    while (numWords < 0) {
        printf("  Dictionary %s not found...\n",dict);
        printf("Enter filename for dictionary: ");
        scanf("%s", dict);
        printf("\n");
        numWords = countWordsOfLength(dict,wordSize);
    }
    
    // allocate heap memory for the word array; only words with desired length
    char** words = (char**)malloc(numWords*sizeof(char*));
    for (int i = 0; i < numWords; ++i) {
        words[i] = (char*)malloc((wordSize+1)*sizeof(char));
    }

    // end program if file does not have at least two words of desired length
    if (numWords < 2) {
        printf("  Dictionary %s contains insufficient %d-letter words...\n",dict,wordSize);
        printf("Terminating program...\n");
        return -1;
    }
    
    // [usedWord] bool array has same size as word array [words];
    //  all elements initialized to [false];
    //  later, usedWord[i] will be set to [true] whenever 
    //      words[i] is added to ANY partial word ladder;
    //      before adding words[i] to another word ladder,
    //      check for previous usage with usedWord[i] 
    bool* usedWord = (bool*)malloc(numWords*sizeof(bool));
    for (int i = 0; i < numWords; ++i) {
        usedWord[i] = false;
    }
    
    // build word array (only words with desired length) from dictionary file
    printf("Building array of %d-letter words... ", wordSize); 
    bool status = buildWordArray(dict,words,numWords,wordSize);
    if (!status) {
        printf("  ERROR in building word array.\n");
        printf("  File not found or incorrect number of %d-letter words.\n",wordSize);
        printf("Terminating program...\n");
        return -1;
    }
    printf("Done!\n"); 

    // set the two ends of the word ladder using interactive user-input
    //  make sure start and final words are in the word array, 
    //  have the correct length (implicit by checking word array), AND
    //  that the two words are not the same
    char startWord[30];
    char finalWord[30];
    printf("Setting the start %d-letter word... \n", wordSize);
    setWord(words, numWords, wordSize, startWord);
    printf("\n");
    printf("Setting the final %d-letter word... \n", wordSize);
    setWord(words, numWords, wordSize, finalWord);
    while (strcmp(finalWord,startWord) == 0) {
        printf("  The final word cannot be the same as the start word (%s).\n",startWord);
        printf("Setting the final %d-letter word... \n", wordSize);
        setWord(words, numWords, wordSize, finalWord);
    }
    printf("\n");

    // run the algorithm to find the shortest word ladder
    WordNode* myLadder = findShortestWordLadder(words, usedWord, numWords, wordSize, startWord, finalWord);

    // display word ladder and its height if one was found
    if (myLadder == NULL) {
        printf("There is no possible word ladder from %s to %s\n",startWord,finalWord);    
    } else {
        printf("Shortest Word Ladder found!\n");
        printLadder(myLadder);
    }
    printf("Word Ladder height = %d\n",getLadderHeight(myLadder));
    
    // free the heap-allocated memory for the shortest ladder
    freeLadder(myLadder);
    // free the heap-allocated memory for the words array
    freeWords(words,numWords);
    free(usedWord);
    
    return 0;
}
