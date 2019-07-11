import deregress
import os

THIS_DIRECTORY = os.path.dirname(__file__)
PATH_TO_GRAPH_RUNNER = os.path.abspath(os.path.join(THIS_DIRECTORY, '../../tools/GraphRunner'))

@deregress.test
def create_rect(test):
    test.runner.executable(PATH_TO_GRAPH_RUNNER) \
        .arg("--input-file={}".format(os.path.join(THIS_DIRECTORY, "create_rect.sel"))) \
        .arg("--execute") \
        .arg("--export-geometry") \
        .arg("--export-path=.") \
        .arg("--dot=create_rect.dot") \
        .run()

    test.tester.file_contents_should_match("geom0.obj")
    test.tester.file_contents_should_match("create_rect.dot")


@deregress.test
def extrusion(test):
    test.runner.executable(PATH_TO_GRAPH_RUNNER) \
        .arg("--input-file={}".format(os.path.join(THIS_DIRECTORY, "extrusion.sel"))) \
        .arg("--execute") \
        .arg("--export-geometry") \
        .arg("--export-path=.") \
        .arg("--dot=extrusion.dot") \
        .run()

    test.tester.file_contents_should_match("geom0.obj")
    test.tester.file_contents_should_match("extrusion.dot")

@deregress.test
def expressions_in_parameters(test):
    test.runner.executable(PATH_TO_GRAPH_RUNNER) \
        .arg("--input-file={}".format(os.path.join(THIS_DIRECTORY, "expression.sel"))) \
        .arg("--execute") \
        .arg("--export-geometry") \
        .arg("--export-path=.") \
        .arg("--dot=expression.dot") \
        .run()

    test.tester.file_contents_should_match("geom0.obj")
    test.tester.file_contents_should_match("expression.dot")

@deregress.test
def parameter_overwrite(test):
    test.runner.executable(PATH_TO_GRAPH_RUNNER) \
        .arg("--input-file={}".format(os.path.join(THIS_DIRECTORY, "parameter_overwrite.sel"))) \
        .arg("--execute") \
        .arg("--export-geometry") \
        .arg("--export-path=.") \
        .arg("--dot=parameter_overwrite.dot") \
        .run()

    test.tester.file_contents_should_match("geom0.obj")
    test.tester.file_contents_should_match("parameter_overwrite.dot")
