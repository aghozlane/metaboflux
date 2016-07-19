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
 * \file equations.h
 * \brief Processes an equation in MathML format
 * \author {Amine Ghozlane}
 * \version 2.0
 * \date 15 janvier 2010
 */

#ifndef EQUATIONS_H_
#define EQUATIONS_H_

#define OUI 0

/**
* \enum Equtype
* \brief Operation constants.
*
* Equtype is a set of predefined constants for the different operators.
*/
typedef enum
{
  constant, /*!< Constant */
  variable, /*!< Variable : ex. transition_x ... */
  addition, /*!< Addition */
  soustraction, /*!< Substraction */
  multiplication, /*!< Multiply */
  division, /*!< Divide */
  equal, /*!< Equality */
  superior, /*!< Superiority */
  superior_equal, /*!< Sup_equality */
  inferior, /*!< Inferiority */
  inferior_equal /*!< Inf_equality */
} Equtype;

/**
* \union Equdata
* \brief Equation data.
*/
typedef union Equdata
{
  double (*op)(double,double); /* Function pointers */
  double data; /*!< Constant */
  char *var; /*!< Variable */
} Equdata;

/**
* \struct Equations
* \brief Function pointers used for operations.
*/
typedef struct Equations
{
  Equtype type; /*!< Operation constants */
  Equdata info; /*!< Equation Data */
  struct Equations *suiv;
} Equations, *pEquations;

/* Operation */
double add(double, double);
double subtract(double, double);
double multiply(double, double);
double divide(double, double);
double equality(double, double);
double sup(double, double);
double sup_equal(double, double);
double inf(double, double);
double inf_equal(double, double);

/* Allocation memoire de la struture pequation  */
pEquations Equations_alloc(void);

/* Identifie l'operateur en jeu */
void Equations_define(pEquations, char *);

/* Determine si la liste est vide */
int Equations_vide(pEquations);

/* Ajout d'un element dans la pile */
pEquations Equations_empiler(pEquations, pEquations);

/* Depile un element */
pEquations Equations_depiler(pEquations);

/* Retourne le dernier element de la liste */
pEquations Equations_sommet(pEquations);

/* Retourne le premier element de la liste */
pEquations Equations_defiler(pEquations);

/* Decalle le pointeur de la liste */
pEquations Equations_defilerSuiv(pEquations);

/* Priorite de l'operateur */
int Equations_priorite(pEquations);

/* Affichage de la liste */
void Equations_print(pEquations);

/* Ajout d'un operateur dans la pile */
pEquations Equations_addOp(pEquations, pEquations, pEquations);

/* Transfuge le contenu de op dans result */
void Equations_emptyOp(pEquations, pEquations);

/* Construction de la pile */
pEquations Equations_pileFormation(char ***, int);

/* Extraction d'info sur la quantite de la molecule durant simulation */
double Equations_findSpecies(char *, char **, double *, int);

/* Calcul du resultat de l'equation */
double Equations_resultat(pEquations, pEquations, pEquations, char **, double *, int);

/* Calcul du score  d'une equation */
extern double Equations_calcul(pEquations, char **, double *, int);

/* Calcul du score des quantites */
extern double Equations_finalQuantite(int, char **, int *, int*, char **, double *, int);

#endif /* EQUATIONS_H_ */
