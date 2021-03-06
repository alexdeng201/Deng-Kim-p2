/*The program needs to operate in a couple different ways */
#define F1 59
#define LEFT_ARROW 75
#define DOWN_ARROW 80
#define RIGHT_ARROW 77
#define UP_ARROW 72

#include <cstdlib>
#include <ncurses.h>
#include <string>
#include <fstream>
#include <iostream>

using namespace std; 


//Prototype
void displayText(char *argv[]); 
void verifyFile (int argc, char *argv[]); 

//new code
WINDOW *create_newwin(int height, int width, int starty, int startx); //Allocates memory for the window
void destroy_win(WINDOW *local_win); //Deallocates memory

//-----------------------------------
// MAIN MAIN MAIN MAIN MAIN MAIN MAIN
// --Where the magic happens---------
//-----------------------------------
int main(int argc, char *argv[])
{
    initscr(); 


  //New code for the fro
    WINDOW *my_win;
    int startx, starty, width, height;
    int ch;

    initscr();/* Start curses mode */
    cbreak();/* Line buffering disabled, Pass on
	      * everty thing to me */
    keypad(stdscr, TRUE);/* I need that nifty F1 */

    height = 3;
    width = 10;
    starty = (LINES - height) / 2;/* Calculating for a center placement */
    startx = (COLS - width) / 2;/* of the window*/


    my_win = create_newwin(height, width, starty, startx);
  //To scroll through the program I planned to look up a scroll ok thing. scrollok();
  //Copied from tlpd. planned to implement something to scroll through the text
  while((ch = getch()) != KEY_F(1))
    {switch(ch)
	{case KEY_LEFT:
	    destroy_win(my_win);
	    my_win = create_newwin(height, width, starty,--startx);
	    break;
	case KEY_RIGHT:
	  destroy_win(my_win);
	  my_win = create_newwin(height, width, starty,++startx);
	  break;
	case KEY_UP:
	  destroy_win(my_win);
	  my_win = create_newwin(height, width, --starty,startx);
	  break;
	case KEY_DOWN:
	  destroy_win(my_win);
	  my_win = create_newwin(height, width, ++starty,startx);
	  break;
	}
    }


    //Enables the use of the function button
    keypad(stdscr, TRUE);

    verifyFile(argc,argv); //Verifies the file and makes sure that it is valid
    clear();

    displayText(argv); //Displays the text
    refresh();

    endwin();                  // end curses mod

return EXIT_SUCCESS; // main
}

/*---------------------------------------------------
The purpose of this function is to take in the pointer
to an array argv and display its contents 
-----------------------------------------------------*/
void displayText(char *argv[]){
  string line;
  ifstream myfile(argv[1]);

  //Print out stuff from file
  if(myfile.is_open())
    while (getline (myfile,line)) 
	  printw("%s \n",line.c_str());



  printw("\n<--Press any key to exit-->");
  
  refresh();
  getch();
  myfile.close();
}

/*---------------------------------------------------------------
The purpose of this function is to verify whether or not the user
inputs the right file. This function takes in the int argc and the
array of pointers to chars
-----------------------------------------------------------------*/

void verifyFile (int argc, char *argv[]){
    bool fole = true;//set to false if the file is valid
    bool nvalid = true;//check whether command line is correct

    //Test whether the command prompt is correct
    if((argc==2)){
      ifstream fileExixst(argv[1]);
      if(!fileExixst)
	nvalid=false;
    }//argc==2


    //If the command prompt is empty, ask the user for a valid filename
    if((argc!=2)||(nvalid==false)){
    while(fole)
      {
      printw("Please enter a valid file name  \n");
      char arrayChar[32];
      getstr(arrayChar);
      printw("\n<--Press any key to continue-->");
      argv[1]=arrayChar;
      refresh();
      //this checks whether the file exists or not
      ifstream fileExist(argv[1]);
      if(fileExist)
	fole = false;
   
      clear();
          }//while
    }//argc =! 2

    //Allows it so that displayText can access this file
    string line;
    ifstream myfile(argv[1]);


}


  //The following lines of code were referenced from tldp.org
WINDOW *create_newwin(int height, int width, int starty, int startx)
{	WINDOW *local_win;

	local_win = newwin(height, width, starty, startx);
	box(local_win, 0 , 0);		/* 0, 0 gives default characters 
					 * for the vertical and horizontal
					 * lines			*/
	wrefresh(local_win);		/* Show that box 		*/

	return local_win;
}

void destroy_win(WINDOW *local_win)
{	
	/* box(local_win, ' ', ' '); : This won't produce the desired
	 * result of erasing the window. It will leave it's four corners 
	 * and so an ugly remnant of window. 
	 */
	wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	/* The parameters taken are 
	 * 1. win: the window on which to operate
	 * 2. ls: character to be used for the left side of the window 
	 * 3. rs: character to be used for the right side of the window 
	 * 4. ts: character to be used for the top side of the window 
	 * 5. bs: character to be used for the bottom side of the window 
	 * 6. tl: character to be used for the top left corner of the window 
	 * 7. tr: character to be used for the top right corner of the window 
	 * 8. bl: character to be used for the bottom left corner of the window 
	 * 9. br: character to be used for the bottom right corner of the window
	 */
	wrefresh(local_win);
	delwin(local_win);
}