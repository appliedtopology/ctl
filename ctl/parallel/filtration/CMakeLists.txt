# Create library for module
# In this case, none
# add_library (abstract_simplex abstract_simplex.cpp)

# add_definitions( "-DDPERSISTENCE")

# Make sure the compiler can find include files
# needs cell_with_coefficient.h in abstract_simplex
include_directories (${COMPLEX_SOURCE_DIR}/abstract_simplex
                     ${COMPLEX_SOURCE_DIR}/cell_set
                     ${COMPLEX_SOURCE_DIR}/io
		     ${COMPLEX_SOURCE_DIR}/hash_integers
		     ${COMPLEX_SOURCE_DIR}/weighted_complex
		     ${COMPLEX_SOURCE_DIR}/persistence
		     ${COMPLEX_SOURCE_DIR}/finite_field
		     ${COMPLEX_SOURCE_DIR}/filtration
		     ${COMPLEX_SOURCE_DIR}/blowup/thread_timer
		     ${COMPLEX_SOURCE_DIR}/boundary_operator
		     ${COMPLEX_SOURCE_DIR}/term
		    )

# Make sure the linker can find libraries
link_directories (${COMPLEX_BINARY_DIR}/abstract_simplex
		 ${COMPLEX_BINARY_DIR}/io
)
