
zval *wing_zend_read_property(zend_class_entry *scope, zval *object, const char *name)
{
	TSRMLS_FETCH();
#if PHP_MAJOR_VERSION >= 7
	return zend_read_property(scope, object, name, strlen(name), 0, 0 TSRMLS_CC);
#else
	return zend_read_property(scope, object, name, strlen(name), 0 TSRMLS_CC);
#endif
}


/**
* ?��?????php??????????????php???????? <?php ???
*
* @param char* file
* @return BOOL
*/
int wing_file_is_php(const char *file)
{

	char find_str[] = " ";
	char *find = strstr((char*)file, find_str);
	char path[MAX_PATH] = { 0 };

	strncpy((char*)path, file, (size_t)(find - file));
	FILE *handle = fopen(path, "r");
	if (!handle) {
		return 0;
	}
	//char *find = NULL;
	char line[8] = { 0 };
	memset(line, 0, 8);
	fgets(line, 7, handle);

	find = strstr(line, "<?php");
	if (find == line) {
		fclose(handle);
		return 1;
	}

	memset(line, 0, 8);
	fgets(line, 7, handle);
	find = strstr(line, "<?php");
	if (find == line) {
		fclose(handle);
		return 1;
	}
	fclose(handle);

	return 0;
}

int wing_write_cmdline(unsigned long process_id, char *cmdline)
{
    char tmp[MAX_PATH] = {0};
    wing_get_tmp_dir((char**)&tmp);
    char path[MAX_PATH] = {0};
    strcpy(path, tmp);
    strcpy((char*)(path+strlen(tmp)), "/");
    char _process_id[32] = {0};
    sprintf(_process_id, "%lu", process_id);
    strcpy((char*)(path+strlen(tmp)+1), _process_id);

    if (access(path, F_OK) != 0) {
        if (0 != mkdir(path, 0777)) {
            return 0;
        }
    }

    strcpy((char*)(path+strlen(tmp)+1+strlen(_process_id)), "/cmdline");

    FILE *handle = fopen((const char*)path, "w");
    if (handle) {
        if (1 != fwrite(cmdline, strlen(cmdline), 1, handle)) {
            fclose(handle);
            //写入失败
            return 0;
        }
        fclose(handle);
        return 1;
    }
    return 0;
}

void wing_get_cmdline(unsigned long process_id, char **buffer)
{
    char tmp[MAX_PATH] = {0};
    wing_get_tmp_dir((char**)&tmp);
    char path[MAX_PATH] = {0};
    strcpy(path, tmp);
    strcpy((char*)(path+strlen(tmp)), "/");
    char _process_id[32] = {0};
    sprintf(_process_id, "%lu", process_id);
    strcpy((char*)(path+strlen(tmp)+1), _process_id);

    if (access(path, F_OK) != 0) {
        *buffer = NULL;
        return;
    }

    strcpy((char*)(path+strlen(tmp)+1+strlen(_process_id)), "/cmdline");

    if (access(path, F_OK) != 0) {
        *buffer = NULL;
        return;
    }
    FILE *handle = fopen((const char*)path, "w");
    if (!handle) {
        *buffer = NULL;
        return;
    }
    fgets(*buffer, MAX_PATH,handle);
    fclose(handle);
}
