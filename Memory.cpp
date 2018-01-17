// Group Members:Zachary Purdy, John Oravetz
// Chandler Ford did not show up with his part of the assignment
// Project Description: A memory game
// Last Modified: 4/6/2015

#include <iostream>
#include "SDL.h"
#include "time.h"
#include <string>
#include <cstring>
#include <cstdlib>

using namespace std;

// Assignment
//  1. Write a function to shuffle the board and call this function where specified.
//		The function declaration is provided for you.
//      In order to shuffle the board, randomly select and then swap two elements and
//      repeat this process 20 times. Hint: to select each element, you must randomly
//      select a valid row and valid column
//  2. The all_revealed function is a stub. Modify this function to return true if all
//      of the images have been revealed (i.e., if all values of the array are 1);
//      the function returns false otherwise
//  3. Write a new_game function that resets the board. This means that the images
//      are shuffled and not displayed (i.e., each element of the revealed array
//      is set to 0).
//  4. Implement the algorithm in main to play the memory game
//  5. Modify the winning board function as described in the function definition


const int Y_OFFSET = 100; // vertical distance to first image
const int X_OFFSET = 65; // horizontal distance to first image

const int NUM_IMAGES = 7; // the number of images in the game (including the '?')

const int NROWS = 3;    // the number of rows in the game
const int NCOLS = 4;    // the number of columns in the game

const int WIDTH = 120;  // the width of each picture
const int GAP = 5;      // the space betweeen pictures


// printArray function displays array 1 row at a time; note that global variable (NROWS and NCOLS)
// do not need to be specified as parameters
void printArray(int x[][NCOLS]) {
	for (int i = 0; i < NROWS; i++) {
		for (int j = 0; j < NCOLS; j++) {
			cout << x[i][j] << " ";
		}
		cout << endl;
	}
}

// precondition: board is initialized
// postcondition: board is shuffled by randomly swapping 20 values
void shuffle(int board[][NCOLS]);

// precondtion: player wants a new game
// postcondition: all images are hidden and board is shuffled
void new_game (int board[][NCOLS], int revealed[][NCOLS]);

// precondition: the player has won!
// postcondition: randomly selects a single image to display,
//      displays the image, then pauses for 100 ms, and this is repeated 100 times
//      TO DO: after this, the images are all hidden then displayed row by row
//              from left to right, with a 100 ms pause in between
void winning_board(int board[][NCOLS], SDL_Surface *images[NUM_IMAGES], SDL_Renderer * renderer, SDL_Texture *texture);


// Precondition: board indicates the the location of each image and revealed indicates
//  whether an image is revealed (=1) or hidden (=0)
// Postcondition: the board is drawn
void draw_board(int board[][NCOLS], int revealed[][NCOLS],
                SDL_Surface * images[NUM_IMAGES], SDL_Renderer *renderer, SDL_Texture *texture);


// precondition: in revealed array, 1 = revealed and 0 = hidden
// postcondition: returns true if all array elements = 1 (i.e., revealed)
bool all_revealed(int revealed[][NCOLS]);

// precondition: there has been a left-click event
// postcondition: sets rowNum and colNum to the corresponding row and column that
//      is clicked; returns true if the click is valid; otherwise returns false
bool valid_click(SDL_Event event, int &rowNum, int &colNum);

// Precondition: renderer, image, and texture are initialized
// Postcondition: plots image at location (x,y) with given height and width
void plot_image(SDL_Renderer *renderer, SDL_Surface *image, SDL_Texture *texture, int x, int y, int height, int width);


