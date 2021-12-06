#include <linux/dcache.h>
#include <linux/exportfs.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/fs_stack.h>
#include <linux/kernel.h>
#include <linux/magic.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/mount.h>
#include <linux/namei.h>
#include <linux/sched.h>
#include <linux/seq_file.h>
#include <linux/slab.h>
#include <linux/statfs.h>
#include <linux/uaccess.h>
#include <linux/xattr.h>

static int __init burstfs_init(void)
{
    int ret = 0;
    struct path lower_path;
    kern_path("hi", 0, &lower_path);
    pr_info("after, lower_path is %s and %s \n", lower_path.mnt->mnt_root->d_parent->d_name.name, lower_path.dentry->d_name.name);
    pr_info("module loaded\n");
end:
    return ret;
}

static void __exit burstfs_exit(void)
{
    int ret = 0;
    // int ret = unregister_filesystem(&burstfs_file_system_type);
    // if (ret)
    //     pr_err("unregister_filesystem() failed\n");

    // burstfs_destroy_inode_cache();

    pr_info("module unloaded\n");
}

module_init(burstfs_init);
module_exit(burstfs_exit);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Leien, Chen");
MODULE_DESCRIPTION("a simple file system");