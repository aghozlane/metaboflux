#!/bin/bash
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

# Default values
DOCUMENTATION_IT=0
INSTALL_IT=0

# User help
help()
{
  {
  echo
  echo Usage: `basename $1` "[-I][-D][-h]"
  echo
  echo '             -I : Compile the software in MetaBoFlux-<version>/bin/'
  echo '             -D : Compile the documentation'
  echo '             -h : this help'
  } 1>&2
}

# If no arguments ==> display help
if [ $# -eq 0 ]
then
  help $0
  exit 1
fi

# Make sure that we are running in the source directory
if [ ! -f src/MetaBoFlux.c -o ! -f lib/libCompute.py ]
then
  echo This script must be run in the top level of the source directory of MetaBoFlux >&2
  exit 1
fi

# Parse the command line
while getopts IDh arg
do
  case ${arg} in
    I) INSTALL_IT=1;;
    D) DOCUMENTATION_IT=1;;
    h) help $0
       exit 0;;
    *) help $0
       exit 1;;
  esac
done

# Make sure that cmake is installed if build or installation or packaging is requested
if [ ${INSTALL_IT} -eq 1 ]
then
  which cmake > /dev/null 2>&1
  if [ $? -ne 0 ]
  then
    echo To be able to compile/install MetaBoFlux, you must install cmake and/or update the PATH variable >&2
    exit 1
  fi

  # Launch cmake
  echo Configuring MetaBoFlux installation
  cmake -G "Unix Makefiles"
  make
fi
if [ ${DOCUMENTATION_IT} -eq 1 ]
then
  cmake -G "Unix Makefiles"
  make html
fi