#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/ide.h>
#include <linux/init.h>
#include <linux/module.h>


#define test_major 200
#define test_name "chrdev_test"

typedef struct chrdev
{
	int major;
	char* name;
}chrdev;

/*文件操作函数*/
//************************************************************************
static int 
chrdev_test_open(struct inode *inode, struct file *file)
{
   printk("kernel open\n");
   return 0;
};
static ssize_t 
chrdev_test_read(struct file *filp, char __user *buf, size_t count, loff_t *ppos)
{
    printk("kernel read\n");
	return 0;
};
static ssize_t 
chrdev_test_write(struct file *filp, const char __user *buf, size_t count, loff_t *ppos)
{
   printk("kernel write\n");
   return 0;
};
static int
chrdev_test_release(struct inode *inode, struct file *file)
{
    printk("kernel release\n");
	return 0;
};
//***********************************************************************


/*文件操作结构体*/
struct file_operations chrdev_test_fops = {
	.owner = THIS_MODULE,
	.read = chrdev_test_read,
	.write = chrdev_test_write,
	.open = chrdev_test_open,
	.release = chrdev_test_release
};

/*结构体赋值*/
chrdev test_chrdev={
   .major=test_major,
   .name=test_name,
  };

/*模块入口函数*/
static int __init chrdevbase_init(void)
{

	int ret=0;
//注册字符设备
	ret = register_chrdev(test_chrdev.major, test_chrdev.name, &chrdev_test_fops);
    if(ret<0)
	{
	  printk("regist chrdev fail");
	}
	

	printk("init ok\n");

	return 0;
};

/*模块出口函数*/
static void __exit chrdevbase_exit(void)
{
     unregister_chrdev(test_chrdev.major, test_chrdev.name);
	 printk("rm ok\n");
};

/*模块出入口*/
module_init(chrdevbase_init);
module_exit(chrdevbase_exit);

MODULE_LICENSE("GPL");