int main(int argc, char ** argv)
{
    srand(time(NULL));
    bool quit = false;
    
    
    /* SDL setup */
    SDL_Event event;
    SDL_Window * window = SDL_CreateWindow("Image Selection Program",
                                           SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 600, 0);
    if (window == NULL)
        SDL_ShowSimpleMessageBox(0, "Window init error", SDL_GetError(),
                                 window);
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 1);
    if (renderer == NULL)
        SDL_ShowSimpleMessageBox(0, "Renderer init error",
                                 SDL_GetError(), window);
  
 //   SDL_Texture * texture;
	SDL_Texture * texture =
		SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, 640, 480);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    
    /* load the images */
    SDL_Surface * images[NUM_IMAGES];
    images[0] = SDL_LoadBMP("img/question.bmp");
    images[1] = SDL_LoadBMP("img/image1.bmp");
    images[2] = SDL_LoadBMP("img/image2.bmp");
    images[3] = SDL_LoadBMP("img/image3.bmp");
    images[4] = SDL_LoadBMP("img/image4.bmp");
    images[5] = SDL_LoadBMP("img/image5.bmp");
    images[6] = SDL_LoadBMP("img/image6.bmp");
    
  
    /* make sure images load correctly */
    for (int i = 0; i < NUM_IMAGES; i++) {
        if (images[i] == NULL) {
            string str = "Image init error, image #";
            //str.append(to_string(i));
            char c[100];
            strcpy_s(c, str.c_str());
        SDL_ShowSimpleMessageBox(0, c, SDL_GetError(),
                                 window);
        }
    }
    
    
	// The board array specifies which image is in each location
    int board[NROWS][NCOLS] = {1,1,2,2,3,3,4,4,5,5,6,6};
    
	cout << "The board is: " << endl;
	printArray(board);
	cout << endl;

    // The revealed array specifies whether the image is displayed (1) or not (0)
    int revealed[NROWS][NCOLS] = {0}; //{1,1,1,1,1,1,1,1,1,1,1,1};
    
    // TO DO: SHUFFLE THE BOARD
	shuffle(board);

    draw_board(board, revealed, images, renderer, texture);
    
    while (!quit)
    {
        SDL_WaitEvent(&event);
        
		switch (event.type)
		{
                
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    
                    int rowNum, colNum;
                    bool valid = valid_click(event, rowNum, colNum);
                    if (!valid) break;
                    if (revealed[rowNum-1][colNum-1] == 1) break;
                   
		    if (revealed[rowNum-1][colNum-1] == 0) {
		    	revealed[rowNum-1][colNum-1] = 1;
		    } 
 
                    cout << "You have selected rowNum, colNum = " << rowNum
                        << "," << colNum << endl;


					cout << "Revealed array is: " << endl;
					printArray(revealed);
					cout << endl;

                    
                    // TO DO: implement the following algorithm for playing memory
                    // If this is the first selection, then
                    //      - remember the selected row and column number
                    //      - display the appropriate image (by setting the
                    //          appropriate element of the 'reveal' array to 1
                    // If this is the second selection, then
                    //      - check whether this selection matches the previous (i.e.,
                    //          whether the corresponding 'board' values are the same)
                    //      - if so, reveal the 2nd card (by setting the appropriate
                    //          element of the 'revealed' array to 1
                    //      - if not a match, then reveal the card, re-draw the board,
                    //          pause for 1 second, then flip over the two cards (i.e.,
                    //          set the corresponding values of the 'revealed' array
                    //          back to 0. Note: to draw the board and pause, use:
                    //           draw_board(board, revealed, images, renderer, texture);
                    //           SDL_Delay(1000);


                
                    
                    draw_board(board, revealed, images, renderer, texture);
                    if (all_revealed(revealed)) {
                       winning_board(board, images, renderer, texture);
                        cout << "YOU WIN!!\n";
                    }
                    
                }
                break;
                /* END case SDL_MOUSEBUTTON*/
                
            case SDL_KEYDOWN:
                
				if (event.key.keysym.sym == SDLK_n) {
					cout << "User wants a new game; call new_game function here\n";
					new_game(board, revealed);
					draw_board(board, revealed, images, renderer, texture);
				}
                break;
                
            case SDL_QUIT:
                quit = true;
                break;
        }

    }
    
    SDL_Quit();
    
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    
    return 0;
}


// TO DO: This function is a stub; modify it so it behaves as below
// precondition: in revealed array, 1 = revealed and 0 = hidden
// postcondition: returns true if all array elements = 1 (i.e., all elements are revealed)
bool all_revealed(int revealed[][NCOLS]) {
	return true;	
}


