/**
 * This file is part of Metaboflux (http://www.cbib.u-bordeaux2.fr/metaboflux/)
 * Copyright (C) 2010 Amine Ghozlane from LaBRI and University of Bordeaux 1
 *
 * Metaboflux is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Metaboflux is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file equations.c
 * \brief Processes an equation in MathML format
 * \author {Amine Ghozlane}
 * \version 2.0
 * \date 15 janvier 2010
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "equations.h"

/**
 * \fn double add(double arg1, double arg2)
 * \author Amine Ghozlane
 * \brief  Add two numbers
 * \param  arg1 Value 1
 * \param  arg2 Value 2
 * \return Result
 */
double add(double arg1, double arg2)
{
  return( arg1 + arg2 );
}

/**
 * \fn double subtract(double arg1, double arg2)
 * \author Amine Ghozlane
 * \brief  Subtraction two numbers
 * \param  arg1 Value 1
 * \param  arg2 Value 2
 * \return Result
 */
double subtract(double arg1, double arg2)
{
  return( arg1 - arg2 );
}

/**
 * \fn double multiply(double arg1, double arg2)
 * \author Amine Ghozlane
 * \brief  Multiply two numbers
 * \param  arg1 Value 1
 * \param  arg2 Value 2
 * \return Result
 */
double multiply(double arg1, double arg2)
{
  return( arg1 * arg2 );
}

/**
 * \fn double divide(double arg1, double arg2)
 * \author Amine Ghozlane
 * \brief  Divide two numbers
 * \param  arg1 Value 1
 * \param  arg2 Value 2
 * \return Result
 */
double divide(double arg1, double arg2)
{
  return( arg1 / arg2 );
}

/**
 * \fn double equality(double arg1, double arg2)
 * \author Amine Ghozlane
 * \brief  Test the equality of two numbers
 * \param  arg1 Value 1
 * \param  arg2 Value 2
 * \return Result
 */
double equality(double arg1, double arg2)
{
  return( arg1 == arg2 );
}

/**
 * \fn double sup(double arg1, double arg2)
 * \author Amine Ghozlane
 * \brief  Test the superiority between two numbers
 * \param  arg1 Value 1
 * \param  arg2 Value 2
 * \return Result
 */
double sup(double arg1, double arg2)
{
  return( arg1 > arg2 );
}

/**
 * \fn double sup_equal(double arg1, double arg2)
 * \author Amine Ghozlane
 * \brief  Test the superiority or equality between two numbers
 * \param  arg1 Value 1
 * \param  arg2 Value 2
 * \return Result
 */
double sup_equal(double arg1, double arg2)
{
  return( arg1 >= arg2 );
}

/**
 * \fn double inf(double arg1, double arg2)
 * \author Amine Ghozlane
 * \brief  Test the inferiority between two numbers
 * \param  arg1 Value 1
 * \param  arg2 Value 2
 * \return Result
 */
double inf(double arg1, double arg2)
{
  return( arg1 < arg2 );
}

/**
 * \fn double inf_equal(double arg1, double arg2)
 * \author Amine Ghozlane
 * \brief  Test the inferiority or equality between two numbers
 * \param  arg1 Value 1
 * \param  arg2 Value 2
 * \return Result
 */
double inf_equal(double arg1, double arg2)
{
  return( arg1 <= arg2 );
}

/**
 * \fn pEquations Equations_alloc(void)
 * \author Amine Ghozlane
 * \brief  Alloc memory and initialize the struct Equations
 * \return Allocated struct Equations
 */
pEquations Equations_alloc(void)
{
  pEquations map;
  map=(pEquations) malloc(1*sizeof(Equations));
  assert(map!=NULL);
  map->suiv=NULL;
  return map;
}

/**
 * \fn void Equations_define(pEquations new, char *operateur)
 * \author Amine Ghozlane
 * \brief  Identify the mathematical operator used
 * \param  new Struct Equations
 * \param  operateur Read operator
 */
