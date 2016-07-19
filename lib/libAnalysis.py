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
 @brief: Extraction and compilation of data
 @author: Amine Ghozlane
 @version: 2.0 
"""
import re, os, sys, commands, threading, socket, subprocess
from libsbml import *

#==============================================================
#Creation des fichiers de sortie
#==============================================================
#Lecture des valeurs
def ReadValue(inputfilename, compilation, nb_parameters):
  """
  Read value from simulated annealing
  @param inputfilename: Simulated annealing files
  @param compilation:  output : Compilation file
  @param nb_parameters: Number of parameters
  """
  tab = []
  try:
    #Ouverture du fichier
    inputfile = open(inputfilename, "r")
    
    #Creation de la regex
    text="\s+\S+"*3
    text+="\s+\[\s+"
    for i in range(nb_parameters):
        text+="\S+\s+"
    text+="\]\s+(\S+)\s+(\S+)\s+"
    regex = re.compile(text)
    
    #Extraction des donnees
    for i in inputfile:
        a = regex.match(i)
        if a:
            tab.append([float(a.group(1)), float(a.group(2)), a.group(0)])
    #Fermeture du fichier
    inputfile.close()
  except IOError:
    sys.exit("file %s is missing" % inputfilename)
  #Selection de la meilleure solution
  tab.sort()
  #Ecriture des donnees
  compilation.write(tab[0][2] + "\n")
  
#Extraction des donnees de simulated annealing   
def extractValue(valuefilename, sim_out, simulationname, nb_simulation, nb_parameters):
  """
  Extract data from simulated annealing
  @param valuefilename: Compilation file
  @param sim_out:  Simulated annealing files repertory
  @param simulationname: Name of simulated annealing file
  @param nb_simulation: Number of simulations
  @param nb_parameters: Number of parameters
  """
  #Supprime tout ancien fichier de compilation
  #subprocess.call("rm -f %s"% valuefilename, shell=True)
  try:
      os.remove("%s"% valuefilename)
  except:
      pass
  #Creation du nouveau fichier
  try:
    #Ouverture du fichier de compilation
    compilation = open(valuefilename, "w")
  
    #Lecture de tous les fichiers 
    for i in range(1, (nb_simulation + 1)):
       #Nom des fichiers de donnees du recuit simule
       a = sim_out + simulationname + "_simulated_annealing_" + str(i) + ".txt"
       #Ecriture de leurs caracteristiques dans le fichier de compilation
       compilation.write("==> %s <==\n" % a)
       #Lecture et compilation des donnees 
       ReadValue(a, compilation, nb_parameters)
    #Fermeture du fichier
    compilation.close()
  except IOError:
    sys.exit("Can't open compilation file")

#Preparation du fichier d'enregistrement
def preparationOutfile(outfilename, outputparameters, parameters, nb_parameters, parameters_initialQuantity, group):
  """
  Preparation of the registration file
  @param outfilename: Output file
  @param outputparameters: List of interest molecules
  @param parameters: List of interest parameters
  @param nb_parameters: Number of parameter
  @param parameters_initialQuantity: Inital quantities of interest 
  @param group: Group called
  """
  #Ouverture du fichier de sortie
  try:
      out = open(outfilename, "wt")
      intitule=""
      #Ecriture de l'intitule
      if(group):
          intitule+="Group\t"
      intitule += "Energy\t"
      for i in outputparameters:
        intitule += i + "\t"
      out.write(intitule)
      for i in parameters:
        out.write(i + "\t")
      out.write("\n")
      
      #Ecriture des valeurs attendues
      if(group):
          out.write("HX\t")
      out.write("0\t")
      for i in parameters_initialQuantity:
        out.write(i + "\t")
      #Ratio sont encore inconnus
      for i in range(nb_parameters):
        out.write("NA\t")
      out.write("\n")
      #fermeture du fichier
      out.close()
  except IOError:
      sys.exit("Impossible d'ouvrir le fichier %s"%outfilename)
      
def WriteFile(fileOut,carac):
    """
    Write data for ratio file
    @param fileOut: Ratio file
    @param carac: Ratios
    """
    try:
        compilation = open(fileOut, "w")
        compilation.write(carac)
        #Fermeture du fichier
        compilation.close()
    except IOError:
        sys.exit("Error : We can't open %s"%fileOut)

def ratioFile(regex,dataFile, fileOut, interest, nb_outputparameters, parameters):
    """
    Read the data which have to be compiled for the ratio file
    @param regex: Regular expression
    @param dataFile: File which contains ratios
    @param interest: Flag for regex
    @param nb_outputparameters: Number of Molecules
    @param parameters: List of interest parameters
    """
    a = None
    carac = ""
    try:
        #Ouverture du fichier de compilation
        input_file=open(dataFile,"r")
        for i in input_file:
          a = regex.match(i)  
          #Si regex detectee
          if a:
              if(interest):
                  for j in range(2 + nb_outputparameters, 2 + nb_outputparameters + parameters):
                      carac += a.group(j) + "\t"
                  carac+="\n"
              else:
                  carac+=a.group(1)
                  carac+="\n"
                  carac.replace(" ","\t")    
    except IOError:
        sys.exit("Error : We can't open %s"%(dataFile))
        
    #Ecriture du fichier de sortie
    WriteFile(fileOut,carac)

#Ecriture du fichier ratio
def refreshRatioFile(inputFilename,ratiofilename,min,nb_outputparameters,parameters):
    """
    Refresh the ratio file
    @param inputFilename: Data file
    @param ratiofilename: Ratio file
    @param min: Flag for create ratio file from minimization file
    @param nb_outputparameters: Number of molecules
    @param parameters: List of reactions ratio
    """
    #Definition de la regex
    if(min):
        regex = re.compile(donnes_etude(nb_outputparameters, parameters,False))
    else:
        regex = re.compile(".+\[.(.+).\].+")
        
    #Creation du fichier de ratio
    ratioFile(regex,inputFilename,ratiofilename,min,nb_outputparameters,parameters)
    
        
#Energie ATP ADP NADH NAD Succ1 Succ2 Acetate parameters...
def donnes_etude(nb_outputparameters, parameters, group):
  """
  Energy quantity parameters.
  @param nb_outputparameters: Number of Molecules
  @param parameters: Number of parameters for reaction ratios 
  """
  sujet_etude =""
  if group:
      sujet_etude += "^H(\d)\s+"

  sujet_etude += "([\-0-9]+.[0-9]+)"
  #Molecules
  for i in range(nb_outputparameters):
    sujet_etude += ".([\-0-9]+)"
  #Reaction ratios
  for i in range(parameters):
    sujet_etude += ".([\-0-9]+.[0-9]+)"
  
  return sujet_etude

#Extraction des donnees du fichier group log
def data_group(parameters):
  """
    Extraction of data from the group log file 
    @param parameters: Number of parameters
    @return: Reaction ratios of groups
  """
  sujet_etude = "(^H\d)\s+"
  for i in range(parameters):
      sujet_etude += "(\S+)\s+"
  return sujet_etude

#Ordonne les valeurs des simulations
def classificationValues(outfilename, nb_outputparameters, parameters,group):
  """
  Ordered values simulations
  @param outfilename: Read file (Modeling or minimization)
  @param nb_outputparameters: Number of molecules
  @param parameters: Number of reaction ratios
  @param group = Group called
  @return: List solutions
  """
  regex = re.compile(donnes_etude(nb_outputparameters, parameters, group))
  result = []
  tab = []
  try:
      simulfile = open(outfilename, 'r')
      #Lecture des fichiers Modelling et Minimization
      for i in simulfile:
        a = regex.match(i)
        if a:
            carac = ""
            #Selection des groupes
            if group:
                for j in range(2, 2 + nb_outputparameters + parameters+1):
                    carac += a.group(j) + "\t"
                tab.append([int(a.group(1)), carac])
            else:
                for j in range(2, 2 + nb_outputparameters + parameters):
                    carac += a.group(j) + "\t"
                #Sauvegarde dans le tabeau
                tab.append([float(a.group(1)), carac])
      simulfile.close()
  except IOError:
      sys.exit("Read error of %s"%(outfilename))
  except:
      sys.exit("Something went wrong with : %s"%(outfilename)) 
  #tri du tableau
  tab.sort()
  return tab

#Compilation des donnees de groupe 
def groupValues(inputfilename, parameters, valuegroupfilename):
  """
   Compile group modelling information
   @param inputfilename: Name of the group modelling file name
   @param parameters: Number of parameters
   @param valuegroupfilename: Group compilation file
  """
  outfile=None
  simulfile=None
  regex = re.compile(data_group(parameters+1))  
  #Ouverture des fichiers de compilation et modelisation des groupes
  try:
      outfile = open(valuegroupfilename, 'w')
  except IOError:
      sys.exit("Error opening the file : %s"%(valuegroupfilename))
  except:
      sys.exit("Something went wrong with : %s"%(valuegroupfilename))   
  try:
      simulfile = open(inputfilename, 'r')
  except IOError:
      sys.exit("Error opening the file : %s"%(inputfilename))
  except:
      sys.exit("Something went wrong with : %s"%(inputfilename))
  #Parsing du fichier
  for i in simulfile:
    a = regex.match(i)
    if a:
      carac = a.group(1) + " [ "
      for i in range(3, 3 + parameters):
          carac += a.group(i) + " "
      carac += " ] \n"
      outfile.write(carac)
  simulfile.close()
  outfile.close()

#Enregistrement des donnees triees
def saveclassifiedValues(tab, simuloutfilename,group):
  """
  Save classified values
  @param tab: List solutions
  @param simuloutfilename: Outfile
  @param group = Group called
  """
  try:
      simuloutfile = open(simuloutfilename, 'a')
      if group:
          for i in range(len(tab)):
            simuloutfile.write("H%d\t%s\n" % (tab[i][0], tab[i][1]))
      else :    
          for i in range(len(tab)):
            simuloutfile.write("%.3f\t%s\n" % (tab[i][0], tab[i][1]))
      simuloutfile.close()
  except IOError:
      sys.exit("Error opening the file : %s"%(simuloutfilename))
  except:
      sys.exit("Something went wrong with : %s"%(simuloutfilename))
 
#Preparation du fichier d'enregistrement
def preparationOutfile2(outfilename):
  """
  Preparation of the standard deviation file
  @param outfilename: Standard deviation file
  """
  out=None
  try:
      out = open(outfilename, "wt")
      #Ecriture de l'entete
      out.write("Modelisation\tEnergy\tStandard_deviation\n")
      #Fermeture du fichier
      out.close()
  except IOError:
      sys.exit("Error opening the file  : %s"%(outfilename))
  except:
      sys.exit("Something went wrong with : %s"%(outfilename))    


#Preparation du fichier d'enregistrement
def preparationOutfile3(outfilename, sbml, parameters, group):
  """
  Head for minimization file
  @param outfilename: Out file
  @param sbml: SBML file
  @param parameters: Reaction ratio
  @param group: Group called
  @return: Number of molecules
  """
  reader = SBMLReader()
  document = reader.readSBML(sbml)  
  #Test si presence d'erreur
  if document.getNumErrors() != 0:
      print("There is some error in the sbml %s"%(sbml))
      sys.exit(document.getErrorLog())
  #Constitution du model
  Model = document.getModel()
  #Nombre de molecules
  nbSpecies = Model.getNumSpecies()
  #Liste des molecules 
  species = ""
  for i in range(0, nbSpecies):
    mol = Model.getSpecies(i)
    species += mol.getId() + "\t"
  
  #Ecriture du fichier
  try:
      out = open(outfilename, "wt")
      if(group):
          out.write("Group\t")
      out.write("Energy\t")
      out.write(species)
      for i in parameters:
        out.write(i + "\t")
      out.write("\n")
      #fermeture du fichier
      out.close()
  except IOError:
      sys.exit("Error opening the file : %s"%(outfilename))
  except:
      sys.exit("Something went wrong with : %s"%(outfilename))  
  return(nbSpecies)

#==============================================================
#Connection a l'interface
#==============================================================
#Connexion
def connectInterface(port):
  """
    Connection to the interface
    @param port: Connection port
    @return: Socket
  """
  s = None
  try:
    #create an INET, STREAMing socket
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
  except socket.error, msg:
    sys.exit("Could not open socket")

  #Connection au serveur
  try:
    s.connect((socket.gethostname(), port))
  except socket.error, msg:
    sys.exit('Could not open socket')
  #Socket
  return(s)

#Deconnexion
def disconnectInterface(s):
  """
    Disconnection of the interface  
    @param s: Socket
  """
  s.close()

  
#==============================================================
#Detection des CPUs
#==============================================================
#Detects the number of CPUs on a system. Cribbed from pp.
def detectCPUs():
  """
  Detects the number of CPUs on a system. Cribbed from pp.
  """
  # Linux, Unix and MacOS: # Linux, Unix et MacOS:
  if hasattr (os, "sysconf"):
    if os . sysconf_names . has_key("SC_NPROCESSORS_ONLN"):
      # Linux & Unix: # Linux et Unix:
      ncpus = os . sysconf("SC_NPROCESSORS_ONLN")
      if isinstance (ncpus, int) and ncpus > 0 :
          return ncpus 
      else : # OSX:
          return int (os . popen2("sysctl -n hw.ncpu")[ 1 ] . read())
    # Windows:
  if os . environ . has_key("NUMBER_OF_PROCESSORS"):
    ncpus = int (os . environ[ "NUMBER_OF_PROCESSORS" ]);
    if ncpus > 0 :
      return ncpus
  return 1 # Default 1 

