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
 * \file mpi_load.c
 * \brief Parallelize the program
 * \author {Amine Ghozlane}
 * \version 2.0
 * \date 9 novembre 2009
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <assert.h>
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
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "xml_parameter.h"
#include "especes.h"
#include "equations.h"
#include "simulation.h"
#include "data_parameters.h"
#include "gsl_recuit.h"
#include "gsl_mod.h"
#include "gsl_min.h"
#include "gsl_sd.h"
#include "mpi.h"
#include "mpi_load.h"
#define BUFSIZE 100

#define MASTER_RANK 0           /* Rang du master */
#define WRITER_RANK 1

/**
 * \fn int Mpi_connectInterface(int port)
 * \author Amine Ghozlane
 * \brief  Enter in the program for standard deviation
 * \param  port Connection port
 * \return Socket
 */
int Mpi_connectInterface(int port)
{
  /* Connexion a l'interface */
  struct sockaddr_in adresse;
  int desc=0;

  /* Creation de la socket */
  if ((desc=socket(AF_INET,SOCK_STREAM,0)) == -1){
    perror("Socket creation is impossible\n");
    fprintf(stderr,"Socket creation is impossible\n");
    return EXIT_FAILURE;
  }

  /* Preparation de l'adresse d'attachement = adresse IP Internet */
  adresse.sin_family=AF_INET;

  /* Indication de l'adresse IP locale de la socket */
  /* Conversion (representation interne) -> (reseau) avec htonl et htons */
  adresse.sin_addr.s_addr=htonl(INADDR_ANY); /* toutes les interfaces presentes */

  /* Indication du port local de la socket */
  /* si port = 0, l'adresse sera choisie au hasard par le système au dessus de 1024 */
  adresse.sin_port=htons(port);

  /* Connexion de la socket */
  if (connect(desc,(struct sockaddr*)&adresse,sizeof(adresse)) == -1) {
    perror("Error cannot connect to the server\n");
    fprintf(stderr,"Error cannot connect to the server\n");
    return EXIT_FAILURE;
  }

  return desc;
}

/**
 * \fn void Mpi_disconnectInterface(int desc)
 * \author Amine Ghozlane
 * \brief  Disconnection to the interface
 * \param  desc Socket
 */
void Mpi_disconnectInterface(int desc)
{
  /* Deconnexion de l'interface */
  close(desc);
}

/**
 * \fn void Mpi_init(int argc, char **argv, int *tab)
 * \author Amine Ghozlane
 * \brief  Enter in the program for standard deviation
 * \param  argc Number of arguments
 * \param  argv List of arguments
 * \param  tab Table
 */
void Mpi_init(int argc, char **argv, int *tab)
{
  /* Initialisation de MPI */
  int namelen;
  char processor_name[MPI_MAX_PROCESSOR_NAME];

  /* Initialise l'execution de l'environnement MPI */
  MPI_Init( &argc, &argv );

  /* Determine le nombre de processus associes au communicateur */
  MPI_Comm_size(MPI_COMM_WORLD,&tab[1]);

  /* Determine le rank du processus associe au communicateur */
  MPI_Comm_rank(MPI_COMM_WORLD,&tab[0]);

  /* Determine le nom du processeur */
  MPI_Get_processor_name(processor_name,&namelen);
}

/**
 * \fn void Mpi_master(pListParameters allone, char **files_path, int activity, int group, int myid, int numprocs, int desc)
 * \author Amine Ghozlane
 * \brief  Master process
 * \param  allone Global parameters : struct ListParameters
 * \param  files_path List of paths
 * \param  activity Chosen activity
 * \param  group Group flag
 * \param  myid Id of the thread
 * \param  numprocs Number of thread
 * \param  desc Socket
 */
