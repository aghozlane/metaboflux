/**
 * This file is part of MetaBoFlux (http://www.cbib.u-bordeaux2.fr/metaboflux/)
 * Copyright (C) 2010 Amine Ghozlane from LaBRI and University of Bordeaux 1
 *
 * MetaBoFlux is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * MetaBoFlux is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file MetaBoFlux.c
 * \brief Program for simulated annealing analysis
 * \author {Amine Ghozlane}
 * \version 3.0
 * \date 9 novembre 2009
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include <getopt.h>
#include <math.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_multimin.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_siman.h>
#include <gsl/gsl_ieee_utils.h>
#include <gsl/gsl_statistics_double.h>
#include <sbml/SBMLTypes.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include "mpi.h"
#include "especes.h"
#include "equations.h"
#include "simulation.h"
#include "xml_parameter.h"
#include "data_parameters.h"
#include "gsl_recuit.h"
#include "gsl_mod.h"
#include "gsl_min.h"
#include "gsl_sd.h"
#include "mpi_load.h"
/*#include <mcheck.h>*/

/***********************/
/* Programme principal */
/***********************/

/* Liste des arguments */
/**
* \struct ListArguments
* \brief List of arguments
*/
typedef struct ListArguments
{
  int activity;          /*!< Activity chosen */
  int debug;             /*!< Debug chosen */
  int port;              /*!< Port chosen */
  int group;             /*!< Group chosen */
  char **files_path;    /*!< Path to files path : 0.SBML 1.PAR 2.OUT 3.RATIO_FILE 4.LOG 5.OUTPUT*/
}ListArguments, *pListArguments;

/**
* \struct option
* \brief List of flag for getopt
*/
const struct option long_options[15] =
    {
        /* These options don't set a flag. We distinguish them by their indices. */
        {"simulated_annealing",     no_argument,       0, 's'}, /*!< Simulated annealing chosen */
        {"minimization",  no_argument,       0, 'm'}, /*!< Minimization chosen */
        {"modelling",  no_argument, 0, 'e'}, /*!< Modelling chosen */
        {"standard_deviation",  no_argument, 0, 't'}, /*!< Standard deviation chosen */
        {"group",  no_argument, 0, 'u'}, /*!< Group chosen */
        {"debug",  no_argument, 0, 'd'}, /*!< Debug chosen */
        {"sbml",    required_argument, 0, 'l'}, /*!< SBML chosen */
        {"par",    required_argument, 0, 'r'}, /*!< Parameter chosen */
        {"log",    required_argument, 0, 'i'}, /*!< Log chosen */
        {"out_file",    required_argument, 0, 'g'}, /*!< Modelling chosen */
        {"out",    required_argument, 0, 'o'}, /*!< Out repertory chosen */
        {"ratio_file",  required_argument, 0, 'a'}, /*!< ratio_file chosen */
        {"port",    required_argument, 0, 'p'},  /*!< Port chosen */
        {"help",    no_argument, 0, 'h'},  /*!< Help chosen */
        {0, 0, 0, 0}  /*!< Empty chosen */
    };

/**
 * \fn pListArguments alloc_arguments(void)
 * \author Amine Ghozlane
 * \brief  Alloc memory for the struct pListArguments
 */
pListArguments alloc_arguments(void)
{
  pListArguments args=NULL;
  int i, nb_files=6;
  args=(pListArguments)malloc(1*sizeof(ListArguments));
  assert(args!=NULL);
  args->files_path=NULL;
  args->files_path=(char**)malloc(nb_files*sizeof(char*));
  assert(args->files_path!=NULL);

  /* Initialisation */
  for(i=0;i<nb_files;i++){
      args->files_path[i]=NULL;
  }
  args->activity=0;
  args->debug=0;
  args->port=0;
  args->group=0;
  /*args->row=0;*/
  return args;
}

/**
 * \fn void free_arguments(pListArguments arguments)
 * \author Amine Ghozlane
 * \brief  Free the struct pListArguments
 * \param  arguments struct pListArguments
 */
void free_arguments(pListArguments args)
{
  if(args->files_path!=NULL) free(args->files_path);
  if(args!=NULL) free(args);
}

/**
 * \fn void help_print(void)
 * \author Amine Ghozlane
 * \brief  Print the usage help
 */
