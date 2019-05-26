set(REGRESSION_TEST_SCRIPTS
    "regression_test.py"
    "create_rect.sel"
    "extrusion.sel"
)

foreach (file ${REGRESSION_TEST_SCRIPTS})
    configure_file(${file} ${CMAKE_CURRENT_BINARY_DIR}/${file} COPYONLY)
endforeach()