void Equations_define(pEquations new, char *operateur)
{
  /* Addition */
  if(!strcmp(operateur,"+")){
      /* on indique le type */
      new->type=addition;
      /* on pointe sur la fonction add */
      new->info.op=&add;
  }
  /* Subtraction */
  else if(!strcmp(operateur,"-")){
      new->type=soustraction;
      new->info.op=&subtract;
  }
  /* Multiplication */
  else if(!strcmp(operateur,"*")){
      new->type=multiplication;
      new->info.op=&multiply;
  }
  /* Division */
  else if(!strcmp(operateur,"/")){
      new->type=division;
      new->info.op=&divide;
  }
  /* Egality */
  else if(!strcmp(operateur,"=")){
      new->type=equal;
      new->info.op=&equality;
  }
  /* Superiority */
  else if(!strcmp(operateur,">")){
      new->type=superior;
      new->info.op=&sup;
  }
  /* Sup-equality */
  else if(!strcmp(operateur,">=")){
      new->type=superior_equal;
      new->info.op=&sup_equal;
  }
  /* Inferiority */
  else if(!strcmp(operateur,"<")){
      new->type=inferior;
      new->info.op=&inf;
  }
  /* Inf-equality */
  else if(!strcmp(operateur,"<=")){
      new->type=inferior_equal;
      new->info.op=&inf_equal;
  }
  /* Operator non recognize */
  else{
      fprintf(stderr,"Error : operator \"%s\" unknown\n",operateur);
      exit(EXIT_FAILURE);
  }
}

/**
 * \fn int Equations_vide(pEquations liste)
 * \author Amine Ghozlane
 * \brief  Test if the struct Equations is empty
 * \param  liste Struct Equations
 * \return Number of elements
 */
int Equations_vide(pEquations liste)
{
  int nb_elements=0;

  /* Decompte des elements de la liste */
  while(liste!=NULL){
      nb_elements++;
      liste=liste->suiv;
  }

  return nb_elements;
}

/**
 * \fn pEquations Equations_empiler(pEquations liste, pEquations new)
 * \author Amine Ghozlane
 * \brief  Stack an element to the struct Equations
 * \param  liste Struct Equations
 * \param  new New struct Equations element
 * \return Pointer on the last element of struct Equations
 */
pEquations Equations_empiler(pEquations liste, pEquations new)
{
  pEquations temp=liste;

  /* Cas ou la liste est vide */
  if(liste==NULL) return new;

  /* Deplacement jusqu'au dernier de la liste */
  while(liste->suiv!=NULL){
      liste=liste->suiv;
  }
  liste->suiv=new;

  return temp;
}


/**
 * \fn pEquations Equations_depiler(pEquations liste)
 * \author Amine Ghozlane
 * \brief  Unstack the last element of the struct Equations.
 * \param  liste Struct Equations.
 * \return Pointer on the "unstack" element of struct Equations.
 */
pEquations Equations_depiler(pEquations liste)
{

  pEquations dernier_element=NULL;

  /* Deplacement jusqu'a l'avant dernier de la liste */
  while(liste->suiv->suiv!=NULL){
      liste=liste->suiv;
  }
  /* Le dernier element est retire de la pile */
  dernier_element=liste->suiv;
  liste->suiv=NULL;

  return dernier_element;
}

/**
 * \fn pEquations Equations_sommet(pEquations liste)
 * \author Amine Ghozlane
 * \brief  Look for the last element of the struct Equations
 * \param  liste Struct Equations
 * \return Pointer on the last element of struct Equations
 */
pEquations Equations_sommet(pEquations liste)
{
  while(liste->suiv!=NULL){
      liste=liste->suiv;
  }
  return liste;
}

/**
 * \fn pEquations Equations_defiler(pEquations liste)
 * \author Amine Ghozlane
 * \brief  Look for the first element of the struct Equations.
 * \param  liste Struct Equations
 * \return Pointer on the first element of struct Equations
 */
pEquations Equations_defiler(pEquations liste)
{
  pEquations top=NULL;
  top=liste;
  return top;
}

/**
 * \fn pEquations Equations_defilerSuiv(pEquations liste)
 * \author Amine Ghozlane
 * \brief  Get the next element of the struct Equations
 * \param  liste Struct Equations
 * \return Pointer on the next element of struct Equations
 */
pEquations Equations_defilerSuiv(pEquations liste)
{
  return liste->suiv;
}

/**
 * \fn int Equations_priorite(pEquations new)
 * \author Amine Ghozlane
 * \brief  Give the priority of the operator. Multiplication and Division have higher priority than addition or subtration...
 * \param  new One element of the struct Equations
 * \return Priority of the operator
 */