void help_print(void)
{
  int i,a=0;
  a=printf("Option\t\tCase\t\tFilename\tType\tDescription\a\n");
  for(i=0;i<(a+40);i++)
    printf("-");
  printf("\n");
  printf("-s\t\tno\t\tno\t\tno\tSimulated_annealing\n");
  printf("-m\t\tno\t\tno\t\tno\tMinimization\n");
  printf("-e\t\tno\t\tno\t\tno\tModelling\n");
  printf("-t\t\tno\t\tno\t\tno\tStandard_deviation\n");
  printf("-p\t\t-s -m -e -t\tno\t\tno\tPort\n");
  printf("--group\t\t-e\t\tno\t\tno\tGroup\n");
  printf("--number_group\t-e\t\tno\t\tInput\tNumber of the group\n");
  printf("--ratio_file\t-m -e -t\tno\t\tInput\tRatio file\n");
  printf("--debug\t\t-s -m -e -t\tno\t\tOutput\tDebug file output\n");
  printf("--sbml\t\t-s -m -e -t\tsbml.xml\tInput\tSBML file\n");
  printf("--par\t\t-s -m -e -t\tparameter.xml\tInput\tParameter file\n");
  printf("--log\t\t-s -m -e -t\tparameter.xml\tInput\tLog file\n");
  printf("--out\t\t-s -m -e -t\trepertory/\tOutput\tResult repertory\n");
}

/**
 * \fn void error_activity(int activity)
 * \author Amine Ghozlane
 * \brief  Print the error notification
 * \param  activity Activity chosen
 */
void error_activity(int activity)
{
  if(activity!=0) fprintf(stderr,"Error : activity [%d] has already been chosen, you can only do one activity at a time\n",activity);
  help_print();
  exit(EXIT_FAILURE);
}

/**
 * \fn void argument_analysis(int argc, char **argv, pListArguments args)
 * \author Amine Ghozlane
 * \brief  Print the error notification
 * \param  argc Number of arguments
 * \param  argv List of arguments
 * \param  args struct ListArguments
 */
void argument_analysis(int argc, char **argv, pListArguments args)
{
  int c=0, option_index=0;
  while (1){
      /* getopt_long stores the option index here. */
      option_index = 0;

      /*c = getopt_long (argc, argv, "smgtl:r:o:dp:h",long_options, &option_index);*/
      c = getopt_long (argc, argv, "smetudl:r:g:o:n:a:i:g:p:h",long_options, &option_index);

      /* Detect the end of the options. */
      if (c == -1) break;

      switch (c)
      {
      /*TODO A virer*/
      case 0:
        /* If this option set a flag, do nothing else now. */
        if (long_options[option_index].flag != 0)
          break;
        printf ("option %s", long_options[option_index].name);
        if (optarg)
          printf (" with arg %s", optarg);
        printf ("\n");
        break;

      /* Choix du recuit simule */
      case 's':
        if(args->activity==0){
            printf("Metaboflux chosen activity : [1] Simulated annealing\n");
            args->activity=1;
        }
        else error_activity(args->activity);
        break;

      /* Choix de la minimisation*/
      case 'm':
        if(!args->activity){
            printf("Metaboflux chosen activity : [2] Minimization\n");
            args->activity=2;
        }
        else error_activity(args->activity);
        break;

      /* Choix du modeling */
      case 'e':
        if(!args->activity){
            printf("Metaboflux chosen activity : [3] Modeling\n");
            args->activity=3;
        }
        else error_activity(args->activity);
        break;

      /* Choix de la standard deviation */
      case 't':
        if(!args->activity){
            printf("Metaboflux chosen activity : [4] Standard deviation\n");
            args->activity=4;
        }
        else error_activity(args->activity);
        break;

      /* Debug */
      case 'd':
        args->debug=1;
        break;

      /* PATH du fichier SBML */
      case 'l':
        args->files_path[0]=(char*)malloc(((int)strlen(optarg)+1)*sizeof(char));
        assert(args->files_path[0]!=NULL);
        strcpy(args->files_path[0], optarg);
        break;

      /* PATH du fichier Parameter */
      case 'r':
        args->files_path[1]=(char*)malloc(((int)strlen(optarg)+1)*sizeof(char));
        assert(args->files_path[1]!=NULL);
        strcpy(args->files_path[1], optarg);
        break;

      /* PATH du repertory out */
      case 'o':
        args->files_path[2]=(char*)malloc(((int)strlen(optarg)+1)*sizeof(char));
        assert(args->files_path[2]!=NULL);
        strcpy(args->files_path[2], optarg);
        break;

      /* PATH du fichier ratio */
      case 'a':
        args->files_path[3]=(char*)malloc(((int)strlen(optarg)+1)*sizeof(char));
        assert(args->files_path[3]!=NULL);
        strcpy(args->files_path[3], optarg);
        break;

      /* PATH du fichier out_file */
      case 'g':
        args->files_path[4]=(char*)malloc(((int)strlen(optarg)+1)*sizeof(char));
        assert(args->files_path[4]!=NULL);
        strcpy(args->files_path[4], optarg);
        break;

      /* PATH du fichier Log */
      case 'i':
        args->files_path[5]=(char*)malloc(((int)strlen(optarg)+1)*sizeof(char));
        assert(args->files_path[5]!=NULL);
        strcpy(args->files_path[5], optarg);
        break;
      /* Port */
      case 'p':
        args->port=atoi(optarg);
        break;
     /* Group */
     case 'u':
       args->group=1;
       break;
     /* Help */
     case 'h':
        help_print();
        exit(0);
        break;
      /* Error */
      case '?':
        /* getopt_long already printed an error message. */
        break;

      default:
        abort ();
      }
  }

  /* Print any remaining command line arguments (not options). */
  if (optind < argc)
    {
      printf ("non-option ARGV-elements: ");
      while (optind < argc)
        printf ("%s ", argv[optind++]);
      putchar ('\n');
    }
}

