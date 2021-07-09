#include <filetree.h>

/* new file tree */
Tree* filetree_new()
{
	Tree* tree = ALLOCATE(sizeof(Tree));
	tree->root = ALLOCATE(sizeof(Directory));
	Node* t = (Node*) tree->root;
	t->flags = FILE_TREE_FLAG_DIRECTORY;
	t->parent = NULL;
	t->name = ALLOCATE(sizeof(char)*2);
	strncpy(t->name,"",2);
	t->next = NULL;
	t->prev = NULL;
	return tree;
}
void filetree_destroy_dir(Directory * dir){
	
	if(dir->first_child==NULL) return;
	Node* childs = dir->first_child;
	while(childs!=NULL){
		if(childs->flags == FILE_TREE_FLAG_DIRECTORY){
			filetree_destroy_dir((Directory*)childs );
		}
		else{
			if(childs->next == NULL)
			{
				FREE(childs);
			}
			else
			{
				childs = childs->next;
				FREE(childs->prev);
			}
		}
		childs = childs->next;
	}
}
/* destroy file tree */
void filetree_destroy(Tree * tree)
{
	Directory* dir = tree->root;
	filetree_destroy_dir(dir);
}

/* mkdir */
FileError filetree_mkdir(Directory * parent, const char * name)
{

	Node* childs = (Node *)parent;

	if(filetree_name_valid(name) == FILE_TREE_ERROR_ILLEGAL_NAME)
		return FILE_TREE_ERROR_ILLEGAL_NAME;

	while(childs!=NULL){
		if(strncmp(childs->name,name,strlen(name))== 0)
			return FILE_TREE_ERROR_DUPLICATE_NAME;
		childs = childs->next;
	}
	Directory* new_d = ALLOCATE(sizeof(Directory));
	new_d->first_child =NULL;
	Node * node = (Node *) new_d;
	node->flags = FILE_TREE_FLAG_DIRECTORY;
	node->parent = parent;
	node->name = ALLOCATE(sizeof(char)*(strlen(name)+1));
	strncpy(node->name,name,strlen(name));
	node->next = parent->first_child;
	node->prev = NULL;
	parent->first_child = (Node *)new_d;
	return FILE_TREE_SUCCESS;
}

/* mkfile */
FileError filetree_mkfile(Directory * parent, const char * name, const void * data, size_t data_len)
{
	Node* childs = (Node *)parent;

	if(filetree_name_valid(name) == FILE_TREE_ERROR_ILLEGAL_NAME)
		return FILE_TREE_ERROR_ILLEGAL_NAME;

	while(childs!=NULL){
		if(strncmp(childs->name,name,strlen(name))== 0)
			return FILE_TREE_ERROR_DUPLICATE_NAME;
		childs = childs->next;
	}

	File * file = ALLOCATE(sizeof(File));
	Node * file_node = (Node *) file;
	file->data = ALLOCATE(sizeof(data));
	file->data = data;
	file->data_len = data_len;
	file_node->flags = FILE_TREE_FLAG_EXECUTABLE;
	file_node->parent = parent;
	file_node->next = parent->first_child;
	file_node->prev = NULL;
	parent->first_child = file_node;
	file_node->name = ALLOCATE(sizeof(char)*(strlen(name)+1));
	strncpy(file_node->name,name,strlen(name));

	return FILE_TREE_SUCCESS;
}

/* list contents of directory */
void filetree_ls(const Directory * dir)
{
	Node * childs = dir->first_child;
	int max = 0;
	while(childs!=NULL){
		if(strlen(childs->name)>=max)
			max = strlen(childs->name);
		
		childs = childs->next;
	}
	childs = dir->first_child;
	while(childs!=NULL){
		if(childs->flags == FILE_TREE_FLAG_DIRECTORY){
			printf("%*s DIRECTORY\n",-max-4,childs->name);
		}
		else{
			printf("%*s FILE\n",-max-4,childs->name);
		}
		childs = childs->next;
	}
	//puts(filetree_error_string(FILE_TREE_ERROR_NOT_IMPLEMENTED));
}


