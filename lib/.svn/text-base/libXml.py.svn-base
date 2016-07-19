#!/usr/bin/python
# -*- coding: utf-8 -*-
###############################################################################
# This file is part of MetaBoFlux (http://www.cbib.u-bordeaux2.fr/metaboflux/)#
# Copyright (C) 2010 Amine Ghozlane from LaBRI and University of Bordeaux 1   #
#                                                                             #
# MetaBoFlux is free software: you can redistribute it and/or modify          #
# it under the terms of the GNU Lesser General Public License as published by #
# the Free Software Foundation, either version 3 of the License, or           #
# (at your option) any later version.                                         #
#                                                                             #
# MetaBoFlux is distributed in the hope that it will be useful,               #
# but WITHOUT ANY WARRANTY; without even the implied warranty of              #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               #
# GNU General Public License for more details.                                #
#                                                                             #
# You should have received a copy of the GNU Lesser General Public License    #
# along with this program.  If not, see <http://www.gnu.org/licenses/>.       #
###############################################################################

"""
 @brief: Read xml parameters
 @author: Amine Ghozlane
 @version: 2.0 
"""
import xml.dom.minidom

#==============================================================
#Ouverture du fichier parameter.xml
#==============================================================
#Retourne le nom du document
def getName(dom):
  """
  Get xml document name
  @param dom: Xml parameter file
  @return: The document name
  """
  node = dom.getElementsByTagName("name")[0]
  return node.childNodes[0].nodeValue

#Retourne le nombre de transition en jeu
def getNbTransition(dom):
  """
  Get the number of transition
  @param dom: Xml parameter file
  @return: Number of transition
  """
  return dom.getElementsByTagName("reaction").length

#Retourne les attributs de noeuds
def getAttribute(nodelist, text):
  """
  Get attributes of nodes
  @param nodelist: Node list
  @param text: Attribute
  @return: Attributes of nodes
  """
  result = []
  for node in nodelist:
    attrs = node.attributes
    result.append(str(attrs[text].nodeValue))
  return result

#Retourne les reactions en jeu
def getTransition(dom):
  """
  Get list of reactions
  @param dom: Xml parameter file
  @return: List of reactions
  """
  return getAttribute(dom.getElementsByTagName("reaction"), "id")

#Retourne les molecules en jeu
def getSpecies(dom):
  """
  Get the list of molecules
  @param dom: Xml parameter file
  @return: List of molecules
  """
  return getAttribute(dom.getElementsByTagName("species"), "id")

#Retourne le nombre de molecules en jeu
def getNbSpecies(dom):
  """
  Get the number of molecules
  @param dom: Xml parameter file
  @return: Number of molecules
  """
  return dom.getElementsByTagName("species").length 

#Retourne les quantites des elements souhaitees
def getInitialQuantity(dom):
  """
  Get the quantity of interest molecules
  @param dom: Xml parameter file
  @return: Quantity of interest molecules
  """
  return getAttribute(dom.getElementsByTagName("species"), "finalAmount")
  
#Retourne le nombre de simulation a effectuer
def getNbSimulation(dom):
  """
  Get the number of simulations
  @param dom: Xml parameter file
  @return: Number of simulations
  """
  node = dom.getElementsByTagName("nbSimulations")[0]
  return node.childNodes[0].nodeValue
  
#Retourne le nombre de groupes a realiser
def getNbGroup(dom):
  """
  Get the number of groupes (statistics)
  @param dom: Xml parameter file
  @return: Number of groupes 
  """
  node = dom.getElementsByTagName("nbGroup")[0]
  return node.childNodes[0].nodeValue

#Extrait les informations du fichier de parametres
def extractParameters(dom):
  """
  Extract informations on the file parameters
  @param dom: Xml parameter file
  @return: All xml informations
  """
  return getName(dom), int(getNbSimulation(dom)), int(getNbGroup(dom)), getTransition(dom), getSpecies(dom), int(getNbTransition(dom)), int(getNbSpecies(dom)), getInitialQuantity(dom)

