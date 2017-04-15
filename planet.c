#define _GNU_SOURCE
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <plot.h>

int MAX_NUMBER_OF_PLANETS = 5;

typedef struct _planet {
	float size, mass, xPosition, yPosition, xVelocity, yVelocity;
	char name[20], color[20];
} Planet;

float maxSpaceSize(Planet** listOfPlanets) {

	float maxPosition = 0.0;

	for(int i = 0; i < MAX_NUMBER_OF_PLANETS; i++) {
		if (fabsf(listOfPlanets[i]->xPosition) > maxPosition) {
		 	maxPosition = listOfPlanets[i]->xPosition;
		}
	}
	return maxPosition;
}

Planet** scan(char* argv[]) {
	char const* const arg = argv[1];
	FILE* fd = fopen(arg, "r");

	if (fd == NULL) {
		fprintf(stderr, "Error: file %s could bot be opened for read.   \n", arg);
		return 0;
	}

	Planet** listOfPlanets = (Planet**) calloc(MAX_NUMBER_OF_PLANETS, sizeof(Planet*));

	for (int i =0; i < MAX_NUMBER_OF_PLANETS; i++) {
		listOfPlanets[i] = NULL;
	}

	float size, mass, xPosition, yPosition, xVelocity, yVelocity;
	char name[20], color[20];


	char *line = NULL;
    size_t len = 0;
    ssize_t read;

    int j = 0;
	while ((read = getline(&line, &len, fd)) != -1 && j < MAX_NUMBER_OF_PLANETS) {
	if (line[0] == '#' || read == 1) {
  	 	continue;
  	}
  	else{
      sscanf(line, "%s %s %f %f %f %f %f %f", name, color, &size, &mass, &xPosition, &yPosition, &xVelocity, &yVelocity);

     Planet* currentPlanet = malloc(sizeof(Planet));
     strcpy(currentPlanet->name, name);
     strcpy(currentPlanet->color, color);
     currentPlanet->size = size;
     currentPlanet->mass = mass;
     currentPlanet->xPosition = xPosition;
     currentPlanet->yPosition = yPosition;
     currentPlanet->xVelocity = xVelocity;
     currentPlanet->yVelocity = yVelocity;
     listOfPlanets[j] = currentPlanet;
    }
    j++;
    }
  	MAX_NUMBER_OF_PLANETS = j;
    fclose(fd);
	return listOfPlanets;
}

plPlotter* erasePlanet(plPlotter *plotter, Planet** listOfPlanets) {

	pl_pentype_r(plotter,1);
	pl_filltype_r(plotter,1);

	for (int i=0; i < MAX_NUMBER_OF_PLANETS; i++) {
		pl_pencolorname_r(plotter, "black");
		pl_fillcolorname_r(plotter, "black");
		pl_fcircle_r(plotter, listOfPlanets[i]->xPosition, listOfPlanets[i]->yPosition, (listOfPlanets[i]->size*100000000000));
	}
	sleep(2);
  	return plotter;
}

plPlotter* plotPlanet(plPlotter *plotter, Planet** listOfPlanets) {

	pl_pentype_r(plotter,1);
	pl_filltype_r(plotter,1);

	for (int i=0; i < MAX_NUMBER_OF_PLANETS; i++) {
		pl_pencolorname_r(plotter, listOfPlanets[i]->color);
		pl_fillcolorname_r(plotter, listOfPlanets[i]->color);
		pl_fcircle_r(plotter, listOfPlanets[i]->xPosition, listOfPlanets[i]->yPosition, (listOfPlanets[i]->size*100000000000));
	}
	sleep(2);
  	return plotter;
}

void createWindow(char** argv){

	/* plotter device structures */
  	plPlotter *plotter;
  	plPlotterParams *plotterParams;

  	/* create a plotter parametric structure */
  	plotterParams = pl_newplparams();
  	pl_setplparam(plotterParams, "BITMAPSIZE", "750x750");
  	pl_setplparam(plotterParams, "USE_DOUBLE_BUFFERING", "no");
	pl_setplparam(plotterParams, "BG_COLOR", "black");

	/* create the plotter device and open it */
  	if ((plotter = pl_newpl_r("X", stdin, stdout, stderr, plotterParams)) == NULL) {
    		fprintf(stderr, "Couldn't create Xwindows plotter\n");
    		exit(1);
  	} else if (pl_openpl_r(plotter) < 0) {
    		fprintf(stderr, "Couldn't open Xwindows plotter\n");
    		exit(1);
  	}

  	Planet** listOfPlanets = scan(argv);

	float max =	1.1*maxSpaceSize(listOfPlanets);

	/* set our coordinate space in the plotter window */
	pl_fspace_r(plotter, -max, -max, max, max);

	plotter = plotPlanet(plotter, listOfPlanets);
	plotter = erasePlanet(plotter, listOfPlanets);
	listOfPlanets[0]->xPosition = -5000000;
	plotter = plotPlanet(plotter, listOfPlanets);
	/* close and cleanup the plotter stuff */
  	if (pl_closepl_r(plotter) < 0) {
    		fprintf(stderr, "Couldn't close plotter\n");
  	} else if (pl_deletepl_r(plotter) < 0) {
    		fprintf(stderr, "Couldn't delete plotter\n");
  	}
}

int main(int argc, char** argv) {

	createWindow(argv);

	return 0;
}