int Equations_priorite(pEquations new)
{
  /* Les operateurs d'addition, soustraction et egalite sont de priorite inferieure que ceux de division et multiplication*/
  if(new->type==addition || new->type==soustraction || new->type==equal || new->type==superior || new->type==superior_equal || new->type==inferior || new->type==inferior_equal ) return 1;
  return 2;
}

/**
 * \fn void Equations_print(pEquations liste)
 * \author Amine Ghozlane
 * \brief  Print all information on the element of the struct Equations
 * \param  liste Struct Equations
 */
void Equations_print(pEquations liste)
{
  /* Affichage de la liste */
  while(liste!=NULL){
      switch(liste->type){
      case constant: fprintf(stderr,"constant : %f\n",liste->info.data);
      break;
      case variable: fprintf(stderr,"variable : %s\n",liste->info.var);
      break;
      default: fprintf(stderr,"operator : %d\n",liste->type);
      }
      liste=liste->suiv;
  }
}

/**
 * \fn pEquations Equations_addOp(pEquations result, pEquations op, pEquations new)
 * \author Amine Ghozlane
 * \brief  Build the list of the Struct Equations result used to compute the equation.
 * \param  result Struct Equations used for computation
 * \param  op Struct Equations used to store the operator
 * \param  new One element of the Struct Equations
 * \return List of operator
 */
pEquations Equations_addOp(pEquations result, pEquations op, pEquations new)
{
  pEquations temp=NULL;

  /* Fonction qui permet placer correctement les operateurs dans la liste chainee de l'equation */
  while( Equations_vide(op) != OUI &&  Equations_priorite(Equations_sommet(op))>=Equations_priorite(new) ){

      /* Cas ou il faut depiler le dernier element de la liste operateur */
      if(op->suiv==NULL){
          temp=op;
          op=NULL;
          temp=Equations_empiler(result,temp);
      }
      /* On place le dernier element de liste operateur (non vide) dans la liste result */
      else temp=Equations_empiler(result,Equations_depiler(op));
  }

  /* Cas ou la pile operateur est vide ou que l'operateur est de priorite faible */
  if(new->type==equal || new->type==superior || new->type==superior_equal || new->type==inferior || new->type==inferior_equal){
      temp=Equations_empiler(result,new);
      return NULL;
  }
  /* Cas ou la pile des operateurs est vide*/
  else temp=Equations_empiler(op,new);

  /* Cas ou la pile des operateurs est vide*/
  if(op==NULL) return temp; /*TODO Mention a retirer*/

  return op;
}

/**
 * \fn void Equations_emptyOp(pEquations result, pEquations op)
 * \author Amine Ghozlane
 * \brief  Empty the Struct Equations op for the struct Equation result.
 * \param  result Struct Equations used for computation
 * \param  op Struct Equation used to store the operator
 */
void Equations_emptyOp(pEquations result, pEquations op)
{
  pEquations top=NULL;
  /* Transfuge le contenu de op dans result */
  while(Equations_vide(op)!=OUI){
      top= Equations_defiler(op);
      op= Equations_defilerSuiv(op);
      top->suiv=NULL;
      Equations_empiler(result,top);
  }
}

/**
 * \fn pEquations Equations_pileFormation(char ***equation, int nb_noeud)
 * \author Amine Ghozlane
 * \brief  Build pile.
 * \param  equation Table with the equation in Mathml format
 * \param  nb_noeud Number of element inside the equation
 * \return Struct Equation needed for compute the equation
 */