void filetree_find_pfad(const Directory * start, const char * name, char* pfad)
{
	/* TODO: show files/directories recursively */
	
	if(start->first_child==NULL) { FREE(pfad);return ;}
	char* old_pfad = ALLOCATE(sizeof(char)*(strlen(pfad)+1));
	strcpy(old_pfad,pfad);
	
	Node * childs = start->first_child;
	while(childs!=NULL){
		if(childs->flags == FILE_TREE_FLAG_DIRECTORY){
			if(name == NULL)
				printf("%s%s\n",old_pfad,childs->name);
			else if (strcmp(name,"") == 0)
				printf("%s%s\n",old_pfad,childs->name);
			else if(strcmp(childs->name,name) == 0)
				printf("%s%s\n",old_pfad,childs->name);
			char* new_pfad = ALLOCATE(sizeof(char)*(strlen(old_pfad)+2+strlen(childs->name)));
			strcpy(new_pfad,old_pfad);
			strcat(new_pfad,childs->name);
			strcat(new_pfad,"/");
			
			FREE(pfad);
			filetree_find_pfad((Directory *) childs, name,new_pfad);
		}
		else{
			if(name == NULL)
				printf("%s%s\n",old_pfad,childs->name);
			else if (strcmp(name,"") == 0)
				printf("%s%s\n",old_pfad,childs->name);
			else if(strcmp(childs->name,name) == 0)
				printf("%s%s\n",old_pfad,childs->name);
		}
		childs = childs->next;
	}
	
}
/* find */
void filetree_find(const Directory * start, const char * name)
{
	/* TODO: show files/directories recursively */
	int n = 0;
	if(start->node.parent == NULL)
		n = 0;
	else
		n = strlen(start->node.name);
	 char* str = ALLOCATE(sizeof(char)*(2+n));
	 strcpy(str,"/");

	if(start->node.parent != NULL)
		{strcat(str,start->node.name);
		//strcat(str,"/");
		}
	if(name == NULL)
				printf("%s\n",str);
	else if (strcmp(name,"") == 0)
				printf("%s\n",str);
	else if(strcmp(start->node.name,name) == 0)
				printf("%s\n",str);
	
	if(start->node.parent != NULL)
		{
			str = ALLOCATE(sizeof(char)*(2+n+1));
			strcpy(str,"/");
			strcat(str,start->node.name);
			strcat(str,"/");
		}
	filetree_find_pfad(start,name,str);
	
}


/* remove file/directory */
FileError filetree_rm(Node * node)
{
	if(node->parent == NULL) return FILE_TREE_ERROR_RM_ROOT;
	Directory* parent = (Directory*) node->parent;
	Node * childs = parent->first_child;
	while(childs!=NULL){
		if(childs == node){
			if(childs->prev == NULL && childs->next == NULL)
			{
				parent->first_child == NULL; break;
			}
			if(childs->prev == NULL && childs->next != NULL)
			{
				parent->first_child == childs->next; break;
			}
			if(childs->prev != NULL && childs->next != NULL)
			{
				child->prev->next = childs->next; 
			}
		}
	}
	if(node->flags == FILE_TREE_FLAG_DIRECTORY){
		childs = ((Directory*)node)->first_child;
		FREE(node);
		while(childs!=NULL){
			filetree_rm(childs);
			childs= childs->next;
		}
	}
	else
	{
		FREE(node->name);
		FREE(node);
	}
	
	return FILE_TREE_SUCCESS;
}

/* move file/directory */
FileError filetree_mv(Node * source, Directory * destination)
{
	if(destination->parent == source || destination == source) return FILE_TREE_ERROR_SUBDIR_OF_ITSELF;
	Node* childs = destination->first_child;
	while(childs !=NULL){
		if(strcmp(childs->name,source->name) == 0)
			return FILE_TREE_ERROR_DUPLICATE_NAME
		childs = childs->next;
	} 
	Directory* parent = (Directory*) source->parent;
	childs = parent->first_child;
	while(childs!=NULL){
		if(childs == source){
			if(childs->prev == NULL && childs->next == NULL)
			{
				parent->first_child == NULL; break;
			}
			if(childs->prev == NULL && childs->next != NULL)
			{
				parent->first_child == childs->next; break;
			}
			if(childs->prev != NULL && childs->next != NULL)
			{
				child->prev->next = childs->next; 
			}
		}
	}
	source->prev = NULL;
	source->next = destination->first_child;
	destination->first_child = source;
	return FILE_TREE_SUCCESS;
}

/* print file */
void filetree_print_file(File * file)
{
	fptr = fopen(filename,"r");
	if(fptr==NULL){
		printf("Cannot open file\n");
		return;
	}
	c = fgetc(fptr)
	while(c!=EOF){
		printf("%c",c);
		c = fgetc(fptr);
	}
	printf("\n");
	fclose(fptr);
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
		if((name[i]>='a' && name[i]<='z') || (name[i]=='A' && name[i]<='Z') || (name[i]>='0' && name[i]<='9') || name[i] == '.' || name[i] == '-' || name[i] == '_')
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
