/*
	Любой процесс имеет дескриптор и это task_struct
*/

struct task_struct {
	volotile long state; // TASK_RUNNING; TASK_ITERRUPTABLE; TASK_UNITERRUPTABLE;
	// TASK_STOPPED; TASK_ZOMBIE;

	//...

	struct mm_struct *m;			// что-то с памятью
	struct mm_struct *active_mm;

	//...

	// info about file system
	struct fs_struct *fs;

	// info about opened files
	struct file_struct *files;

	// namespaces
	struct nsproxy *nsproxy;

	//...
}