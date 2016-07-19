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
 * \file mpi_load.h
 * \brief Parallelize the program
 * \author {Amine Ghozlane}
 * \version 2.0
 * \date 9 novembre 2009
 */
#ifndef MPI_LOAD_H_
#define MPI_LOAD_H_

/* Connexion a l'interface */
int Mpi_connectInterface(int);

/* Deconnexion de l'interface */
void Mpi_disconnectInterface(int);

/* Initialisation de MPI */
void Mpi_init(int, char **, int *);

/* Initialise le fichier SBML */
Model_t *Mpi_initSBML(char *);

/* Initialise le fichier de parametre */
xmlConfig_t *Mpi_initXML(char *);

/* Processus maitre */
void Mpi_master(pListParameters, char **, int, int, int, int, int);

/* Processus ecrivain */
void Mpi_writer(pListParameters, char **, int, int, int);

/* Determine la taille du tableau de resultat */
int Mpi_sizeResultTab(pListParameters, int, int, int);

/* Allocation du tableau de resultat */
double *Mpi_allocResultTab(int);

/* Sortie du resultat des simulations */
void Mpi_writeSimFile(pListParameters, FILE *, FILE *, double *, int, int, int);

/* Ecriture standard deviation */
void Mpi_writeSdFile(FILE *, double *, int);

/* Processus esclave */
void Mpi_slave(pListParameters, char **, int, int,  int);

/* Termine l'execution de l'environnement MPI */
void Mpi_finalize(void);

/* Lance le calcul du recuit simule par les methodes mpi*/
extern void compute_mpi(int, char **, char**, int, int, int, int);

#endif /* MPI_LOAD_H_ */