/**
 * \fn void check_common(pListArguments args)
 * \author Amine Ghozlane
 * \brief  Check obligatory arguments
 * \param  args struct ListArguments
 */
void check_common(pListArguments args)
{
  /* Pas de SBML */
  if(args->files_path[0]==NULL){
      fprintf(stderr,"SBML file is missing\n");
      help_print();
      exit(EXIT_FAILURE);
  }
  /* Pas de par */
  if(args->files_path[1]==NULL){
      fprintf(stderr,"Parameter file is missing\n");
      help_print();
      exit(EXIT_FAILURE);
  }
  /* Pas de repertoire de sortie des resultats */
  if(args->files_path[2]==NULL){
      fprintf(stderr,"Out repertory is missing\n");
      help_print();
      exit(EXIT_FAILURE);
  }

}

/**
 * \fn void check_arguments(pListArguments args)
 * \author Amine Ghozlane
 * \brief  Check program arguments
 * \param  args struct ListArguments
 */
void check_arguments(pListArguments args)
{
  /* Verifie les arguments obligatoires */
  check_common(args);
  /* Verifications des paths */
  if(args->activity==2||args->activity==3||args->activity==4){
      /* Pas de fichier ratio */
      if(args->files_path[3]==NULL){
          fprintf(stderr,"ratio_file is missing\n");
          help_print();
          exit(EXIT_FAILURE);
      }
      /* Pas de out file */
      if(args->files_path[4]==NULL){
          fprintf(stderr,"out_file is missing\n");
          help_print();
          exit(EXIT_FAILURE);
      }
  }
  /* Verification du path log */
  if(args->activity==2||args->activity==3){
      /* Pas de out file */
      if(args->files_path[5]==NULL){
          fprintf(stderr,"log file is missing\n");
          help_print();
          exit(EXIT_FAILURE);
      }
  }
}

/**
 * \fn int main(int argc, char **argv)
 * \author Amine Ghozlane
 * \brief  Enter in the program for simulated annealing
 * \param  argc Number of arguments
 * \param  argv List of arguments
 * \return EXIT_SUCCESS Normal stop of the program
 */
int main(int argc, char **argv)
{
  /*mtrace();*/
  /* File adress */
  pListArguments args=NULL;

  if(argc==1){
      fprintf(stderr,"Do './MetaBoFlux.exe -h' for a usage summary\n");
      return EXIT_FAILURE;
  }
  /* Allocation memoire de la structure contenant les informations des arguments */
  args=alloc_arguments();

  /* Analyse des arguments fournis avec getopt */
  argument_analysis(argc,argv,args);

  /* Verification des arguments */
  check_arguments(args);

  /* Realise les simulations */
  compute_mpi(argc,argv,args->files_path,args->activity,args->group, args->debug,args->port);

  /* Liberation de la memoire */
  free_arguments(args);

  return EXIT_SUCCESS;
}
