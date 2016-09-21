#ifndef CMENU_H_INCLUDED
#define CMENU_H_INCLUDED


int HMenu(int indent, int vPos, int space_between_Options, char *pointer_shape, int pointer_color, char *splitter_shape, int splliter_color, int text_color, int selectedText_color, int number_of_options, ...);// For Horizontal Menu


int VMenu(int indent, int vPos, int space_between_Options, char *pointer_shape, int pointer_color, char *splitter_shape, int splliter_color, int text_color, int selectedText_color,  int number_of_options, ...);// For Vertical Menu

void displayColorsValues(); // display colors options and their values that can be used in the above menus
#endif // CMENU_H_INCLUDED
