Find_Program(CLHEP_CONFIG_EXE NAMES clhep-config PATHS
   /cvmfs/cms.cern.ch/slc6_amd64_gcc491/external/clhep/2.2.0.4/bin
	${SIMPATH}/cern/clhep/bin
	${SIMPATH}/cern/clhep/
	${SIMPATH}/bin
	NO_DEFAULT_PATH
)

If(CLHEP_CONFIG_EXE)
	# Use clhep-config to find libs and includes
	execute_process(COMMAND ${CLHEP_CONFIG_EXE} --libs OUTPUT_VARIABLE CLHEP_CONFIG_LIBS OUTPUT_STRIP_TRAILING_WHITESPACE)
	execute_process(COMMAND ${CLHEP_CONFIG_EXE} --include OUTPUT_VARIABLE CLHEP_CONFIG_INCLUDE_DIR OUTPUT_STRIP_TRAILING_WHITESPACE)
	execute_process(COMMAND ${CLHEP_CONFIG_EXE} --prefix OUTPUT_VARIABLE CLHEP_BASE_DIR OUTPUT_STRIP_TRAILING_WHITESPACE)

	# clean the variables because they are of the form -I/<dir> or -L/<dir> -l<lib>
	String (REGEX MATCHALL "((-L|-l|-Wl)([^\" ]+|\"[^\"]+\")|/[^\" ]+(a|so|dll))" _all_tokens "${CLHEP_CONFIG_LIBS}")
	Set(_directory_list)
	ForEach (token ${_all_tokens})
		If (token MATCHES "-L([^\" ]+|\"[^\"]+\")")
			# If it's a library path, add it to the list
			string (REGEX REPLACE "^-L" "" token ${token})
			string (REGEX REPLACE "//" "/" token ${token})
			list (APPEND _directory_list ${token})
		EndIf (token MATCHES "-L([^\" ]+|\"[^\"]+\")")
	EndForEach (token ${_all_tokens})
	Set(CLHEP_LIBRARY_DIR ${_directory_list})

	String (REGEX MATCHALL "-I([^\" ]+|\"[^\"]+\")" _all_tokens "${CLHEP_CONFIG_INCLUDE_DIR}")
	
	Set (_incs_found)
	ForEach (token ${_all_tokens})
		String (REGEX REPLACE "^-I" "" token ${token})
		String (REGEX REPLACE "\"" "" token ${token})
#		If (EXISTS ${token})
			List (APPEND _incs_found ${token})
#		EndIf (EXISTS ${token})
	EndForEach (token)
	List (LENGTH _incs_found _length)
	If (_length GREATER 1)
		List (REMOVE_DUPLICATES _incs_found)
	EndIf (_length GREATER 1)
	Set (CLHEP_INCLUDE_DIR ${_incs_found})
	set(CLHEP_LIBRARY "-L${CLHEP_LIBRARY_DIR} -lCLHEP") 

Else(CLHEP_CONFIG_EXE)

	FIND_PATH(CLHEP_INCLUDE_DIR NAMES CLHEP PATHS
		${SIMPATH}/cern/clhep/include
		${SIMPATH}/include
		NO_DEFAULT_PATH
	)

	FIND_PATH(CLHEP_BASE_DIR NAMES ClhepVersion.h PATHS
		${SIMPATH}/cern/clhep
		${SIMPATH}/include
		NO_DEFAULT_PATH
	)

	FIND_PATH(CLHEP_LIBRARY_DIR NAMES libCLHEP.so libCLHEP.dylib PATHS
		${SIMPATH}/cern/clhep/lib
		${SIMPATH}/lib
		NO_DEFAULT_PATH
	)

	set(CLHEP_LIBRARY "-L${CLHEP_LIBRARY_DIR} -lCLHEP")

EndIf(CLHEP_CONFIG_EXE)

IF (CLHEP_INCLUDE_DIR AND CLHEP_LIBRARY)
     SET(CLHEP_FOUND TRUE)
ENDIF (CLHEP_INCLUDE_DIR AND CLHEP_LIBRARY)

IF (CLHEP_FOUND)
     IF (NOT CLHEP_FIND_QUIETLY)
        MESSAGE(STATUS "Found CLHEP: ${CLHEP_LIBRARY}")
        MESSAGE(STATUS "Found CLHEP include: ${CLHEP_INCLUDE_DIR}")
     ENDIF (NOT CLHEP_FIND_QUIETLY)
ELSE (CLHEP_FOUND)
     IF (CLHEP_FIND_REQUIRED)
        MESSAGE(FATAL_ERROR "Could not find CLHEP. We search first in 
the normal library paths, then in $CLHEPDIR")
     ELSE(CLHEP_FIND_REQUIRED)
        IF(NOT CLHEP_FIND_QUIETLY)
           MESSAGE(STATUS "Could not find CLHEP.  We search first in 
the normal library paths, then in $CLHEPDIR")
        ENDIF(NOT CLHEP_FIND_QUIETLY)
     ENDIF (CLHEP_FIND_REQUIRED)

ENDIF (CLHEP_FOUND)
