#include<stdio.h>
#include<windows.h>
#include<stdarg.h>
#include<string.h>
#include "cMenu.h"




int HMenu(int indent, int vPos, int space_between_Options, char *pointer_shape, int pointer_color, char *splitter_shape, int splliter_color, int text_color, int selectedText_color, int number_of_options, ...){
if (indent<=0 || space_between_Options<=0 || number_of_options<=0 || pointer_color<0 || splliter_color<0 || text_color<0 || selectedText_color<0 ){printf("Invalid Input\n"); return -1;}
if (vPos < 0){ // That Means The Vertical Position Is Not Precisely Known!
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO SBInfo;
    GetConsoleScreenBufferInfo(hOut, &SBInfo);
    vPos = SBInfo.dwCursorPosition.Y;
    }

HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
COORD arrow_pos;

unsigned int i; // loop variable and index

char *container[number_of_options]; //this contains the menu's options (strings passed to this function)
unsigned int length[number_of_options]; // the length of each word(option) in the menu
unsigned int hPos[number_of_options];  // the horizontal position at the beginning of each word (option)

va_list ap; // arguments parameter
va_start (ap, number_of_options); // initialize parameter

for(i=0; i<number_of_options;){//fetch arguments (the strings)

    if((container[i] = va_arg(ap, char*)) != NULL){ // check for errors and assign string
        length[i] = strlen(container[i]);
        if (i!=0){hPos[i] = (hPos[i-1]+length[i-1]+space_between_Options);} //incrementally estimate horizontal position for every word
        else{hPos[0] = indent;}
    i++;}
    else{printf("The Number Of Options Do Not Match The Numbers Of Strings Passed To This Function");exit(0);}
}



/*Create Menu Template*/
for (i=0;i<number_of_options;i++){
/*First Create The Arrow For The First Option Only*/
if (i==0){
arrow_pos.X = hPos[i]-1;//Exact Before The First Char of The First Word (Option) in The Menu and so on
arrow_pos.Y = vPos;
SetConsoleCursorPosition(hOutput, arrow_pos);
SetConsoleTextAttribute(hOutput,  pointer_color );
WriteConsoleA(hOutput, pointer_shape, strlen(pointer_shape), NULL, NULL);
}

/*Create The Options For First Time*/

arrow_pos.X = hPos[i];
arrow_pos.Y = vPos;
SetConsoleCursorPosition(hOutput, arrow_pos);
SetConsoleTextAttribute(hOutput, text_color);
WriteConsoleA(hOutput, container[i], length[i], NULL, NULL);


if (i==0){
arrow_pos.X = hPos[i];
arrow_pos.Y = vPos;
SetConsoleCursorPosition(hOutput, arrow_pos);
SetConsoleTextAttribute(hOutput,  selectedText_color);
WriteConsoleA(hOutput, container[i], length[i], NULL, NULL);

}
/*Draw The Splitter Between Strings (Options) on the Menu*/
if (i!=0){ // start after the first string
arrow_pos.X = hPos[i]-(space_between_Options/2)-1;//Exact in the middle between strings
arrow_pos.Y = vPos;
SetConsoleCursorPosition(hOutput, arrow_pos);
SetConsoleTextAttribute(hOutput, splliter_color);
WriteConsoleA(hOutput, splitter_shape, strlen(splitter_shape), NULL, NULL);
}
}




unsigned int newPosition=0; //To Know What Option We Are At Now

/*Switch Between Options According To The Arrow Pressed "Left" or "Right"*/

unsigned int ch=0;
while(ch!=13){ /*13 is the ENTER key*/

/*HIDE THE CURSOR BEFORE "GETCH" FOR PRESENTABLE LOOK*/
HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
CONSOLE_CURSOR_INFO info; //Contains information about the console cursor.
info.dwSize = 100;//The percentage of the character cell that is filled by the cursor. This value is between 1 and 100.
info.bVisible = FALSE;
SetConsoleCursorInfo(consoleHandle, &info);//Sets the size and visibility of the cursor for the specified console screen buffer.

ch = getch();/*Get Char*/

if (ch == 0 || ch == 224){

        switch(getch()){

        case 77:// Right Arrow

        if (newPosition < (number_of_options-1)){// if it is not already on the last option in the menu

            /*CLEAR THE POINTER IN The PREVIOUS STRING*/
            if (newPosition!=0){ // The Mean We Are Not At The First String
                arrow_pos.X = hPos[newPosition]-1;
                arrow_pos.Y = vPos;
                SetConsoleCursorPosition(hOutput, arrow_pos);
                SetConsoleTextAttribute(hOutput, 0); // make it black
                WriteConsoleA(hOutput, "", strlen(pointer_shape), NULL, NULL);
            }
            else if (newPosition==0){//We Are At The First String
                arrow_pos.X = indent-1;
                arrow_pos.Y = vPos;
                SetConsoleCursorPosition(hOutput, arrow_pos);
                SetConsoleTextAttribute(hOutput, 0); // make it black
                WriteConsoleA(hOutput,"", strlen(pointer_shape), NULL, NULL);
            }

            /*DRAW THE STRINGS & SPLIITERS BEFORE THE POINTED-AT-STRING IF ANY*/
            for (i=0; i<=newPosition; i++){ // do it at least one time if it's already at position 0 (i.e first option)
                 /*For STRINGS*/
                 arrow_pos.X = hPos[i];
                 arrow_pos.Y = vPos;
                 SetConsoleCursorPosition(hOutput, arrow_pos);
                 SetConsoleTextAttribute(hOutput, text_color);
                 WriteConsoleA(hOutput, container[i], length[i], NULL, NULL);
                 /*For Splitters*/
                 arrow_pos.X = hPos[i+1]-(space_between_Options/2)-1;//Exact in the middle between strings
                 arrow_pos.Y = vPos;
                 SetConsoleCursorPosition(hOutput, arrow_pos);
                 SetConsoleTextAttribute(hOutput, splliter_color);
                 WriteConsoleA(hOutput, splitter_shape, strlen(splitter_shape), NULL, NULL);
            }


            /*DRAW THE POINTER*/
            arrow_pos.X = hPos[newPosition+1]-1;//Exact Before The First Char of The First Word (Option) in The Menu and so on
            arrow_pos.Y = vPos;
            SetConsoleCursorPosition(hOutput, arrow_pos);
            SetConsoleTextAttribute(hOutput, pointer_color);
            WriteConsoleA(hOutput, pointer_shape, strlen(pointer_shape), NULL, NULL);


            /*Draw The Pointed-At-String With Attributes*/
            SetConsoleTextAttribute(hOutput, selectedText_color); // make attributes
            arrow_pos.X = hPos[newPosition+1];
            arrow_pos.Y = vPos;
            SetConsoleCursorPosition(hOutput, arrow_pos);
            WriteConsoleA(hOutput, container[newPosition+1], length[newPosition+1], NULL, NULL);

            /*Draw The Rest of the Menu*/
            for (i=newPosition+2; i<number_of_options;i++){
                /*For STRINGS*/
                 arrow_pos.X = hPos[i];
                 arrow_pos.Y = vPos;
                 SetConsoleCursorPosition(hOutput, arrow_pos);
                 SetConsoleTextAttribute(hOutput, text_color);
                 WriteConsoleA(hOutput, container[i], length[i], NULL, NULL);
                 /*For Splitters*/
                 if(i!= (number_of_options-1)){
                     arrow_pos.X = hPos[i+1]-(space_between_Options/2)-1;//Exact in the middle between strings
                     arrow_pos.Y = vPos;
                     SetConsoleCursorPosition(hOutput, arrow_pos);
                     SetConsoleTextAttribute(hOutput, splliter_color);
                     WriteConsoleA(hOutput, splitter_shape, strlen(splitter_shape), NULL, NULL);
                 }
            }


        newPosition++;
        break;

        }
        else{ /*We Are Already At Last String and Want To Back To First Option*/

            /*CLEAR THE POINTER AT LAST STRING*/
            arrow_pos.X = hPos[number_of_options-1]-1;
            arrow_pos.Y = vPos;
            SetConsoleTextAttribute(hOutput, 0); // make it black
            SetConsoleCursorPosition(hOutput, arrow_pos);
            SetConsoleTextAttribute(hOutput, 0); // make it black
            WriteConsoleA(hOutput, "", strlen(pointer_shape), NULL, NULL);

            /*Create The Menu Template Again*/
            for (i=0;i<number_of_options;i++){
                /*First Create The Arrow For The First Option Only*/
                if (i==0){
                arrow_pos.X = hPos[i]-1;
                arrow_pos.Y = vPos;
                SetConsoleCursorPosition(hOutput, arrow_pos);
                SetConsoleTextAttribute(hOutput,  pointer_color);
                WriteConsoleA(hOutput, pointer_shape, 1, NULL, NULL);
                }
                if(i!=0){
                /*Create The Options as if First Time*/
                arrow_pos.X = hPos[i];
                arrow_pos.Y = vPos;
                SetConsoleCursorPosition(hOutput, arrow_pos);
                SetConsoleTextAttribute(hOutput,  text_color);
                WriteConsoleA(hOutput, container[i], length[i], NULL, NULL);
                }

                if (i==0){
                arrow_pos.X = hPos[i];
                arrow_pos.Y = vPos;
                SetConsoleCursorPosition(hOutput, arrow_pos);
                SetConsoleTextAttribute(hOutput,  selectedText_color);
                WriteConsoleA(hOutput, container[i], length[i], NULL, NULL);

                }
                /*Create The Splitter Between Strings (Options) on the Menu*/
                if (i!=0){ // start after the first string
                arrow_pos.X = hPos[i]-(space_between_Options/2)-1;//Exact in the middle between strings
                arrow_pos.Y = vPos;
                SetConsoleCursorPosition(hOutput, arrow_pos);
                SetConsoleTextAttribute(hOutput, splliter_color);
                WriteConsoleA(hOutput, splitter_shape, strlen(splitter_shape), NULL, NULL);
                }
                }


        newPosition=0; // Because We Are Now At First Option
        break;
        }



        case 75: //Left Arrow

        if (newPosition > 0){// if it is not already on the first option in the menu

            /*CLEAR THE POINTER IN THE PREVIOUS STRING*/
                arrow_pos.X = hPos[newPosition]-1;
                arrow_pos.Y = vPos;
                SetConsoleCursorPosition(hOutput, arrow_pos);
                SetConsoleTextAttribute(hOutput, 0); // make it black
                WriteConsoleA(hOutput, "", strlen(pointer_shape), NULL, NULL);



            /*DRAW THE POINTER*/
            arrow_pos.X = hPos[newPosition-1]-1;//Exact Before The First Char of The First Word (Option) in The Menu and so on
            arrow_pos.Y = vPos;
            SetConsoleCursorPosition(hOutput, arrow_pos);
            SetConsoleTextAttribute(hOutput, pointer_color);
            WriteConsoleA(hOutput, pointer_shape, strlen(pointer_shape), NULL, NULL);


            /*Draw The Pointed-At-String With Attributes*/
            SetConsoleTextAttribute(hOutput, selectedText_color); // make attributes
            arrow_pos.X = hPos[newPosition-1];
            arrow_pos.Y = vPos;
            SetConsoleCursorPosition(hOutput, arrow_pos);
            WriteConsoleA(hOutput, container[newPosition-1], length[newPosition-1], NULL, NULL);

            /*DRAW THE STRINGS & SPLIITERS BEFORE THE POINTED-AT-STRING IF ANY*/
            for (i=newPosition; i<number_of_options; i++){ // do it at least one time if it's already at position 0 (i.e first option)

                 /*For Splitters*/
                 arrow_pos.X = hPos[i]-(space_between_Options/2)-1;//Exact in the middle between strings
                 arrow_pos.Y = vPos;
                 SetConsoleCursorPosition(hOutput, arrow_pos);
                 SetConsoleTextAttribute(hOutput, splliter_color);
                 WriteConsoleA(hOutput, splitter_shape, strlen(splitter_shape), NULL, NULL);
                 /*For STRINGS*/
                 arrow_pos.X = hPos[i];
                 arrow_pos.Y = vPos;
                 SetConsoleCursorPosition(hOutput, arrow_pos);
                 SetConsoleTextAttribute(hOutput, text_color);
                 WriteConsoleA(hOutput, container[i], length[i], NULL, NULL);
            }



        newPosition--;
        break;

        }
        else{ /*We Are Already At First String and Want To Go To Last Option*/

            /*Clear The Pointer*/
            arrow_pos.X = indent-1;
            arrow_pos.Y = vPos;
            SetConsoleCursorPosition(hOutput, arrow_pos);
            SetConsoleTextAttribute(hOutput, 0); // make it black
            WriteConsoleA(hOutput,"", strlen(pointer_shape), NULL, NULL);





             /*DRAW THE STRINGS & SPLIITERS BEFORE THE POINTED-AT-STRING IF ANY*/
            for (i=0; i<number_of_options-1; i++){ // do it at least one time if it's already at position 0 (i.e first option)
                 /*For STRINGS*/
                 arrow_pos.X = hPos[i];
                 arrow_pos.Y = vPos;
                 SetConsoleCursorPosition(hOutput, arrow_pos);
                 SetConsoleTextAttribute(hOutput, text_color);
                 WriteConsoleA(hOutput, container[i], length[i], NULL, NULL);

                 /*For Splitters*/
                 arrow_pos.X = hPos[i+1]-(space_between_Options/2)-1;//Exact in the middle between strings
                 arrow_pos.Y = vPos;
                 SetConsoleCursorPosition(hOutput, arrow_pos);
                 SetConsoleTextAttribute(hOutput, splliter_color);
                 WriteConsoleA(hOutput, splitter_shape, strlen(splitter_shape), NULL, NULL);

            }

            /*DRAW THE POINTER*/
            arrow_pos.X = hPos[number_of_options-1]-1;//Exact Before The First Char of The Last Word (Option) in The Menu and so on
            arrow_pos.Y = vPos;
            SetConsoleCursorPosition(hOutput, arrow_pos);
            SetConsoleTextAttribute(hOutput, pointer_color);
            WriteConsoleA(hOutput, pointer_shape, strlen(pointer_shape), NULL, NULL);

            /*Draw The Pointed-At-String With Attributes*/
            SetConsoleTextAttribute(hOutput, selectedText_color); // make attributes
            arrow_pos.X = hPos[number_of_options-1];
            arrow_pos.Y = vPos;
            SetConsoleCursorPosition(hOutput, arrow_pos);
            WriteConsoleA(hOutput, container[number_of_options-1], length[number_of_options-1], NULL, NULL);



        newPosition=number_of_options-1;
        break;
        }
}
}
}

arrow_pos.X = 0;
arrow_pos.Y = vPos+1;
SetConsoleCursorPosition(hOutput,arrow_pos);
SetConsoleTextAttribute(hOutput, 15);
CONSOLE_CURSOR_INFO info; //Contains information about the console cursor.
info.dwSize = 1;//The percentage of the character cell that is filled by the cursor. This value is between 1 and 100.
info.bVisible = TRUE;
SetConsoleCursorInfo(hOutput, &info);//Sets the size and visibility of the cursor for the specified console screen buffer.
return newPosition+1;
}

