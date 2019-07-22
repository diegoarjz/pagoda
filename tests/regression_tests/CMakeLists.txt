set(REGRESSION_TEST_SCRIPTS
    "regression_test.py"
    "create_rect.sel"
    "extrusion.sel"
    "export_geometry.sel"
    "expression.sel"
    "parameter_definition.sel"
    "parameter_overwrite.sel"
    "parameter_renaming.sel"
)

foreach (file ${REGRESSION_TEST_SCRIPTS})
    configure_file(${file} ${CMAKE_CURRENT_BINARY_DIR}/${file} COPYONLY)
endforeach()

