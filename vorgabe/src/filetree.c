#include <filetree.h>

/* new file tree */
Tree* filetree_new()
{
	Tree* tree = ALLOCATE(sizeof(Tree));
	tree->root = ALLOCATE(sizeof(Directory));
	Node* t = (Node*) tree->root;
	t->flags = FILE_TREE_FLAG_DIRECTORY;
	t->parent = NULL;
	t->name = malloc(sizeof(char)*2);
	strncpy(t->name,"",2);
	t->next = NULL;
	t->prev = NULL;
	return NULL; /* <-- remove this */
}

/* destroy file tree */
void filetree_destroy(Tree * tree)
{
	Directory* dir = (Directory*) tree;
	if(strncmp(dir->node.name,"",strlen(dir->node.name))) return;
	Node* childs = dir->first_child;
	while(childs->next!=NULL){
		if(childs->flags == FILE_TREE_FLAG_DIRECTORY){
			filetree_destroy((Tree *)childs);
		}
		else{
			childs = childs->next;
			FREE(childs->prev);
		}
		childs = childs->next;
	}
}

/* mkdir */
FileError filetree_mkdir(Directory * parent, const char * name)
{
	/* TODO: create new directory */
	return FILE_TREE_ERROR_NOT_IMPLEMENTED;
}

/* mkfile */
FileError filetree_mkfile(Directory * parent, const char * name, const void * data, size_t data_len)
{
	/* TODO: create new file */
	return FILE_TREE_ERROR_NOT_IMPLEMENTED;
}

/* list contents of directory */
void filetree_ls(const Directory * dir)
{
	/* TODO: show contents of given directory */
	puts(filetree_error_string(FILE_TREE_ERROR_NOT_IMPLEMENTED));
}

/* find */
void filetree_find(const Directory * start, const char * name)
{
	/* TODO: show files/directories recursively */
	puts(filetree_error_string(FILE_TREE_ERROR_NOT_IMPLEMENTED));
}

/* remove file/directory */
FileError filetree_rm(Node * node)
{
	/* TODO: remove a file or directory and all of its children */
	return FILE_TREE_ERROR_NOT_IMPLEMENTED;
}

/* move file/directory */
FileError filetree_mv(Node * source, Directory * destination)
{
	/* TODO: move a file or directory to a new location */
	return FILE_TREE_ERROR_NOT_IMPLEMENTED;
}

/* print file */
void filetree_print_file(File * file)
{
	/* TODO: print contents of file to stdout */
	puts(filetree_error_string(FILE_TREE_ERROR_NOT_IMPLEMENTED));
}

/* resolve path */
FileError filetree_resolve_path(Tree * tree, const char * path, const Directory * current_dir, Node ** resulting_node)
{
	/* TODO: parse path and get corresponding node */
	return FILE_TREE_ERROR_NOT_IMPLEMENTED;
}

/* get path */
char* filetree_get_path(Node * n)
{
	/* TODO: return path as string for given node */
	return NULL; /* <-- remove this */
}

/* is string valid file/directory name? */
FileError filetree_name_valid(const char * name)
{
	if(strncmp(name,".",strlen(name))==0 || strncmp(name,"..",strlen(name)) == 0 || strncmp(name,"",strlen(name)) == 0){
		return FILE_TREE_ERROR_ILLEGAL_NAME;
	}
	for(int i = 0;i<strlen(name);i++){
		if(isalpha(name[i]) || (name[i]>='0' && name[i]<='9') || name[i] == '.' || name[i] == '-' || name[i] == '_')
		{
			continue;
		}
		else{
			return FILE_TREE_ERROR_ILLEGAL_NAME;
		}
	}
	return FILE_TREE_SUCCESS;
}

/* get error string */
const char * filetree_error_string(FileError e){
	static const char * ERROR_CODE_STRINGS[FILE_TREE_NUM_ERROR_CODES] = {
		[FILE_TREE_ERROR_NOT_FOUND]         = "no such file or directory",
		[FILE_TREE_SUCCESS]                 = "success",
		[FILE_TREE_ERROR_DUPLICATE_NAME]    = "name does already exist",
		[FILE_TREE_ERROR_ILLEGAL_NAME]		= "illegal name",
		[FILE_TREE_ERROR_SUBDIR_OF_ITSELF]  = "can't move a directory to a subdirectory of itself",
		[FILE_TREE_ERROR_RM_ROOT]           = "root can't be removed",
		[FILE_TREE_ERROR_NOT_IMPLEMENTED]   = "function not implemented yet"
	};

	if(e >= FILE_TREE_NUM_ERROR_CODES || e < 0){
		return "unknown error";
	}

	return ERROR_CODE_STRINGS[e];
}
