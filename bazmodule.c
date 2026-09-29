#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>

// The kernel is written in C, and C has no classes. What you see are plain functions, not methods.
// The kernel still uses object-like ideas, built by hand:
//
// ┌─────────────────────────────┬───────────────────────────────────────────────────────────────────────┐
// │          OOP idea           │                      How the kernel does it in C                      │
// ├─────────────────────────────┼───────────────────────────────────────────────────────────────────────┤
// │ Interface / virtual methods │ a struct of function pointers, such as proc_ops                       │
// ├─────────────────────────────┼───────────────────────────────────────────────────────────────────────┤
// │ Implementing the interface  │ filling that struct with your functions (.proc_read = bazmodule_read) │
// ├─────────────────────────────┼───────────────────────────────────────────────────────────────────────┤
// │ Private members             │ static functions and variables, visible only in this file             │
// ├─────────────────────────────┼───────────────────────────────────────────────────────────────────────┤
// │ Class name / namespace      │ a name prefix, such as bazmodule_                                     │
// ├─────────────────────────────┼───────────────────────────────────────────────────────────────────────┤
// │ Object data                 │ a struct holding the state, passed around by pointer                  │
// └─────────────────────────────┴───────────────────────────────────────────────────────────────────────┘
//

static char buf[128];
static size_t len;

static ssize_t bazmodule_read(struct file *f, char __user *u, size_t n, loff_t *off)
{
    return simple_read_from_buffer(u, n, off, buf, len);
}

static ssize_t bazmodule_write(struct file *f, const char __user *u, size_t n, loff_t *off)
{
    len = min(n, sizeof(buf));
    if (copy_from_user(buf, u, len))
    return -EFAULT;
pr_info("bazmodule: got %zu bytes\n", len);
return n;
}

// This works like a small vtable.
// The kernel doesn't know your functions by name. It calls them through that table, much as a class's methods are called through an interface.
static const struct proc_ops bazmodule_ops = {
    .proc_read  = bazmodule_read,
    .proc_write = bazmodule_write,
};

static int __init bazmodule_init(void)
{
    if (!proc_create("baz", 0666, NULL, &bazmodule_ops))
        return -ENOMEM;
    pr_info("bazmodule: loaded\n");
    return 0;
}

static void __exit bazmodule_exit(void)
{
    remove_proc_entry("baz", NULL);
    pr_info("bazmodule: unloaded\n");
}

module_init(bazmodule_init);
module_exit(bazmodule_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Baz");
MODULE_DESCRIPTION("Baz's experimental module");
