/* ---------------------------------------------
Program 3: Best Wordle Word
  Find the best word that matches with a variety
  other words as well as the second in line best
  word after the first best word has been applied
  in a wordle like game

Course: CS 211, Fall 2022. Tues 12pm lab
System: Windows using Repl.it
Author: John Ho
---------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>   // for exit( -1)
#include <string.h>


struct wordAndScore {
  char word[6]; // 6 spaces for null character at end
  int score;
};

//-----------------------------------------------------------------------------------------
// Comparator for use in built-in qsort(..) function.  
// Parameters are declared to be a
// generic type, so they will match with anything.
// This is a two-part comparison.  First the scores are 
// compared.  If they are the same,
// then the words themselves are also compared, so that the results are in descending
// order by score, and within score they are in alphabetic order.
int compareFunction( const void * a, const void * b) {
    // Before using parameters we have cast them into the actual type they are in our program
    // and then extract the numerical value used in comparison
  int firstScore = ((struct wordAndScore*) a)->score;
  int secondScore = ((struct wordAndScore *) b)->score;

    // If scores are different, then that's all we need for our comparison.
    if (firstScore != secondScore) {
        // We reverse the values, so the result is in descending vs. the otherwise ascending order
        // return firstScore - secondScore;   // ascending order
        return secondScore - firstScore;      // descending order
    }
    else {
        // Scores are equal, so check words themselves, to put them in alphabetical order
	  // Hint: return the value of strcmp( parameter a word,  parameter b word)
        return strcmp( ((struct wordAndScore *) a)->word,((struct wordAndScore *) b)->word);
    }
} //end compareFunction()


// Calculates score for a single word within the entire answersFile, takes in an inputWord, the answersArray, and counter
int calculateScore(struct wordAndScore inputWord,struct wordAndScore* allWords, int count){

  int score =0;
  char inputStrCopy[6];
  char guessStrCopy[6];
  strcpy(inputStrCopy, inputWord.word);

  for (int i = 0; i<count; i++){ // for ever word
    strcpy(guessStrCopy, allWords[i].word);
    strcpy(inputStrCopy, inputWord.word);
    for (int k = 0; k<5; k++){ // for every letter and specific position
      if(inputStrCopy[k]==guessStrCopy[k]){
        score+=3;
        guessStrCopy[k]='-';
        inputStrCopy[k]='_';
      }
    }

    for (int j = 0; j<5; j++){// for every letter of the inputWord
      for(int x = 0; x<5; x++){// for every letter of the compared word
        if (inputStrCopy[j]==guessStrCopy[x]){
          score+=1;
          inputStrCopy[j]='_';
          guessStrCopy[x]='-';
        }
      }
    }
  }
  
  
  return score;
}//end of calculateScore


// Processes all of the letters out of one word for entire array, takes in the inputWord, array of allSecondWords and counter
void applyFirstWord(struct wordAndScore inputWord, struct wordAndScore *secondAnswerWords, int counter){

  char inputStrCopy[6];
  
  for (int i = 0; i<counter; i++){ // for ever word
    strcpy(inputStrCopy, inputWord.word);
    for (int k = 0; k<5; k++){ // for every letter and specific position
      if(inputStrCopy[k]==(secondAnswerWords[i].word)[k]){
        (secondAnswerWords[i].word)[k]='-';
        inputStrCopy[k]='_';
      }
    }

    for (int j = 0; j<5; j++){// for every letter of the inputWord
      for(int x = 0; x<5; x++){// for every letter of the compared word
        if (inputStrCopy[j]==(secondAnswerWords[i].word)[x]){
          inputStrCopy[j]='_';
          (secondAnswerWords[i].word)[x]='-';
        }
      }
    }
  }
}// end of applyFirstWord()


//Puts strings into a pre made array, takes in length, destination array and source array
void applyStringsIntoArray(int length, struct wordAndScore* destArray, struct wordAndScore *sourceArray){
  for (int i = 0; i<length; i++){
    strcpy(destArray[i].word, sourceArray[i].word);
  }
}


// -----------------------------------------------------------------------------------------
int main() {
    char answersFileName[81];  // Stores the answers file name
    char guessesFileName[81];  // Stores the guesses file name
    int fileContentCountAnswers=0;    // Stores the amount of strings in a file
    int fileContentCountGuesses=0; // Stores the amount of strings in the guesses file
    int counter=0; // for adding elements to arrays and doubles as size of the dynamic array

    // Set default file names
    strcpy(answersFileName, "answersTiny.txt");
    strcpy(guessesFileName, "guessesTiny.txt");

    printf("Default file names are %s and %s\n", answersFileName, guessesFileName);

    // Display menu, to allow partial credit for different program components
    int menuOption = 0;
    do {
        printf("\n");
        printf("Menu Options:\n");
        printf("  1. Display best first words only\n");
        printf("  2. Display best first and best second words\n");
        printf("  3. Change answers and guesses filenames\n");
        printf("  4. Exit\n");
        printf("Your choice: ");
        scanf("%d", &menuOption);

        // Handle menu option 3 to exit the program
        if (menuOption == 4) {
            exit(1); // Exit the program
        } else if (menuOption == 3) {
            // Change file names.  Menu will then be redisplayed.
            printf("Enter new answers and guesses filenames: ");
            scanf("%s %s", answersFileName, guessesFileName);

        }
    } while( menuOption == 3); // program only runs once

    // Sample code to read in from a file
    FILE *inFilePtr  = fopen(answersFileName, "r");  // Connect logical name to filename
    char inputString[ 81];

    // Sanity check: ensure file open worked correctly
    if( inFilePtr == NULL ) {
        printf("Error: could not open %s for reading\n", answersFileName);
        exit(-1);    // must include stdlib.h
    }

    // Read each word from file and print it.  You could do other things along the
    // way, like counting how many words there are.

    // answers file pass through
    while( fscanf( inFilePtr, "%s", inputString) != EOF) {
        fileContentCountAnswers++;
    }

    // Close the file
    fclose( inFilePtr);

    printf("%s has %d words \n", answersFileName, fileContentCountAnswers);
  
    // Repeat file read sequence for guesses file

    inFilePtr = fopen(guessesFileName, "r"); 

    // Sanity check
    if ( inFilePtr ==NULL){
      printf("Error: could not open %s for reading\n", answersFileName);
      exit(-1);
    }

    while( fscanf( inFilePtr, "%s", inputString) !=EOF){
      fileContentCountGuesses++;
    }

    fclose( inFilePtr);
    printf("%s has %d words \n", guessesFileName, fileContentCountGuesses);

    struct wordAndScore *allWords;
    struct wordAndScore *answerWords;

    allWords = (struct wordAndScore*)malloc((fileContentCountGuesses+fileContentCountAnswers)*sizeof(struct wordAndScore));
    answerWords = (struct wordAndScore*)malloc(fileContentCountAnswers*sizeof(struct wordAndScore));

  
    // CREATE 2D char* array
    inFilePtr=fopen(answersFileName,"r");

    // 2D char* array addition for answersFile as well as creating the answers array
  
    while ( fscanf( inFilePtr, "%s", inputString) !=EOF){ // loop through elements of answers file until end of file
      inputString[strlen(inputString)]='\0'; // add null character at the end of string in case

      // put strings into word arrays
      strcpy(answerWords[counter].word, inputString);
      strcpy(allWords[counter].word, inputString);
      
      counter++; // increment counter for guesses array addition use
    }
    
    fclose( inFilePtr);

    //2D char* array addition for guessesFile
    inFilePtr=fopen(guessesFileName, "r");

    while (fscanf(inFilePtr, "%s", inputString)!=EOF){
      inputString[strlen(inputString)]='\0';
      // allWords[counter]=(struct wordAndScore*)malloc(strlen(inputString)*sizeof(struct wordAndScore));
      strcpy(allWords[counter].word, inputString);
      counter++;
    }
  fclose(inFilePtr);

  // create score for the first best word array
  int tempScore;
  for (int i = 0; i<counter; i++){
    tempScore = calculateScore(allWords[i], answerWords, fileContentCountAnswers);
    allWords[i].score=tempScore;
  }

  //sorting list
  qsort( allWords, counter, sizeof(struct wordAndScore), compareFunction);

  printf("\n");

  //menu option 1 and 2 handling
  if (menuOption ==1){
    
    printf("Words and scores for top first words:\n");
    tempScore=allWords[0].score; // highest score

    // print statement to handle tiebreakers
    for(int i =0; i<counter;i++){
      if (allWords[i].score==tempScore){
        printf("%s %d\n", allWords[i].word, allWords[i].score);
      } else{ // break when tempScore / highest score is no longer highest score
        break;
      }
    }
    
  }else if (menuOption==2){
    // creation of copy of answerWords and allWords for menuOption 2
    struct wordAndScore *secondAnswerWords; 
    struct wordAndScore *secondAllWords;
  
    secondAnswerWords=(struct wordAndScore*)malloc(fileContentCountAnswers*sizeof(struct wordAndScore));
    secondAllWords=(struct wordAndScore*)malloc((fileContentCountGuesses+fileContentCountAnswers)*sizeof(struct wordAndScore));
  
    // fill arrays of secondAnswerWords and secondAllWords, with respective arrays
    applyStringsIntoArray(fileContentCountAnswers, secondAnswerWords, answerWords);
    applyStringsIntoArray(counter, secondAllWords, allWords);

    printf("Words and scores for top first words and second words:\n");
    tempScore=allWords[0].score; // highest score
    int tempSecondScore=0; // for tiebreaker of second word highest score

    // for loop to determine the second best words and tiebreakers
    for(int specificWord =0; specificWord<counter;specificWord++){
      
      if (tempScore==allWords[specificWord].score){ // accounts for multiple highest scores
        printf("%s %d\n", allWords[specificWord].word, allWords[specificWord].score);

        applyFirstWord(allWords[specificWord], secondAnswerWords, fileContentCountAnswers);
        //applied first word list now created

        //create score for the secondAllWords array
        for (int k = 0; k<counter; k++){
          secondAllWords[k].score=calculateScore(secondAllWords[k], secondAnswerWords, fileContentCountAnswers);
        }
        
        qsort( secondAllWords, counter, sizeof(struct wordAndScore), compareFunction);

        //print the second best words, including for those of ties
        tempSecondScore=secondAllWords[0].score;
        for(int z = 0; z<counter; z++){
          if(tempSecondScore==secondAllWords[z].score){//tie breaker if statement
            printf("  %s %d", secondAllWords[z].word, secondAllWords[z].score);
          } else{
            break;
          }
          
        }
        printf("\n");
      }else{
        break; // out of highest score tiebreakers
      }

      //recopy answerWords to secondAnswerWords
      for (int i = 0; i<fileContentCountAnswers; i++){
      strcpy(secondAnswerWords[i].word, answerWords[i].word);
      }
    }
    
  }//end of menuOption2

  
    printf("Done\n");
    return 0;
} // end main()