pEquations Equations_pileFormation(char ***equation, int nb_noeud)
{
  pEquations op=NULL, result=NULL, new=NULL;
  int i;

  /* On considere chaque noeud */
  for(i=0;i<nb_noeud;i++){
      new=Equations_alloc();
      /* Operateur */
      if( !strcmp(equation[0][i],"mo") ){
          /* Define the type of the operator */
          Equations_define(new, equation[1][i]);
          /* Place the operateur inside the equation */
          op=Equations_addOp(result,op,new);
      }
      /* Variable */
      else if(!strcmp(equation[0][i],"mi")){
          /* Definition du type */
          new->type=variable;
          /* Stockage de la variable */
          new->info.var=(char*)malloc(((int)strlen(equation[1][i])+1)*sizeof(char));
          strcpy(new->info.var,equation[1][i]);
          /* Empilement dans la structure result */
          result=Equations_empiler(result,new);
      }
      /* Constante */
      else if(!strcmp(equation[0][i],"mn")){
          /* Definition du type */
          new->type=constant;
          /* Stockage de la variable */
          new->info.data=atof(equation[1][i]);
          /* Empilement dans la structure result */
          result=Equations_empiler(result,new);
      }
      /* Noeud au format inconnu */
      else{
          fprintf(stderr,"Typing error with MathML file tags \n");
          exit(EXIT_FAILURE);
      }
  }

  Equations_emptyOp(result,op);

  return result;
}

/**
 * \fn double Equations_extractData(pEquations new, char **name, double *quantite, int taille)
 * \author Amine Ghozlane
 * \brief  Extract information on element of type Struct Equation
 * \param  new Element of type Struct Equation
 * \param  name Name of the interest element
 * \param  quantite List of the quantity of the molecules
 * \param  taille Number of molecules in the list
 * \return Quantity of the molecule of interest
 */
double Equations_extractData(pEquations new, char **name, double *quantite, int taille)
{
  /* Extraction d'infos sur valeur disposee dans la pile  */
  int i;
  /* Cas ou l'element est une constante */
  if(new->type==constant) return new->info.data;

  /* Cas ou c'est une variable */
  for(i=0;i<taille; i++){
      /* Cherche la molecule en question */
      if(!strcmp(new->info.var,name[i])){
          /* Detruit l'element */
          if(new!=NULL){
              free(new);
              new=NULL;
          }
          /* Retourne sa quantite */
          return quantite[i];
      }
  }
  /* Erreur */
  fprintf(stderr,"Error : Molecule \"%s\" not found in the stack\n",name[i]);
  exit(EXIT_FAILURE);
  return 0.0;
}

/**
 * \fn double Equations_findSpecies(char *species, char **name, double *quantite, int taille)
 * \author Amine Ghozlane
 * \brief  Look for the quantity of a molecule in the list
 * \param  species Name of a molecule
 * \param  name List of molecules
 * \param  quantite List of the quantity of the molecules
 * \param  taille Number of molecules in the list
 * \return Quantity of the molecule of interest
 */
double Equations_findSpecies(char *species, char **name, double *quantite, int taille)
{
  /* Extraction d'info sur la quantite de la molecule durant simulation */
  int i;
  for(i=0;i<taille; i++){
      /* Si la molecule est retrouve, on retourne sa quantite */
      if(!strcmp(species,name[i])){
          return quantite[i];
      }
  }
  /* Erreur */
  fprintf(stderr,"Error : Molecule \"%s\" not found in the list of molecules of the parameter file\n",name[i]);
  exit(EXIT_FAILURE);
  return 0.0;
}

/**
 * \fn double Equations_resultat(pEquations result, pEquations result1, pEquations eq, char **name, double *quantite, int taille)
 * \author Amine Ghozlane
 * \brief  Compute the result of two pile
 * \param  result Struct Equation result
 * \param  result1 Struct Equation result after the operator
 * \param  eq Operator
 * \param  name List of molecules
 * \param  quantite List of the quantity of the molecules
 * \param  taille Number of molecules in the list
 * \return Result of the two pile
 */
double Equations_resultat(pEquations result, pEquations result1, pEquations eq, char **name, double *quantite, int taille)
{
  /* Calcul du resultat de l'equation */
  double resul=0.0, op1=0.0, op2=0.0;

  /* Extraction de la valeur des deux elements */
  op1=Equations_extractData(result1,name,quantite,taille);
  op2=Equations_extractData(result,name,quantite,taille);

  /* Cas d'Egalite */
  if(eq->type==equal){
      resul=pow(((op1-op2)/op2),2.0);
  }
  /* Superieur */
  else if(eq->type==superior){
      /* Test de la superiorite */
      if(op1>op2) resul=0.0;
      /* Inflige une penalite de 1000 a l'energie */
      else resul = 1000;
  }
  /* Inferieur */
  else if(eq->type==inferior){
      if(op1<op2) resul=0.0;
      else resul = 1000;
  }
  /* Superieur_egal */
  else if(eq->type==superior_equal){
      if(op1>=op2) resul=0.0;
      else resul = 1000;
  }
  /* Inferieur_egal */
  else if(eq->type==inferior_equal){
      if(op1<=op2) resul=0.0;
      else resul = 1000;
  }
  /* Erreur type non reconnu */
  else{
      fprintf(stderr,"Type \"%d\" unknown\n",eq->type);
      exit(EXIT_FAILURE);
  }
  return resul;
}

