# Try to find gnu scientific library LIBSBML
# See
# http://sbml.org/Software/libSBML
#
# Script of Amine Ghozlane
# (http://www.labri.fr/~ghozlane/)
# It defines the following variables:
#  LIBSBML_FOUND - system has LIBSBML lib
#  LIBSBML_INCLUDE_DIR - where to find headers
#  LIBSBML_LIBRARIES - full path to the libraries

SET( LIBSBML_FOUND OFF )

# Look for LIBSBML include files
FIND_PATH(LIBSBML_INCLUDE
          NAMES sbml
          PATHS /usr/include
                /usr/local/include)

IF (LIBSBML_INCLUDE)
   set(LIBSBML_INCLUDE_DIR ${LIBSBML_INCLUDE})
    
   # Look for LBSBML library
   FIND_LIBRARY(LIBSBML_LIBRARY
             NAMES sbml
	         PATHS /lib
                   /usr/lib
                   /usr/local/lib)
   IF(LIBSBML_LIBRARY)
      set(LIBSBML_FOUND ON)
      set( LIBSBML_LIBRARIES ${LIBSBML_LIBRARY})
      set( LIBSBML_LIBRARIES ${LIBSBML_LIBRARY})
      message("LIBSBML_INCLUDE_DIR=${LIBSBML_INCLUDE_DIR}")
      message("LIBSBML_LIBRARIES=${LIBSBML_LIBRARIES}")
   ELSE(LIBSBML_LIBRARY)
      message(STATUS "Unable to find 'libsbml' library")
   mark_as_advanced(
      LIBSBML_INCLUDE
      LIBSBML_LIBRARY
   )
   ENDIF(LIBSBML_LIBRARY)
ELSE(LIBSBML_INCLUDE)
   MESSAGE(STATUS "Unable to find 'libsbml' include files")  
ENDIF(LIBSBML_INCLUDE)