void Mpi_master(pListParameters allone, char **files_path, int activity, int group, int myid, int numprocs, int desc)
{
  /* Processus Maitre */
  FILE *ratioFile=NULL;
  double *tabRatio=NULL;
  int i=0,j=0,k=0,count=0, destination=0, sender=0, request=0, answer=0,row=0, operation=0, proc=0,a=0,flag=0;
  /*double startwtime = 0.0, endwtime=0.0;*/
  char s[255];
  MPI_Status status;

  /* Lecture du nombre d'operation a realiser */
  if(group) operation=Xml_getNbGroup(allone->conf);
  else operation=Xml_getNbSimulations(allone->conf);

  /* Ouverture du fichier de ratio */
  if(activity==2 ||activity==3 ||activity==4){
      if((ratioFile=fopen(files_path[3],"rt"))==NULL){
          fprintf(stderr,"Error opening the file : %s\n",files_path[3]);
          exit(EXIT_FAILURE);
      }
      tabRatio=(double *)malloc((allone->nb_parameters)*sizeof(double));
  }

  /* Debut du chronometre */
 /* startwtime = MPI_Wtime();*/

  /* Message de broadcast initiant les processus esclaves */
  MPI_Bcast(&operation, 1, MPI_INT, MASTER_RANK, MPI_COMM_WORLD);

  /* Envoie leur indice de tache*/
  for (destination = 0; destination < numprocs; destination++) {
      if (destination != myid) {
          if(activity==1){
              MPI_Send(&request,1,MPI_INT, destination, count,MPI_COMM_WORLD);
              k++;
              count = count + 1;
              proc = proc +1;
          }
          else if(destination!=WRITER_RANK){
              MPI_Send(&request,1,MPI_INT, destination, count,MPI_COMM_WORLD);
              k++;

              /*Lecture des ratio */
              for(j=0;j<allone->nb_parameters;j++){
                  a=fscanf(ratioFile,"%lf\t",&tabRatio[j]);
              }

              /* Envoie des ratio */
              MPI_Send(tabRatio, allone->nb_parameters, MPI_DOUBLE,  destination, count,MPI_COMM_WORLD);

              count = count + 1;
              proc = proc +1;
          }
      }
  }

  /* On doit faire x fois la tache */
  i=0;
  if(activity==1) count = numprocs-2;
  else count = numprocs-3;

  while(i<operation&&proc>0){
      /* Attend les confirmations de fin de tache*/
      while(!flag){
          sleep(0.1);
          MPI_Iprobe(MPI_ANY_SOURCE,MPI_ANY_TAG, MPI_COMM_WORLD,&flag,&status);
      }
      MPI_Recv (&answer,1, MPI_INT, MPI_ANY_SOURCE,MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      proc = proc - 1;
      flag=0;
      /* Chargement du status de l'esclave qui a termine sa tache  */
      sender = status.MPI_SOURCE;
      row = status.MPI_TAG;

      /* Lancement d'un calcul */
      if(i<operation){
          i++;
          count = count + 1;
          printf("(%d/%d)\n",i,operation);
          sprintf(s, "(%i/%i)",i,operation);
          send(desc,s,strlen(s),0);
          k++;

          /* Envoie d'une nouvelle tache */
          if(k<=operation && a!=-1){
              MPI_Send(&request, 1, MPI_INT, sender, count,MPI_COMM_WORLD);

              /* Envoie des ratios */
              if(activity!=1){
                  /* Lecture des ratios du fichier ratio */
                  for(j=0;j<allone->nb_parameters;j++){
                      a=fscanf(ratioFile,"%lf\t",&tabRatio[j]);
                  }
                  /* Envoie des ratios */
                  MPI_Send(tabRatio, allone->nb_parameters, MPI_DOUBLE, sender, count,MPI_COMM_WORLD);
              }
          }
          proc=proc+1;
      }
  }

  /* Termine les processus */
  for (destination = 0; destination < numprocs; destination++) {
      if (destination != myid) {
          MPI_Send(&request, 0, MPI_INT, destination,operation,MPI_COMM_WORLD);
      }
  }

  if(activity==2 ||activity==3 ||activity==4){
      free(tabRatio);
      fclose(ratioFile);
  }
}

/**
 * \fn void Mpi_writer(pListParameters allone, char **files_path, int activity, int group, int myid)
 * \author Amine Ghozlane
 * \brief  Master process
 * \param  allone Global parameters : struct ListParameters
 * \param  files_path List of paths
 * \param  activity Chosen activity
 * \param  group Group flag
 * \param  myid Id of the thread
 */
void Mpi_writer(pListParameters allone, char **files_path, int activity, int group, int myid)
{
  /* Processus ecrivain */
  FILE *out=NULL, *logOut=NULL;
  int answer=0, operation=0, i=0, tailleTab=0, tailleSpecies=(int)Model_getNumSpecies(allone->model),nb_species=Xml_getNbSpecies(allone->conf), flag=0;
  double *result_tab=NULL;
  MPI_Status status; MPI_Request req;

  /* Allocation du tableau de resultats */
  tailleTab=Mpi_sizeResultTab(allone, activity, group, nb_species);
  result_tab=Mpi_allocResultTab(tailleTab);

  /* Min and Mod */
  if(activity!=4){
      /* Sortie du log */
      if((logOut=fopen(files_path[5],"at"))==NULL){
          fprintf(stderr,"Error opening the file : %s\n",files_path[5]);
          exit(EXIT_FAILURE);
      }
  }
  /* Sortie des resultats */
  if((out=fopen(files_path[4],"at"))==NULL){
    fprintf(stderr,"Error opening the file : %s\n",files_path[4]);
    exit(EXIT_FAILURE);
  }

  /* Reception du message de Broadcast */
  MPI_Bcast(&operation, 1, MPI_INT, MASTER_RANK, MPI_COMM_WORLD);

  while (i < operation) {
      while(!flag){
          sleep(0.1);
          MPI_Iprobe(MPI_ANY_SOURCE,MPI_ANY_TAG, MPI_COMM_WORLD,&flag,&status);
      }
      MPI_Recv (result_tab,tailleTab, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG,MPI_COMM_WORLD, &status);
      flag=0;
      /* Ecriture modeling / minimization */
      if(activity!=4){
          Mpi_writeSimFile(allone, out, logOut, result_tab, group, tailleSpecies, nb_species);
      }
      /* Ecriture standard deviation */
      else  Mpi_writeSdFile(out, result_tab, tailleTab);
      i++;
  }
  /* Mise en attente d'un message de poursuite ou de fin   */
  while(!flag){
      sleep(0.1);
      MPI_Iprobe(MASTER_RANK,MPI_ANY_TAG, MPI_COMM_WORLD,&flag,&status);
  }
  MPI_Irecv (&answer,1, MPI_INT, MASTER_RANK, MPI_ANY_TAG,MPI_COMM_WORLD,  &req);
  flag=0;
  /* Fermeture des fichiers */
  if(out!=NULL)fclose(out);
  if(logOut!=NULL)fclose(logOut);

  /* Desallocation de la memoire du tableau de resultat */
  if(result_tab!=NULL) free(result_tab);
}

/**
 * \fn int Mpi_sizeResultTab(pListParameters allone, int activity, int group, int nb_species)
 * \author Amine Ghozlane
 * \brief  Determine the size of the result tab
 * \param  allone Global parameters : struct ListParameters
 * \param  activity Chosen activity
 * \param  group Group flag
 * \param  nb_species Number of interest species
 * \return Size of Result tab
 */
int Mpi_sizeResultTab(pListParameters allone, int activity, int group, int nb_species)
{
  int tailleTab=0;
  /* Cas standard deviation */
  if(activity==4) tailleTab=2+Xml_getNbTriesMod(allone->conf);
  /* Cas modeling and minimization */
  else if(activity!=1){
      /* Cas des groupes */
      if(group) tailleTab=2+(int)Model_getNumSpecies(allone->model)+allone->nb_parameters+nb_species;
      else tailleTab=1+(int)Model_getNumSpecies(allone->model)+allone->nb_parameters+nb_species;
  }
  return tailleTab;
}

/**
 * \fn double *Mpi_allocResultTab(int tailleTab)
 * \author Amine Ghozlane
 * \brief  Allocate the result tab
 * \param  tailleTab Size of Result tab
 * \return Adress of the allocated space
 */
double *Mpi_allocResultTab(int tailleTab)
{
  double *result_tab=NULL;
  result_tab=(double*)malloc(tailleTab*sizeof(double));
  assert(result_tab!=NULL);
  return result_tab;
}

/**
 * \fn void Mpi_writeSimFile(pListParameters allone, FILE *out, FILE *logOut, double *result_tab, int group, int tailleSpecies, int nb_species)
 * \author Amine Ghozlane
 * \brief  Determine the size of the result tab
 * \param  allone Global parameters : struct ListParameters
 * \param  out Result file
 * \param  logOut Log file
 * \param  result_tab Result table
 * \param  group Group flag
 * \param  tailleSpecies Number of species
 * \param  nb_species Number of interest species
 */
void Mpi_writeSimFile(pListParameters allone, FILE *out, FILE *logOut, double *result_tab, int group, int tailleSpecies, int nb_species)
{
  /* Sortie du resultat des simulations */
    int i,j=0;

    /* Affichage du numero de groupe */
    if(group){
        fprintf(out,"H%.0f\t",result_tab[j]);
        fprintf(logOut,"H%.0f\t",result_tab[j]);
        j++;
    }

    /* Affiche l'energie du systeme */
    fprintf(out,"%.3f\t",result_tab[j]);
    fprintf(logOut,"%.3f\t",result_tab[j]);
    j++;

    /* Affiche la quantite des especes etudiees */
    for(i=j;i<(j+nb_species);i++){
        fprintf(out,"%.0f\t", result_tab[i]);
    }
    j+=nb_species;

    /* Impression des quantites de toutes les especes */
    for(i=j;i<(j+tailleSpecies);i++){
      fprintf(logOut,"%.0f\t",result_tab[i]);
    }
    j+=tailleSpecies;

    /* Affiche les ratios identifies pour les reactions du systeme */
    for (i = j; i < (j+allone->nb_parameters); i++){
      fprintf(out,"%.2f\t",result_tab[i]);
      fprintf(logOut,"%.2f\t",result_tab[i]);
    }
    fprintf(out,"\n");
    fprintf(logOut,"\n");
}

/**
 * \fn void Mpi_writeSdFile(FILE *out, double *result_tab, int tailleTab)
 * \author Amine Ghozlane
 * \brief  Determine the size of the result tab
 * \param  out Result file
 * \param  result_tab Result table
 * \param  tailleTab Size of result table
 */
void Mpi_writeSdFile(FILE *out, double *result_tab, int tailleTab)
{
  /* Ecriture standard deviation */
  int i;
  /* Affichage de l'energie */
  for(i=2;i<tailleTab;i++){
      fprintf(out,"%.0f %.3f %.3f\n",result_tab[0],result_tab[i],result_tab[1]);
  }
}

/**
 * \fn void Mpi_slave(pListParameters allone, char **files_path, int activity, int group, int debug)
 * \author Amine Ghozlane
 * \param  allone Global parameters : struct ListParameters
 * \param  files_path List of paths
 * \param  activity Activity chosen
 * \param  group Group flag
 * \param  debug Debug flag
 */
void Mpi_slave(pListParameters allone, char **files_path, int activity, int group, int debug)
{
  /* Processus esclave */
  int row=0, request=0, answer=0, operation, tailleTab=0, nb_species=Xml_getNbSpecies(allone->conf), flag=0;
  double *fluxRatio=NULL, *result_tab=NULL;
  MPI_Status status; /*MPI_Request req;*/
  gsl_siman_params_t params = {Xml_getNbTriesSa(allone->conf),Xml_getNbIters(allone->conf),
      Xml_getStepSize(allone->conf),Xml_getBoltzmann(allone->conf), Xml_getTinitial(allone->conf),
      Xml_getMuT(allone->conf), Xml_getTmin(allone->conf)};

  /* Alloc flux */
  fluxRatio=(double*)malloc(allone->nb_parameters*sizeof(double));
  assert(fluxRatio!=NULL);

  /* Reception du message de Broadcast */
  MPI_Bcast(&operation, 1, MPI_INT, MASTER_RANK, MPI_COMM_WORLD);

  /* Reception de son tag */
  MPI_Recv(&answer, 1, MPI_INT, MASTER_RANK, MPI_ANY_TAG,MPI_COMM_WORLD, &status);

  /* Allocation du tableau de resultats */
  tailleTab=Mpi_sizeResultTab(allone, activity, group,nb_species);
  result_tab=Mpi_allocResultTab(tailleTab);

  /* Debut calcul*/
  while (status.MPI_TAG < operation) { /* The job is not finished */
    row = status.MPI_TAG;
    request=0;

    /* Lancement du calcul */
    switch(activity){
    case 1:
      /* Simulated annealing */
      Recuit_compute_recuit( files_path, debug, row, allone, params);
      break;
    case 2:
      flag=0;
      /* Reception des ratios */
      MPI_Recv (fluxRatio,allone->nb_parameters, MPI_DOUBLE, MASTER_RANK, MPI_ANY_TAG,MPI_COMM_WORLD, &status);

      /* Minimization */
      Min_compute_minimization(allone, fluxRatio, result_tab, files_path, row, debug);
      MPI_Send(result_tab, tailleTab, MPI_DOUBLE, WRITER_RANK, row, MPI_COMM_WORLD);
      break;
    case 3:
      flag=0;
      /* Reception des ratios */
      MPI_Recv (fluxRatio,allone->nb_parameters, MPI_DOUBLE, MASTER_RANK, MPI_ANY_TAG,MPI_COMM_WORLD, &status);

      /* Modelling */
      Mod_compute_modeling(allone, fluxRatio, result_tab, files_path, row, group, debug);
      MPI_Send(result_tab, tailleTab, MPI_DOUBLE, WRITER_RANK, row, MPI_COMM_WORLD);
      break;
    case 4:
      flag=0;
      /* Reception des ratios */
      MPI_Recv (fluxRatio,allone->nb_parameters, MPI_DOUBLE, MASTER_RANK, MPI_ANY_TAG,MPI_COMM_WORLD, &status);
      /* Standard deviation */
      Sd_compute_standard_deviation(allone, fluxRatio, result_tab, files_path, row, debug);
      MPI_Send(result_tab, tailleTab, MPI_DOUBLE, WRITER_RANK, row, MPI_COMM_WORLD);
      break;
    default:
      fprintf(stderr,"Error : activity %d unknown\n",activity);
      exit(EXIT_FAILURE);
      break;
    }

    /* Confirme qu'il a termine son calcul */
    request=1;
    MPI_Send (&request, 1, MPI_INT, MASTER_RANK, row, MPI_COMM_WORLD);

    /* Mise en attente d'un message de poursuite ou de fin   */
    MPI_Recv (&answer,1, MPI_INT, MASTER_RANK, MPI_ANY_TAG,MPI_COMM_WORLD, &status);
  }

  /* Desallocation de la memoire du tableau de parametres */
  if(fluxRatio!=NULL) free(fluxRatio);

  /* Desallocation de la memoire du tableau de resultat */
  if(result_tab!=NULL) free(result_tab);
}

/**
 * \fn void Mpi_finalize(void)
 * \author Amine Ghozlane
 * \brief  End MPI execution environment
 */
void Mpi_finalize(void)
{
  /* Termine l'execution de l'environnement MPI */
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();
}

/**
 * \fn void compute_mpi(int argc, char **argv, char **files_path, int activity, int group, int debug, int port)
 * \author Amine Ghozlane
 * \brief  Compute the simulated annealing through mpi
 * \param  argc Number of arguments
 * \param  argv List of arguments
 * \param  files_path List of paths
 * \param  activity Chosen activity
 * \param  group Group flag
 * \param  debug Debug flag
 * \param  port Interface port
 */
void compute_mpi(int argc, char **argv, char **files_path, int activity, int group, int debug, int port)
{
  /* Lance le calcul du recuit simule par les methodes mpi */
  pListParameters allone=NULL;
  int tab[2]={0,0}, desc=0, pid=(int)getpid();
  char s[255];

  /* Connexion a l'interface */
  if(port!=0) desc=Mpi_connectInterface(port);
  setvbuf( stdout, NULL, _IONBF, 0 );

  /* Initialisation */
  allone=(pListParameters)malloc(1*sizeof(ListParameters));

  /* Chargement des parametres de simulation */
  Data_parameters(allone, files_path);

  /* Envoie des pids */
  sprintf(s, "%i", pid);
  send(desc,s,strlen(s),0);

  /* Lancement de la procedure mpi */
  Mpi_init(argc, argv, tab);
  if (tab[0] == MASTER_RANK) Mpi_master(allone, files_path, activity, group,tab[0], tab[1], desc);
  else if(tab[0] == WRITER_RANK &&activity!=1) Mpi_writer(allone, files_path, activity, group, tab[0]);
  else Mpi_slave(allone, files_path, activity, group, debug);

  /* Finalisation */
  Mpi_finalize();

  /* Desallocation memoire */
  Data_desallocation(allone);
  Mpi_disconnectInterface(desc);
}
