/*
	Структура содержит информацию о ФС,
	которой принадлежит процесс
	Любая программа принадлежит какой-то ФС
*/

struct fs_struct{
	atomic_t count;			// счетчик ссылок на структуру 
	// ...

	struct dentry umask;	// права доступа к файлу, используемые по умолчанию ?

	struct dentry *root;	// объект dentry корневого каталога
	struct dentry *pwd;		// объект dentry текущего рабочего каталога
	struct dentry *altroot; 

	struct vfsmount *rootmnt;
	struct vfsmount *pwdmnt;
	struct vfsmount *altmnt;
	// ...

}