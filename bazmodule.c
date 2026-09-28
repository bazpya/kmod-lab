#include <linux/module.h>
#include <linux/init.h>

static int __init bazmodule_init(void)
{
    pr_info("bazmodule: loaded\n");
    return 0;
}

static void __exit bazmodule_exit(void)
{
    pr_info("bazmodule: unloaded\n");
}

module_init(bazmodule_init);
module_exit(bazmodule_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Baz");
MODULE_DESCRIPTION("Baz's experimental module");