/**
 * \fn double Equations_calcul(pEquations liste, char **name, double *quantite, int taille)
 * \author Amine Ghozlane
 * \brief  Compute the score of the equation
 * \param  liste pile of type Struct Equations
 * \param  name List of molecules
 * \param  quantite List of the quantity of the molecules
 * \param  taille Number of molecules in the list
 * \return Result of the equation (First part of the energy)
 */
double Equations_calcul(pEquations liste, char **name, double *quantite, int taille)
{
  /* Calcul du score d'une equation */
  pEquations result=NULL, result1=NULL, eq=NULL, top=NULL, temp1=NULL, temp2=NULL, new=NULL;
  double op1=0.0, op2=0.0;

  while(Equations_vide(liste)!=OUI){
      /*Defiler se fait en deux coups*/
      top= Equations_defiler(liste);
      liste= Equations_defilerSuiv(liste);
      top->suiv=NULL;

      /* Operation */
      if(top->type==addition || top->type==soustraction || top->type==multiplication || top->type==division){
          /*temp1*/
          if(result->suiv==NULL){
              temp1=result;
              result=NULL;
          }
          else temp1=Equations_depiler(result);
          /* temp2 */
          if(result->suiv==NULL){
              temp2=result;
              result=NULL;
          }
          else temp2=Equations_depiler(result);

          /* Extraction des valeurs et liberation de la memoire */
          op2=Equations_extractData(temp1,name,quantite,taille);
          op1=Equations_extractData(temp2,name,quantite,taille);

          /* Ajout dans la pile du resultat */
          new=Equations_alloc();
          /* Recuperation de la valeur */
          new->info.data=top->info.op(op1,op2);
          /* Type defini a constant */
          new->type=constant;
          /* On empile */
          result=Equations_empiler(result,new);
          if(top!=NULL){
              free(top);
              top=NULL;
          }
      }
      /* Cas de l'equation */
      else if(top->type==equal || top->type==superior || top->type==superior_equal || top->type==inferior || top->type==inferior_equal){
          result1=result;
          result=NULL;
          eq=top;
      }
      else  result=Equations_empiler(result,top);
  }
  /* Calcul du resultat final */
  return Equations_resultat(result,result1,eq,name,quantite,taille);
}

/**
 * \fn double Equations_finalQuantite(int file_nb_especes, char **file_species, int *file_amount, int *file_weight, char **sim_name, double *sim_quantite, int sim_taille)
 * \author Amine Ghozlane
 * \brief  Compute the score of the quantity which means the difference between what is expected to what is simulated.
 * \param  file_nb_especes Number of species in the parameter file
 * \param  file_species List of species in the parameter file
 * \param  file_amount List of species expected quantity
 * \param  file_weight Weight defined for the species
 * \param  sim_name List of species simulated (sbml file)
 * \param  sim_quantite List of the quantity of the species simulated (sbml file)
 * \param  sim_taille Number of species simulated (sbml file)
 * \return Result of the difference (Second part of the Energy)
 */
double Equations_finalQuantite(int file_nb_especes, char **file_species, int *file_amount, int *file_weight,
    char **sim_name, double *sim_quantite, int sim_taille)
{
  /* Calcul du score des quantites */
  int i;
  double result=0.0, simulated_quantite=0.0;

  /* Estimation de la difference */
  for(i=0;i<file_nb_especes;i++){
      if( file_amount[i]!=(-1) ){
          simulated_quantite=Equations_findSpecies(file_species[i],sim_name,sim_quantite,sim_taille);
          result+=(double)file_weight[i]*pow(((simulated_quantite-(double)file_amount[i])/(double)file_amount[i]),2.0);
      }
  }
  return result;
}
