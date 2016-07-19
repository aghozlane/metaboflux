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
 @brief: Run MetaBoFlux.exe
 @author: Amine Ghozlane
 @version: 2.0 
"""
import subprocess
import sys
import os

def runCommand(cmd):
    """
    Run command
    @param cmd: Command to run
    """
    try:
        #Execution de la ligne de commande
        retcode = subprocess.call(cmd, shell=True)
        #Cas aucun retour du soft
        if retcode == None:
            sys.exit("Child was terminated")
    except OSError, e:
        sys.exit("Execution failed: %s"%(e))
    except:
        sys.exit("There is something wrong with the command: %s"%(cmd))

#Lance le recuit simule
def simulated_annealing(thread, sbml, par, sim_out, debug, port):
    """
    @param thread: Number of thread
    @param sbml: SBML file
    @param par: Parameter file
    @param sim_out: Simulation repertory
    @param debug: Debug called
    @param port: Port for socket connection
    """
    run="mpiexec -n %d %s/MetaBoFlux.exe -s --sbml %s --par %s --out %s --port %d" % (thread, os.path.join(os.path.dirname(__file__),"../bin"), sbml, par, sim_out, port)
    if debug:
        run+=" --debug "
    runCommand(run)
      
#Lance la minimisation des resultats
def minimization(minioutfilename, logoutfilename,ratiofilename, nb_simulation, thread, sbml, par, min_out, debug, port):
  """
  Run minimization program
  @param minioutfilename: Minimization file
  @param logoutfilename: Log file
  @param nb_simulation: Number of simulation
  @param thread: Number of thread
  @param sbml: SBML file
  @param par: Parameter file
  @param min_out: Minimization repertory
  @param debug: Debug called
  @param port: Port for socket connection  
  """
  thread+=1
  run="mpiexec -n %d %s/MetaBoFlux.exe -m --sbml %s --par %s --ratio_file %s --out_file %s --log %s --out %s --port %d" % (thread, os.path.join(os.path.dirname(__file__),"../bin"), sbml, par, ratiofilename, minioutfilename, logoutfilename, min_out, port)
  if debug:
      run+=" --debug "
  #Lancement de la minimisation
  runCommand(run)

#Lance la modelisation des resultats
def modeling(outfilename, modellogoutfilename, ratiofilename, nb_simulation, thread, sbml, par, min_out, debug, port, group):
  """
  Run modeling program
  @param outfilename: Output file
  @param modellogoutfilename: Model log outfile name
  @param nb_simulation: Number of simulation
  @param thread: Number of thread 
  @param sbml: SBML file
  @param par: Parameter file
  @param min_out: Minimization file
  @param debug: Debug called
  @param port: Port for socket connection
  @param group: Group called
  """
  thread+=1
  run="mpiexec -n %d %s/MetaBoFlux.exe -e --sbml %s --par %s --ratio_file %s --out_file %s --log %s --out %s --port %d" % (thread, os.path.join(os.path.dirname(__file__),"../bin"), sbml, par, ratiofilename, outfilename, modellogoutfilename, min_out, port)
  if debug:
      run+=" --debug "
  if group:
      run+=" --group "
  #Lancement de la modelisation
  runCommand(run)

#Lance l'estimation de la variance
def standard_deviation(sdoutfilename,ratiofilename, nb_simulation, thread, sbml, par, min_out, debug, port):
  """
  Run standard deviation program
  @param sdoutfilename: Standard deviation file
  @param nb_simulation: Number of simulation
  @param thread: Number of thread
  @param sbml: SBML file
  @param par: Parameter file
  @param min_out: Minimization repertory
  @param debug: Debug called
  @param port: Port for socket connection
  """
  thread+=1
  run="mpiexec -n %d %s/MetaBoFlux.exe -t --sbml %s --par %s --ratio_file %s --out_file %s --out %s --port %d" % (thread, os.path.join(os.path.dirname(__file__),"../bin"), sbml, par, ratiofilename, sdoutfilename, min_out, port)
  if debug:
      run+=" --debug "
  #Lancement du calcul de la standard deviation
  runCommand(run)

#Dessin des figures et calcul des groupes
def statistics(par, modeloutfilename, sdoutfilename, stat_out):
    """
    Run statistic program
    @param par: Parameter file
    @param modeloutfilename: Modelisation file
    @param sdoutfilename: Standard deviation file
    @param stat_out: Statistic repertory file
    """
    run="%s/MetaBoFlux_stat.R %s %s %s %s" % (os.path.join(os.path.dirname(__file__),"../bin"), par, modeloutfilename, sdoutfilename, stat_out)
    runCommand(run)