VMenu(int indent, int vPos, int space_between_Options, char *pointer_shape, int pointer_color, char *splitter_shape, int splliter_color, int text_color, int selectedText_color,  int number_of_options, ...){
if (indent<=0 || space_between_Options<=0 || number_of_options<=0 || pointer_color<0 || splliter_color<0 || text_color<0 || selectedText_color<0){printf("Invalid Input\n"); return -1;}
if (vPos < 0){ // That Means The Vertical Position Is Not Precisely Known!
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO SBInfo;
    GetConsoleScreenBufferInfo(hOut, &SBInfo);
    vPos = SBInfo.dwCursorPosition.Y+1;
    }

unsigned int i; // loop variable and index

char *container[number_of_options]; //this contains the menu's options (strings passed to this function)
unsigned int length[number_of_options]; // the length of each word(option) in the menu
unsigned int SplitterVposition[number_of_options];

va_list ap; // arguments parameter
va_start (ap, number_of_options); // initialize parameter

HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
COORD arrow_pos;

for(i=0; i<number_of_options;){//fetch arguments (the strings)

    if((container[i] = va_arg(ap, char*)) != NULL){ // check for errors and assign string
        length[i] = strlen(container[i]);
        SplitterVposition[i] = vPos+(space_between_Options*i)+1;
    i++;}
    else{printf("The Number Of Options Does Not Match The Numbers Of Strings Passed To This Function");exit(0);}
}



/*Create Menu Template*/
for (i=0;i<number_of_options;i++){
/*First Create The Arrow For The First Option Only*/
    if(i==0){
        arrow_pos.X = indent;//Exact Before The First Char of The First Word (Option) in The Menu and so on
        if(i==0){arrow_pos.Y = vPos;}
        else {arrow_pos.Y = vPos+(space_between_Options*i);}
        SetConsoleCursorPosition(hOutput, arrow_pos);
        SetConsoleTextAttribute(hOutput,  pointer_color );
        WriteConsoleA(hOutput, pointer_shape, strlen(pointer_shape), NULL, NULL);

        /*Create The pointed-at string*/
        arrow_pos.X = indent+1;
        arrow_pos.Y = vPos;
        SetConsoleTextAttribute(hOutput, selectedText_color);
        SetConsoleCursorPosition(hOutput, arrow_pos);
        WriteConsoleA(hOutput, container[i], length[i], NULL, NULL);
    }
/*Create The Options For First Time*/
    if(i!=0){
        arrow_pos.X = indent+1;
        arrow_pos.Y = vPos+(space_between_Options*i);
        SetConsoleCursorPosition(hOutput, arrow_pos);
        SetConsoleTextAttribute(hOutput, text_color);
        WriteConsoleA(hOutput, container[i], length[i], NULL, NULL);
    }


/*Draw The Splitter Between Strings (Options) on the Menu*/
    arrow_pos.X = indent+1;
    arrow_pos.Y = SplitterVposition[i];//Exact in the middle between strings
    SetConsoleCursorPosition(hOutput, arrow_pos);
    SetConsoleTextAttribute(hOutput, splliter_color);
    WriteConsoleA(hOutput, splitter_shape, strlen(splitter_shape), NULL, NULL);

}




unsigned int newPosition=0; //To Know What Option We Are At Now

/*Switch Between Options According To The Arrow Pressed "Left" or "Right"*/

unsigned int ch=0;
while(ch!=13){ /*13 is the ENTER key*/

/*HIDE THE CURSOR BEFORE "GETCH" FOR PRESENTABLE LOOK*/
HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
CONSOLE_CURSOR_INFO info; //Contains information about the console cursor.
info.dwSize = 100;//The percentage of the character cell that is filled by the cursor. This value is between 1 and 100.
info.bVisible = FALSE;
SetConsoleCursorInfo(consoleHandle, &info);//Sets the size and visibility of the cursor for the specified console screen buffer.

ch = getch();/*Get Char*/

if (ch == 0 || ch == 224){

        switch(getch()){

        case 80:// DOWN Arrow

        if (newPosition < (number_of_options-1)){// if it is not already on the last option in the menu

            /*CLEAR THE POINTER IN The PREVIOUS STRING*/
                arrow_pos.X = indent;
                arrow_pos.Y = vPos+(space_between_Options*newPosition);
                SetConsoleCursorPosition(hOutput, arrow_pos);
                SetConsoleTextAttribute(hOutput, 0); // make it black
                WriteConsoleA(hOutput, "", strlen(pointer_shape), NULL, NULL);


            /*DRAW THE STRINGS & SPLIITERS BEFORE THE POINTED-AT-STRING IF ANY*/
            for (i=0; i<=newPosition; i++){ // do it at least one time if it's already at position 0 (i.e first option)
                 /*For STRINGS*/
                 arrow_pos.X = indent+1;
                 arrow_pos.Y = vPos+(space_between_Options*i);
                 SetConsoleCursorPosition(hOutput, arrow_pos);
                 SetConsoleTextAttribute(hOutput, text_color);
                 WriteConsoleA(hOutput, container[i], length[i], NULL, NULL);
                 /*For Splitters*/
                 arrow_pos.X = indent+1;
                 arrow_pos.Y = SplitterVposition[i];//Exact in the middle between strings
                 SetConsoleCursorPosition(hOutput, arrow_pos);
                 SetConsoleTextAttribute(hOutput, splliter_color);
                 WriteConsoleA(hOutput, splitter_shape, strlen(splitter_shape), NULL, NULL);
            }


            /*DRAW THE POINTER*/
            arrow_pos.X =indent;//Exact Before The First Char of The First Word (Option) in The Menu and so on
            arrow_pos.Y = vPos+(space_between_Options*(newPosition+1));
            SetConsoleCursorPosition(hOutput, arrow_pos);
            SetConsoleTextAttribute(hOutput, pointer_color);
            WriteConsoleA(hOutput, pointer_shape, strlen(pointer_shape), NULL, NULL);


            /*Draw The Pointed-At-String With Attributes*/
            SetConsoleTextAttribute(hOutput, selectedText_color); // make attributes
            arrow_pos.X = indent+1;
            arrow_pos.Y = vPos+(space_between_Options*(newPosition+1));
            SetConsoleCursorPosition(hOutput, arrow_pos);
            WriteConsoleA(hOutput, container[newPosition+1], length[newPosition+1], NULL, NULL);

            /*Draw The Rest of the Menu*/
            for (i=newPosition+2; i<number_of_options;i++){
                /*For STRINGS*/
                 arrow_pos.X = indent+1;
                 arrow_pos.Y = vPos+(space_between_Options*i);
                 SetConsoleCursorPosition(hOutput, arrow_pos);
                 SetConsoleTextAttribute(hOutput, text_color);
                 WriteConsoleA(hOutput, container[i], length[i], NULL, NULL);
                 /*For Splitters*/
                 arrow_pos.X = indent+1;
                 arrow_pos.Y = SplitterVposition[i];
                 SetConsoleCursorPosition(hOutput, arrow_pos);
                 SetConsoleTextAttribute(hOutput, splliter_color);
                 WriteConsoleA(hOutput, splitter_shape, strlen(splitter_shape), NULL, NULL);

            }


        newPosition++;
        break;
        }

        else{ /*We Are Already At Last String and Want To Back To First Option*/

            /*CLEAR THE POINTER AT LAST STRING*/
            arrow_pos.X = indent;
            arrow_pos.Y = vPos+(space_between_Options*(number_of_options-1));// last option
            SetConsoleCursorPosition(hOutput, arrow_pos);
            SetConsoleTextAttribute(hOutput, 0); // make it black
            WriteConsoleA(hOutput, "", strlen(pointer_shape), NULL, NULL);

            /*DRAW THE POINTER*/
            arrow_pos.X =indent;//Exact Before The First Char of The First Word (Option) in The Menu and so on
            arrow_pos.Y = vPos;
            SetConsoleCursorPosition(hOutput, arrow_pos);
            SetConsoleTextAttribute(hOutput, pointer_color);
            WriteConsoleA(hOutput, pointer_shape, strlen(pointer_shape), NULL, NULL);


            /*Draw The Pointed-At-String With Attributes*/
            SetConsoleTextAttribute(hOutput, selectedText_color); // make attributes
            arrow_pos.X = indent+1;
            arrow_pos.Y = vPos;
            SetConsoleCursorPosition(hOutput, arrow_pos);
            WriteConsoleA(hOutput, container[0], length[0], NULL, NULL);


            /*DRAW THE STRINGS & SPLIITERS BEFORE THE POINTED-AT-STRING IF ANY*/
            for (i=1; i<number_of_options; i++){ // do it at least one time if it's already at position 0 (i.e first option)
                 /*For Splitters*/
                 arrow_pos.X = indent+1;
                 arrow_pos.Y = SplitterVposition[i];//Exact in the middle between strings
                 SetConsoleCursorPosition(hOutput, arrow_pos);
                 SetConsoleTextAttribute(hOutput, splliter_color);
                 WriteConsoleA(hOutput, splitter_shape, strlen(splitter_shape), NULL, NULL);

                 /*For STRINGS*/
                 arrow_pos.X = indent+1;
                 arrow_pos.Y = vPos+(space_between_Options*i);
                 SetConsoleCursorPosition(hOutput, arrow_pos);
                 SetConsoleTextAttribute(hOutput, text_color);
                 WriteConsoleA(hOutput, container[i], length[i], NULL, NULL);

            }



        newPosition=0;
        break;
        }



        case 72: //UP Arrow

        if (newPosition > 0){// if it is not already on the first option in the menu


           /*CLEAR THE POINTER IN The PREVIOUS STRING*/
                arrow_pos.X = indent;
                arrow_pos.Y = vPos+(space_between_Options*newPosition);
                SetConsoleCursorPosition(hOutput, arrow_pos);
                SetConsoleTextAttribute(hOutput, 0); // make it black
                WriteConsoleA(hOutput, "", strlen(pointer_shape), NULL, NULL);


            /*DRAW THE STRINGS & SPLIITERS BEFORE THE POINTED-AT-STRING IF ANY*/
            for (i=(number_of_options-1); i>=newPosition; i--){ // do it at least one time if it's already at position 0 (i.e first option)
                   /*For Splitters*/
                 arrow_pos.X = indent+1;
                 arrow_pos.Y = SplitterVposition[i];//Exact in the middle between strings
                 SetConsoleCursorPosition(hOutput, arrow_pos);
                 SetConsoleTextAttribute(hOutput, splliter_color);
                 WriteConsoleA(hOutput, splitter_shape, strlen(splitter_shape), NULL, NULL);

                 /*For STRINGS*/
                 arrow_pos.X = indent+1;
                 arrow_pos.Y = vPos+(space_between_Options*i);
                 SetConsoleCursorPosition(hOutput, arrow_pos);
                 SetConsoleTextAttribute(hOutput, text_color);
                 WriteConsoleA(hOutput, container[i], length[i], NULL, NULL);
            }


            /*DRAW THE POINTER*/
            arrow_pos.X =indent;//Exact Before The First Char of The First Word (Option) in The Menu and so on
            arrow_pos.Y = vPos+(space_between_Options*(newPosition-1));
            SetConsoleCursorPosition(hOutput, arrow_pos);
            SetConsoleTextAttribute(hOutput, pointer_color);
            WriteConsoleA(hOutput, pointer_shape, strlen(pointer_shape), NULL, NULL);


            /*Draw The Pointed-At-String With Attributes*/
            SetConsoleTextAttribute(hOutput, selectedText_color); // make attributes
            arrow_pos.X = indent+1;
            arrow_pos.Y = vPos+(space_between_Options*(newPosition-1));
            SetConsoleCursorPosition(hOutput, arrow_pos);
            WriteConsoleA(hOutput, container[newPosition-1], length[newPosition-1], NULL, NULL);

            /*Draw The Rest of the Menu*/
            for (i=newPosition-1; i>=number_of_options-1;i--){
                /*For STRINGS*/
                 arrow_pos.X = indent+1;
                 arrow_pos.Y = vPos+(space_between_Options*i);
                 SetConsoleCursorPosition(hOutput, arrow_pos);
                 SetConsoleTextAttribute(hOutput, text_color);
                 WriteConsoleA(hOutput, container[i], length[i], NULL, NULL);
                 /*For Splitters*/
                 arrow_pos.X = indent+1;
                 arrow_pos.Y = SplitterVposition[i];
                 SetConsoleCursorPosition(hOutput, arrow_pos);
                 SetConsoleTextAttribute(hOutput, splliter_color);
                 WriteConsoleA(hOutput, splitter_shape, strlen(splitter_shape), NULL, NULL);

            }


        newPosition--;
        break;
        }

        else{ /*We Are Already At First String and Want To Back To First Option*/

            /*CLEAR THE POINTER AT LAST STRING*/
            arrow_pos.X = indent;
            arrow_pos.Y = vPos;// last option
            SetConsoleCursorPosition(hOutput, arrow_pos);
            SetConsoleTextAttribute(hOutput, 0); // make it black
            WriteConsoleA(hOutput, "", strlen(pointer_shape), NULL, NULL);

                /*DRAW THE STRINGS & SPLIITERS BEFORE THE POINTED-AT-STRING IF ANY*/
            for (i=0; i<number_of_options-2; i++){ // do it at least one time if it's already at position 0 (i.e first option)
                 /*For STRINGS*/
                 arrow_pos.X = indent+1;
                 arrow_pos.Y = vPos+(space_between_Options*i);
                 SetConsoleCursorPosition(hOutput, arrow_pos);
                 SetConsoleTextAttribute(hOutput, text_color);
                 WriteConsoleA(hOutput, container[i], length[i], NULL, NULL);

                 /*For Splitters*/
                 arrow_pos.X = indent+1;
                 arrow_pos.Y = SplitterVposition[i];//Exact in the middle between strings
                 SetConsoleCursorPosition(hOutput, arrow_pos);
                 SetConsoleTextAttribute(hOutput, splliter_color);
                 WriteConsoleA(hOutput, splitter_shape, strlen(splitter_shape), NULL, NULL);

            }

            /*DRAW THE POINTER*/
            arrow_pos.X =indent;
            arrow_pos.Y = vPos+(space_between_Options*(number_of_options-1)); // the last option
            SetConsoleCursorPosition(hOutput, arrow_pos);
            SetConsoleTextAttribute(hOutput, pointer_color);
            WriteConsoleA(hOutput, pointer_shape, strlen(pointer_shape), NULL, NULL);


            /*Draw The Pointed-At-String With Attributes*/
            SetConsoleTextAttribute(hOutput, selectedText_color); // make attributes
            arrow_pos.X = indent+1;
            arrow_pos.Y = vPos+(space_between_Options*(number_of_options-1));
            SetConsoleCursorPosition(hOutput, arrow_pos);
            WriteConsoleA(hOutput, container[number_of_options-1], length[number_of_options-1], NULL, NULL);





        newPosition=(number_of_options-1);
        break;
        }

}
}
}
arrow_pos.X = 0;
arrow_pos.Y = vPos+(space_between_Options* number_of_options);
SetConsoleCursorPosition(hOutput,arrow_pos);
SetConsoleTextAttribute(hOutput, 15);
CONSOLE_CURSOR_INFO info; //Contains information about the console cursor.
info.dwSize = 1;//The percentage of the character cell that is filled by the cursor. This value is between 1 and 100.
info.bVisible = TRUE;
SetConsoleCursorInfo(hOutput, &info);//Sets the size and visibility of the cursor for the specified console screen buffer.
return newPosition+1;
}



void displayColorsValues(){
printf("\n*color your text in Windows console mode:\n*colors are 0=black 1=blue 2=green and so on to 15=white\n*color Attribute = ( Foreground + (Background * 16) )\n*e.g To get Blue text on Red background use: (1 + (4 * 16) = 65)\n*The Following Are The Colors Will Be Produced For Every Value Passed To This Function\n\n");
unsigned int k;
HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
for(k = 1; k < 255; k++){
SetConsoleTextAttribute(hOutput, k);
printf("Value %u Gives This Color\n", k);
}
}


