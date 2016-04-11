#pragma once

/*global variable holding the warn level load*/
extern double warn_lvl;

/*gets server load*/
int get_load(double *loads);

/*creates the warnload thread function*/
static void *warn_thread(void *param);

/*creates the thread for warnload*/
void create_warnthread();
