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
 * \file simulation.h
 * \brief Simulate a petri net
 * \author {Amine Ghozlane}
 * \version 2.0
 * \date 27 octobre 2009
 */
#ifndef SIMULATION_H_
#define SIMULATION_H_

/* SBML */
#define OK -1
#define END 0
#define PURSUE 1


/* Structure de test des reactions */
/**
* \struct TestReaction
* \brief Structure used to test reactions
*/
typedef struct TestReaction
{
  Reaction_t **tabReactions; /*!< Stock temporairement une reaction */
  int *minStepTab; /*!< Stock le nombre de realisation possible pour cette reaction */
}TestReaction,*pTestReaction;

/* Structure contenant toutes les informations issues des simulations */
/**
* \struct Score
* \brief Structure containing all information from simulations
*/
typedef struct Score
{
  char **name; /*!< Molecules id */
  double *quantite; /*!< Quantity of molecules */
  char **reaction; /*!< Interest reaction */
  int nb_reaction; /*!< Number of reaction */
  int tailleReactions; /*!< Number of reaction in the SBML file */
  int tailleSpecies; /*!< Number of molecules in the SBML file */
  int taille;   /*!< Total of other two sizes */
  char **species; /*!< Interest molecules */
  int nb_species; /*!< Number of molecules */
  int *species_amount; /*!< Quantity of the interest molecules */
  int *species_weight; /*!< Weight of the interest molecules */
}Score, *pScore;

/* Initialisation de la quantite des especes */
void SBML_initEspeceAmounts(Model_t *, pEspeces, int);

/* Determine si la reation indique est etudiee */
int SBML_findReaction(char **, const char *, int);

/* Caste la formule en double */
double SBML_evalExpression(const char *);

/* Initialisation des reactions et des ratios auquelles participent chaque espece */
void SBML_setReactions(Model_t *, pEspeces, pScore, double *, int, int);

/* Determine le nombre de reaction possible a partir de la quantite des reactifs */
int SBML_checkQuantite(Model_t *, Reaction_t *, int, pEspeces);

/* Determine aleatoire la reaction a realiser pour les noeuds de plusieurs reactions */
Reaction_t * SBML_reactChoice(pEspeces, const gsl_rng *, int);

/* Simulation d'une transision discrete */
void SBML_reaction(Model_t *, pEspeces, Reaction_t *, int);

/* Alloue la memoire necessaire a la struture de test des reactions */
void SBML_allocTest(pTestReaction, int);

/* Libere la memoire allouee a la struture de test des reactions */
void SBML_freeTest(pTestReaction);

/* Estimation du nombre de reaction realisable par reaction */
int SBML_EstimationReaction(Model_t *, pTestReaction, pEspeces, int, int);

/* Simulation des reactions */
int SBML_simulate( Model_t *, pEspeces, const gsl_rng *, pTestReaction, char **, int, int, int);

/* Fonction de score */
void SBML_score(Model_t *, pEspeces, pScore,  double *, int, int);

/* Affichage de l'entete du fichier debug */
void SBML_debugPrintHead(FILE *,int, char **);

/* Affiche les resultats necessaires au calcul de l'ecart type*/
void SBML_debugPrint(FILE *, int, int, double *, double);

/* Simulation du reseau metabolique  */
extern void SBML_compute_simulation(pScore, Model_t *, double *, gsl_rng *, char **, int);

/* Addition des scores */
void SBML_score_add(pScore, pScore, FILE *);

/* Moyenne des resultats */
void SBML_score_mean(pScore, int);

/* Simulation du reseau metabolique  */
extern void SBML_compute_simulation_mean(FILE *, pScore, pScore, Model_t *, double *, gsl_rng *, char **, int, int);

#endif /* SIMULATION_H_ */
