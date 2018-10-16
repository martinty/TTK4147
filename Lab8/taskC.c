#include <linux/module.h> // included for all kernel modules
#include <linux/init.h> // included for __init and __exit macros
#include <linux/kernel.h> // included for KERN_INFO
#include <linux/proc_fs.h>

struct proc_dir_entry *myproc;

int procfile_read(char *buffer, char **buffer_location, off_t offset,
										int buffer_length, int *eof, void *data){
	if (offset > 0){
		return 0;
	} 
	else{
		return sprintf(buffer, "Hello world c\n");
	}
}


static int __init task_init(void){
	myproc = create_proc_entry("taskC", 0644, NULL);
	
	myproc->read_proc = &procfile_read;

	
	return 0; // Non-zero return means that the module couldn't be loaded.
}

static void __exit task_exit(void){
	remove_proc_entry("taskC",NULL);
}


module_init(task_init);
module_exit(task_exit);
