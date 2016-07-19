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
 * \file data_parameters.h
 * \brief Load data parameters
 * \author {Amine Ghozlane}
 * \version 2.0
 * \date 10 novembre 2010
 */

#ifndef DATA_PARAMETERS_H_
#define DATA_PARAMETERS_H_

/* Parametre constant a donner a toutes les simulations */
/**
* \struct ListParameters
* \brief Global list parameters
*/
typedef struct ListParameters
{
  Model_t *model;       /*!< SBML file */
  xmlConfig_t *conf;    /*!< Parameter file */
  char ****equation;    /*!< Equations text */
  int nb_parameters;    /*!< Number of parameters */
  int interest_parameters; /*!< Number of interest parameters */
  int nb_couples;       /*!< Number of couples */
  int nb_equations;     /*!< Number of equations */
  int nb_reaction;      /*!< Number of reaction */
  int *noeud;           /*!< Number of nodes in an equation */
  int *parameters;      /*!< Details on the parameters of reactions */
  char **banned;        /*!< Names of banned molecules : ex ATP, ADP ... */
  int nb_banned;        /*!< Number of banned molecules */
  int nb_triesMod;      /*!< Number of tries for modelling*/
  int nb_triesSa;       /*!< Number of tries for simulated annealing and minimization*/
}ListParameters, *pListParameters;

/* Parametres specifiques a chaque simulation */
/**
* \struct SimParameters
* \brief List of parameters specific to each simulation
*/
typedef struct SimParameters
{
  Equations **pile;     /*!< Interpreted Equations */
  pScore out;           /*!< Score */
  gsl_rng * r;          /*!< Random number generator */
  double *y;            /*!< Table of reaction parameters*/
  FILE *debugFile;      /*!< Debug file : storage of intermediate values of the simulations */
}SimParameters, *pSimParameters;

/* XML */
/* Initialise le fichier de parametre */
xmlConfig_t *Data_initXML(char *);

/* SBML */
/* Initialise le fichier SBML */
Model_t *Data_initSBML(char *);

/* ALEATOIRE */
/* Preparation pour la prochaine utilisation de l'aleatoire */
/*void Data_newRng(gsl_rng *);*/

/* Allocation du generateur de chiffre aleatoire */
gsl_rng *Data_rngAlloc(gsl_rng *);

/* EQUATIONS */
/* Allocation de la structure pEquations */
Equations **Data_equationsAlloc(pListParameters, Equations **);

/* Initialisation des equations */
void Data_equationsInit(pListParameters, Equations **);

/* Alloue les differents elements lies aux equations */
void Data_allocEquations(pListParameters a);

/* SCORE */
/* Alloue les differents element de la structure pScore */
void Data_allocScore(pScore, xmlConfig_t *, Model_t *);

/* Initialise la structure de score */
pScore Data_scoreAlloc(pListParameters);

/* Initialise la structure score */
void Data_scoreInit(pScore);

/* Libere la memoire alloue a la structure de score */
void Data_scoreFree(pScore);

/* TABLEAU */
/* Copie des deux tableaux */
int Data_copieTab(pSimParameters, double *, int, int, int);

/* Copie les donnees du tableau dans une version donnant la valeur parametre pour les 3 */
void Data_updateTab(pListParameters, pSimParameters, double *);

/* LISTPARAMETERS */
/* Allocation des tableaux necessaires aux simulations */
void Data_allocSimParameters(double *, pListParameters);

/* Alloue les differents elements de parametre de la simulation */
void Data_allocParameters(pListParameters);

/* Enregistrement des parametres de la simulation */
void Data_parameters(pListParameters, char **);

/* Libere la memoire alloue aux differents objets du programme */
void Data_desallocation(pListParameters);

/* SIMPARAMETERS */

/* Enregistrement des parametres specifiques a chaque simulation */
void Data_simParameters(pListParameters, pSimParameters, char *, char *, int, int/*, int*/);

/* Desallocation des parametres specifiques a chaque simulation */
void Data_desallocSim(pSimParameters);




#endif /* DATA_PARAMETERS_H_ */