// precondition: the player has won!
// postcondition: randomly selects a single image to display,
//      displays the image, then pauses for 100 ms, and this is repeated 100 times
//      TO DO: after this, the images are all hidden then displayed row by row
//              from left to right, with a 100 ms pause in between

void winning_board(int board[][NCOLS], SDL_Surface *images[NUM_IMAGES], SDL_Renderer * renderer, SDL_Texture *texture) {

	for (int i = 0; i < 20; i++) {
		int show[NROWS][NCOLS] = { 0 };          // do not show any pictures
		show[rand() % NROWS][rand() % NCOLS] = 1;  // randomly select a picture to show
		draw_board(board, show, images, renderer, texture); // draw the board
		SDL_Delay(100);  // brief pause
	}

	int show[NROWS][NCOLS] = { 0 };

	for(int i = 0; i < NROWS; i++){
		for(int j = 0; j < NCOLS; j++){
			show[i][j] = 1;
			draw_board(board, show, images, renderer, texture);
			SDL_Delay(100);
		}
	}
}


// Precondition: board indicates the the location of each image and revealed indicates
//  whether an image is revealed (=1) or hidden (=0)
// Postcondition: the board is drawn
void draw_board(int board[][NCOLS], int revealed[][NCOLS],
                SDL_Surface * images[NUM_IMAGES], SDL_Renderer *renderer, SDL_Texture *texture) {
    
    SDL_RenderClear(renderer);
    int yloc = Y_OFFSET;
    for (int i = 0; i < NROWS; i++) {
        int xloc = X_OFFSET;
        for (int j = 0; j < NCOLS; j++) {
            int val = revealed[i][j];
            if (val == 1) {
                val = board[i][j];
                plot_image(renderer, images[val], texture, xloc, yloc,
                           WIDTH, WIDTH);
            } else {
                plot_image(renderer, images[0], texture, xloc, yloc,
                           WIDTH, WIDTH);
            }
            xloc = xloc + WIDTH + GAP;
        }
        yloc = yloc + WIDTH + GAP;
    }
    
    /* draw pictures */
    SDL_RenderPresent(renderer);
    
}



// precondition: there has been a left-click event
// postcondition: sets rowNum and colNum to the corresponding row and column that
//      is clicked; returns true if the click is valid; otherwise returns false
bool valid_click(SDL_Event event, int &rowNum, int &colNum) {
    /* get X and Y values */
    int X = event.motion.x;
    int Y = event.motion.y;
    
    // find the column number
    colNum = 0;
    while (X_OFFSET + colNum*WIDTH + (colNum-1)*GAP < X) {
        colNum++;
    }
    // find the row number
    rowNum = 0;
    while (Y_OFFSET + rowNum*WIDTH + (rowNum-1)*GAP < Y) {
        rowNum++;
    }
    
    if (rowNum < 1 || rowNum > NROWS || colNum < 1 || colNum > NCOLS) return false;
    return true;
    
}


// Precondition: renderer, image, and texture are initialized
// Postcondition: plots image at location (x,y) with given height and width
void plot_image(SDL_Renderer *renderer, SDL_Surface *image, SDL_Texture *texture, int x, int y, int height, int width) {
    texture= SDL_CreateTextureFromSurface(renderer, image);
    SDL_Rect dstrect= { x, y, height, width };
    SDL_RenderCopy(renderer, texture, NULL, &dstrect);
}

void new_game(int board[][NCOLS], int revealed[][NCOLS]){
	for (int i = 0; i < NROWS; i++){
		for (int j = 0; j < NCOLS; j++){
			revealed[i][j] = 0;
		}
	}

	shuffle(board);

}

void shuffle(int board[][NCOLS]){
	int holder;

	for (int i = 0; i < 20; i++){
		int randRow1 = rand() % NROWS;
		int randCol1 = rand() % NCOLS;
		int randRow2 = rand() % NROWS;
		int randCol2 = rand() % NCOLS;

		holder = board[randRow1][randCol1];
		board[randRow1][randCol1] = board[randRow2][randCol2];
		board[randRow2][randCol2] = holder;
	}
}






