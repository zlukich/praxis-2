#include "filetree_helpers.h"

const char * expected_output = 
	"this_is_a_directory_with_a_super_long_name     DIRECTORY\n"
	"a                                              DIRECTORY\n"
	"this_is_a_file                                 FILE\n"
	"this_is_a_directory                            DIRECTORY\n";

int main(int argc, char ** argv)
{
	/* initialize filetree testing */
	filetree_test_init();
	
	/* new tree */
	Tree * t = filetree_new();

	/* create a bunch of directories and files */
	filetree_mkdir(t->root, "this_is_a_directory");
	filetree_mkfile(t->root, "this_is_a_file", NULL, 0);
	filetree_mkdir(t->root, "a");
	filetree_mkdir(t->root, "this_is_a_directory_with_a_super_long_name");

	/* ls */
	filetree_ls(t->root);

	/* free tree */
	filetree_destroy(t);

	/* check and write results */
	filetree_test_check(argv[0], expected_output);

	return 0;
}
